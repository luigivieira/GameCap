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

#ifndef GAME_H
#define GAME_H

#include <QProcess>

namespace gc
{
	/**
	 * Abstract class used to access the games. Must be inherited to create a concrete
	 * class for each game used.
	 */
	class Game: public QObject
	{
		Q_OBJECT
	public:

		/**
		 * Class constructor.
		 * @param sExecutable QString with the path and filename of the game's executable.
		 * @param pParent Instance for the QObject that is the parent of this one. Default is NULL.
		 */
		Game(QString sExecutable, QObject *pParent = NULL);

		/**
		 * Runs the game by starting a new process for its executable.
		 * @param iTimeout Integer value with the timeout (in seconds) to be used for the game.
		 * The default is 600 seconds (10 minutes). After that time expires, if the game is still
		 * running it will be forcefully stopped.
		 */
		virtual void run(int iTimeout = 600);

		/**
		 * Queries the name of the game. Must be implemented by subclasses.
		 * @return String with the game's name.
		 */
		virtual QString name() = 0;

		/**
		 * Queries the genre of the game. Must be implemented by subclasses.
		 * @return String with the game's genre.
		 */
		virtual QString genre() = 0;

		/**
		 * Queries the goal of the game. Must be implemented by subclasses.
		 * @return String with the game's goal.
		 */
		virtual QString goal() = 0;

		/**
		 * Queries the help on how to play game. Must be implemented by subclasses.
		 * @return String with the game's help on how to play.
		 */
		virtual QString howToPlay() = 0;

	protected slots:

		/**
		 * Captures the signal of game started.
		 */
		void onGameStarted();

		/**
		 * Captures the signal of game finished.
		 * @param iExitCode Integer with the exit code given by the process.
		 * @param eExitStatus QProcess::ExitStatus enumeration with the status of the
		 * application when it exited (either normal or crashed).
		 */
		void onGameFinished(int iExitCode, QProcess::ExitStatus eExitStatus);

		/**
		 * Captures the signal of game error.
		 * @param eError QProcess::ProcessError enumeration with the reason for the error.
		 */
		void onGameError(QProcess::ProcessError eError);

	protected:

		/**
		 * Prints to the log file information on the game created.
		 * This method must be called by the constructor of each inherited class
		 * because it calls pure virtual methods that can not be called on the base
		 * abstract class.
		 */
		void logInfo();

	private:

		/** Path and filename of the game's executable. */
		QString m_sFileName;

		/** Handles the process used to run the game. */
		QProcess m_oProcess;
	};
}

#endif // GAME_H