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

#ifndef GAME_PLAY_DATA_H
#define GAME_PLAY_DATA_H

#include "application.h"
#include <QWizardPage>
#include <QLabel>

// Size of the GEQ questionnaire.
#define GEQ_SIZE 33

namespace gc
{
	/**
	 * Handles the storage and saving of gameplay data collected in the experiment.
	 * The class stores answers for frustration, involvement and fun levels for a
	 * given number of review samples (taken during the review of the gameplay video),
	 * and also the answers for the GEQ (Game Experience Questionnaire) taken at the end
	 * of the participation.
	 */
	class GamePlayData : public QObject
	{
		Q_OBJECT
	public:

		/** Possible answer values for the (Likert-scaled) questions. */
		enum AnswerValue
		{
			Undefined  = 42,
			NotAtAll   = -2,
			Slightly   = -1,
			Moderately =  0,
			Fairly     =  1,
			Extremely  =  2
		};
		Q_ENUM(AnswerValue)

		/** Questions asked during gameplay review. */
		enum ReviewQuestion	{ Frustation, Involvement, Fun };
		Q_ENUM(ReviewQuestion)

        /**
         * Class constructor.
		 * @param iVideoDuration Integer with the duration of the videos captured, in seconds.
         */
		GamePlayData(const uint iVideoDuration, const uint iSamples, const uint iInterval);

		/**
		 * Gets the stored answer value for the given question at the given timestamp.
		 * @param eQuestion Value of the ReviewQuestion enumeration with the question to
		 * get the answer for.
		 * @param iTimestamp Unsigned integer with the timestamp in seconds to get the
		 * answer for.
		 * @return Value of the AnswerValue enumeration with the answer for frustration
		 * in the given timestamp.
		 */
		gc::GamePlayData::AnswerValue getReviewAnswer(const gc::GamePlayData::ReviewQuestion eQuestion, const uint iTimestamp) const;

		/**
		 * Sets the stored answer value for the given question at the given timestamp.
		 * @param eQuestion Value of the ReviewQuestion enumeration with the question to
		 * set the answer.
		 * @param iTimestamp Unsigned integer with the timestamp in seconds to set the
		 * answer.
		 * @param eAnswer Value of the AnswerValue enumeration with the answer for frustation
		 * in the given timestamp.
		 */
		void setReviewAnswer(const gc::GamePlayData::ReviewQuestion eQuestion, const uint iTimestamp, const gc::GamePlayData::AnswerValue eAnswer);

	protected:


	private:

		/** Structure to store the answers for a givem sample. */
		typedef struct
		{
			AnswerValue eFrustration;
			AnswerValue eInvolvement;
			AnswerValue eFun;
		} SampleData;

		/** Definition of the map to store the review answers. */
		typedef QMap<uint, SampleData> ReviewAnswers;

		/** Map with the answers for the gameplay review. */
		ReviewAnswers m_mpReviewAnswers;
		
		/** Array with the answers for the GEQ questionnaire. */
		AnswerValue m_aGEQ[GEQ_SIZE];
		
	};
}

#endif // GAME_PLAY_DATA_H
