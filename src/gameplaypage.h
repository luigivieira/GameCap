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
#include "basepage.h"
#include <QLabel>

namespace gc
{
	/**
	 * Page class in the wizard application displayed while the game is played.
	 */
	class GameplayPage : public BasePage
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this page.
         * The default is NULL.
         */
		GameplayPage(QWidget *pParent = NULL);

	protected:

		/**
		 * Initialization method called everytime the page is displayed.
		 */
		void initializePage();

	protected slots:

		/**
		 * Starts the gameplay after a brief time has elapsed in initialization.
		 */
		void startGameplay();

		/**
		 * Captures the remaining gameplay time, once the game is running.
		 * @param iTimeRemaining Remaining time in seconds.
		 */
		void onGameplayTimeRemaining(unsigned int iTimeRemaining);

		/**
		 * Captures the indication that the gameplay ended successfully.
		 */
		void onGameplayCompleted();

		/**
		 * Captures the indication that the gameplay was cancelled by the user.
		 */
		void onGameplayCancelled();

		/**
		 * Captures the indication that the gameplay failed to start.
		 * @param eReason Value of the enumeration GameplayFailureReason with the reason to fail.
		 */
		void onGameplayFailed(Application::GameplayFailureReason eReason);

	private:

		/** Info message presented in this page. */
		QLabel *m_pMessage;

		/** Remaining gameplay time displayed in the page. */
		QLabel *m_pRemainingTime;
	};
}

#endif // GAME_PLAY_PAGE_H
