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

#include "volumebutton.h"
#include <QBoxLayout>
#include <QWidgetAction>
#include <QStyle>

// +-----------------------------------------------------------
gc::VolumeButton::VolumeButton(QWidget *pParent) : QToolButton(pParent)
{
	setPopupMode(QToolButton::InstantPopup);

	QWidget *pPopup = new QWidget(this);

	m_pSlider = new QSlider(Qt::Horizontal, pPopup);
	m_pSlider->setRange(0, 100);
	m_pSlider->setValue(100);
	setProperty("volume", "high");
	connect(m_pSlider, &QAbstractSlider::valueChanged, this, &VolumeButton::onSliderValueChanged);

	m_pLabel = new QLabel(m_pSlider);
	m_pLabel->setAlignment(Qt::AlignCenter);
	m_pLabel->setText("100%");
	m_pLabel->setMinimumWidth(m_pLabel->sizeHint().width());

	QBoxLayout *pPopupLayout = new QHBoxLayout(pPopup);
	pPopupLayout->setMargin(2);
	pPopupLayout->addWidget(m_pSlider);
	pPopupLayout->addWidget(m_pLabel);

	QWidgetAction *pAction = new QWidgetAction(this);
	pAction->setDefaultWidget(pPopup);

	m_pMenu = new QMenu(this);
	m_pMenu->addAction(pAction);
	setMenu(m_pMenu);
}

// +-----------------------------------------------------------
void gc::VolumeButton::onSliderValueChanged(int iValue)
{
	m_pLabel->setText(QString("%1%").arg(iValue));
	emit volumeChanged(iValue);

	QString sClass;
	if (iValue == 0)
		sClass = "mute";
	else if (iValue <= 50)
		sClass = "low";
	else
		sClass = "high";

	setProperty("volume", sClass);
	style()->unpolish(this);
	style()->polish(this);
}
