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

#ifndef PROGRESS_SLIDER_H
#define PROGRESS_SLIDER_H

#include <QSlider>
#include <QMouseEvent>

namespace gc
{
	/**
	 * Implements a custom slider for displaying the progress of videos.
	 */
	class ProgressSlider : public QSlider
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this component.
         * The default is NULL.
         */
		ProgressSlider(QWidget *pParent = NULL);

	protected:

		/**
		 * Overloads the tickPosition setter as protected, so it can not be used
		 * by outside classes.
		 */
		void setTickPosition(TickPosition ePosition);

		/**
		 * Overloads the orientation setter as protected, so it can not be used
		 * by outside classes.
		 */
		void setOrientation(Qt::Orientation eOrientation);

		/**
		 * Captures the paint event, so the ticks can be drawn.
		 * @param pEvent Instance of the QPaintEvent with the event data.
		 */
		void paintEvent(QPaintEvent *pEvent);

		void mousePressEvent(QMouseEvent *pEvent);

	};
}

#endif // PROGRESS_SLIDER_H
