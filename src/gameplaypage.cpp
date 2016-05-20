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

// +-----------------------------------------------------------
gc::GamePlayPage::GamePlayPage(QWidget *pParent) : QWizardPage(pParent)
{
	// Main page layout
	QVBoxLayout *pLayout = new QVBoxLayout(this);
	pLayout->setMargin(50);
	pLayout->setAlignment(Qt::AlignCenter);

	m_pMessage = new QLabel(this);
	m_pMessage->setAlignment(Qt::AlignCenter);
	m_pMessage->setWordWrap(true);

	pLayout->addStretch();
	pLayout->addWidget(m_pMessage);
	pLayout->addStretch();
}

// +-----------------------------------------------------------
void gc::GamePlayPage::initializePage()
{
	Game *pGame = ((Application*)qApp)->gameControl()->currentGame();
	QString sRemainingTime;
	QString sText = tr("\
You are now playing %1. If you wish to quit the experiment,\n\
please quit from the game first.\
\n\
\n\
Remaining time playing: %2\
").arg(pGame->name()).arg(sRemainingTime);
	m_pMessage->setText(sText);
}