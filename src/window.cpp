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
#include "gameplaypage.h"
#include "reviewinfopage.h"
#include "gamereviewpage.h"
#include "etnodatapage.h"
#include "fundatapage.h"
#include "endpage.h"
#include "application.h"
#include "messagebox.h"
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QPushButton>
#include <QKeyEvent>

 // +-----------------------------------------------------------
gc::Window::Window(QWidget *pParent) : QWizard(pParent)
{
	setWizardStyle(ModernStyle);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	
	setOption(HaveHelpButton, false);
	setOption(HaveNextButtonOnLastPage, false);
	setOption(HaveFinishButtonOnEarlyPages, false);

	button(QWizard::BackButton)->setCursor(Qt::PointingHandCursor);
	button(QWizard::NextButton)->setCursor(Qt::PointingHandCursor);
	button(QWizard::FinishButton)->setCursor(Qt::PointingHandCursor);
	button(QWizard::CancelButton)->setCursor(Qt::PointingHandCursor);

	QList<QWizard::WizardButton> lLayout;
	lLayout << QWizard::CancelButton << QWizard::Stretch << QWizard::NextButton << QWizard::FinishButton;
	setButtonLayout(lLayout);

	setStyleSheet(((Application *) qApp)->getStyleSheet());

	setPage(Page_Start, new StartPage(this));
	setPage(Page_Intro, new IntroPage(this));
	setPage(Page_GameInfo, new GameInfoPage(this));
	setPage(Page_GamePlay, new GamePlayPage(this));
	setPage(Page_ReviewInfo, new ReviewInfoPage(this));
	setPage(Page_GameReview, new GameReviewPage(this));
	setPage(Page_EtnoData, new EtnoDataPage(this));
	setPage(Page_FunData, new FunDataPage(this));
	setPage(Page_End, new EndPage(this));

	setStartId(Page_Start);

	// Connect to the application to receive notifications on language changes
	connect(qApp, SIGNAL(languageChanged(Application::Language)), this, SLOT(languageChanged(Application::Language)));
	((StartPage *)page(Page_Start))->checkLanguageButton(Application::EN_UK);

	// Capture the signal of page changed
	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(pageChanged(int)));

	// Captures the events in order to handle keypresses
	installEventFilter(this);
}

// +-----------------------------------------------------------
void gc::Window::pageChanged(int iPageID)
{
	switch (iPageID)
	{
		case Page_GamePlay:
			button(QWizard::NextButton)->setVisible(false);
			button(QWizard::NextButton)->setEnabled(false);
		case Page_Start:
			button(QWizard::CancelButton)->setVisible(false);
			button(QWizard::CancelButton)->setEnabled(false);
			break;

		default:
			button(QWizard::CancelButton)->setVisible(true);
			button(QWizard::CancelButton)->setEnabled(true);
			button(QWizard::NextButton)->setVisible(true);
			button(QWizard::NextButton)->setEnabled(true);
	}
}

// +-----------------------------------------------------------
void gc::Window::languageChanged(Application::Language eLanguage)
{
	setButtonText(QWizard::BackButton, tr("Go Back"));
	setButtonText(QWizard::NextButton, tr("Continue"));
	setButtonText(QWizard::FinishButton, tr("Conclude"));
	setButtonText(QWizard::CancelButton, tr("Quit"));
}

// +-----------------------------------------------------------
void gc::Window::reject()
{
	// Do nothing if the current page is the start
	if (currentId() == Page_Start)
		return;

	// If current page is GamePlay, check if the game is still running.
	// If it is not, then an error happened.
	if (currentId() == Page_GamePlay)
	{
		if (((Application *)qApp)->gameControl()->currentGame()->running())
			return;
		else
		{
			restart();
			return;
		}
	}

	// Confirm quiting the experiment only if not in any of the previous conditions
	if (currentId() == Page_GamePlay || MessageBox::yesNoQuestion(this, tr("Are you sure you want to quit the experiment?")))
		restart();
}

// +-----------------------------------------------------------
void gc::Window::done(int iRet)
{
	((Application*)qApp)->gameControl()->selectNextGame();
	restart();
}

// +-----------------------------------------------------------
bool gc::Window::eventFilter(QObject *pSender, QEvent *pEvent)
{
	if (pSender == this && pEvent->type() == QEvent::KeyPress)
	{
		QKeyEvent *pKeyEvent = (QKeyEvent*) pEvent;

		// The Ctrl or the Q keys clean up the hooking buffer
		if (pKeyEvent->key() == Qt::Key_Control || pKeyEvent->key() == Qt::Key_Q)
			m_sHookedKeys = "";

		// If Ctrl is pressed together with other keys, then search for
		// the "Q-U-I-T" sequence by "hooking" the last 4 pressed letters
		if (pKeyEvent->modifiers() & Qt::ControlModifier && 
			(pKeyEvent->key() >= Qt::Key_0 && pKeyEvent->key() <= Qt::Key_Z))
		{
			m_sHookedKeys += (char) pKeyEvent->key();
			if (m_sHookedKeys.length() == 4)
			{
				if (m_sHookedKeys.left(4) == "QUIT")
					Application::quit();
				else // Wrong sequence
					m_sHookedKeys = "";
			}
		}
	}
	return QWizard::eventFilter(pSender, pEvent);
}