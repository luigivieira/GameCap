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

#ifndef QUESTIONNAIRE_H
#define QUESTIONNAIRE_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QBoxLayout>
#include <QSignalMapper>

namespace gc
{
	/**
	 * Base class for the questionnaires.
	 */
	class Questionnaire: public QWidget
	{
		Q_OBJECT
	
	public:

		/** Types of questions allowed in questionnaires. */
		enum QuestionType { Integer, String, Likert };
		Q_ENUM(QuestionType)

		/**
		 * Class constructor.
		 * @param pParent Instance for the QObject that is the parent of this one. Default is NULL.
		 */
		Questionnaire(QWidget *pParent = NULL);

		/**
		 * Sets the questionnaire title.
		 * @param sTitle QString with the questionnaire title.
		 */
		void setTitle(QString sTitle);

		/**
		 * Sets the questionnaire description.
		 * @param sDescription QString with the questionnaire description.
		 */
		void setDescription(QString sDescription);

		/**
		 * Adds a new question to this questionnaire.
		 * @param eType Value of the enumeration QuestionType with the type of the question
		 * to add.
		 * @param iOptions Integer with the number of options. Default is 0, because this
		 * parameter is only used if eType is Likert.
		 * @return Boolean indicating if the question was successfully added or not.
		 */
		bool addQuestion(const QuestionType eType, uint iOptions = 0);

		/**
		 * Gets the number of questions added to this questionnaire.
		 * @return Unsigned integer with the number of questions in the questionnaire.
		 */
		uint getNumberOfQuestions() const;

		/**
		 * Sets the title for the given question.
		 * @param iIndex Index of the question in the questionnaire.
		 * @param sTitle String with the new title for the question.
		 */
		void setQuestionTitle(const uint iIndex, const QString sTitle);

		/**
		 * Sets the value of the given question.
		 * @param iIndex Index of the question in the questionnaire.
		 * @param oValue QVariant with the new value for the question (stored accordingly
		 * to the question type defined when it was added).
		 */
		void setQuestionValue(const uint iIndex, QVariant oValue);

		/**
		 * Queries if the questionnaire is completed.
		 * @return Boolean value indicating if the questionnaire is completed or not.
		 */
		bool isCompleted() const;

		/**
		 * Sets the option titles for the given likert question. This method only
		 * executes if the given question is indeed of the type Likert.
		 * @param iIndex Index of the question in the questionnaire.
		 * @param lTitles QStringList with the list of titles.
		 */
		void setLikertOptionTitles(const uint iIndex, const QStringList &lTitles);

	protected slots:

		/**
		 * Captures signals indicating changes in question fields made by the participant.
		 * @param iIndex Index of the question field that has changed, in range [0, n-1]
		 * (where n is the number of questions in the questionnaire).
		 */
		void fieldChanged(const uint iIndex);

	signals:

		/**
		 * Signals the completion of the questionnaire, when the participant has
		 * filled up all answers.
		 */
		void completed();

		/**
		 * Signals that the value of a question has changed.
		 * @param iIndex Unsigned integer with the index of the question that has changed.
		 * @param eType Type of the question that changed.
		 * @param oValue QVariant with the new content of the question (depending on the type).
		 */
		void questionChanged(const uint iIndex, const QuestionType eType, const QVariant oValue);

	private:

		/** Main layout for adding controls. */
		QBoxLayout *m_pMainLayout;

		/** Title of the questionnaire. */
		QLabel *m_pTitle;

		/** Short text with additional information for the participant. */
		QLabel *m_pDescription;
		
		/** Vector with the question types. */
		QVector<QuestionType> m_vQuestionTypes;

		/** Vector with the question label objects (used for presenting the titles). */
		QVector<QLabel*> m_vQuestionLabels;

		/** Vector with the question field objects (used for capturing the data). */
		QVector<QWidget*> m_vQuestionFields;

		/** Signal mapper used for triggering the changed signal upon field updates. */
		QSignalMapper *m_pMapper;
	};
}

#endif // QUESTIONNAIRE_H