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
#include <QTimer>

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

		/** Reasons for the gameplay session to end. */
		enum EndReason { Concluded, Cancelled, Failed };
		Q_ENUM(EndReason);

		/**
		 * Class constructor.
		 * @param pParent Instance for the QObject that is the parent of this one. Default is NULL.
		 */
		Game(QObject *pParent = NULL);

		/**
		 * Runs the game by starting a new process for its executable.
		 * @param iTimeLimit Integer value with the time (in seconds) to limit the game session.
		 * The default is 600 seconds (10 minutes). After that time expires, if the game is still
		 * running it will be forcefully stopped.
		 */
		virtual void run(int iTimeLimit = 600);

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

		/**
		 * Indicates if the game is running.
		 */
		bool running();

	signals:

		/**
		 * Indicates the termination of the game process.
		 * @param eReason Value of the EndReason enumeration indicating the reason
		 * for the game to end: failure/crash, premature exiting by the participant
		 * or correct conclusion in the given time.
		 */
		void gameEnded(Game::EndReason eReason);

		/**
		 * Indicates the remaining time for the participant to play the game.
		 * @param iSeconds Integer with the remaining time of gameplay in seconds.
		 */
		void gameRemainingTime(int iSeconds);

	protected slots:

		/**
		 * Captures the signal of process started.
		 */
		void onProcessStarted();

		/**
		 * Captures the signal of process finished.
		 * @param iExitCode Integer with the exit code given by the process.
		 * @param eExitStatus QProcess::ExitStatus enumeration with the status of the
		 * application when it exited (either normal or crashed).
		 */
		void onProcessFinished(int iExitCode, QProcess::ExitStatus eExitStatus);

		/**
		 * Captures the signal of process error.
		 * @param eError QProcess::ProcessError enumeration with the reason for the error.
		 */
		void onProcessError(QProcess::ProcessError eError);

		/**
		 * Captures the timer timeout signal (each second).
		 */
		void onTimeout();

	protected:

		/**
		 * Sets up the game (reading needed data from the settings and logging game info).
		 * This method must be called by the constructor of each inherited class
		 * because it calls pure virtual methods that can not be called on the base
		 * abstract class.
		 */
		void setup();

	private:

		/** Path and filename of the game's executable. */
		QString m_sFileName;

		/** Handles the process used to run the game. */
		QProcess m_oProcess;

		/** Remaining time (in seconds) for the game session. */
		int m_iRemainingTime;

		/** Timer used to limit the game session. */
		QTimer m_oTimer;
	};
}

#endif // GAME_H
