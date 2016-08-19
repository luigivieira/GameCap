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
#include "questionnaire.h"
#include "volumebutton.h"
#include "gameplaydata.h"
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

		/**
		 * Plays the video with the given filename.
		 * @param sFileName Strig with the complete path and file name of the video to play.
		 */
		void playVideo(QString sFileName);

	signals:

		/**
		 * Signals that the review has been completed (i.e. the subject answered
		 * the questions for all samples).
		 */
		void reviewCompleted();

	protected:

		/**
		 * Shows the questionnaire and disable the controls of the video, so the subject
		 * will have to answer it to proceed on the experiment.
		 */
		void showQuestionnaire();

		/**
		 * Starts or continues playing the video of the gameplay, updating the icons on
		 * playback controls accordingly.
		 */
		void playVideo();

		/**
		 * Pauses the video of the gameplay, updating the icons on
		 * playback controls accordingly.
		 */
		void pauseVideo();

	protected slots:

		/**
		 * Captures the signal indicating the buffering status of a video being loaded.
		 * @param iPercentFilled Integer with the percentage loaded.
		 */
		void onBufferStatusChanged(int iPercentFilled);

		/**
		 * Captures the signal indicating the duration of the video loaded.
		 * @param iDuration Integer of 64 bits with the duration in milliseconds of the video loaded.
		 */
		void onDurationChanged(qint64 iDuration);

		/**
		 * Captures the signal indicating changes in the position of video playback.
		 * @param iPosition Integer of 64 bits with the current position of video in milliseconds.
		 */
		void onPositionChanged(qint64 iPosition);

		/**
		 * Captures the event of error in the media player.
		 * @param eError Value of the QMediaPlayer::Error with the error that happened.
		 */
		void onError(QMediaPlayer::Error eError);

		/**
		 * Captures the event of changes in the media status.
		 * @param eStatus Value of the enumeration QMediaPlayer::MediaStatus with the
		 * current status of the media player.
		 */
		void onMediaStatusChanged(QMediaPlayer::MediaStatus eStatus);

		/**
		 * Captures the change events on the video progress slider, as the participant
		 * or the system itself moves the slider position.
		 * @param iAction Constant indicating the type of action performed on the
		 * slider. It is not used, since all actions simply update the video current
		 * position.
		 */
		void onActionTriggered(int iAction);

		/**
		 * Captures the play/paused toggle event of the video control button.
		 */
		void onPlayPauseClicked();

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

		/**
		 * Captures the signal indicating that the questionnaire shown has been completed.
		 */
		void onQuestionnaireCompleted();

	private:

		/** Media player used to access and play the video files. */
		QMediaPlayer *m_pMediaPlayer;

		/** Video widget used to reproduce the video to the user. */
		QVideoWidget *m_pVideoWidget;

		/** Progress slider for controlling the movie playing position. */
		ProgressSlider *m_pProgressSlider;

		/** Button to control the playing/pause of the video. */
		QPushButton *m_pPlayPauseButton;

		/** Button used to control the volume of the video. */
		VolumeButton *m_pVolumeButton;

		/** Display of the video elapsed time. */
		QLabel *m_pElapsedTime;

		/** Display of the video remaining time. */
		QLabel *m_pRemainingTime;

		/** Holds the questionnaire displayed to the user at each video tick. */
		Questionnaire *m_pQuestionnaire;

		/** Ticks for the sample capture in gameplay review. */
		QVector<uint> m_vTicks;

		/** Instance of the GameplayData used to store the provided answers. */
		GameplayData *m_pData;

		/** Previous state of the video playback when the questionnaire is shown. */
		QMediaPlayer::State m_ePreviousState;
	};
}

#endif // VIDEO_REVIWER_H
