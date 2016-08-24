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

// +-----------------------------------------------------------
gc::ReviewInfoPage::ReviewInfoPage(QWidget *pParent) : BasePage(pParent)
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
void gc::ReviewInfoPage::initializePage()
{
	Game *pGame = static_cast<Application*>(qApp)->gamePlayer()->currentGame();
	QString sText = tr("\
You will now review your game session and answer a few questions about it.\
\n\
\n\
The following screen will present the recorded video of your game session, with \
the same controls you are used to find in Youtube: you can play, pause and seek \
through the video. During the playback the video will automatically stop a few \
times in the moments marked by vertical red bars, so you can answer 3 questions \
related to how you were feeling at that particular moment.\
\n\
\n\
While in playback, the Continue button of the following screen will only be enabled \
once you have answered all 3 questions for all marked moments.\
");

	m_pMessage->setText(sText);
}