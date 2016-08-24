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

#include "fundatainfopage.h"
#include "application.h"

// +-----------------------------------------------------------
gc::FunDataInfoPage::FunDataInfoPage(QWidget *pParent) : BasePage(pParent)
{
	layout()->setMargin(50);

	m_pMessage = new QLabel(this);
	m_pMessage->setAlignment(Qt::AlignJustify);
	m_pMessage->setWordWrap(true);

	static_cast<QBoxLayout*>(layout())->addStretch();
	layout()->addWidget(m_pMessage);
	static_cast<QBoxLayout*>(layout())->addStretch();
}

// +-----------------------------------------------------------
void gc::FunDataInfoPage::initializePage()
{
	Game *pGame = static_cast<Application*>(qApp)->gamePlayer()->currentGame();
	QString sText = tr("\
You will now answer a questionnaire with 33 questions about your experience \
with the game. The questions are all of multiple choice, in the same format \
of the questionnaire you answered during the game session review. \
\n\
\n\
However, please notice that differently than that review, now you will be \
asked on your opinion about the whole game session, and not about a specific \
moment in time.\
\n\
\n\
The Continue button of the following screen will only be enabled once you have \
answered all the 33 questions.\
");

	m_pMessage->setText(sText);
}