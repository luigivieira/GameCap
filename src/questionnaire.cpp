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

#include "questionnaire.h"
#include <QSpinBox>
#include <QLineEdit>
#include "likertscale.h"
#include <QFrame>

// +-----------------------------------------------------------
gc::Questionnaire::Questionnaire(const QString sTitle, const QString sDescription, QWidget *pParent): QWidget(pParent)
{
	m_pMainLayout = new QVBoxLayout(this);

	QBoxLayout *pLayout = new QHBoxLayout();
	m_pMainLayout->addLayout(pLayout);

	QBoxLayout *pHeaderLayout = new QVBoxLayout();
	m_pMainLayout->addLayout(pHeaderLayout);

	m_pTitle = new QLabel(sTitle, this);
	m_pTitle->setWordWrap(true);
	m_pTitle->setObjectName("questionnaireTitle");
	pHeaderLayout->addWidget(m_pTitle);

	m_pDescription = new QLabel(sDescription, this);
	m_pDescription->setWordWrap(true);
	m_pDescription->setObjectName("questionnaireDescription");
	pHeaderLayout->addWidget(m_pDescription);

	QFrame *pLine = new QFrame(this);
	pLine->setFrameStyle(QFrame::HLine | QFrame::Raised);
	pLine->setLineWidth(1);
	m_pMainLayout->addWidget(pLine);
	m_pMainLayout->addSpacing(10);
}

// +-----------------------------------------------------------
bool gc::Questionnaire::addQuestion(const QuestionType eType, const QString sTitle)
{
	QBoxLayout *pLayout = NULL;
	QWidget *pField = NULL;
	switch(eType)
	{
		case Integer:
			pLayout = new QHBoxLayout();
			pField = new QSpinBox(this);
			break;

		case String:
			pLayout = new QHBoxLayout();
			pField = new QLineEdit(this);
			break;

		case Likert:
			pLayout = new QVBoxLayout();
			pField = new LikertScale(this);
			break;

		default:
			return false;
	}
	QLabel *pLabel = new QLabel(sTitle, this);
	pLabel->setObjectName("questionnaireQuestion");
	
	pLayout->addWidget(pLabel);
	pLayout->addWidget(pField);
	m_pMainLayout->addLayout(pLayout);

	m_vQuestionTypes.push_back(eType);
	m_vQuestionLabels.push_back(pLabel);
	m_vQuestionFields.push_back(pField);

	return true;
}