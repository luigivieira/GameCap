/* Copyright (C) 2016 Luiz Carlos Vieira (http://www.luiz.vieira.nom.br)
 *
 * This file is part of GameCap.
 *
 * GameCap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GameCap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "application.h"
#include "version.h"
#include <typeinfo>
#include <QDebug>
#include <QDateTime>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QTranslator>
#include <iostream>
#include <QMetaType>

using namespace std;

#define GROUP_MAIN "Main"
#define SETTING_LOG_LEVEL "logLevel"
#define SETTING_DATA_PATH "dataPath"
#define SETTING_GAMEPLAY_TIME_LIMIT "gameplayTimeLimit"

// +-----------------------------------------------------------
gc::Application::Application(int& argc, char** argv): QApplication(argc, argv)
{
	// Create/Open the log file
	m_eLogLevel = Fatal;
	QFileInfo oAppFile = QFileInfo(QCoreApplication::applicationFilePath());
	QString sLogFile = QString("%1%2%3.log").arg(QCoreApplication::applicationDirPath()).arg(QDir::separator()).arg(oAppFile.baseName());

#ifdef _DEBUG
	// Remove the existing log file if compiled in debug
	QFile::remove(sLogFile);
#endif

	m_oLogFile.open(qPrintable(sLogFile), ios::app);
	if (!m_oLogFile.is_open())
		qFatal("Error opening log file %s for writing", qPrintable(sLogFile));
	qInstallMessageHandler(&gc::Application::handleLogOutput);

	// Read the configuration file
	QString sIniFile = QString("%1%2configuration.ini").arg(QCoreApplication::applicationDirPath()).arg(QDir::separator());
	if (!QFileInfo::exists(sIniFile))
		qFatal("Configuration file %s does not exist", qPrintable(sIniFile));
	m_pSettings = new QSettings(sIniFile, QSettings::IniFormat);

	m_pSettings->beginGroup(GROUP_MAIN);
	m_eLogLevel = (LOG_LEVEL) m_pSettings->value(SETTING_LOG_LEVEL, Fatal).toInt();
	m_iGameplayTimeLimit = m_pSettings->value(SETTING_GAMEPLAY_TIME_LIMIT, 60).toUInt();
	m_sDataPath = m_pSettings->value(SETTING_DATA_PATH, "").toString();
	m_pSettings->endGroup();

	// Validate the configurations
	if (m_eLogLevel < Fatal)
		m_eLogLevel = Fatal;
	else if (m_eLogLevel > Debug)
		m_eLogLevel = Debug;

	if (!m_sDataPath.length())
		qFatal("The group/key [%s/%s] is missing in the configuration file.", GROUP_MAIN, SETTING_DATA_PATH);
	if (!QDir(m_sDataPath).exists())
	{
		qDebug("Configured data path does not exist. Trying to create it...");
		if (!QDir().mkpath(m_sDataPath))
			qFatal("Data path %s does not exist and could not be created.", qPrintable(m_sDataPath));
	}

	// Load the style sheet file from resources
	QFile oFile(":/resources/stylesheet.css");
	if (oFile.open(QIODevice::ReadOnly | QIODevice::Text))
		m_sStyleSheet = oFile.readAll();
	else
		qFatal("Error reading style sheet from resources.");
	m_pCurrentTranslator = NULL;

	// Load the translations
	m_pPTBRTranslator = new QTranslator(this);
	if (!m_pPTBRTranslator->load(QLocale("pt"), ":/translations/pt-br.qm"))
		qFatal("Could not load the pt-br translation from resource");

	// Conclude the application start-up
	qInfo("GameCap (v%s) started.", GC_VERSION);
	qDebug("Running from %s", qPrintable(QCoreApplication::applicationFilePath()));
	qDebug() << "Configured log level is: " << m_eLogLevel;
	qDebug() << "Configured gameplay time limit is: " << m_iGameplayTimeLimit;
	qDebug() << "Configured data path is: " << m_sDataPath;

	// Prepares the access to the video recordings and the games
	m_pVideoControl = new VideoControl(this);
	m_pGameControl = new GameControl(this);

	connect(m_pGameControl, &GameControl::gameplayStarted, this, &Application::onGameplayStarted);
	connect(m_pGameControl, &GameControl::gameplayEnded, this, &Application::onGameplayEnded);

	connect(m_pVideoControl, &VideoControl::captureStarted, this, &Application::onCaptureStarted);
	connect(m_pVideoControl, &VideoControl::captureEnded, this, &Application::onCaptureEnded);

	connect(&m_oTimer, &QTimer::timeout, this, &Application::onTimeout);
}

// +-----------------------------------------------------------
gc::Application::~Application()
{
	// Update and close the log file
    m_oLogFile.flush();
	m_oLogFile.close();
}

// +-----------------------------------------------------------
unsigned int gc::Application::getSubjectID() const
{
	return m_iSubjectID;
}

// +-----------------------------------------------------------
QSettings* gc::Application::getSettings()
{
	return m_pSettings;
}

// +-----------------------------------------------------------
QString gc::Application::getStyleSheet()
{
	return m_sStyleSheet;
}

// +-----------------------------------------------------------
void gc::Application::setLanguage(Language eLanguage)
{
	// Remove current translator (if any)
	if (m_pCurrentTranslator)
	{
		qApp->removeTranslator(m_pCurrentTranslator);
		m_pCurrentTranslator = NULL;
	}

	// Identify the translator according to the language
	switch (eLanguage)
	{
		case PT_BR:
			m_pCurrentTranslator = m_pPTBRTranslator;
			break;
	}

	// Istall the required translator and emit the change signal
	if (m_pCurrentTranslator)
		qApp->installTranslator(m_pCurrentTranslator);

	emit languageChanged(eLanguage);
}

// +-----------------------------------------------------------
gc::GameControl* gc::Application::gameControl()
{
	return m_pGameControl;
}

// +-----------------------------------------------------------
gc::VideoControl* gc::Application::videoControl()
{
	return m_pVideoControl;
}

// +-----------------------------------------------------------
int gc::Application::exec()
{
	int iRet = QApplication::exec();
	if (iRet == 0)
		qInfo("GameCap terminated successfully.");
	else
		qInfo("GameCap terminated with error code: %d", iRet);
	return iRet;
}

// +-----------------------------------------------------------
gc::Application::LOG_LEVEL gc::Application::getLogLevel()
{
	return m_eLogLevel;
}

// +-----------------------------------------------------------
void gc::Application::setLogLevel(Application::LOG_LEVEL eLevel)
{
	m_eLogLevel = eLevel;
}

// +-----------------------------------------------------------
bool gc::Application::notify(QObject* pReceiver, QEvent* pEvent)
{
    try
	{
		// Retransmit the event notification
        return QApplication::notify(pReceiver, pEvent);
    }
    catch (std::exception &e)
	{
        qFatal("The exception %s ocurred while sending event [%s] to object [%s] (%s)",
            e.what(), typeid(*pEvent).name(), qPrintable(pReceiver->objectName()), typeid(*pReceiver).name());
    }
    catch (...)
	{
        qFatal("An exception ocurred while sending event [%s] to object [%s] (%s)",
            typeid(*pEvent).name(), qPrintable(pReceiver->objectName()), typeid(*pReceiver).name());
    }

    return false;
}

// +-----------------------------------------------------------
void gc::Application::handleLogOutput(QtMsgType eType, const QMessageLogContext &oContext, const QString &sMsg)
{
	// Convert the qt message type to a log level
	LOG_LEVEL eLevel = (LOG_LEVEL) (eType == QtDebugMsg ? 4 : (eType == QtInfoMsg ? 3 : std::abs(eType - QtFatalMsg)));

	// Do not log the message if its level is bigger then the maximum configured.
	LOG_LEVEL eMax = ((Application*) qApp)->m_eLogLevel;
	if(eLevel > ((Application*) qApp)->m_eLogLevel)
		return;

	QString sNow = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

	QString sDebugInfo = " ";
#ifdef _DEBUG
	if(oContext.line != 0)
	{
		QString sSource = QFileInfo(QFile(oContext.file).fileName()).fileName();
		sDebugInfo = QString(" [%1:%2, %3] ").arg(sSource).arg(oContext.line).arg(oContext.function);
	}
#endif

	QString sLevel;
	switch (eType)
	{
		case QtDebugMsg:
			sLevel = "DEBUG: ";
			break;

		case QtInfoMsg:
			sLevel = "INFO: ";
			break;

		case QtWarningMsg:
			sLevel = "WARNING: ";
			break;

		case QtCriticalMsg:
			sLevel = "CRITICAL: ";
			break;

		case QtFatalMsg:
			sLevel = "FATAL: ";
			break;
	}

	((Application*) qApp)->m_oLogFile << qPrintable(sNow) << qPrintable(sDebugInfo) << qPrintable(sLevel) << qPrintable(sMsg) << endl;
	((Application*) qApp)->m_oLogFile.flush();

	if (eType == QtFatalMsg)
	{
		cerr << qPrintable(sNow) << qPrintable(sDebugInfo) << qPrintable(sLevel) << qPrintable(sMsg) << endl;

		QApplication::beep();
		QMessageBox::critical(NULL, qApp->translate("Main", "Runtime Error"), qApp->translate("Main", "A fatal exception ocurred and the application must be terminated. Please check the log file for details."), QMessageBox::Ok);
	}
}

// +-----------------------------------------------------------
void gc::Application::onTimeout()
{
	m_iTimeRemaining--;
	if (m_iTimeRemaining <= 0)
		stopGameplay();
	else
		emit gameplayTimeRemaining(m_iTimeRemaining);
}

// +-----------------------------------------------------------
void gc::Application::startGameplay()
{
	m_pVideoControl->startCapture();
}

// +-----------------------------------------------------------
void gc::Application::stopGameplay()
{
	m_pVideoControl->stopCapture();
	m_oTimer.stop();
	m_iTimeRemaining = 0;
}

// +-----------------------------------------------------------
void gc::Application::onGameplayStarted()
{
	m_iTimeRemaining = m_iGameplayTimeLimit;
	m_oTimer.start(1000);
	emit gameplayTimeRemaining(m_iTimeRemaining);
}

// +-----------------------------------------------------------
void gc::Application::onGameplayEnded(GameControl::GameplayResult eResult)
{
	switch(eResult)
	{
		case GameControl::Failed:
			stopGameplay();
			emit gameplayFailedToStart();
			break;

		case GameControl::ClosedByUser:
			stopGameplay();
			emit gameplayCancelled();
			break;

		default: //case GameControl::ClosedBySystem:
			emit gameplayCompleted();
			break;
	}
}

// +-----------------------------------------------------------
void gc::Application::onCaptureStarted()
{
	m_pGameControl->startGameplay();
}

// +-----------------------------------------------------------
void gc::Application::onCaptureEnded(gc::VideoControl::CaptureResult eResult)
{
	if (eResult == VideoControl::FailedToStart)
		emit gameplayFailedToStart();
	else // if(eResult == VideoControl::Closed )
		m_pGameControl->stopGameplay();
}