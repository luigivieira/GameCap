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

#include "game.h"
#include "application.h"
#include <QFileInfo>

#define SETTING_FILE_NAME "fileName"

// +-----------------------------------------------------------
gc::Game::Game(QObject *pParent): QObject(pParent)
{
	connect(&m_oProcess, &QProcess::started, this, &Game::onProcessStarted);
	connect(&m_oProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &Game::onProcessFinished);
	connect(&m_oProcess, &QProcess::errorOccurred, this, &Game::onProcessError);
	connect(&m_oTimer, &QTimer::timeout, this, &Game::onTimeout);
}

// +-----------------------------------------------------------
void gc::Game::setup()
{
	QSettings *pSettings = ((Application*) qApp)->getSettings();

	// Read the game settings
	pSettings->beginGroup(name());
	m_sFileName = pSettings->value(SETTING_FILE_NAME).toString();
	pSettings->endGroup();

	if (!m_sFileName.length())
	{
		QString sMsg = QString("the group/key [%1/%2] is missing in the configuration file.").arg(name()).arg(SETTING_FILE_NAME);
		qFatal(qPrintable(sMsg));
	}
	else if(!QFileInfo::exists(m_sFileName))
	{
		QString sMsg = QString("the file name [%1] configured for the game %1 does not exist.").arg(m_sFileName).arg(name());
		qFatal(qPrintable(sMsg));
	}

	qInfo("Game %s (%s) configured to run from %s", qPrintable(name()), qPrintable(genre()), qPrintable(m_sFileName));
}

// +-----------------------------------------------------------
void gc::Game::run(int iTimeLimit)
{
	// Run only if not already running
	if (m_oProcess.state() != QProcess::NotRunning)
		return;

	m_iRemainingTime = iTimeLimit;

	QString sCommand = QFileInfo(m_sFileName).fileName();
	QString sPath = QFileInfo(m_sFileName).absolutePath();
	m_oProcess.setWorkingDirectory(sPath);
	m_oProcess.start(m_sFileName, QStringList());
}

// +-----------------------------------------------------------
bool gc::Game::running()
{
	return (m_oProcess.state() != QProcess::NotRunning);
}

// +-----------------------------------------------------------
void gc::Game::onProcessStarted()
{
	qInfo("Game %s started.", qPrintable(name()));
	m_oTimer.start(1000);
}

// +-----------------------------------------------------------
void gc::Game::onProcessFinished(int iExitCode, QProcess::ExitStatus eExitStatus)
{
	qDebug("Game %s finished with exit code [%d] and exit status [%s]", qPrintable(name()), iExitCode, (eExitStatus == QProcess::NormalExit ? "normal" : "crashed"));

	if (m_oTimer.isActive())
		m_oTimer.stop();

	if (m_iRemainingTime > 0)
	{
		qInfo("Game %s closed earlier by participant", qPrintable(name()));
		emit gameEnded(Cancelled);
	}
	else
	{
		qInfo("Game %s concluded in the correct time", qPrintable(name()));
		emit gameEnded(Concluded);
	}
}

// +-----------------------------------------------------------
void gc::Game::onProcessError(QProcess::ProcessError eError)
{
	if (eError == QProcess::FailedToStart)
	{
		qInfo("Game %s failed to start", qPrintable(name()));
		emit gameEnded(Failed);
	}
}

// +-----------------------------------------------------------
void gc::Game::onTimeout()
{
	m_iRemainingTime--;
	if (m_iRemainingTime <= 0)
	{
		m_oProcess.kill();
		m_oTimer.stop();
	}
	else
		emit gameRemainingTime(m_iRemainingTime);
}