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

#include "etnodatapage.h"

// +-----------------------------------------------------------
gc::EtnoDataPage::EtnoDataPage(QWidget *pParent) : BasePage(pParent)
{
	// Main page layout
	QVBoxLayout *pLayout = new QVBoxLayout(this);
	pLayout->setMargin(50);

	m_pQuestionnaire = new Questionnaire(this);
	pLayout->addWidget(m_pQuestionnaire);

	m_pQuestionnaire->addQuestion(Questionnaire::Integer); // Age
	m_pQuestionnaire->addQuestion(Questionnaire::Likert, 2); // Sex
	m_pQuestionnaire->addQuestion(Questionnaire::Likert, 2); // Play games
	m_pQuestionnaire->addQuestion(Questionnaire::Likert, 4); // Hours per week playing
	m_pQuestionnaire->addQuestion(Questionnaire::Likert, 2); // Playged game before
}

// +-----------------------------------------------------------
void gc::EtnoDataPage::initializePage()
{
	Game *pGame = static_cast<Application*>(qApp)->gamePlayer()->currentGame();

	m_pQuestionnaire->setTitle(tr("Etnographic Information"));
	m_pQuestionnaire->setDescription(tr("Please answer the questions bellow. They won't allow to identify you, but are useful to help with the analysis of the collected data."));

	m_pQuestionnaire->setQuestionTitle(0, tr("How old are you?"));

	m_pQuestionnaire->setQuestionTitle(1, tr("What is your sex?"));
	m_pQuestionnaire->setLikertOptionTitles(1, QStringList({ tr("Male"), tr("Female") }));

	m_pQuestionnaire->setQuestionTitle(2, tr("Do you usually play digital games?"));
	m_pQuestionnaire->setLikertOptionTitles(2, QStringList({ tr("Yes"), tr("No") }));
	
	m_pQuestionnaire->setQuestionTitle(3, tr("How many hours per week do you spend playing digital games?"));
	m_pQuestionnaire->setLikertOptionTitles(3, QStringList({ tr("0-2 hours"), tr("2-5 hours"), tr("5-10 hours"), tr("10+ hours") }));

	m_pQuestionnaire->setQuestionTitle(4, tr("Have you played %1 (the game you just played) before?").arg(pGame->name()));
	m_pQuestionnaire->setLikertOptionTitles(4, QStringList({ tr("Yes"), tr("No") }));
}
