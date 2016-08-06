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
}

// +-----------------------------------------------------------
gc::GamePlayData::AnswerValue gc::GamePlayData::getReviewAnswer(const gc::GamePlayData::ReviewQuestion eQuestion, const uint iTimestamp) const
{
	ReviewAnswers::const_iterator it = m_mpReviewAnswers.find(iTimestamp);
	if (it != m_mpReviewAnswers.constEnd())
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
				return AnswerValue::Undefined;
		}
	}
	else
		return AnswerValue::Undefined;
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
