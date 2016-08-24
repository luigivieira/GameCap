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

#include "gamecogs.h"

 // +-----------------------------------------------------------
gc::GameCogs::GameCogs(QObject *pParent): Game(pParent)
{
	setup();
}

// +-----------------------------------------------------------
QString gc::GameCogs::name()
{
	return "Cogs";
}

// +-----------------------------------------------------------
QString gc::GameCogs::genre()
{
	return tr("puzzle");
}

// +-----------------------------------------------------------
QString gc::GameCogs::goal()
{
	return tr("to take all the penguins to safety");
}

// +-----------------------------------------------------------
QString gc::GameCogs::howToPlay()
{
	return tr("with the right button of the mouse, select an ability and then select a penguin to do it");
}
