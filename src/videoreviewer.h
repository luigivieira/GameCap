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

#ifndef VIDEO_REVIWER_H
#define VIDEO_REVIWER_H

#include "progressslider.h"
#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>

namespace gc
{
	/**
	 * Page class in the wizard application used to display introductory information.
	 */
	class VideoReviewer : public QWidget
	{
		Q_OBJECT
	public:

        /**
         * Class constructor.
         * @param pParent Instance of a QWidget with the parent of this component.
         * The default is NULL.
         */
		VideoReviewer(QWidget *pParent = NULL);

		void playVideo(QString sFileName);

	protected:

	protected slots:

		void onBufferStatusChanged(int iPercentFilled);

		void onDurationChanged(qint64 iDuration);

		void onPositionChanged(qint64 iPosition);

		void onError(QMediaPlayer::Error eError);

		void onMediaStatusChanged(QMediaPlayer::MediaStatus eStatus);

		void onActionTriggered(int iAction);

		void onPlayPauseClicked();

	private:

		/** Media player used to access and play the video files. */
		QMediaPlayer *m_pMediaPlayer;

		/** Video widget used to reproduce the video to the user. */
		QVideoWidget *m_pVideoWidget;

		/** Progress slider for controlling the movie playing position. */
		ProgressSlider *m_pProgressSlider;

		/** Button to control the playing/pause of the video. */
		QPushButton *m_pPlayPauseButton;

		/** Display of the video elapsed time. */
		QLabel *m_pElapsedTime;

		/** Display of the video remaining time. */
		QLabel *m_pRemainingTime;

		/** Holds the questionnaire displayed to the user at each video tick. */
		QWidget *m_pQuestionnaire;

	};
}

#endif // VIDEO_REVIWER_H
