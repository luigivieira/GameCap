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

#ifndef WINDOW_H
#define WINDOW_H

#include "application.h"
#include <QWizard>

namespace gc
{
	/**
	* Custom Wizard class for the main window in the application.
	*/
	class Window : public QWizard
	{
		Q_OBJECT
	public:

		/**
		 * Class constructor.
		 * @param pParent QWidget instance of the parent of the window. Default is NULL.
		 */
		Window(QWidget *pParent = NULL);

	protected slots:

		/**
		* Captures the notification of language changed from the application.
		* @param eLanguage Value of the Language enumeration indicating
		* the language that is now active.
		*/
		void languageChanged(Application::Language eLanguage);

		/**
		 * Captures the changes in the current page.
		 * @param iPageID Integer with the ID of the page the application is now on.
		 */
		void pageChanged(int iPageID);

		/**
		* Captures the application reject event to handle data elimination and restart.
		*/
		void reject();

		/**
		* Captures the application done event to handle data storage and restart.
		* @param iRet Integer with the return code.
		*/
		void done(int iRet);

	protected:

		/**
		 * Captures the events happening in this dialog to implement a
		 * secret close shortcut.
		 * @param pSender Instance of the QObject sending the event.
		 * @param QEvent pEvent Instance of the QEvent with the event data.
		 * @return Boolean value indicating if the event should be filtered or not.
		 */
		bool eventFilter(QObject *pSender, QEvent *pEvent);

	private:

		/** Enumerations of the page IDs. */
		enum { Page_Start, Page_Intro, Page_GameInfo, Page_GamePlay, Page_ReviewInfo, Page_GameReview, Page_EtnoData, Page_FunData, Page_End };

		/** Stores the hooked keys for the secret key combination used to close the application. */
		QString m_sHookedKeys;
	};
}

#endif // WINDOW_H
