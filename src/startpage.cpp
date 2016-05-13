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

#include "startpage.h"
#include "application.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QPushbutton>
#include <QButtonGroup>

// +-----------------------------------------------------------
gc::StartPage::StartPage(QWidget *pParent) : QWizardPage(pParent)
{
	// Main page layout
	QVBoxLayout *pLayout = new QVBoxLayout();
	setLayout(pLayout);

	// ----------------------------------------------
	// Create the header with the logo
	// ----------------------------------------------
	QHBoxLayout *pLogoLayout = new QHBoxLayout();
	pLayout->addLayout(pLogoLayout);

	pLogoLayout->addStretch();

	QLabel *pLogo = new QLabel();
	pLogo->setScaledContents(false);
	pLogo->setPixmap(QPixmap(":/resources/uk_logo.png"));
	pLogoLayout->addWidget(pLogo);

	pLogoLayout->addStretch();
	// ----------------------------------------------

	pLayout->addStretch();

	// ----------------------------------------------
	// Create the message header
	// ----------------------------------------------
	QHBoxLayout *pHeaderLayout = new QHBoxLayout();
	pLayout->addLayout(pHeaderLayout);

	QLabel *pLabel = new QLabel(tr("Please choose the language to be used in the experiment:"));
	pLabel->setWordWrap(true);
	pLabel->setAlignment(Qt::AlignCenter);
	pHeaderLayout->addStretch();
	pHeaderLayout->addWidget(pLabel);
	pHeaderLayout->addStretch();
	// ----------------------------------------------

	pLayout->addStretch();

	// ----------------------------------------------
	// Create the flag buttons
	// ----------------------------------------------
	QHBoxLayout *pFlagsLayout = new QHBoxLayout();
	pLayout->addLayout(pFlagsLayout);

	pFlagsLayout->addStretch();

	QPushButton *pFlagUK = new QPushButton();
	pFlagUK->setObjectName("flagUK");
	pFlagUK->setCursor(Qt::PointingHandCursor);
	pFlagUK->setCheckable(true);
	pFlagUK->setChecked(true);
	pFlagsLayout->addWidget(pFlagUK);

	pFlagsLayout->addStretch();

	QPushButton *pFlagBR = new QPushButton();
	pFlagBR->setObjectName("flagBR");
	pFlagBR->setCursor(Qt::PointingHandCursor);
	pFlagBR->setCheckable(true);
	pFlagsLayout->addWidget(pFlagBR);

	QButtonGroup *pGroup = new QButtonGroup();
	pGroup->addButton(pFlagUK, Application::Language::EN_UK);
	pGroup->addButton(pFlagBR, Application::Language::PT_BR);

	connect(pGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(languageToggled(int, bool)));

	pFlagsLayout->addStretch();
	// ----------------------------------------------

	pLayout->addStretch();
}

// +-----------------------------------------------------------
void gc::StartPage::languageToggled(int iId, bool bChecked)
{
	if (bChecked)
		((Application*)qApp)->setLanguage((Application::Language) iId);
}