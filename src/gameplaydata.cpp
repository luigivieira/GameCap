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
	// Initialize the data structures used for storage
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
bool gc::GamePlayData::save(const QString &sPath)
{
	QString sSavePath;
	if(sPath.right(1) != QDir::separator())
		sSavePath = sPath + QDir::separator();
	else
		sSavePath = sPath;

	// Save the review answers
	QString sRevFile = QString("%1review_answers.csv").arg(sSavePath);
	QFile oRevFile(sRevFile);
	if(!oRevFile.open(QFile::WriteOnly | QFile::Truncate))
		return false;

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