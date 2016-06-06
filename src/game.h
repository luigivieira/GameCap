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
	 * Indicates the existance of the GameControl class, without defining it, just to the
	 * friend declaration above.
	 */
	class GameControl;

	/**
	 * Abstract class used to access the games. Must be inherited to create a concrete
	 * class for each game used.
	 */
	class Game: public QObject
	{
		Q_OBJECT
	public:

		/** Allows the GameControl class to access the protected method ::run(). */
		friend GameControl;

		/** Reasons for the gameplay session to end. */
		enum EndReason { Concluded, Failed };
		Q_ENUM(EndReason);

		/**
		 * Class constructor.
		 * @param pParent Instance for the QObject that is the parent of this one. Default is NULL.
		 */
		Game(QObject *pParent = NULL);

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

	signals:

		/**
		 * Indicates the starting of the game process.
		 */
		void gameStarted();

		/**
		 * Indicates the ending of the game process.
		 * @param eReason Value of the EndReason enumeration indicating the reason
		 * for the game to end: failure/crash, premature exiting by the participant
		 * or correct conclusion in the given time.
		 */
		void gameEnded(Game::EndReason eReason);

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

	protected:

		/**
		 * Indicates if the game is running.
		 */
		bool running();

		/**
		 * Starts the game by creating a new process for its executable.
		 */
		void start();

		/**
		 * Forcibly stops the game by killing its process.
		 */
		void stop();

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
	};
}

#endif // GAME_H
