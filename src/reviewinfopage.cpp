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

#include "reviewinfopage.h"
#include "application.h"
#include <QBoxLayout>

// +-----------------------------------------------------------
gc::ReviewInfoPage::ReviewInfoPage(QWidget *pParent) : BasePage(pParent)
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
void gc::ReviewInfoPage::initializePage()
{
	Game *pGame = ((Application*) qApp)->gamePlayer()->currentGame();
	QString sText = tr("\
Now please review the video of the last 5 minutes of your game session while you answer a few \
questions.\
\n\
\n\
The following screen will present the video with the same controls you would find in Youtube, \
allowing you to play, pause and rewind the video. Also, as you play it, the video will \
automatically stop a few times for you to answer 3 questions related to the levels of fun, \
frustration and involvement you experienced with the game.\
\n\
\n\
When answering the questions, please consider the feelings you were having with the game at \
the moment the video paused.\
");

	m_pMessage->setText(sText);
}