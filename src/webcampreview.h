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

#ifndef WEBCAM_PREVIEW_H
#define WEBCAM_PREVIEW_H

#include "webcamwidget.h"
#include <QDialog>
#include <QCamera>

namespace gc
{
	/**
	 * Implements a preview of the webcam for adjustment purposes.
	 */
	class WebcamPreview : public QDialog
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this component.
         * The default is NULL.
         */
		WebcamPreview(QWidget *pParent = NULL);

	public slots:

		/**
		 * Overwrites original method in order to start the camera.
		 */
		void showFullScreen();

		/**
		 * Overwrites original method in order to stop the camera.
		 */
		void close();

	private:
		
		/** Camera used to capture the video input from the webcam. */
		QCamera *m_pCamera;

		/** Widget used to display the video images. */
		WebcamWidget *m_pVideoOutput;
	};
}

#endif // WEBCAM_PREVIEW_H