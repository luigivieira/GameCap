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
#include <QBoxLayout>

// +-----------------------------------------------------------
gc::IntroPage::IntroPage(QWidget *pParent) : QWizardPage(pParent)
{
	// Main page layout
	QVBoxLayout *pLayout = new QVBoxLayout(this);
	pLayout->setMargin(50);
	setLayout(pLayout);

	m_pText = new QLabel(this);
	m_pText->setAlignment(Qt::AlignJustify);
	m_pText->setWordWrap(true);
	pLayout->addWidget(m_pText);

	// Connect to the application to receive notifications on language changes
	connect(qApp, SIGNAL(languageChanged(gc::Application::Language)), this, SLOT(languageChanged(gc::Application::Language)));
}

// +-----------------------------------------------------------
void gc::IntroPage::languageChanged(gc::Application::Language eLanguage)
{
	QString sText = tr("\
Welcome and thank you for your help.\
\n\
\n\
You are about to join an experiment to help producing a tool to support \
the design of video games. The whole experiment will take around 20 minutes \
to complete. To participate you will have to play a game for 10 minutes, \
then review a video of the last 5 minutes of the game session you just played \
while answering 3 simple questions. Finally you will also have to answer a \
short questionnaire of multiple choices.\
\n\
\n\
You should have received, read and signed a term of consent explaining the \
details of this experiment and the ethics rules applicable. If you have not \
received this document or if you still have any doubt regarding anything, \
then please do not proceed. Also, please remember that you can quit whenever \
you want if you no longer wish to participate, with no explaining or \
justification required.\
");

	m_pText->setText(sText);
}
