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

#include "intropage.h"

// +-----------------------------------------------------------
gc::IntroPage::IntroPage(QWidget *pParent) : BasePage(pParent)
{
	layout()->setMargin(50);

	m_pText = new QLabel(this);
	m_pText->setAlignment(Qt::AlignJustify);
	m_pText->setWordWrap(true);

	static_cast<QBoxLayout*>(layout())->addStretch();
	layout()->addWidget(m_pText);
	static_cast<QBoxLayout*>(layout())->addStretch();
}

// +-----------------------------------------------------------
void gc::IntroPage::initializePage()
{
	QString sText = tr("\
Welcome and thank you for your participation.\
\n\
\n\
This experiment will take around 20 minutes to be completed. \
You will have to play a randomly assigned digital game (10 minutes), review a video of your \
game session (5 minutes) and then answer two short questionnaires (5 minutes).\
\n\
\n\
You should have received, read and signed a term of consent explaining the details of the \
experiment and the ethics rules applicable. If you have not received it or if you still \
have doubts, then please do not proceed.\
\n\
\n\
You can quit at any time of the experiment if you no longer wish to participate, \
with no explanation or justification required. In that case, any data collected from you \
so far will be automatically removed.\
");

	m_pText->setText(sText);
}
