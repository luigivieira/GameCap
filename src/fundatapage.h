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

#ifndef FUN_DATA_PAGE_H
#define FUN_DATA_PAGE_H

#include "basepage.h"
#include "questionnaire.h"
#include "gameplaydata.h"

namespace gc
{
	/**
	* Page class in the wizard application used to capture fun data regarding the game.
	*/
	class FunDataPage : public BasePage
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this page.
         * The default is NULL.
         */
		FunDataPage(QWidget *pParent = NULL);

	protected:

		/**
		 * Initialization method called everytime the page is displayed.
		 */
		void initializePage();

		/**
		 * Overload the method that indicates to the main window that the questionnaire
		 * is completed (so the continue button can be displayed).
		 */
		bool isComplete() const;

	protected slots:

		/**
		 * Captures the signal indicating that the questionnaire has been completed.
		 */
		void onCompleted();

		/**
		 * Captures the signal indicating that a question in the questionnaire shown has
		 * been changed by the participant.
		 * @param iIndex Unsigned integer with the index of the question changed.
		 * @param eType Value of the Questionnaire::QuestionType enumeration with the
		 * type of the question changed.
		 * @param oValue QVariant with the new value of the question. The contents depend
		 * upon the question type. If the type is Integer, the value is an unsigned integer
		 * with the content typed by the participant on the question. If the type is String,
		 * the value is a QString with the content typed by the participant on the question.
		 * If the type is Likert, the value is an integer representing the index of the option
		 * chosen by the participant on the question.
		 */
		void onQuestionChanged(const uint iIndex, const Questionnaire::QuestionType eType, const QVariant oValue);

	private:

		/** Holds the questionnaire displayed to the user at each video tick. */
		Questionnaire *m_pQuestionnaire;

		/** Indication that the questionnaire has been completed by the subject. */
		bool m_bCompleted;

		/** Instance of the GameplayData used to store the provided answers. */
		GameplayData *m_pData;
	};
}

#endif // FUN_DATA_PAGE_H
