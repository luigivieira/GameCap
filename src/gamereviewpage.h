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

#ifndef GAME_REVIEW_PAGE_H
#define GAME_REVIEW_PAGE_H

#include "basepage.h"
#include "videoreviewer.h"

namespace gc
{
	/**
	* Page class in the wizard application used to review the gameplay and collect review data.
	*/
	class GameReviewPage : public BasePage
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this page.
         * The default is NULL.
         */
		GameReviewPage(QWidget *pParent = NULL);

	protected:

		/**
		 * Initialization method called everytime the page is displayed.
		 */
		void initializePage();


		/** 
		 * Validates and terminates the page, stopping and unloading the video when
		 * the subject clicks on "Continue".
		 * @return True if the process can continue, false otherwise.
		 */
		bool validatePage();

		/**
		 * Overload the method that indicates to the main window that the review
		 * is completed (so the continue button can be displayed).
		 */
		bool isComplete() const;

	protected slots:

		/**
		 * Captures the indication from the VideoReviwer that the review has been
		 * completed by the subject.
		 */
		void onReviewCompleted();

	private:

		/** Instance of the class used to allow the user reviewing the gameplay. */
		VideoReviewer *m_pReviewer;

		/** Indication that the review has been completed by the subject. */
		bool m_bCompleted;
	};
}

#endif // GAME_REVIEW_PAGE_H
