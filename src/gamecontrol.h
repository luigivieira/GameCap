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
	 * Class used to allow game selection, execution and termination.
	 */
	class GameControl: public QObject
	{
		Q_OBJECT
	public:

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

	private:

		/** Current game being used in the experiment. */
		Game *m_pCurrentGame;

		/** List of available games. */
		std::vector<Game*> m_vGames;
	};
}

#endif // GAME_CONTROL_H
