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
#include <QFileInfo>

 // Declare the enum so it can be used with signals
Q_DECLARE_METATYPE(gc::Game::EndReason);

// +-----------------------------------------------------------
gc::Game::Game(QString sExecutable, QObject *pParent): QObject(pParent)
{
	// Register the enum so it can be used with signals and slots.
	// IMPORTANT: The full qualified name (i.e. "gc::Game::EndReason") must be
	// used EVERYWHERE (that is, in the signal/slot signatures and in the calls to
	// QObject::connect), in order for the signal delivery to work!
	qRegisterMetaType<gc::Game::EndReason>("gc::Game::EndReason");

	m_sFileName = sExecutable;

	connect(&m_oProcess, &QProcess::started, this, &Game::onProcessStarted);
	connect(&m_oProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &Game::onProcessFinished);
	connect(&m_oProcess, &QProcess::errorOccurred, this, &Game::onProcessError);

	connect(&m_oTimer, &QTimer::timeout, this, &Game::onTimeout);
}

// +-----------------------------------------------------------
void gc::Game::logInfo()
{
	qInfo("Game %s (%s) configured from path %s", qPrintable(name()), qPrintable(genre()), qPrintable(m_sFileName));
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
	m_oProcess.start(m_sFileName);
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