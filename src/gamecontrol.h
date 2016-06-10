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
		enum GameplayResult { Failed, ClosedBySystem, ClosedByUser };
		Q_ENUM(GameplayResult)

		/**
		 * Class constructor.
		 * @param pParent Instance for the QObject that is the parent of this one. Default is NULL.
		 */
		GameControl(QObject *pParent = NULL);

		/**
		 * Selects the next game to be used in the next execution.
		 * @return Instance of the Game to be next used.
		 */
		Game* selectNextGame();

		/**
		 * Gets the current game being used.
		 * @return Instance of the Game in use.
		 */
		Game* currentGame();

		/**
		 * Indicates if a game is running.
		 */
		bool running();

		/**
		 * Runs the current game.
		 */
		void startGameplay();

		/**
		 * Stops the current game.
		 */
		void stopGameplay();

	protected slots:

		/**
		 * Captures the indication that the game started.
		 */
		void onGameStarted();

		/**
		 * Captures the indication that the game ended.
		 * @param eReason Value of the EndReason enumeration with the reason for the
		 * game to end.
		 */
		void onGameEnded(Game::EndReason eReason);

	signals:

		/**
		 * Indicates that the gameplay started.
		 */
		void gameplayStarted();

		/**
		 * Indicates that the gameplay ended.
		 * @param eResult Value of the GameplayResult enumeration with the gameplay
		 * session result.
		 */
		void gameplayEnded(GameControl::GameplayResult eResult);

	private:

		/** Current game being used in the experiment. */
		Game *m_pCurrentGame;

		/** List of available games. */
		std::vector<Game*> m_vGames;

		/** Indicates that the game has been closed by the system instead of the user. */
		bool m_bClosedBySystem;
	};
}

#endif // GAME_CONTROL_H
