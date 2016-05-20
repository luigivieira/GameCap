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

#ifndef GAME_SLENDER_H
#define GAME_SLENDER_H

#include "game.h"

namespace gc
{
	/**
	 * Class used to access the game Slender.
	 */
	class GameSlender: public Game
	{
		Q_OBJECT
	
	public:

		/**
		 * Class constructor.
		 * @param sExecutable QString with the path and filename of the game's executable.
		 * @param pParent Instance for the QObject that is the parent of this one. Default is NULL.
		 */
		GameSlender(QString sExecutable, QObject *pParent = NULL);

		/**
		 * Queries the name of the game.
		 * @return String with the game's name.
		 */
		virtual QString name();

		/**
		 * Queries the genre of the game.
		 * @return String with the game's genre.
		 */
		virtual QString genre();

		/**
		 * Queries the goal of the game.
		 * @return String with the game's goal.
		 */
		virtual QString goal();

		/**
		 * Queries the help on how to play game.
		 * @return String with the game's help on how to play.
		 */
		virtual QString howToPlay();
	};
}

#endif // GAME_SLENDER_H