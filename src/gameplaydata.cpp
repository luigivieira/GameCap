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

// +-----------------------------------------------------------
gc::GamePlayData::GamePlayData(const uint iVideoDuration, const uint iSamples, const uint iInterval): QObject(NULL)
{
	// Initialization of the data with default values
	m_iAge = 0;
	m_eSex = Unknown;
	m_bPlayVideogames = false;
	m_eHoursPlayingVideogames = Empty;
	m_bPlayedGameBefore = false;

	uint iTimestamp = iVideoDuration;
	for(uint i = 0; i < iSamples; i++)
	{
		m_mpReviewAnswers.insert(iTimestamp, { Undefined, Undefined, Undefined });
		iTimestamp -= iInterval;
	}

	for (uint i = 0; i < GEQ_SIZE; i++)
		m_aGEQ[i] = Undefined;
}

// +-----------------------------------------------------------
uint gc::GamePlayData::GamePlayData::getAge() const
{
	return m_iAge;
}

// +-----------------------------------------------------------
void gc::GamePlayData::setAge(const uint iAge)
{
	m_iAge = iAge;
}

// +-----------------------------------------------------------
gc::GamePlayData::Sex gc::GamePlayData::getSex() const
{
	return m_eSex;
}

// +-----------------------------------------------------------
void gc::GamePlayData::setSex(const Sex eSex)
{
	m_eSex = eSex;
}

// +-----------------------------------------------------------
bool gc::GamePlayData::getPlaysVideogames() const
{
	return m_bPlayVideogames;
}

// +-----------------------------------------------------------
void gc::GamePlayData::setPlaysVideogames(const bool bPlays)
{
	m_bPlayVideogames = bPlays;
}

// +-----------------------------------------------------------
gc::GamePlayData::HoursPlayingVideogames gc::GamePlayData::getHoursPlayingVideogames() const
{
	return m_eHoursPlayingVideogames;
}

// +-----------------------------------------------------------
void gc::GamePlayData::setHoursPlayingVideogames(const HoursPlayingVideogames eHours)
{
	m_eHoursPlayingVideogames = eHours;
}

// +-----------------------------------------------------------
bool gc::GamePlayData::hasPlayedGameBefore() const
{
	return m_bPlayedGameBefore;
}

// +-----------------------------------------------------------
void gc::GamePlayData::setHasPlayedGameBefore(const bool bHasPlayed)
{
	m_bPlayedGameBefore = bHasPlayed;
}

// +-----------------------------------------------------------
gc::GamePlayData::AnswerValue gc::GamePlayData::getReviewAnswer(const gc::GamePlayData::ReviewQuestion eQuestion, const uint iTimestamp) const
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
void gc::GamePlayData::setReviewAnswer(const gc::GamePlayData::ReviewQuestion eQuestion, const uint iTimestamp, const gc::GamePlayData::AnswerValue eAnswer)
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
gc::GamePlayData::AnswerValue gc::GamePlayData::getGEQAnswer(const uint iQuestion) const
{
	if(iQuestion < GEQ_SIZE)
		return m_aGEQ[iQuestion];
	else
		return Undefined;
}

// +-----------------------------------------------------------
void gc::GamePlayData::setGEQAnswer(const int iQuestion, const gc::GamePlayData::AnswerValue eAnswer)
{
	if(iQuestion < GEQ_SIZE)
		m_aGEQ[iQuestion] = eAnswer;
}

// +-----------------------------------------------------------
bool gc::GamePlayData::save(const QString &sPath) const
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
bool gc::GamePlayData::isReviewCompleted() const
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
bool gc::GamePlayData::isGEQCompleted() const
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