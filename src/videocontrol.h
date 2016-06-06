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

#ifndef VIDEO_CONTROL_H
#define VIDEO_CONTROL_H

#include <QProcess>
#include <QTimer>

namespace gc
{
	/**
	 * Handles the capture and exibition of the videos of the gameplay and player's face.
	 */
	class VideoControl: public QObject
	{
		Q_OBJECT
	public:
		/** Possible states of the video capturing process. */
		enum State { Starting, Started, Stopping, Stopped };
		Q_ENUM(State)

		/**
		 * Class constructor.
		 * @param pParent Instance for the QObject that is the parent of this one. Default is NULL.
		 */
		VideoControl(QObject *pParent = NULL);

		void startCapture();

		void stopCapture();

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

	private:

		/** Current state of the video capturing process. */
		State m_eState;

		bool m_aStartingFlags[2];

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

		/** Profile name used for recording the player. */
		QString m_sPlayerProfile;

		/** Collection name used for recording the player. */
		QString m_sPlayerCollection;

		/** Scene name used for recording the player. */
		QString m_sPlayerScene;
	};
}

#endif // GAME_H
