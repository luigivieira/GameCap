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
#include <QScreen>

// +-----------------------------------------------------------
gc::StartPage::StartPage(QWidget *pParent) : QWizardPage(pParent)
{
	QSize oMaxSize = QApplication::primaryScreen()->availableSize();

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

	QLabel *pFlagBR = new QLabel();
	pFlagBR->setScaledContents(true);
	pFlagBR->setPixmap(QPixmap(":/resources/br_flag.png"));
	pFlagsLayout->addWidget(pFlagBR);

	pFlagsLayout->addStretch();

	QLabel *pFlagUK = new QLabel();
	pFlagUK->setScaledContents(true);
	pFlagUK->setPixmap(QPixmap(":/resources/uk_flag.png"));
	pFlagsLayout->addWidget(pFlagUK);

	pFlagsLayout->addStretch();
	// ----------------------------------------------

	pLayout->addStretch();
}
