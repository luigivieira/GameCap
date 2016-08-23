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

#ifndef BASE_PAGE_H
#define BASE_PAGE_H

#include "window.h"
#include <QWizardPage>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace gc
{
	/**
	 * Base page class for the window pages.
	 */
	class BasePage : public QWizardPage
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this page.
         * The default is NULL.
         */
		BasePage(QWidget *pParent = NULL);

		/**
		 * Queries the pointer to the main window that contains this page.
		 * @return Instance of Window with the window that contains this page.
		 */
		Window* window() const;
	};
}

#endif // GAME_INFO_PAGE_H
