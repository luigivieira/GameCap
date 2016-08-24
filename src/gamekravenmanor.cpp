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

#include "gamekravenmanor.h"

 // +-----------------------------------------------------------
gc::GameKravenManor::GameKravenManor(QObject *pParent): Game(pParent)
{
	setup();
}

// +-----------------------------------------------------------
QString gc::GameKravenManor::name()
{
	return "Kraven Manor";
}

// +-----------------------------------------------------------
QString gc::GameKravenManor::genre()
{
	return tr("horror");
}

// +-----------------------------------------------------------
QString gc::GameKravenManor::goal()
{
	return tr("explore the manor and discover its misteries");
}

// +-----------------------------------------------------------
QString gc::GameKravenManor::howToPlay()
{
	return tr("use the mouse to look at stuff and the keyboard keys to interact with it: W-S-A-D moves around, E activates, R throws, J opens the journal, F turns the torch on/off and Shift runs");
}
