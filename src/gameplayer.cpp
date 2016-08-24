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
#include <ctime>
#include <QFileInfo>

#define GROUP_GAMEPLAY           "Gameplay"
#define SETTING_GAME_ORDER       "gameOrder"
#define SETTING_LAST_PLAYED_GAME "lastPlayedGame"

// +-----------------------------------------------------------
gc::GamePlayer::GamePlayer(QObject *pParent): QObject(pParent)
{
	// Create the games
	addGame(new GameCogs(this));
	addGame(new GameKravenManor(this));
	addGame(new GameMelterMan(this));

	// Create or read the order of the games
	QSettings *pSettings = static_cast<Application*>(qApp)->getSettings();

	// Read the game settings
	pSettings->beginGroup(GROUP_GAMEPLAY);
	QString sOrder = pSettings->value(SETTING_GAME_ORDER, "").toString();
	QString sLastPlayed = pSettings->value(SETTING_LAST_PLAYED_GAME, "").toString();

	m_lOrder = sOrder.split(";");
	if(sOrder.length() == 0 || m_lOrder.size() != m_mGames.size())
	{
		m_lOrder.clear();
		QMap<QString, Game*>::const_iterator it;
		for(it = m_mGames.cbegin(); it != m_mGames.cend(); ++it)
			m_lOrder.push_back(it.value()->name());

		// Randomly permutes the games so they are assigned to new participants in a random order.
		std::srand(time(NULL));
		std::random_shuffle(m_lOrder.begin(), m_lOrder.end());

		sOrder = m_lOrder.join(";");
		pSettings->setValue(SETTING_GAME_ORDER, sOrder);
	}

	pSettings->endGroup();
	
	// Select the first playable game
	int iIndex;
	if(sLastPlayed.length() != 0 && m_lOrder.contains(sLastPlayed))
	{
		iIndex = m_lOrder.indexOf(sLastPlayed) + 1;
		if(iIndex >= m_lOrder.size())
			iIndex = 0;
	}
	else
		iIndex = 0;
	m_pCurrentGame = m_mGames[m_lOrder[iIndex]];
}

// +-----------------------------------------------------------
void gc::GamePlayer::addGame(Game* pGame)
{
	connect(pGame, &Game::gameStarted, this, &GamePlayer::onGameStarted); \
	connect(pGame, &Game::gameEnded, this, &GamePlayer::onGameEnded); \
	m_mGames.insert(pGame->name(), pGame);
}

// +-----------------------------------------------------------
gc::Game* gc::GamePlayer::selectNextGame()
{
	// Save the last played game
	QSettings *pSettings = static_cast<Application*>(qApp)->getSettings();
	pSettings->beginGroup(GROUP_GAMEPLAY);
	pSettings->setValue(SETTING_LAST_PLAYED_GAME, m_pCurrentGame->name());
	pSettings->endGroup();

	// Select the next game in the order list
	int iIndex = m_lOrder.indexOf(m_pCurrentGame->name()) + 1;
	if(iIndex >= m_lOrder.size())
		iIndex = 0;
	m_pCurrentGame = m_mGames[m_lOrder[iIndex]];

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