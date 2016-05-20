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

#include "gamecontrol.h"
#include "gamepingus.h"
#include "gameslender.h"
#include <QtGlobal>
#include <algorithm>

using namespace std;

// +-----------------------------------------------------------
gc::GameControl::GameControl(QObject *pParent): QObject(pParent)
{
	m_pCurrentGame = NULL; // No game initially assigned as current

	// Add the available games
	//m_vGames.push_back(new GamePingus("C:\\Program Files(x86)\\Pingus\\pingus.exe", this));
	//m_vGames.push_back(new GameSlender("C:\\Users\\Luiz\\Dropbox\\Doutorado\\Games\\Slender v0.9.7\\Slender - The Eight Pages.exe", this));

	m_vGames.push_back(new GameSlender("C:\\Windows\\notepad.exe", this));

	// Randomly permutes the games so they are assigned to new participants in a non-ordered fashion.
	random_shuffle(m_vGames.begin(), m_vGames.end());

	// Starts with a game selected
	selectNextGame();
}

// +-----------------------------------------------------------
gc::Game* gc::GameControl::selectNextGame()
{
	int iIndex;
	if (!m_pCurrentGame)
		iIndex = qrand() % m_vGames.size();
	else
	{
		vector<Game*>::iterator it = find(m_vGames.begin(), m_vGames.end(), m_pCurrentGame);
		iIndex = (it - m_vGames.begin()) + 1;
		if (iIndex >= (int) m_vGames.size())
			iIndex = 0;
	}
	m_pCurrentGame = m_vGames[iIndex];
	return m_pCurrentGame;
}

// +-----------------------------------------------------------
gc::Game* gc::GameControl::currentGame()
{
	return m_pCurrentGame;
}