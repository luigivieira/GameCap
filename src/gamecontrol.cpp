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
#include <QFileInfo>

using namespace std;

// Macro for easy repetition of required code for new game handling
#define ADD_GAME(GAMEPTR, VECTOR) connect(GAMEPTR, &Game::gameStarted, this, &GameControl::onGameStarted); \
				                  connect(GAMEPTR, &Game::gameEnded, this, &GameControl::onGameEnded); \
								  VECTOR.push_back(GAMEPTR);

// +-----------------------------------------------------------
gc::GameControl::GameControl(QObject *pParent): QObject(pParent)
{
	Game *pGame;

	// Game: Pingus
	pGame = new GamePingus(this);
	ADD_GAME(pGame, m_vGames)
	
	// Game: Slender
	pGame = new GameSlender(this);
	ADD_GAME(pGame, m_vGames)

	// Randomly permutes the games so they are assigned to new participants in a non-ordered fashion.
	random_shuffle(m_vGames.begin(), m_vGames.end());

	// Randomly select a game to start
	m_pCurrentGame = NULL;
	selectNextGame();

	// Connect to the timer used to limit the gameplay session
	connect(&m_oTimer, &QTimer::timeout, this, &GameControl::onTimeout);
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

// +-----------------------------------------------------------
void gc::GameControl::onTimeout()
{
	m_iRemainingTime--;
	if (m_iRemainingTime <= 0)
	{
		m_pCurrentGame->stop();
		m_oTimer.stop();
		m_iRemainingTime = 0;
	}
	else
		emit gameRemainingTime(m_iRemainingTime);
}

// +-----------------------------------------------------------
void gc::GameControl::onGameStarted()
{
	// Start the one second timer to control the remaining time
	m_oTimer.start(1000);
	emit gameRemainingTime(m_iRemainingTime);

	// Start the video capturing of the gameplay and player's face
	m_oVideoControl.startCapture();
}

// +-----------------------------------------------------------
void gc::GameControl::onGameEnded(Game::EndReason eReason)
{
	if (m_oTimer.isActive())
		m_oTimer.stop();
	
	if(eReason == Game::Failed)
		emit gameplayEnded(Error);
	else if (m_iRemainingTime > 0)
		emit gameplayEnded(Cancelled);
	else
		emit gameplayEnded(Success);

	m_oVideoControl.stopCapture();

	m_iRemainingTime = 0;
}

// +-----------------------------------------------------------
bool gc::GameControl::running()
{
	return m_pCurrentGame->running();
}

// +-----------------------------------------------------------
void gc::GameControl::run(int iTimeLimit)
{
	// Start the game
	m_pCurrentGame->start();
	m_iRemainingTime = iTimeLimit;
}