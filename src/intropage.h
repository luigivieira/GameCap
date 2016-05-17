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

#ifndef INTRO_PAGE_H
#define INTRO_PAGE_H

#include "application.h"
#include <QWizardPage>
#include <QLabel>

namespace gc
{
	/**
	* Page class in the wizard application used to display introductory information.
	*/
	class IntroPage : public QWizardPage
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this page.
         * The default is NULL.
         */
		IntroPage(QWidget *pParent = NULL);

	protected slots:
	
		/**
		 * Handles the notification of language changed from the application.
		 * @param eLanguage Value of the Language enumeration indicating
		 * the language that is now active.
		 */
		void languageChanged(gc::Application::Language eLanguage);

	private:

		/** Text presented in this page. */
		QLabel *m_pText;

	};
}

#endif // INTRO_PAGE_H
