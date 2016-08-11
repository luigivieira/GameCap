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

#include "videocapturer.h"
#include "application.h"
#include <QFileInfo>
#include <vector>
#include <QDir>
#include <QFile>

#define GROUP_OBS "OBS"
#define SETTING_PATH "path"
#define SETTING_GAMEPLAYPROFILE "gameplayProfile"
#define SETTING_GAMEPLAYCOLLECTION "gameplayCollection"
#define SETTING_GAMEPLAYSCENE "gameplayScene"
#define SETTING_GAMEPLAYPATH "gameplayPath"
#define SETTING_PLAYERPROFILE "playerProfile"
#define SETTING_PLAYERCOLLECTION "playerCollection"
#define SETTING_PLAYERSCENE "playerScene"
#define SETTING_PLAYERPATH "playerPath"

using namespace std;

// +-----------------------------------------------------------
gc::VideoCapturer::VideoCapturer(QObject *pParent): QObject(pParent)
{
	m_eState = Stopped;

	// Read the OBS settings
	QSettings *pSettings = ((Application*) qApp)->getSettings();

	pSettings->beginGroup(GROUP_OBS);
	m_sOBSFileName = pSettings->value(SETTING_PATH).toString();
	m_sGameplayProfile = pSettings->value(SETTING_GAMEPLAYPROFILE).toString();
	m_sGameplayCollection = pSettings->value(SETTING_GAMEPLAYCOLLECTION).toString();
	m_sGameplayScene = pSettings->value(SETTING_GAMEPLAYSCENE).toString();
	m_sGameplayPath = pSettings->value(SETTING_GAMEPLAYPATH).toString();

	m_sPlayerProfile = pSettings->value(SETTING_PLAYERPROFILE).toString();
	m_sPlayerCollection = pSettings->value(SETTING_PLAYERCOLLECTION).toString();
	m_sPlayerScene = pSettings->value(SETTING_PLAYERSCENE).toString();
	m_sPlayerPath = pSettings->value(SETTING_PLAYERPATH).toString();
	pSettings->endGroup();

	vector<QString> vKeys = { SETTING_PATH,
							  SETTING_GAMEPLAYPROFILE, SETTING_GAMEPLAYCOLLECTION,
							  SETTING_GAMEPLAYSCENE, SETTING_GAMEPLAYPATH,
							  SETTING_PLAYERPROFILE, SETTING_PLAYERCOLLECTION,
							  SETTING_PLAYERSCENE, SETTING_PLAYERPATH };
	for (vector<QString>::iterator it = vKeys.begin(); it != vKeys.end(); ++it)
	{
		if (!it->length())
			qFatal("The group/key [%s/%s] is missing in the configuration file.", qPrintable(GROUP_OBS), qPrintable(*it));
	}

	vector<QString> vValues = { m_sGameplayProfile, m_sGameplayCollection,
								m_sGameplayScene, m_sPlayerProfile,
								m_sPlayerCollection, m_sPlayerScene };
	for (vector<QString>::iterator it = vValues.begin(); it != vValues.end(); ++it)
	{
		if (it->contains(' '))
			qFatal("The value [%s] (used in the group [%s] of the configuration) can not contain spaces.", qPrintable(*it), qPrintable(GROUP_OBS));
	}

	if (!QFileInfo::exists(m_sOBSFileName))
		qFatal("The file name [%s] (used in the group [%s] of the configuration) does not exist.", qPrintable(m_sOBSFileName), qPrintable(GROUP_OBS));

	connect(&m_oGameplayCap, &QProcess::started, this, &VideoCapturer::onProcessStarted);
	connect(&m_oGameplayCap, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &VideoCapturer::onProcessFinished);
	connect(&m_oGameplayCap, &QProcess::errorOccurred, this, &VideoCapturer::onProcessError);

	connect(&m_oPlayerCap, &QProcess::started, this, &VideoCapturer::onProcessStarted);
	connect(&m_oPlayerCap, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &VideoCapturer::onProcessFinished);
	connect(&m_oPlayerCap, &QProcess::errorOccurred, this, &VideoCapturer::onProcessError);

	qDebug("OBS configured from path [%s]", qPrintable(m_sOBSFileName));
	qDebug("Gameplay capture settings: profile [%s], collection [%s], scene [%s]", qPrintable(m_sGameplayProfile), qPrintable(m_sGameplayCollection), qPrintable(m_sGameplayScene));
	qDebug("Player capture settings: profile [%s], collection [%s], scene [%s]", qPrintable(m_sPlayerProfile), qPrintable(m_sPlayerCollection), qPrintable(m_sPlayerScene));
}

// +-----------------------------------------------------------
void gc::VideoCapturer::startCapture(QString sGameplayTargetName, QString sPlayerTargetName)
{
	if(m_eState == Stopped)
	{
		m_sGameplayTargetName = sGameplayTargetName;
		m_sPlayerTargetName = sPlayerTargetName;
		deleteFiles();

		m_bFailSignalSent = false;
		m_eState = Starting;

		m_aStartedFlags[0] = m_aStartedFlags[1] = false;
		QStringList lGameplayParams;
		lGameplayParams << "--profile" << m_sGameplayProfile <<
						   "--collection" << m_sGameplayCollection <<
						   "--scene" << m_sGameplayScene <<
						   "--startrecording";
		m_oGameplayCap.setWorkingDirectory(QFileInfo(m_sOBSFileName).absolutePath());
		m_oGameplayCap.start(m_sOBSFileName, lGameplayParams);

		QStringList lPlayerParams;
		lPlayerParams << "--profile" << m_sPlayerProfile <<
						 "--collection" << m_sPlayerCollection <<
						 "--scene" << m_sPlayerScene <<
						 "--startrecording";
		m_oPlayerCap.setWorkingDirectory(QFileInfo(m_sOBSFileName).absolutePath());
		m_oPlayerCap.start(m_sOBSFileName, lPlayerParams);
	}
}

// +-----------------------------------------------------------
void gc::VideoCapturer::stopCapture()
{
	if (m_eState == Started)
	{
		m_oGameplayCap.kill();
		m_oPlayerCap.kill();
		m_eState = Stopping;
	}
}

// +-----------------------------------------------------------
void gc::VideoCapturer::onProcessStarted()
{
	QProcess *pSender = qobject_cast<QProcess*>(sender());

	if (pSender == &m_oGameplayCap)
		m_aStartedFlags[0] = true;
	else
		m_aStartedFlags[1] = true;

	if (m_aStartedFlags[0] && m_aStartedFlags[1])
	{
		qDebug() << "Video recording started.";
		m_eState = Started;
		emit captureStarted();
	}
}

// +-----------------------------------------------------------
void gc::VideoCapturer::onProcessFinished(int iExitCode, QProcess::ExitStatus eExitStatus)
{
	if (sender() == &m_oGameplayCap)
	{
		qDebug("OBS for gameplay capture ended with exit code [%d] and exit status [%s]", iExitCode, (eExitStatus == QProcess::NormalExit ? "normal" : "closed"));
		m_aStartedFlags[0] = false;
	}
	else
	{
		qDebug("OBS for player capture ended with exit code [%d] and exit status [%s]", iExitCode, (eExitStatus == QProcess::NormalExit ? "normal" : "closed"));
		m_aStartedFlags[1] = false;
	}

	if (!m_aStartedFlags[0] && !m_aStartedFlags[1])
	{
		qDebug() << "Video recording ended.";
		m_eState = Stopped;
		if (!m_bFailSignalSent)
		{
			if(saveFiles())
				emit captureEnded(Closed);
			else
			{
				qWarning() << "Could not save video files recorded.";
				deleteFiles();
				emit captureEnded(FailedToSave);
			}
		}
	}
}

// +-----------------------------------------------------------
void gc::VideoCapturer::onProcessError(QProcess::ProcessError eError)
{
	if (eError == QProcess::FailedToStart)
	{
		if (sender() == &m_oGameplayCap)
		{
			qWarning() << "OBS for gameplay capture failed to start.";
			m_eState = Stopping;
			if (m_oPlayerCap.state() == QProcess::Running)
				m_oPlayerCap.kill();
			if (!m_bFailSignalSent)
			{
				deleteFiles();
				m_bFailSignalSent = true;
				emit captureEnded(FailedToStart);
			}
		}
		else
		{
			qWarning() << "OBS for player's face capture failed to start.";
			m_eState = Stopping;
			if (m_oGameplayCap.state() == QProcess::Running)
				m_oGameplayCap.kill();

			if (!m_bFailSignalSent)
			{
				deleteFiles();
				m_bFailSignalSent = true;
				emit captureEnded(FailedToStart);
			}
		}
	}
}

// +-----------------------------------------------------------
void gc::VideoCapturer::deleteFiles()
{
	QDir oGameplayDir(m_sGameplayPath);
	oGameplayDir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	foreach(QString sFile, oGameplayDir.entryList())
		oGameplayDir.remove(sFile);

	QDir oPlayerDir(m_sPlayerPath);
	oPlayerDir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	foreach(QString sFile, oPlayerDir.entryList())
		oPlayerDir.remove(sFile);
}

// +-----------------------------------------------------------
bool gc::VideoCapturer::saveFiles()
{
	// First, get the source file
	QDir oGameplayDir(m_sGameplayPath);
	oGameplayDir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	if (oGameplayDir.entryList().size() <= 0) // sanity check
		return false;
	QString sGameplaySourceName = oGameplayDir.entryInfoList().at(0).absoluteFilePath();
	QFile oGameplaySourceFile(sGameplaySourceName);
	if (!oGameplaySourceFile.exists())
		return false;
	
	QDir oPlayerDir(m_sPlayerPath);
	oPlayerDir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	if (oPlayerDir.entryList().size() <= 0) // sanity check
		return false;
	QString sPlayerSourceName = oPlayerDir.entryInfoList().at(0).absoluteFilePath();
	QFile oPlayerSourceFile(sPlayerSourceName);
	if (!oPlayerSourceFile.exists())
		return false;

	// Then, remove the target files
	// (just in case they exist - the copy fails if they do)
	QFile::remove(m_sGameplayTargetName);
	QFile::remove(m_sPlayerTargetName);

	// Then, copy the recorded files with the target names
	bool bOk = oGameplaySourceFile.copy(m_sGameplayTargetName) &&
	           oPlayerSourceFile.copy(m_sPlayerTargetName);

	// Finally, remove the source files if the copy succeeded
	if (!bOk)
	{
		QFile::remove(m_sGameplayTargetName);
		QFile::remove(m_sPlayerTargetName);
		return false;
	}
	else
	{
		QFile::remove(sGameplaySourceName);
		QFile::remove(sPlayerSourceName);
		return true;
	}
}