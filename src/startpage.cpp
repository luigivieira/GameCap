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
#include "version.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QButtonGroup>

// +-----------------------------------------------------------
gc::StartPage::StartPage(QWidget *pParent) : QWizardPage(pParent)
{
	// Main page layout
	QVBoxLayout *pLayout = new QVBoxLayout(this);
	setLayout(pLayout);

	// ----------------------------------------------
	// Create the header with the logo
	// ----------------------------------------------
	QHBoxLayout *pLogoLayout = new QHBoxLayout(this);
	pLayout->addLayout(pLogoLayout);

	pLogoLayout->addStretch();

	m_pLogo = new QLabel(this);
	m_pLogo->setScaledContents(false);
	pLogoLayout->addWidget(m_pLogo);

	pLogoLayout->addStretch();
	// ----------------------------------------------

	pLayout->addStretch();

	// ----------------------------------------------
	// Create the message header
	// ----------------------------------------------
	QHBoxLayout *pHeaderLayout = new QHBoxLayout(this);
	pLayout->addLayout(pHeaderLayout);

	m_pMessage = new QLabel(this);
	m_pMessage->setWordWrap(true);
	m_pMessage->setAlignment(Qt::AlignCenter);
	pHeaderLayout->addStretch();
	pHeaderLayout->addWidget(m_pMessage);
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
	m_aFlagButtons.append(pFlagUK);
	m_aFlagButtons.append(pFlagBR);

	connect(pGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(languageToggled(int, bool)));

	pFlagsLayout->addStretch();
	// ----------------------------------------------

	pLayout->addStretch();

	m_pInfo = new QLabel(this);
	m_pInfo->setObjectName("info");
	pLayout->addWidget(m_pInfo);

	// ----------------------------------------------
	// Other initializations
	// ----------------------------------------------
	// Connect to the application to receive notifications on language changes
	connect(qApp, SIGNAL(languageChanged(gc::Application::Language)), this, SLOT(languageChanged(gc::Application::Language)));
}

// +-----------------------------------------------------------
void gc::StartPage::checkLanguageButton(Application::Language eLanguage)
{
	QPushButton *pButton = m_aFlagButtons.at((int)eLanguage);
	if (pButton)
		pButton->setChecked(true);
}

// +-----------------------------------------------------------
void gc::StartPage::languageToggled(int iId, bool bChecked)
{
	if (bChecked)
		((Application*)qApp)->setLanguage((Application::Language) iId);
}

// +-----------------------------------------------------------
void gc::StartPage::languageChanged(gc::Application::Language eLanguage)
{
	m_pMessage->setText(tr("Please choose the language to be used in the experiment:"));
	m_pInfo->setText(tr("version %1\nflag images from %2").arg(GC_VERSION).arg("http://www.freeflagicons.com/"));
	switch (eLanguage)
	{
		case Application::Language::EN_UK:
			m_pLogo->setPixmap(QPixmap(":/resources/uk_logo.png"));
			break;

		case Application::Language::PT_BR:
			m_pLogo->setPixmap(QPixmap(":/resources/br_logo.png"));
			break;
	}
}