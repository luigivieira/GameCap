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

#include "fundatapage.h"

// +-----------------------------------------------------------
gc::FunDataPage::FunDataPage(QWidget *pParent) : BasePage(pParent)
{
	// Main page layout
	QVBoxLayout *pLayout = new QVBoxLayout(this);
	pLayout->setMargin(50);

	m_pQuestionnaire = new Questionnaire(this);
	pLayout->addWidget(m_pQuestionnaire);

	for(uint i = 0; i < GEQ_SIZE; i++)
		m_pQuestionnaire->addQuestion(Questionnaire::Likert, 5);

	connect(m_pQuestionnaire, &Questionnaire::completed, this, &FunDataPage::onCompleted);
	connect(m_pQuestionnaire, &Questionnaire::questionChanged, this, &FunDataPage::onQuestionChanged);

	m_pData = static_cast<Application*>(qApp)->getGameplayData();
}

// +-----------------------------------------------------------
void gc::FunDataPage::initializePage()
{
	Game *pGame = static_cast<Application*>(qApp)->gamePlayer()->currentGame();

	m_pQuestionnaire->setTitle(tr("Experience with the Game"));
	m_pQuestionnaire->setDescription(tr("Please answer the questions bellow, considering your whole experience playing %1 (the game you just played).").arg(pGame->name()));

	QStringList lQuestions = QStringList({
		tr("1. I felt content"),
		tr("2. I felt skilful"),
		tr("3. I was interested in the game's story"),
		tr("4. I thought it was fun"),
		tr("5. I was fully occupied with the game"),
		tr("6. I felt happy"),
		tr("7. It gave me a bad mood"),
		tr("8. I thought about other things"),
		tr("9. I found it tiresome"),
		tr("10. I felt competent"),
		tr("11. I thought it was hard"),
		tr("12. It was aesthetically pleasing"),
		tr("13. I forgot everything around me"),
		tr("14. I felt good"),
		tr("15. I was good at it"),
		tr("16. I felt bored"),
		tr("17. I felt successful"),
		tr("18. I felt imaginative"),
		tr("19. I felt that I could explore things"),
		tr("20. I enjoyed it"),
		tr("21. I was fast at reaching the game's targets"),
		tr("22. I felt annoyed"),
		tr("23. I felt pressured"),
		tr("24. I felt irritable"),
		tr("25. I lost track of time"),
		tr("27. I felt challenged"),
		tr("27. I found it impressive"),
		tr("28. I was deeply concentrated in the game"),
		tr("29. I felt frustrated"),
		tr("30. It felt like a rich experience"),
		tr("31. I lost connection with the outside world"),
		tr("32. I felt time pressure"),
		tr("33. I had to put a lot of effort into it")
	});

	QStringList lOptions = {
		tr("not at all"),
		tr("slightly"),
		tr("moderately"),
		tr("fairly"),
		tr("extremely")
	};

	for(uint i = 0; i < GEQ_SIZE; i++)
	{
		m_pQuestionnaire->setQuestionTitle(i, lQuestions[i]);
		m_pQuestionnaire->setLikertOptionTitles(i, lOptions);
	}

	m_bCompleted = false;
	emit completeChanged();
}

// +-----------------------------------------------------------
bool gc::FunDataPage::isComplete() const
{
	return m_bCompleted;
}

// +-----------------------------------------------------------
void gc::FunDataPage::onCompleted()
{
	m_bCompleted = true;
	emit completeChanged();
}

// +-----------------------------------------------------------
void gc::FunDataPage::onQuestionChanged(const uint iIndex, const Questionnaire::QuestionType eType, const QVariant oValue)
{
	m_pData->setGEQAnswer(iIndex, static_cast<GameplayData::AnswerValue>(oValue.toInt() + 2));
}