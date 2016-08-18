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

#ifndef VIDEO_CAPTURER_H
#define VIDEO_CAPTURER_H

#include <QProcess>

namespace gc
{
	/**
	 * Handles the recording of the videos of the gameplay and player's face.
	 */
	class VideoCapturer: public QObject
	{
		Q_OBJECT
	public:
		/** Possible states of the video capturing process. */
		enum State { Starting, Started, Stopping, Stopped };
		Q_ENUM(State)

		/** Enumeration with the possible results for the video capture. */
		enum CaptureResult { FailedToStart, FailedToSave, Closed };
		Q_ENUM(CaptureResult)

		/**
		 * Class constructor.
		 * @param pParent Instance for the QObject that is the parent of this one. Default is NULL.
		 */
		VideoCapturer(QObject *pParent = NULL);

		/**
		 * Starts the capture of the gameplay and player's face videos.
		 * @param sGameplayTargetName Name of the file to save the gameplayer video file.
		 * @param sPlayerTargetName Name of the file to save the player's face video file.
		 */
		void startCapture(QString sGameplayTargetName, QString sPlayerTargetName);

		/**
		 * Stops the capture of the gameplay and player's face videos.
		 */
		void stopCapture();

		/**
		 * Get the list of video files created by the video capturer.
		 * @return QStringList with the list of files created. The files are given
		 * with the complete path + name + extension.
		 */
		QStringList getCapturedVideoFiles() const;

	protected slots:

		/**
		 * Captures the signal of process started.
		 */
		void onProcessStarted();

		/**
		 * Captures the signal of process finished.
		 * @param iExitCode Integer with the exit code given by the process.
		 * @param eExitStatus QProcess::ExitStatus enumeration with the status of the
		 * application when it exited (either normal or crashed).
		 */
		void onProcessFinished(int iExitCode, QProcess::ExitStatus eExitStatus);

		/**
		 * Captures the signal of process error.
		 * @param eError QProcess::ProcessError enumeration with the reason for the error.
		 */
		void onProcessError(QProcess::ProcessError eError);

	signals:

		/**
		 * Signal indicating that the capture started.
		 */
		void captureStarted();

		/**
		 * Signal indicating that the capture ended.
		 * @param eResult Value of the CaptureResult enumeration with the video
		 * capture result.
		 */
		void captureEnded(VideoCapturer::CaptureResult eResult);

	protected:

		/**
		 * Deletes all video files in the configured OBS output directories.
		 */
		void deleteFiles();

		/**
		 * Save the video files recorded to the path and file names provided.
		 */
		bool saveFiles();

	private:

		/** Current state of the video capturing process. */
		State m_eState;

		/** Individual indications of process started for the two instances of OBS. */
		bool m_aStartedFlags[2];

		/**
		 * Indication that the fail to start signal has been just sent.
		 * This is used because when one instance of the OBS software fails to start
		 * it is very likely that the other one will also fail to start. So, by using
		 * this flag, the code can avoid emitting two signals in a row.
		 */
		bool m_bFailSignalSent;

		/**
		 * Indicates that one or two of the OBS instances crashed after they started
		 * capturing the videos.
		 */
		bool m_bCrashedDuringExecution;

		/** Name of the file to save the gameplayer video file. */
		QString m_sGameplayTargetName;

		/** Name of the file to save the player's face video file. */
		QString m_sPlayerTargetName;

		/** Handles the OBS process used to capture the gameplay video. */
		QProcess m_oGameplayCap;

		/** Handles the OBS process used to capture the player video. */
		QProcess m_oPlayerCap;

		/** Path and file name of the OBS executable (read from the configuration). */
		QString m_sOBSFileName;

		/** Profile name used for recording the gameplay. */
		QString m_sGameplayProfile;

		/** Collection name used for recording the gameplay. */
		QString m_sGameplayCollection;

		/** Scene name used for recording the gameplay. */
		QString m_sGameplayScene;

		/** Path where OBS will store the gameplay videos captured. */
		QString m_sGameplayPath;

		/** Profile name used for recording the player. */
		QString m_sPlayerProfile;

		/** Collection name used for recording the player. */
		QString m_sPlayerCollection;

		/** Scene name used for recording the player. */
		QString m_sPlayerScene;

		/** Path where OBS will store the player's face videos captured. */
		QString m_sPlayerPath;
	};
}

#endif // VIDEO_CAPTURER_H
