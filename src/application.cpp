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

#define GROUP_GENERAL "General"
#define SETTING_LOG_LEVEL "logLevel"

// +-----------------------------------------------------------
gc::Application::Application(int& argc, char** argv): QApplication(argc, argv)
{
	// Create the QSettings from the configuration.ini file
	QString sIniFile = QString("%1%2configuration.ini").arg(QCoreApplication::applicationDirPath()).arg(QDir::separator());
	m_pSettings = new QSettings(sIniFile, QSettings::IniFormat);

	// Read the general settings
	m_pSettings->beginGroup(GROUP_GENERAL);
	m_eLogLevel = (QtMsgType)m_pSettings->value(SETTING_LOG_LEVEL, QtInfoMsg).toInt();
	m_pSettings->endGroup();

	// Create the log file
	QFileInfo oAppFile = QFileInfo(QCoreApplication::applicationFilePath());
    QString sLogFile = QString("%1%2%3.log").arg(QCoreApplication::applicationDirPath()).arg(QDir::separator()).arg(oAppFile.baseName());

    m_oLogFile.open(qPrintable(sLogFile), ios::app);
    if(!m_oLogFile.is_open())
		qFatal("Error opening log file %s for writing", qPrintable(sLogFile));

	// Load the style sheet file from resources
	QFile oFile(":/resources/stylesheet.css");
	if (oFile.open(QIODevice::ReadOnly | QIODevice::Text))
		m_sStyleSheet = oFile.readAll();
	else
		qFatal("Error reading style sheet from resources.");

	m_pCurrentTranslator = NULL; // The default language (English) does not require a translator

	// Load the translations
	m_pPTBRTranslator = new QTranslator(this);
	if (!m_pPTBRTranslator->load(QLocale("pt"), ":/translations/pt-br.qm"))
		qFatal("Could not load the pt-br translation from resource");

	// Setup the log message handler
    qInstallMessageHandler(&gc::Application::handleLogOutput);
	qInfo("GameCap (v%s) started.", GC_VERSION);
	qDebug("Running from %s", qPrintable(QCoreApplication::applicationFilePath()));

	// Creates the access to the games
	m_pGameControl = new GameControl(this);
}

// +-----------------------------------------------------------
gc::Application::~Application()
{
	// Update and close the log file
    m_oLogFile.flush();
	m_oLogFile.close();
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
QtMsgType gc::Application::getLogLevel()
{
	return m_eLogLevel;
}

// +-----------------------------------------------------------
void gc::Application::setLogLevel(QtMsgType eLevel)
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
void gc::Application::handleLogOutput(QtMsgType eType, const QMessageLogContext &oContext, const QString &sMsg) {
	// Do not log the message if its type is bigger then the maximum configured log level.
	// Fatal messages are always logged.
	if(eType != QtFatalMsg && eType < ((Application*)qApp)->m_eLogLevel)
		return;

	QString sNow = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

	// Only include context information if it exists (when compiled in debug)
	QString sDebugInfo;
	if(oContext.line != 0)
	{
		QString sSource = QFileInfo(QFile(oContext.file).fileName()).fileName();
		sDebugInfo = QString(" [%1:%2, %3] ").arg(sSource).arg(oContext.line).arg(oContext.function);
	}
	else
		sDebugInfo = " ";

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

	((Application*)qApp)->m_oLogFile << qPrintable(sNow) << qPrintable(sDebugInfo) << qPrintable(sLevel) << qPrintable(sMsg) << endl;
	((Application*)qApp)->m_oLogFile.flush();

	if (eType == QtFatalMsg)
	{
		cerr << qPrintable(sNow) << qPrintable(sDebugInfo) << qPrintable(sLevel) << qPrintable(sMsg) << endl;

		QApplication::beep();
		QMessageBox::critical(NULL, qApp->translate("Main", "Runtime Error"), qApp->translate("Main", "A fatal exception ocurred and the application must be terminated. Please check the log file for details."), QMessageBox::Ok);

		abort();
	}
}