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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QKeySequence>
#include <QDebug>
#include <QObject>
#include <fstream>
#include <vector>

namespace gc
{
	/**
	 * Custom Qt application class, mainly used to allow log messages to be
     * redirected to a file.
	 */
	class Application: public QApplication
	{
		Q_OBJECT
	public:
		/**
		* Class constructor.
		* @param argc Number of arguments received from the command line.
		* @param argv Array of char pointers with the arguments received from the command line.
		*/
		Application(int argc, char* argv[]);

		/**
		* Class destructor.
		*/
		virtual ~Application();
        
		/**
		 * Overload of the exec method for logging startup and termination messages.
		 * @return Integer with the exit code.
		 */
		int exec();

		/**
		 * Handles the notification of messages in the application event loop.
		 * @param pReceiver Pointer to the QObject that shall receive the message.
		 * @param pEvent Pointer to the QEvent with the message information.
		 */
        bool notify(QObject* pReceiver, QEvent* pEvent);

		/**
		 * Gets the current maximum log level.
		 * @return QtMsgType with the maximum log level.
		 */
		QtMsgType getLogLevel();

		/**
		* Sets the maximum log level.
		* @param eLevel QtMsgType with the maximum log level.
		*/
		void setLogLevel(QtMsgType eLevel);

		/**
		 * Gets the style sheet to be used by the application interface.
		 * @return QString with the style sheet to be used.
		 */
		QString getStyleSheet();

		/** Enumeration of the available languages for this application. */
		enum Language {EN_UK, PT_BR};

		/**
		 * Sets the current language for the application, in order to update
		 * the translations.
		 * @param eLanguage Value of the enumeration Language with the language identifier.
		 */
		void setLanguage(Language eLanguage);

	protected:

		/**
		 * Log and exception message handler for application events.
 		 * @param eType QtMsgType enum value with the type of the log event.
		 * @param oContext QMessageLogContext instance with information on
         * where the event happened (function, line, etc)
		 * @param sMsg QString instance with the event message.
		 */
		static void handleLogOutput(QtMsgType eType, const QMessageLogContext& oContext, const QString& sMsg);

		/**
		 * Helper method for reading the settings from the INI file.
		 */
		void readSettings();

		/**
		* Helper method for updating the settings to the INI file.
		*/
		void updateSettings();

	private:
    
		/** File stream used to log application messages. */
		std::ofstream m_oLogFile;

		/**
		 * Maximum level of the messages to log.
		 * Ranges from 4 (only QtInfoMsg) downwards to 0 (all to QtDebugMsg).
		 */
		QtMsgType m_eLogLevel;

		/** List of window titles for integration with OBS (used to capture player and game videos). */
		std::vector<QString> m_vOBSWindows;

		/** Keyboard shortcut used by the OBS windows to toggle the recording. */
		QKeySequence m_oOBSShortcut;

		/**
		 * String with the stylesheet to be used by the application interface
		 * (loaded from an external .css file).
		 */
		QString m_sStyleSheet;

		/** Current translator installed. */
		QTranslator *m_pCurrentTranslator;

		/** Translator used for Brazilian Portuguese. */
		QTranslator *m_pPTBRTranslator;
	};
}

#endif // APPLICATION_H
