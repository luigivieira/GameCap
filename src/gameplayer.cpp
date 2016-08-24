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

#include "gameplayer.h"
#include "gamekravenmanor.h"
#include "gamecogs.h"
#include "gamemelterman.h"
#include "application.h"
#include <QtGlobal>
#include <algorithm>
#include <QFileInfo>

using namespace std;

// Macro for easy repetition of required code for new game handling
#define ADD_GAME(GAMEPTR, VECTOR) connect(GAMEPTR, &Game::gameStarted, this, &GamePlayer::onGameStarted); \
				                  connect(GAMEPTR, &Game::gameEnded, this, &GamePlayer::onGameEnded); \
								  VECTOR.push_back(GAMEPTR);

// +-----------------------------------------------------------
gc::GamePlayer::GamePlayer(QObject *pParent): QObject(pParent)
{
	// Game: Cogs
	Game *pGame = new GameCogs(this);
	ADD_GAME(pGame, m_vGames)
	
	// Game: Kraven Manor
	pGame = new GameKravenManor(this);
	ADD_GAME(pGame, m_vGames)

	// Game: MelterMan
	pGame = new GameMelterMan(this);
	ADD_GAME(pGame, m_vGames)

	// Randomly permutes the games so they are assigned to new participants in a non-ordered fashion.
	random_shuffle(m_vGames.begin(), m_vGames.end());

	// Randomly select a game to start
	m_pCurrentGame = NULL;
	selectNextGame();
}

// +-----------------------------------------------------------
gc::Game* gc::GamePlayer::selectNextGame()
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
gc::Game* gc::GamePlayer::currentGame()
{
	return m_pCurrentGame;
}

// +-----------------------------------------------------------
void gc::GamePlayer::onGameStarted()
{
	emit gameplayStarted();
}

// +-----------------------------------------------------------
void gc::GamePlayer::onGameEnded(Game::EndReason eReason)
{
	if(eReason == Game::FailedToStart)
		emit gameplayEnded(Failed);
	else if(m_bClosedBySystem)
		emit gameplayEnded(ClosedBySystem);
	else
		emit gameplayEnded(ClosedByUser);
}

// +-----------------------------------------------------------
bool gc::GamePlayer::running()
{
	return m_pCurrentGame->running();
}

// +-----------------------------------------------------------
void gc::GamePlayer::startGameplay()
{
	m_bClosedBySystem = false;
	m_pCurrentGame->start();
}

// +-----------------------------------------------------------
void gc::GamePlayer::stopGameplay()
{
	m_bClosedBySystem = true;
	m_pCurrentGame->stop();
}