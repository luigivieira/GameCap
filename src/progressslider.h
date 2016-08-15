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
#include <QVector>

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
		 * @param iNumberOfTicks Integer with the number of ticks to be presented,
		 * from the end of the progress bar towards the start according to the
		 * configured interval between them.
		 * @param iTickInterval Integer with the time interval, in seconds, 
		 * between ticks. The number of ticks configured will be spaced according
		 * to this time interval.
         * @param pParent Instance of a QWidget with the parent of this component.
         * The default is NULL.
         */
		ProgressSlider(const uint iNumberOfTicks, const uint iTickInterval, QWidget *pParent = NULL);

		/**
		 * Queries the vector of all ticks in the progress of the video replay.
		 * @return QVector<uint> with the seconds of each tick in the video.
		 */
		QVector<uint> getTicks() const;

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

		/**
		 * Captures the mouse press event, so the slider can be "jump" upon
		 * the user's click on the bar.
		 * @param pEvent Instance of the QMouseEvent with the event data.
		 */
		void mousePressEvent(QMouseEvent *pEvent);

	private:

		/** Number of ticks to display. */
		uint m_iNumberOfTicks;
	};
}

#endif // PROGRESS_SLIDER_H
