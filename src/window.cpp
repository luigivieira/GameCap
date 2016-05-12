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

#include "window.h"
#include "startpage.h"
#include "intropage.h"
#include "gameinfopage.h"
#include "reviewinfopage.h"
#include "gamereviewpage.h"
#include "etnodatapage.h"
#include "fundatapage.h"
#include "endpage.h"
#include "application.h"
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QPushButton>

 // +-----------------------------------------------------------
gc::Window::Window(QWidget *pParent) : QWizard(pParent)
{
	setWizardStyle(ModernStyle);
	setWindowState(windowState() | Qt::WindowFullScreen);
	
	setOption(HaveHelpButton, false);
	setOption(NoBackButtonOnStartPage, true);
	setOption(NoBackButtonOnLastPage, true);
	setOption(HaveNextButtonOnLastPage, false);
	setOption(HaveFinishButtonOnEarlyPages, false);

	button(QWizard::BackButton)->setCursor(Qt::PointingHandCursor);
	button(QWizard::NextButton)->setCursor(Qt::PointingHandCursor);
	button(QWizard::FinishButton)->setCursor(Qt::PointingHandCursor);
	button(QWizard::CancelButton)->setCursor(Qt::PointingHandCursor);

	setButtonText(QWizard::BackButton, tr("Go Back"));
	setButtonText(QWizard::NextButton, tr("Continue"));
	setButtonText(QWizard::FinishButton, tr("Conclude"));
	setButtonText(QWizard::CancelButton, tr("Quit"));

	QList<QWizard::WizardButton> lLayout;
	lLayout << QWizard::CancelButton << QWizard::Stretch << QWizard::BackButton << QWizard::NextButton << QWizard::FinishButton;
	setButtonLayout(lLayout);

	setStyleSheet(((Application *) qApp)->getStyleSheet());

	setPage(Page_Main, new StartPage(this));
	setPage(Page_Intro, new IntroPage(this));
	setPage(Page_GameInfo, new GameInfoPage(this));
	setPage(Page_ReviewInfo, new ReviewInfoPage(this));
	setPage(Page_GameReview, new GameReviewPage(this));
	setPage(Page_EtnoData, new EtnoDataPage(this));
	setPage(Page_FunData, new FunDataPage(this));
	setPage(Page_End, new EndPage(this));

	setStartId(Page_Main);
}