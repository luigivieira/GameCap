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

#ifndef WEBCAM_WIDGET_H
#define WEBCAM_WIDGET_H

#include <QCameraViewfinder>

namespace gc
{
	/**
	 * Implements a QCameraViewFinder that maintains an aspect ratio.
	 */
	class WebcamWidget : public QCameraViewfinder
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
		 * @param fAspectRatio Float with the aspect ratio to mantain. The default
		 * is 1.77 (widescreen or 16:9).
         * @param pParent Instance of a QWidget with the parent of this component.
         * The default is NULL.
         */
		WebcamWidget(float fAspectRatio = 1.77f, QWidget *pParent = NULL);

	public slots:

		/**
		 * Indicates if the widget height depends on its width.
		 */
		bool hasHeightForWidth() const;

		/**
		 * Gets the widget height based on the given width.
		 */
		int heightForWidth(int iWidth) const;

	private:
		
		/** The aspect ratio to be used. */
		float m_fAspectRatio;
	};
}

#endif // WEBCAM_WIDGET_H