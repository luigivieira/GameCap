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
#include "ethnodatapage.h"
#include "fundatapage.h"
#include "ethnodatainfopage.h"
#include "fundatainfopage.h"
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

	setPage(Page_Start, new StartPage(this));
	setPage(Page_Intro, new IntroPage(this));
	setPage(Page_GameInfo, new GameInfoPage(this));
	setPage(Page_GamePlay, new GameplayPage(this));
	setPage(Page_ReviewInfo, new ReviewInfoPage(this));
	setPage(Page_GameReview, new GameReviewPage(this));
	setPage(Page_FunDataInfo, new FunDataInfoPage(this));
	setPage(Page_FunData, new FunDataPage(this));
	setPage(Page_EthnoDataInfo, new EthnoDataInfoPage(this));
	setPage(Page_EthnoData, new EthnoDataPage(this));
	setPage(Page_End, new EndPage(this));

	setStartId(Page_Start);

	// Connect to the application to receive notifications on language changes
	connect(qApp, SIGNAL(languageChanged(Application::Language)), this, SLOT(languageChanged(Application::Language)));

	// Capture the signal of page changed
	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(pageChanged(int)));

	// Captures the events in order to handle keypresses
	installEventFilter(this);

	// Simulate an event of language changed with the current language, so the wizard
	// can start with the proper language
	Application::Language eLanguage = static_cast<Application*>(qApp)->getLanguage();
	languageChanged(eLanguage);
}

// +-----------------------------------------------------------
void gc::Window::pageChanged(int iPageID)
{
	if(iPageID == -1)
		return;

	switch (iPageID)
	{
		case Page_GamePlay:
			button(QWizard::NextButton)->setVisible(false);
			button(QWizard::NextButton)->setEnabled(false);
			button(QWizard::CancelButton)->setVisible(false);
			button(QWizard::CancelButton)->setEnabled(false);
			break;

		case Page_Start:
			button(QWizard::CancelButton)->setVisible(false);
			button(QWizard::CancelButton)->setEnabled(false);
			break;

		case Page_End:
			button(QWizard::NextButton)->setVisible(false);
			button(QWizard::NextButton)->setEnabled(false);
			break;

		case Page_Intro:
			static_cast<Application*>(qApp)->newSubject();

		default:
			button(QWizard::CancelButton)->setVisible(true);
			button(QWizard::CancelButton)->setEnabled(true);
			button(QWizard::NextButton)->setVisible(currentPage()->isComplete());
			button(QWizard::NextButton)->setEnabled(currentPage()->isComplete());
	}
}

// +-----------------------------------------------------------
void gc::Window::languageChanged(Application::Language eLanguage)
{
	setButtonText(QWizard::NextButton, tr("Continue"));
	setButtonText(QWizard::FinishButton, tr("Finish"));
	setButtonText(QWizard::CancelButton, tr("Quit"));
}

// +-----------------------------------------------------------
void gc::Window::reject()
{
	switch(currentId())
	{
		// Prevent from rejecting if the current page is the start
		case Page_Start:
			return;

		// In  Gameplay, check if the game is still running.
		// If it is not, then an error happened (and a message has already
		// been displayed to the subject).
		// It it is running, prevent from rejecting.
		case Page_GamePlay:
			if(!static_cast<Application*>(qApp)->gamePlayer()->running())
			{
				static_cast<Application*>(qApp)->rejectSubject();
				restart();
			}
			return;

		// Confirm quiting the experiment only if not in any of the previous conditions
		default:
			MessageBox oBox(this);
			if(oBox.yesNoQuestion(tr("Do you confirm quitting the experiment?")))
			{
				// Stop and unload the video, if the user confirmed quitting
				if(currentId() == Page_GameReview)
					currentPage()->validatePage();

				oBox.infoMessage(tr("All your data has been removed. If you have further questions, you can ask the researcher in charge.\nThank you very much for your time."));
				static_cast<Application*>(qApp)->rejectSubject();
				restart();
			}
	}
}

// +-----------------------------------------------------------
void gc::Window::done(int iRet)
{
	static_cast<Application*>(qApp)->confirmSubject();
	restart();
}

// +-----------------------------------------------------------
bool gc::Window::eventFilter(QObject *pSender, QEvent *pEvent)
{
	if(pSender == this && pEvent->type() == QEvent::KeyPress)
	{
		QKeyEvent *pKeyEvent = (QKeyEvent*) pEvent;

		// The Ctrl or the Q keys clean up the hooking buffer
		if(pKeyEvent->key() == Qt::Key_Control || pKeyEvent->key() == Qt::Key_Q)
			m_sHookedKeys = "";

		// If Ctrl is pressed together with other keys, then search for
		// the "Q-U-I-T" sequence by "hooking" the last 4 pressed letters
		if(pKeyEvent->modifiers() & Qt::ControlModifier && 
			(pKeyEvent->key() >= Qt::Key_0 && pKeyEvent->key() <= Qt::Key_Z))
		{
			m_sHookedKeys += (char) pKeyEvent->key();
			if(m_sHookedKeys.length() == 4)
			{
				if(m_sHookedKeys.left(4) == "QUIT")
				{
					if(currentId() == Page_GameReview)
						currentPage()->validatePage();
					else if(currentId() == Page_GamePlay)
						static_cast<Application*>(qApp)->stopGameplay();

					static_cast<Application*>(qApp)->rejectSubject();
					Application::quit();
				}
				else // Wrong sequence
					m_sHookedKeys = "";
			}
		}
	}
	return QWizard::eventFilter(pSender, pEvent);
}