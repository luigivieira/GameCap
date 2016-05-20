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
#include <QBoxLayout>
#include <QDateTime>
#include <QMessageBox>

// +-----------------------------------------------------------
gc::GamePlayPage::GamePlayPage(QWidget *pParent) : QWizardPage(pParent)
{
	// Main page layout
	QVBoxLayout *pLayout = new QVBoxLayout(this);
	pLayout->setMargin(50);
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

}

// +-----------------------------------------------------------
void gc::GamePlayPage::onGameTime(int iSeconds)
{
	QString sTime = QDateTime::fromTime_t(iSeconds).toUTC().toString("mm:ss");
	m_pRemainingTime->setText(tr("Remaining time playing: %1").arg(sTime));
}

// +-----------------------------------------------------------
void gc::GamePlayPage::onGameEnded(gc::Game::EndReason eReason)
{
	Game *pGame = ((Application*)qApp)->gameControl()->currentGame();
	disconnect(pGame, &Game::gameRemainingTime, this, &GamePlayPage::onGameTime);

	if (eReason == Game::Concluded)
		wizard()->next();
	else
	{
		if (eReason == Game::Failed)
			QMessageBox::warning(this, tr("Game failure"), tr("The game %1 seems not to be working. Please, inform the researcher in charge.").arg(pGame->name()));
		wizard()->reject();
	}
}

// +-----------------------------------------------------------
void gc::GamePlayPage::initializePage()
{
	Game *pGame = ((Application*)qApp)->gameControl()->currentGame();
	connect(pGame, &Game::gameRemainingTime, this, &GamePlayPage::onGameTime);
	connect(pGame, &Game::gameEnded, this, &GamePlayPage::onGameEnded);
	pGame->run(10);

	QString sRemainingTime;
	m_pMessage->setText(tr("You are now playing %1. If you wish to quit the experiment, please quit from the game first.").arg(pGame->name()));
	m_pRemainingTime->setText("");
}