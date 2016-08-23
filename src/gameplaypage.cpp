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

#include "gameplaypage.h"
#include "window.h"
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
#include "messagebox.h"

// +-----------------------------------------------------------
gc::GameplayPage::GameplayPage(QWidget *pParent) : BasePage(pParent)
{
	QBoxLayout *pLayout = static_cast<QBoxLayout*>(layout());

	pLayout->setAlignment(Qt::AlignCenter);

	pLayout->addStretch();

	m_pMessage = new QLabel(this);
	m_pMessage->setAlignment(Qt::AlignCenter);
	m_pMessage->setWordWrap(true);
	pLayout->addWidget(m_pMessage);

	pLayout->addSpacing(20);

	m_pRemainingTime = new QLabel(this);
	m_pRemainingTime->setAlignment(Qt::AlignCenter);
	m_pRemainingTime->setWordWrap(true);
	pLayout->addWidget(m_pRemainingTime);

	pLayout->addStretch();

	// Connect to the required signals
	connect(((Application*) qApp), &Application::gameplayTimeRemaining, this, &GameplayPage::onGameplayTimeRemaining);
	connect(((Application*) qApp), &Application::gameplayCompleted, this, &GameplayPage::onGameplayCompleted);
	connect(((Application*) qApp), &Application::gameplayCancelled, this, &GameplayPage::onGameplayCancelled);
	connect(((Application*) qApp), &Application::gameplayFailed, this, &GameplayPage::onGameplayFailed);
}

// +-----------------------------------------------------------
void gc::GameplayPage::initializePage()
{
	Game *pGame = ((Application*) qApp)->gamePlayer()->currentGame();
	m_pMessage->setText(tr("You are now playing %1. If you wish to quit the experiment, please quit from the game first.").arg(pGame->name()));
	m_pRemainingTime->setText("");
	
	// Starts the gameplay with a timer, in order to prevent disturbing the
	// page initialization in case of immediate errors in the startup.
	QTimer::singleShot(100, this, SLOT(startGameplay()));
}

// +-----------------------------------------------------------
void gc::GameplayPage::startGameplay()
{
	static_cast<Application*>(qApp)->startGameplay();
}

// +-----------------------------------------------------------
void gc::GameplayPage::onGameplayTimeRemaining(unsigned int iTimeRemaining)
{
	QString sTime = QDateTime::fromTime_t(iTimeRemaining).toUTC().toString("mm:ss");
	m_pRemainingTime->setText(tr("Remaining time playing: %1").arg(sTime));
}

// +-----------------------------------------------------------
void gc::GameplayPage::onGameplayCompleted()
{
	wizard()->next();
}

// +-----------------------------------------------------------
void gc::GameplayPage::onGameplayCancelled()
{
	Game *pGame = ((Application*) qApp)->gamePlayer()->currentGame();
	MessageBox::infoMessage(this, tr("You have quit the game %1 before the required play time, hence quitting the experiment. Nevertheless, thank you very much for your time.").arg(pGame->name()));
	wizard()->reject();
}

// +-----------------------------------------------------------
void gc::GameplayPage::onGameplayFailed(gc::Application::GameplayFailureReason eReason)
{
	Game *pGame = ((Application*)qApp)->gamePlayer()->currentGame();
	if(eReason == Application::FailedToStart)
		MessageBox::infoMessage(this, tr("An error ocurred and the gameplay session of the game %1 could not be started. Please, inform the researcher in charge. Nevertheless, thank you very much for your time.").arg(pGame->name()));
	else if(eReason == Application::FailedToConclude)
		MessageBox::infoMessage(this, tr("An error ocurred and the gameplay session of the game %1 could not be concluded. Please, inform the researcher in charge. Nevertheless, thank you very much for your time.").arg(pGame->name()));
	wizard()->reject();
}
