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

#ifndef LIKERT_SCALE_H
#define LIKERT_SCALE_H

#include "gameplaydata.h"
#include <QWidget>
#include <QLabel>
#include <QRadioButton>

namespace gc
{
	/**
	 * Implements a 5-values likert-scaled question.
	 */
	class LikertScale : public QWidget
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this component.
         * The default is NULL.
         */
		LikertScale(QWidget *pParent = NULL);

		/**
		 * Gets the currently selected option.
		 * @return Value of the GameplayData::AnswerValue enumeration with the selected option.
		 */
		GameplayData::AnswerValue getSelected() const;

		/**
		 * Sets the currently selected option.
		 * @param eSelected Value of the GameplayData::AnswerValue enumeration with the selected option.
		 */
		void setSelected(const GameplayData::AnswerValue eSelected);

		/**
		 * Updates the strings due to locale changes.
		 */
		void updateTranslations();

	protected slots:

		/**
		 * Captures the toggle indications from the radio buttons.
		 * @param bChecked Boolean value with the value of the radio button (checked or not).
		 */
		void onButtonToggled(bool bChecked);

	signals:

		/**
		 * Indicates that an answer has been selected/changed.
		 * @param eSelected Value of the GameplayData::AnswerValue enumeration with the selected option.
		 */
		void answerSelected(const GameplayData::AnswerValue eSelected);

	private:
		
		/** List of buttons for the options. */
		QList<QRadioButton *> m_lButtons;

		/** Currently selected option. */
		GameplayData::AnswerValue m_eSelected;
	};
}

#endif // VOLUME_BUTTON_H
