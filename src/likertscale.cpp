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

#include "likertscale.h"
#include <QBoxLayout>

// +-----------------------------------------------------------
gc::LikertScale::LikertScale(QWidget *pParent) : QWidget(pParent)
{
	QVBoxLayout *pLayout = new QVBoxLayout();
	setLayout(pLayout);

	QHBoxLayout *pOptionsLayout = new QHBoxLayout();
	pLayout->addLayout(pOptionsLayout);

	m_lButtons.append(new QRadioButton(this));
	m_lButtons.append(new QRadioButton(this));
	m_lButtons.append(new QRadioButton(this));
	m_lButtons.append(new QRadioButton(this));
	m_lButtons.append(new QRadioButton(this));

	foreach(QRadioButton *pButton, m_lButtons)
	{
		pButton->setCursor(Qt::PointingHandCursor);
		pOptionsLayout->addWidget(pButton);
		connect(pButton, &QRadioButton::toggled, this, &LikertScale::onButtonToggled);
	}

	m_eSelected = GameplayData::Undefined;
	updateTranslations();
}

// +-----------------------------------------------------------
void gc::LikertScale::onButtonToggled(bool bChecked)
{
	if (bChecked)
	{
		m_eSelected = static_cast<GameplayData::AnswerValue>(m_lButtons.indexOf((QRadioButton *) sender()) - 2);
		emit answerSelected(m_eSelected);
	}
}

// +-----------------------------------------------------------
void gc::LikertScale::updateTranslations()
{
	m_lButtons[0]->setText(tr("not at all"));
	m_lButtons[1]->setText(tr("slightly"));
	m_lButtons[2]->setText(tr("moderately"));
	m_lButtons[3]->setText(tr("fairly"));
	m_lButtons[4]->setText(tr("extremely"));
}

// +-----------------------------------------------------------
gc::GameplayData::AnswerValue gc::LikertScale::getSelected() const
{
	return m_eSelected;
}

// +-----------------------------------------------------------
void gc::LikertScale::setSelected(const GameplayData::AnswerValue eSelected)
{
	foreach(QRadioButton *pButton, m_lButtons)
		pButton->blockSignals(true);

	if(eSelected == GameplayData::Undefined)
	{
		if(m_eSelected != GameplayData::Undefined)
		{
			m_lButtons[m_eSelected + 2]->setAutoExclusive(false);
			m_lButtons[m_eSelected + 2]->setChecked(false);
			m_lButtons[m_eSelected + 2]->setAutoExclusive(true);
		}
	}
	else
		m_lButtons[eSelected + 2]->setChecked(true);

	foreach(QRadioButton *pButton, m_lButtons)
		pButton->blockSignals(false);

	m_eSelected = eSelected;
}