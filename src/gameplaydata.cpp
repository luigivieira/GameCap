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

#include "gameplaydata.h"
#include "likertscale.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

// +-----------------------------------------------------------
gc::GameplayData::GameplayData(QObject *pParent): QObject(pParent)
{
}

// +-----------------------------------------------------------
void gc::GameplayData::newSubject(const uint iVideoDuration, const uint iSamples, const uint iInterval)
{
	// Initialization of the data with default values
	m_iAge = 0;
	m_eSex = Unknown;
	m_bPlayVideogames = false;
	m_eHoursPlayingVideogames = Empty;
	m_bPlayedGameBefore = false;

	uint iTimestamp = iVideoDuration - 5; // Ignore the last 5 seconds of the video
	m_mpReviewAnswers.clear();
	for (uint i = 0; i < iSamples; i++)
	{
		m_mpReviewAnswers.insert(iTimestamp, { Undefined, Undefined, Undefined });
		iTimestamp -= iInterval;
	}

	for (uint i = 0; i < GEQ_SIZE; i++)
		m_aGEQ[i] = Undefined;
}

// +-----------------------------------------------------------
uint gc::GameplayData::getAge() const
{
	return m_iAge;
}

// +-----------------------------------------------------------
void gc::GameplayData::setAge(const uint iAge)
{
	m_iAge = iAge;
}

// +-----------------------------------------------------------
gc::GameplayData::Sex gc::GameplayData::getSex() const
{
	return m_eSex;
}

// +-----------------------------------------------------------
void gc::GameplayData::setSex(const Sex eSex)
{
	m_eSex = eSex;
}

// +-----------------------------------------------------------
bool gc::GameplayData::getPlaysVideogames() const
{
	return m_bPlayVideogames;
}

// +-----------------------------------------------------------
void gc::GameplayData::setPlaysVideogames(const bool bPlays)
{
	m_bPlayVideogames = bPlays;
}

// +-----------------------------------------------------------
gc::GameplayData::HoursPlayingVideogames gc::GameplayData::getHoursPlayingVideogames() const
{
	return m_eHoursPlayingVideogames;
}

// +-----------------------------------------------------------
void gc::GameplayData::setHoursPlayingVideogames(const HoursPlayingVideogames eHours)
{
	m_eHoursPlayingVideogames = eHours;
}

// +-----------------------------------------------------------
bool gc::GameplayData::hasPlayedGameBefore() const
{
	return m_bPlayedGameBefore;
}

// +-----------------------------------------------------------
void gc::GameplayData::setHasPlayedGameBefore(const bool bHasPlayed)
{
	m_bPlayedGameBefore = bHasPlayed;
}

// +-----------------------------------------------------------
gc::GameplayData::AnswerValue gc::GameplayData::getReviewAnswer(const ReviewQuestion eQuestion, const uint iTimestamp) const
{
	ReviewAnswers::const_iterator it = m_mpReviewAnswers.find(iTimestamp);
	if(it != m_mpReviewAnswers.constEnd())
	{
		switch(eQuestion)
		{
			case Frustation:
				return it.value().eFrustration;
			case Involvement:
				return it.value().eInvolvement;
			case Fun:
				return it.value().eFun;
			default:
				return Undefined;
		}
	}
	else
		return Undefined;
}

// +-----------------------------------------------------------
void gc::GameplayData::setReviewAnswer(const ReviewQuestion eQuestion, const uint iTimestamp, const AnswerValue eAnswer)
{
	ReviewAnswers::iterator it = m_mpReviewAnswers.find(iTimestamp);
	if(it != m_mpReviewAnswers.end())
	{
		switch(eQuestion)
		{
			case Frustation:
				it.value().eFrustration = eAnswer;
				break;
			case Involvement:
				it.value().eInvolvement = eAnswer;
				break;
			case Fun:
				it.value().eFun = eAnswer;
				break;
		}
	}
}

// +-----------------------------------------------------------
gc::GameplayData::AnswerValue gc::GameplayData::getGEQAnswer(const uint iQuestion) const
{
	if(iQuestion < GEQ_SIZE)
		return m_aGEQ[iQuestion];
	else
		return Undefined;
}

// +-----------------------------------------------------------
void gc::GameplayData::setGEQAnswer(const int iQuestion, const AnswerValue eAnswer)
{
	if(iQuestion < GEQ_SIZE)
		m_aGEQ[iQuestion] = eAnswer;
}

// +-----------------------------------------------------------
bool gc::GameplayData::save(const QString &sPath) const
{
	QString sSavePath;
	if(sPath.right(1) != QDir::separator())
		sSavePath = sPath + QDir::separator();
	else
		sSavePath = sPath;

	// Save the demographic information
	QString sDemFile = QString("%1demographic_answers.csv").arg(sSavePath);
	QFile oDemFile(sDemFile);
	if (!oDemFile.open(QFile::WriteOnly | QFile::Truncate))
		return false;

	QTextStream oDemOut(&oDemFile);
	oDemOut << "Age;Sex;PlayVideogames;HoursPlayingVideogames;PlayedGameBefore" << endl;
	oDemOut << m_iAge << ";" << m_eSex << ";" << m_bPlayVideogames << ";" <<
		       m_eHoursPlayingVideogames << ";" << m_bPlayedGameBefore;

	oDemFile.close();

	// Save the review answers
	QString sRevFile = QString("%1review_answers.csv").arg(sSavePath);
	QFile oRevFile(sRevFile);
	if(!oRevFile.open(QFile::WriteOnly | QFile::Truncate))
	{
		oDemFile.remove();
		return false;
	}

	QTextStream oRevOut(&oRevFile);
	oRevOut << "Seconds;Frustration;Involvement;Fun" << endl;
	ReviewAnswers::const_iterator it;
	for(it = m_mpReviewAnswers.cbegin(); it != m_mpReviewAnswers.cend(); ++it)
	{
		if(it != m_mpReviewAnswers.cbegin())
			oRevOut << endl;
		oRevOut << it.key() << ";" << it.value().eFrustration << ";" << it.value().eInvolvement << ";" << it.value().eFun;
	}

	oRevFile.close();

	// Save the GEQ answers
	QString sGEQFile = QString("%1GEQ_answers.csv").arg(sSavePath);
	QFile oGEQFile(sGEQFile);
	if(!oGEQFile.open(QFile::WriteOnly | QFile::Truncate))
	{
		oDemFile.remove();
		oRevFile.remove();
		return false;
	}

	QTextStream oGEQOut(&oGEQFile);
	oGEQOut << "Question;Answer" << endl;
	for(uint i = 0; i < GEQ_SIZE; i++)
	{
		if(i != 0)
			oGEQOut << endl;
		oGEQOut << i + 1 << ";" << m_aGEQ[i];
	}

	oGEQFile.close();

	return true;
}

// +-----------------------------------------------------------
bool gc::GameplayData::isReviewSampleCompleted(const uint iTimestamp)
{
	ReviewAnswers::const_iterator it = m_mpReviewAnswers.find(iTimestamp);
	if(it != m_mpReviewAnswers.cend())
		return it.value().eFrustration != Undefined &&
			   it.value().eInvolvement != Undefined &&
			   it.value().eFun != Undefined;
	else
		return true;
}

// +-----------------------------------------------------------
bool gc::GameplayData::isReviewCompleted() const
{
	bool bRet = true;
	ReviewAnswers::const_iterator it;
	for(it = m_mpReviewAnswers.cbegin(); it != m_mpReviewAnswers.cend(); ++it)
	{
		if(it.value().eFrustration == Undefined || it.value().eInvolvement == Undefined || it.value().eFun == Undefined)
		{
			bRet = false;
			break;
		}
	}
	return bRet;
}

// +-----------------------------------------------------------
bool gc::GameplayData::isGEQCompleted() const
{
	bool bRet = true;
	for(uint i = 0; i < GEQ_SIZE; i++)
	{
		if(m_aGEQ[i] == Undefined)
		{
			bRet = false;
			break;
		}
	}
	return bRet;
}

// +-----------------------------------------------------------
QVector<uint> gc::GameplayData::getReviewTimestamps() const
{
	QVector<uint> vRet;
	ReviewAnswers::const_iterator it;
	for(it = m_mpReviewAnswers.cbegin(); it != m_mpReviewAnswers.cend(); ++it)
		vRet.push_back(it.key());
	return vRet;
}