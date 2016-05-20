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

// +-----------------------------------------------------------
gc::Game::Game(QString sExecutable, QObject *pParent): QObject(pParent)
{
	m_sFileName = sExecutable;

	QString sPath = QFileInfo(m_sFileName).absolutePath();
	m_oProcess.setWorkingDirectory(sPath);

	connect(&m_oProcess, &QProcess::started, this, &Game::onGameStarted);
	connect(&m_oProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &Game::onGameFinished);
	connect(&m_oProcess, &QProcess::errorOccurred, this, &Game::onGameError);
}

// +-----------------------------------------------------------
void gc::Game::logInfo()
{
	qInfo("Game %s (%s) configured from path %s", qPrintable(name()), qPrintable(genre()), qPrintable(m_sFileName));
}

// +-----------------------------------------------------------
void gc::Game::run(int iTimeout)
{
	// Run only if not already running
	if (m_oProcess.state() != QProcess::NotRunning)
		return;

	m_oProcess.start(m_sFileName);
}

// +-----------------------------------------------------------
void gc::Game::onGameStarted()
{
	qInfo("Game %s started.", qPrintable(name()));
}

// +-----------------------------------------------------------
void gc::Game::onGameFinished(int iExitCode, QProcess::ExitStatus eExitStatus)
{
	qInfo("Game %s finished with exit code [%d] and exit status [%s]", qPrintable(name()), iExitCode, (eExitStatus == QProcess::NormalExit ? "normal" : "crashed"));
}

// +-----------------------------------------------------------
void gc::Game::onGameError(QProcess::ProcessError eError)
{
	QString sReason;
	switch (eError)
	{
		case QProcess::FailedToStart: //### file not found, resource error
			sReason = "the game failed to start (either the executable is missing or it lacks the correct permissions)";
			break;

		case QProcess::Crashed:
			sReason = "the game crashed after it started successfully";
			break;

		case QProcess::Timedout:
			sReason = "the game took to long to start";
			break;

		case QProcess::ReadError:
		case QProcess::WriteError:
			sReason = "it was not possible to communicate with the game after it started successfully";
			break;

		default: //case QProcess::UnknownError:
			sReason = "an unknown error ocurred";
			break;
	}

	qInfo("Game %s closed with error [%d]", qPrintable(name()), qPrintable(sReason));
}