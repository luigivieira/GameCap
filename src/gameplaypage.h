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

#ifndef GAME_PLAY_PAGE_H
#define GAME_PLAY_PAGE_H

#include "application.h"
#include <QWizardPage>
#include <QLabel>

namespace gc
{
	/**
	 * Page class in the wizard application displayed while the game is played.
	 */
	class GamePlayPage : public QWizardPage
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this page.
         * The default is NULL.
         */
		GamePlayPage(QWidget *pParent = NULL);

	protected slots:

		/**
		 * Captures the remaining game time, once the game is running.
		 * @param iSeconds Remaining time in seconds.
		 */
		void onGameTime(int iSeconds);

		/**
		 * Captures the indication that the game ended.
		 * @param eReason Value of the EndReason enumeration with the reason for the
		 * game to end (among Concluded, Cancelled and Failed).
		 */
		void onGameEnded(Game::EndReason eReason);

		/**
		 * Captures the timeout of the timer used to start the game outside
		 * the initialization method.
		 */
		void onTimeout();

	protected:

		/**
		 * Initialization method called everytime the page is displayed.
		 */
		void initializePage();

	private:

		/** Info message presented in this page. */
		QLabel *m_pMessage;

		/** Remaining gameplay time displayed in the page. */
		QLabel *m_pRemainingTime;
	};
}

#endif // GAME_PLAY_PAGE_H
