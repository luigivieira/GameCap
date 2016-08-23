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

#ifndef END_PAGE_H
#define END_PAGE_H

#include "basepage.h"
#include <QLabel>

namespace gc
{
	/**
	* Page class in the wizard application used to conclude the experiment.
	*/
	class EndPage : public BasePage
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this page.
         * The default is NULL.
         */
		EndPage(QWidget *pParent = NULL);

	protected:

		/**
		* Initialization method called everytime the page is displayed.
		*/
		void initializePage();

	private:

		/** Text presented in this page. */
		QLabel *m_pText;
	};
}

#endif // END_PAGE_H
