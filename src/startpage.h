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

#ifndef START_PAGE_H
#define START_PAGE_H

#include "application.h"
#include "basepage.h"
#include <QLabel>
#include <QPushButton>
#include <QList>

namespace gc
{
	/**
	* Page class in the wizard application used to initiate the experiment.
	*/
	class StartPage : public BasePage
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this page.
         * The default is NULL.
         */
		StartPage(QWidget *pParent = NULL);

		/**
		 * Set the button corresponding for the given language to be checked.
		 * This automatically triggers the application delivery of the language changed signal.
		 * @param eLanguage Enumeration value with the language of the button to be checked.
		 */
		void checkLanguageButton(Application::Language eLanguage);
	
	protected:

		/**
		 * Initialization method called everytime the page is displayed.
		 */
		void initializePage();

	protected slots:
	
		/**
		 * Handles the toggling of the selected language through the page buttons.
		 * @param iId Integer with the identifier of the language button toggled.
		 * @param bChecked Boolean with the current status of the language button.
		 */
		void languageToggled(int iId, bool bChecked);

		/**
		 * Handles the notification of language changed from the application.
		 * @param eLanguage Value of the Language enumeration indicating
		 * the language that is now active.
		 */
		void languageChanged(Application::Language eLanguage);

	private:

		/** Image with the university logo. */
		QLabel *m_pLogo;

		/** Message about selecting the language. */
		QLabel *m_pMessage;

		/** Version and subject number information. */
		QLabel *m_pInfo;

		/**
		 * List of pointers to the language buttons, stored in the same order of the
		 * Application::Language enumeration.
		 */
		QList<QPushButton *> m_aFlagButtons;
	};
}

#endif // START_PAGE_H
