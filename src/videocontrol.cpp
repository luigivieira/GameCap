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

#include "videocontrol.h"
#include "application.h"
#include <QFileInfo>

#define GROUP_OBS "OBS"
#define SETTING_PATH "path"
#define SETTING_GAMEPLAYPROFILE "gameplayProfile"
#define SETTING_GAMEPLAYCOLLECTION "gameplayCollection"
#define SETTING_GAMEPLAYSCENE "gameplayScene"
#define SETTING_PLAYERPROFILE "playerProfile"
#define SETTING_PLAYERCOLLECTION "playerCollection"
#define SETTING_PLAYERSCENE "playerScene"

// +-----------------------------------------------------------
gc::VideoControl::VideoControl(QObject *pParent): QObject(pParent)
{
	m_eState = Stopped;

	// Read the OBS settings
	QSettings *pSettings = ((Application*) qApp)->getSettings();

	pSettings->beginGroup(GROUP_OBS);
	m_sOBSFileName = pSettings->value(SETTING_PATH).toString();
	m_sGameplayProfile = pSettings->value(SETTING_GAMEPLAYPROFILE).toString();
	m_sGameplayCollection = pSettings->value(SETTING_GAMEPLAYCOLLECTION).toString();
	m_sGameplayScene = pSettings->value(SETTING_GAMEPLAYSCENE).toString();
	m_sPlayerProfile = pSettings->value(SETTING_PLAYERPROFILE).toString();
	m_sPlayerCollection = pSettings->value(SETTING_PLAYERCOLLECTION).toString();
	m_sPlayerScene = pSettings->value(SETTING_PLAYERSCENE).toString();
	pSettings->endGroup();

	QString aKeys[] = { SETTING_PATH, SETTING_GAMEPLAYPROFILE,
						SETTING_GAMEPLAYCOLLECTION, SETTING_GAMEPLAYSCENE,
						SETTING_PLAYERPROFILE, SETTING_PLAYERCOLLECTION,
						SETTING_PLAYERSCENE };
	for (int i = 0; i < 7; i++)
	{
		if (!aKeys[i].length())
		{
			QString sMsg = QString("the group/key [%1/%2] is missing in the configuration file.").arg(GROUP_OBS).arg(aKeys[i]);
			qFatal(qPrintable(sMsg));
		}
		if (i > 0 && aKeys[i].contains(' '))
		{
			QString sMsg = QString("the value in group/key [%1/%2] can not contain spaces.").arg(GROUP_OBS).arg(aKeys[i]);
			qFatal(qPrintable(sMsg));
		}
	}

	QString aValues[] = { m_sGameplayProfile, m_sGameplayCollection,
						  m_sGameplayScene, m_sPlayerProfile,
						  m_sPlayerCollection, m_sPlayerScene };
	for (int i = 0; i < 6; i++)
	{
		if (aValues[i].contains(' '))
		{
			QString sMsg = QString("the value in group/key [%1/%2] can not contain spaces.").arg(GROUP_OBS).arg(aKeys[i+1]);
			qFatal(qPrintable(sMsg));
		}
	}

	if (!QFileInfo::exists(m_sOBSFileName))
	{
		QString sMsg = QString("the file name [%1] configured for the OBS applicatoin does not exist.").arg(m_sOBSFileName);
		qFatal(qPrintable(sMsg));
	}

	connect(&m_oGameplayCap, &QProcess::started, this, &VideoControl::onProcessStarted);
	connect(&m_oGameplayCap, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &VideoControl::onProcessFinished);
	connect(&m_oGameplayCap, &QProcess::errorOccurred, this, &VideoControl::onProcessError);

	qInfo("OBS configured from path [%s]", qPrintable(m_sOBSFileName));
	qInfo("Gameplay capture settings: profile [%s], collection [%s], scene [%s]", qPrintable(m_sGameplayProfile), qPrintable(m_sGameplayCollection), qPrintable(m_sGameplayScene));
	qInfo("Player capture settings: profile [%s], collection [%s], scene [%s]", qPrintable(m_sPlayerProfile), qPrintable(m_sPlayerCollection), qPrintable(m_sPlayerScene));
}

// +-----------------------------------------------------------
void gc::VideoControl::startCapture()
{
	if (m_eState == Stopped)
	{
		m_aStartingFlags[0] = m_aStartingFlags[1] = false;
		QStringList lGameplayParams;
		lGameplayParams << "--profile" << m_sGameplayProfile <<
						   "--collection" << m_sGameplayCollection <<
						   "--scene" << m_sGameplayScene <<
						   "--startrecording";
		m_oGameplayCap.setWorkingDirectory(QFileInfo(m_sOBSFileName).absolutePath());
		m_oGameplayCap.start(m_sOBSFileName, lGameplayParams);
		qInfo("starting OBS for Gameplay capture with parameters: %s", qPrintable(lGameplayParams.join(" ")));

		QStringList lPlayerParams;
		lPlayerParams << "--profile" << m_sPlayerProfile <<
						 "--collection" << m_sPlayerCollection <<
						 "--scene" << m_sPlayerScene <<
						 "--startrecording";
		m_oPlayerCap.setWorkingDirectory(QFileInfo(m_sOBSFileName).absolutePath());
		m_oPlayerCap.start(m_sOBSFileName, lPlayerParams);
		qInfo("starting OBS for Gameplay capture with parameters: %s", qPrintable(lPlayerParams.join(" ")));

		m_eState = Starting;
	}
}

// +-----------------------------------------------------------
void gc::VideoControl::stopCapture()
{
	if (m_eState == Started)
	{
		m_oGameplayCap.kill();
		m_oPlayerCap.kill();
		m_eState = Stopping;
	}
}

// +-----------------------------------------------------------
void gc::VideoControl::onProcessStarted()
{
	if (sender() == &m_oGameplayCap)
		m_aStartingFlags[0] = true;
	else
		m_aStartingFlags[1] = true;

	if (m_aStartingFlags[0] && m_aStartingFlags[1])
		m_eState = Started;
}

// +-----------------------------------------------------------
void gc::VideoControl::onProcessFinished(int iExitCode, QProcess::ExitStatus eExitStatus)
{
	if (sender() == &m_oGameplayCap)
		m_aStartingFlags[0] = false;
	else
		m_aStartingFlags[1] = false;

	if (m_aStartingFlags[0] && m_aStartingFlags[1])
		m_eState = Stopped;
}

// +-----------------------------------------------------------
void gc::VideoControl::onProcessError(QProcess::ProcessError eError)
{
	if (eError == QProcess::FailedToStart)
	{
		if (sender() == &m_oGameplayCap && m_oPlayerCap.state() == QProcess::Running)
		{
			m_eState = Stopping;
			m_oPlayerCap.kill();
		}
		else if (m_oGameplayCap.state() == QProcess::Running)
		{
			m_eState = Stopping;
			m_oGameplayCap.kill();
		}
		else
			m_eState = Stopped;
	}
}