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

#include "gamemelterman.h"

 // +-----------------------------------------------------------
gc::GameMelterMan::GameMelterMan(QObject *pParent): Game(pParent)
{
	setup();
}

// +-----------------------------------------------------------
QString gc::GameMelterMan::name()
{
	return "MelterMan";
}

// +-----------------------------------------------------------
QString gc::GameMelterMan::genre()
{
	return tr("platform");
}

// +-----------------------------------------------------------
QString gc::GameMelterMan::goal()
{
	return tr("reach the exit door without being killed");
}

// +-----------------------------------------------------------
QString gc::GameMelterMan::howToPlay()
{
	return tr("use the left mouse button to melt stuff in your way, the right mouse button to vacuum the melted stuff and the S keyboard key to shoot the vacuumed stuff and build new stuff");
}
