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

#include "gameinfopage.h"
#include <QBoxLayout>

// +-----------------------------------------------------------
gc::GameInfoPage::GameInfoPage(QWidget *pParent) : QWizardPage(pParent)
{
	// Main page layout
	QVBoxLayout *pLayout = new QVBoxLayout(this);
	pLayout->setMargin(50);

	m_pMessage = new QLabel(this);
	m_pMessage->setAlignment(Qt::AlignJustify);
	m_pMessage->setWordWrap(true);
	pLayout->addWidget(m_pMessage);
}

// +-----------------------------------------------------------
void gc::GameInfoPage::initializePage()
{
	Game *pGame = ((Application*)qApp)->gamePlayer()->currentGame();
	QString sText = tr("\
You will now play %1, a %2 game. You will have 10 minutes to play it as you wish and as many \
times as you want to. If you wish to interrupt your participation in the experiment before that \
period expires, simply quit or close the game.\
\n\
\n\
The objective of the game is %3. To play %1, do as follows: %4.\
\n\
\n\
When you are ready to start playing, click on \"Continue\".\
").arg(pGame->name()).arg(pGame->genre()).arg(pGame->goal()).arg(pGame->howToPlay());

	m_pMessage->setText(sText);
}