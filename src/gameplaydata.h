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

#include <QObject>
#include <QMap>

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
	class GameplayData : public QObject
	{
		Q_OBJECT
	public:

		/** Possible values for answers in (Likert-scaled) questions. */
		enum AnswerValue
		{
			Undefined  = 99,
			NotAtAll   = -2,
			Slightly   = -1,
			Moderately =  0,
			Fairly     =  1,
			Extremely  =  2
		};
		Q_ENUM(AnswerValue)

		/** Possible values for the sex of the respondent. */
		enum Sex
		{
			Unknown = -1,
			Male    = 0,
			Female  = 1
		};

		/** Possible values for the number of hours playing digital games per week. */
		enum HoursPerWeekPlayingGames
		{
			NoAnswer = -1,
			_0To2    = 0,
			_2To5    = 1,
			_5To10   = 2,
			_10Plus  = 3
		};

		/** Questions asked during gameplay review. */
		enum ReviewQuestion	{ Frustation = 0, Involvement = 1, Fun = 2 };

        /**
         * Class constructor.
		 * @param pParent Instance of a QObject to parent this instance. Default is NULL.
         */
		GameplayData(QObject *pParent = NULL);

		/**
		 * Setups the class so it can be used to collect data for another subject.
		 * @param iVideoDuration Integer with the duration of the videos captured, in seconds.
		 * @param iSamples Integer with the number of samples collected in the gameplay review.
		 * @param iInterval Integer with the time interval (in seconds) for each collection in the gameplay review.
		 */
		void setup(const uint iVideoDuration, const uint iSamples, const uint iInterval);

		/**
		 * Gets the stored age of the subject.
		 * @return Unsigned integer with the age of the subject.
		 */
		uint getAge() const;

		/**
		 * Sets the storad age of the subject.
		 * @param iAge Unsigned integer with the age of the subject.
		 */
		void setAge(const uint iAge);

		/**
		 * Gets the stored sex of the subject.
		 * @return Value of the Sex enumeration with the sex of the subject.
		 */
		Sex getSex() const;

		/**
		 * Sets the storad sex of the subject.
		 * @param eSex Value of the Sex enumeration with the sex of the subject.
		 */
		void setSex(const Sex eSex);

		/**
		 * Gets the stored indication on whether the subject plays diital games.
		 * @return Boolean indicating if the subject plays or not digital games.
		 */
		bool getPlaysGames() const;

		/**
		 * Sets the stored indication on whether the subject plays digital games.
		 * @param bPlays Boolean indicating if the subject plays or not digital games.
		 */
		void setPlaysGames(const bool bPlays);

		/**
		 * Gets the stored number of hours per week the subject plays games.
		 * @return Value of the HoursPerWeekPlayingGames enumeration with the number of hours.
		 */
		HoursPerWeekPlayingGames getHoursPerWeekPlayingGames() const;

		/**
		 * Sets the stored number of hours per week the subject plays videogames.
		 * @param eHours Value of the HoursPlayingVideogames enumeration with the number of hours.
		 */
		void setHoursPerWeekPlayingGames(const HoursPerWeekPlayingGames eHours);

		/**
		 * Gets the stored indication on whether the subject has played the tested game before.
		 * @return Boolean indicating if the subject has played the game or not.
		 */
		bool hasPlayedGameBefore() const;

		/**
		 * Sets the stored indication on whether the subject has played the tested game before.
		 * @param bHasPlayed Boolean indicating if the subject has played the game or not.
		 */
		void setHasPlayedGameBefore(const bool bHasPlayed);

		/**
		 * Gets the stored answer value for the given question at the given timestamp.
		 * @param eQuestion Value of the ReviewQuestion enumeration with the question to
		 * get the answer for.
		 * @param iTimestamp Unsigned integer with the timestamp in seconds to get the
		 * answer for.
		 * @return Value of the AnswerValue enumeration with the answer for frustration
		 * in the given timestamp.
		 */
		AnswerValue getReviewAnswer(ReviewQuestion eQuestion, const uint iTimestamp) const;

		/**
		 * Sets the stored answer value for the given question at the given timestamp.
		 * @param eQuestion Value of the ReviewQuestion enumeration with the question to
		 * set the answer.
		 * @param iTimestamp Unsigned integer with the timestamp in seconds to set the
		 * answer.
		 * @param eAnswer Value of the AnswerValue enumeration with the answer for frustation
		 * in the given timestamp.
		 */
		void setReviewAnswer(ReviewQuestion eQuestion, const uint iTimestamp, const AnswerValue eAnswer);

		/**
		 * Gets the stored answer value for the given GEQ question.
		 * @param iQuestion Unsigned integer with the index of the question (in range [0, GEQ_SIZE-1]).
		 * @return Value of the AnswerValue enumeration with the answer for the question.
		 */
		AnswerValue getGEQAnswer(const uint iQuestion) const;

		/**
		 * Sets the stored answer value for the given GEQ question.
		 * @param iQuestion Unsigned integer with the index of the question (in range [0, GEQ_SIZE-1]).
		 * @param eAnswer Value of the AnswerValue enumeration with the answer for the question.
		 */
		void setGEQAnswer(const int uiQuestion, const AnswerValue eAnswer);

		/**
		 * Saves the gameplay data to CSV files in the given path.
		 * @sPath QString with the path for saving the CSV files with the gameplay data.
		 */
		bool save(const QString &sPath) const;

		/**
		 * Queries if the review answers for the given timestamp were all set to a differente value than Undefined.
		 * @param Returns a boolean indicating true if the review answers were all set.
		 */
		bool isReviewSampleCompleted(const uint iTimestamp);

		/**
		 * Queries if the review answers for all timestamps were all set to a differente value than Undefined.
		 * @param Returns a boolean indicating true if the review answers were all set.
		 */
		bool isReviewCompleted() const;

		/**
		 * Queries if the GEQ answers were all set to a differente value than Undefined.
		 * @param Returns a boolean indicating true if the GEQ answers were all set.
		 */
		bool isGEQCompleted() const;

		/**
		 * Get the list of the review timestamps.
		 * @return QVector of unsigned integers with the timestamps used for sampling the reviews.
		 */
		QVector<uint> getReviewTimestamps() const;

	private:

		/** Age of the subject. */
		uint m_iAge;

		/** Sex of the subject. */
		Sex m_eSex;

		/** Indication if the subject usually plays digital games. */
		bool m_bPlayGames;

		/** Number of hours per week the subject usually plays games. */
		HoursPerWeekPlayingGames m_eHoursPerWeekPlayingGames;

		/** Indication if the subject has played before the game used in the experiment. */
		bool m_bPlayedGameBefore;

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
