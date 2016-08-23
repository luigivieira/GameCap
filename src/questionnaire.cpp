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
#include <QScrollArea>
#include <QStyle>

// +-----------------------------------------------------------
gc::Questionnaire::Questionnaire(QWidget *pParent): QWidget(pParent)
{
	m_pMainLayout = new QVBoxLayout(this);

	// Create the header
	QBoxLayout *pLayout = new QHBoxLayout();
	m_pMainLayout->addLayout(pLayout);

	QBoxLayout *pHeaderLayout = new QVBoxLayout();
	m_pMainLayout->addLayout(pHeaderLayout);

	m_pTitle = new QLabel(this);
	m_pTitle->setWordWrap(true);
	m_pTitle->setObjectName("questionnaireTitle");
	pHeaderLayout->addWidget(m_pTitle);

	m_pDescription = new QLabel(this);
	m_pDescription->setWordWrap(true);
	m_pDescription->setObjectName("questionnaireDescription");
	pHeaderLayout->addWidget(m_pDescription);

	// Create a separating line
	QFrame *pLine = new QFrame(this);
	pLine->setFrameStyle(QFrame::HLine | QFrame::Raised);
	pLine->setLineWidth(1);
	m_pMainLayout->addWidget(pLine);
	m_pMainLayout->addSpacing(10);

	// Create the questions area
	QScrollArea *pScrollArea = new QScrollArea(this);
	pScrollArea->setWidgetResizable(true);
	pScrollArea->setLayout(new QVBoxLayout());
	pScrollArea->layout()->setMargin(0);
	m_pMainLayout->addWidget(pScrollArea);

	m_pQuestionsArea = new QWidget(this);
	pScrollArea->setWidget(m_pQuestionsArea);
	m_pQuestionsArea->setLayout(new QVBoxLayout());
	m_pQuestionsArea->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);
	m_pQuestionsArea->layout()->setMargin(0);

	// Connections
	m_pMapper = new QSignalMapper(this);
	connect(m_pMapper, static_cast<void(QSignalMapper::*)(int)>(&QSignalMapper::mapped), this, &Questionnaire::fieldChanged);
}

// +-----------------------------------------------------------
void gc::Questionnaire::setTitle(QString sTitle)
{
	m_pTitle->setText(sTitle);
}

// +-----------------------------------------------------------
void gc::Questionnaire::setDescription(QString sDescription)
{
	m_pDescription->setText(sDescription);
}

// +-----------------------------------------------------------
uint gc::Questionnaire::getNumberOfQuestions() const
{
	return static_cast<uint>(m_vQuestionFields.size());
}

// +-----------------------------------------------------------
bool gc::Questionnaire::addQuestion(const QuestionType eType, uint iOptions)
{
	Q_ASSERT(!(eType == Likert && iOptions == 0));
	QWidget *pField = NULL;
	switch(eType)
	{
		case Integer:
			pField = new QSpinBox(this);
			static_cast<QSpinBox*>(pField)->setButtonSymbols(QAbstractSpinBox::NoButtons);
			pField->setCursor(Qt::PointingHandCursor);
			connect(static_cast<QSpinBox*>(pField), static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_pMapper, static_cast<void(QSignalMapper::*)()>(&QSignalMapper::map));
			break;

		case String:
			pField = new QLineEdit(this);
			connect(static_cast<QLineEdit*>(pField), &QLineEdit::editingFinished, m_pMapper, static_cast<void(QSignalMapper::*)()>(&QSignalMapper::map));
			break;

		case Likert:
			pField = new LikertScale(iOptions, this);
			connect(static_cast<LikertScale*>(pField), &LikertScale::selectionChanged, m_pMapper, static_cast<void(QSignalMapper::*)()>(&QSignalMapper::map));
			break;

		default:
			return false;
	}

	// Create the row area
	QWidget *pQuestionBg = new QWidget(this);
	pQuestionBg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_pQuestionsArea->layout()->addWidget(pQuestionBg);

	QBoxLayout *pQuestionLayout = new QVBoxLayout(pQuestionBg);
	pQuestionLayout->setAlignment(Qt::AlignLeft);

	// Create the question label
	QLabel *pLabel = new QLabel(this);
	pLabel->setObjectName("questionnaireQuestion");
	pLabel->setProperty("undefined", true);
	pLabel->style()->unpolish(pLabel);
	pLabel->style()->polish(pLabel);
	pQuestionLayout->addWidget(pLabel);

	// Create the question field
	QHBoxLayout *pFieldLayout = new QHBoxLayout();
	pFieldLayout->setAlignment(Qt::AlignLeft);

	pQuestionLayout->addLayout(pFieldLayout);
	pFieldLayout->addSpacing(40);
	pFieldLayout->addWidget(pField);

	// Save the question created
	m_vQuestionTypes.push_back(eType);
	m_vQuestionLabels.push_back(pLabel);
	m_vQuestionFields.push_back(pField);

	if(m_vQuestionTypes.size() % 2)
		pQuestionBg->setObjectName("highlightedRow");

	// Connections
	m_pMapper->setMapping(pField, m_vQuestionFields.length() - 1);
	return true;
}

// +-----------------------------------------------------------
void gc::Questionnaire::setQuestionTitle(const uint iIndex, const QString sTitle)
{
	Q_ASSERT(static_cast<uint>(m_vQuestionLabels.size()) >= iIndex);
	m_vQuestionLabels[iIndex]->setText(sTitle);
}

// +-----------------------------------------------------------
bool gc::Questionnaire::isCompleted() const
{
	bool bRet = true;
	for(int i = 0; i < m_vQuestionTypes.size(); i++)
	{
		Q_ASSERT(m_vQuestionTypes.size() > i);
		switch(m_vQuestionTypes[i])
		{
			case Integer:
				bRet = bRet && static_cast<QSpinBox*>(m_vQuestionFields[i])->value() != 0;
				break;

			case String:
				bRet = bRet && static_cast<QLineEdit*>(m_vQuestionFields[i])->text().length() != 0;
				break;

			case Likert:
				bRet = bRet && static_cast<LikertScale*>(m_vQuestionFields[i])->getSelected() != -1;
				break;
		}
		if(!bRet)
			break;
	}
	return bRet;
}

// +-----------------------------------------------------------
void gc::Questionnaire::fieldChanged(const uint iIndex)
{
	Q_ASSERT(static_cast<uint>(m_vQuestionTypes.size()) >= iIndex);
	QuestionType eType = m_vQuestionTypes[iIndex];

	Q_ASSERT(static_cast<uint>(m_vQuestionFields.size()) >= iIndex);
	QVariant vValue = QVariant::Invalid;
	bool bUndefined = false;
	switch(eType)
	{
		case Integer:
			vValue = static_cast<QSpinBox*>(m_vQuestionFields[iIndex])->value();
			if(vValue.toInt() <= 0)
				bUndefined = true;
			break;

		case String:
			vValue = static_cast<QLineEdit*>(m_vQuestionFields[iIndex])->text();
			if(vValue.toString().length() == 0)
				bUndefined = true;
			break;

		case Likert:
			vValue = static_cast<LikertScale*>(m_vQuestionFields[iIndex])->getSelected();
			if(vValue.toInt() == -1)
				bUndefined = true;
			break;
	}
	emit questionChanged(iIndex, eType, vValue);

	if(isCompleted())
		emit completed();

	QLabel *pLabel = m_vQuestionLabels[iIndex];
	pLabel->setProperty("undefined", bUndefined);
	pLabel->style()->unpolish(pLabel);
	pLabel->style()->polish(pLabel);
}

// +-----------------------------------------------------------
void gc::Questionnaire::setQuestionValue(const uint iIndex, QVariant oValue)
{
	Q_ASSERT(static_cast<uint>(m_vQuestionTypes.size()) >= iIndex);
	QuestionType eType = m_vQuestionTypes[iIndex];

	Q_ASSERT(static_cast<uint>(m_vQuestionFields.size()) >= iIndex);
	switch(eType)
	{
		case Integer:
			static_cast<QSpinBox*>(m_vQuestionFields[iIndex])->setValue(oValue.toInt());
			break;

		case String:
			static_cast<QLineEdit*>(m_vQuestionFields[iIndex])->setText(oValue.toString());
			break;

		case Likert:
			static_cast<LikertScale*>(m_vQuestionFields[iIndex])->setSelected(oValue.toInt());
			break;
	}
}

// +-----------------------------------------------------------
void gc::Questionnaire::setLikertOptionTitles(const uint iIndex, const QStringList &lTitles)
{
	Q_ASSERT(static_cast<uint>(m_vQuestionTypes.size()) >= iIndex);
	QuestionType eType = m_vQuestionTypes[iIndex];

	if(eType != Likert)
		return;

	LikertScale *pQuestion = static_cast<LikertScale*>(m_vQuestionFields[iIndex]);
	pQuestion->setOptionTitles(lTitles);
}