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

#include "ethnodatainfopage.h"
#include "application.h"

// +-----------------------------------------------------------
gc::EthnoDataInfoPage::EthnoDataInfoPage(QWidget *pParent) : BasePage(pParent)
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
void gc::EthnoDataInfoPage::initializePage()
{
	Game *pGame = static_cast<Application*>(qApp)->gamePlayer()->currentGame();
	QString sText = tr("\
You will now answer a questionnaire with 5 questions about yourself. \
It is an ethnographic questionnaire intended to collect your age, sex and \
preferences regarding digital games.\
\n\
\n\
The questionnaire can not be used to identify you, and it is very useful for \
the analysis of the collected data. Also, I promise it is the last thing you \
will have to do in this experiment. :)\
\n\
\n\
The Continue button of the following screen will only be enabled once you have \
answered all the 5 questions.\
");

	m_pMessage->setText(sText);
}