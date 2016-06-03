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

#include "gameslender.h"

 // +-----------------------------------------------------------
gc::GameSlender::GameSlender(QObject *pParent): Game(pParent)
{
	setup();
}

// +-----------------------------------------------------------
QString gc::GameSlender::name()
{
	return "Slender";
}

// +-----------------------------------------------------------
QString gc::GameSlender::genre()
{
	return tr("horror");
}

// +-----------------------------------------------------------
QString gc::GameSlender::goal()
{
	return tr("to collect all 8 letters without looking at or being caught by the Slender Man");
}

// +-----------------------------------------------------------
QString gc::GameSlender::howToPlay()
{
	return tr("move around with the W-A-S-D keys (press shift to run), turn the torch on and off with the left mouse button, and collect the letters by clicking them with right mouse button");
}
