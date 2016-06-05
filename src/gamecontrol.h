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

#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include "game.h"
#include <QObject>
#include <vector>
#include <QProcess>
#include <QTimer>

namespace gc
{
	/**
	 * Class used to handle game selection, execution and termination, as well as the
	 * video capture of gameplay and player's face.
	 */
	class GameControl: public QObject
	{
		Q_OBJECT
	public:

		/** Enumeration with the possible results for the gameplay session. */
		enum GameplaySessionResult { Success, Cancelled, Error };
		Q_ENUM(GameplaySessionResult)

		/**
		* Class constructor.
		* @param pParent Instance for the QObject that is the parent of this one. Default is NULL.
		*/
		GameControl(QObject *pParent = NULL);

		/**
		 * Selects the next game to be used in the experiment.
		 * @return Instance of the Game to be next used in the experiment.
		 */
		Game* selectNextGame();

		/**
		 * Gets the current game being used in the experiment.
		 * @return Instance of the Game used in the experiment.
		 */
		Game* currentGame();

		/**
		 * Indicates if the game is running.
		 */
		bool running();

		/**
		 * Runs the current game.
		 * @param iTimeLimit Integer value with the time (in seconds) to limit the game session.
		 * The default is 600 seconds (10 minutes). After that time expires, if the game is still
		 * running it will be forcefully stopped.
		 */
		void run(int iTimeLimit = 600);

	protected slots:

		/**
		 * Captures the timer timeout signal (each second).
		 */
		void onTimeout();

		/**
		 * Captures the indication that the game ended.
		 * @param eReason Value of the EndReason enumeration with the reason for the
		 * game to end (among Concluded and Failed).
		 */
		void onGameEnded(Game::EndReason eReason);

	signals:

		/**
		 * Indicates the remaining game time, once the game is running.
		 * @param iSeconds Remaining time in seconds.
		 */
		void gameRemainingTime(int iSeconds);

		/**
		 * Indicates that the gameplay ended.
		 * @param eResult Value of the GameplaySessionResult enumeration with the gameplay
		 * session result (among Success, Cancelled and Error).
		 */
		void gameplayEnded(GameControl::GameplaySessionResult eResult);

	private:

		/** Current game being used in the experiment. */
		Game *m_pCurrentGame;

		/** List of available games. */
		std::vector<Game*> m_vGames;

		/** Remaining time (in seconds) for the game session. */
		int m_iRemainingTime;

		/** Timer used to limit the game session. */
		QTimer m_oTimer;

		QProcess m_oGameplayCap;

		QProcess m_oPlayerCap;
	};
}

#endif // GAME_CONTROL_H
