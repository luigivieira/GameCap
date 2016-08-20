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

#include <QWidget>
#include <QLabel>
#include <QRadioButton>

namespace gc
{
	/**
	 * Implements a likert-scaled question.
	 */
	class LikertScale : public QWidget
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
		 * @param iAnswers Unsigned integer with the number of answers allowed.
         * @param pParent Instance of a QWidget with the parent of this component.
         * The default is NULL.
         */
		LikertScale(const uint iAnswers, QWidget *pParent = NULL);

		/**
		 * Gets the currently selected option.
		 * @return Integer with the index of the currently selected option, or -1 if no
		 * option is selected.
		 */
		uint getSelected() const;

		/**
		 * Sets the currently selected option.
		 * @param iSelected Integer with the index of the currently selected option, or -1
		 * if no option is selected.
		 */
		void setSelected(const uint iSelected);

		/**
		 * Sets the titles for the options in the likert scale.
		 * @param lTitles QStringList with the list of titles.
		 */
		void setOptionTitles(const QStringList &lTitles);

		/**
		 * Gets the number of options in this likert scale.
		 * @return Unsigned integer with the number of configured options.
		 */
		uint getNumberOfOptions() const;

	protected slots:

		/**
		 * Captures the toggle indications from the radio buttons.
		 * @param bChecked Boolean value with the value of the radio button (checked or not).
		 */
		void onButtonToggled(bool bChecked);

	signals:

		/**
		 * Indicates that an answer has been selected/changed.
		 * @param iSelected Integer with the index of the selected option (or -1 if no
		 * option is selected).
		 */
		void selectionChanged(const uint iSelected);

	private:
		
		/** List of buttons for the options. */
		QList<QRadioButton *> m_lButtons;

		/** Currently selected option. */
		uint m_iSelected;
	};
}

#endif // VOLUME_BUTTON_H
