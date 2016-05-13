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

#include <QWizardPage>
#include <QLabel>

namespace gc
{
	/**
	* Page class in the wizard application used to initiate the experiment.
	*/
	class StartPage : public QWizardPage
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this page.
         * The default is NULL.
         */
		StartPage(QWidget *pParent = NULL);

	protected slots:
	
		/**
		 * Handles the toggling of the selected language.
		 * @param iId Integer with the identifier of the language button toggled.
		 * @param bChecked Boolean with the current status of the language button.
		 */
		void languageToggled(int iId, bool bChecked);
	};
}

#endif // START_PAGE_H
