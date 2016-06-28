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
#include <QRadioButton>
#include <QBoxLayout>

// +-----------------------------------------------------------
gc::LikertScale::LikertScale(QString sTitle, QWidget *pParent) : QWidget(pParent)
{
	QVBoxLayout *pLayout = new QVBoxLayout();
	setLayout(pLayout);

	m_pTitle = new QLabel(sTitle, this);
	pLayout->addWidget(m_pTitle);

	QHBoxLayout *pOptionsLayout = new QHBoxLayout();
	pLayout->addLayout(pOptionsLayout);

	QRadioButton *pButton = new QRadioButton(this);
	pButton->setText(tr("not at all"));
	pButton->setObjectName("0");
	pOptionsLayout->addWidget(pButton);

	pButton = new QRadioButton(this);
	pButton->setText(tr("slightly"));
	pButton->setObjectName("1");
	pOptionsLayout->addWidget(pButton);

	pButton = new QRadioButton(this);
	pButton->setText(tr("moderately"));
	pButton->setObjectName("2");
	pOptionsLayout->addWidget(pButton);

	pButton = new QRadioButton(this);
	pButton->setText(tr("fairly"));
	pButton->setObjectName("3");
	pOptionsLayout->addWidget(pButton);

	pButton = new QRadioButton(this);
	pButton->setText(tr("extremely"));
	pButton->setObjectName("4");
	pOptionsLayout->addWidget(pButton);
}

// +-----------------------------------------------------------
void gc::LikertScale::onButtonToggled(bool bChecked)
{
	QRadioButton *pButton = (QRadioButton *) sender();

}

