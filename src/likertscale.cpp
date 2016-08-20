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
gc::LikertScale::LikertScale(const uint iAnswers, QWidget *pParent) : QWidget(pParent)
{
	QVBoxLayout *pLayout = new QVBoxLayout();
	setLayout(pLayout);

	QHBoxLayout *pOptionsLayout = new QHBoxLayout();
	pOptionsLayout->setAlignment(Qt::AlignLeft);
	pLayout->addLayout(pOptionsLayout);

	for(uint i = 0; i < iAnswers; i++)
		m_lButtons.append(new QRadioButton(this));

	foreach(QRadioButton *pButton, m_lButtons)
	{
		pButton->setCursor(Qt::PointingHandCursor);
		if(pOptionsLayout->count() > 0)
			pOptionsLayout->addSpacing(60);
		pOptionsLayout->addWidget(pButton);
		connect(pButton, &QRadioButton::toggled, this, &LikertScale::onButtonToggled);
	}

	m_iSelected = -1;
}

// +-----------------------------------------------------------
void gc::LikertScale::onButtonToggled(bool bChecked)
{
	if (bChecked)
	{
		m_iSelected = m_lButtons.indexOf((QRadioButton *) sender());
		emit selectionChanged(m_iSelected);
	}
}

// +-----------------------------------------------------------
void gc::LikertScale::setOptionTitles(const QStringList &lTitles)
{
	Q_ASSERT(lTitles.size() == m_lButtons.size());
	for(int i = 0; i < m_lButtons.size(); i++)
	{
		QRadioButton *pButton = m_lButtons[i];
		pButton->setText(lTitles[i]);
	}
}

// +-----------------------------------------------------------
uint gc::LikertScale::getNumberOfOptions() const
{
	return static_cast<uint>(m_lButtons.size());
}

// +-----------------------------------------------------------
int gc::LikertScale::getSelected() const
{
	return m_iSelected;
}

// +-----------------------------------------------------------
void gc::LikertScale::setSelected(const int iSelected)
{
	Q_ASSERT(iSelected == -1 || iSelected < m_lButtons.size());
	foreach(QRadioButton *pButton, m_lButtons)
		pButton->blockSignals(true);

	if(iSelected == -1)
	{
		if(m_iSelected != -1)
		{
			m_lButtons[m_iSelected]->setAutoExclusive(false);
			m_lButtons[m_iSelected]->setChecked(false);
			m_lButtons[m_iSelected]->setAutoExclusive(true);
		}
	}
	else
		m_lButtons[iSelected]->setChecked(true);

	foreach(QRadioButton *pButton, m_lButtons)
		pButton->blockSignals(false);

	m_iSelected = iSelected;
}