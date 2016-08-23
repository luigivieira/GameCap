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

#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include "window.h"
#include "basepage.h"
#include <QDialog>
#include <QLabel>
#include <QDialogButtonBox>
#include <QWizardPage>

namespace gc
{
	/**
	 * Provides custom MessageBox methods without title bars and locally translated
	 * buttons (i.e. avoiding to have to include all other translations from Qt). Also,
	 * applies fade in/out effects to the base Window as it is displayed.
	 */
	class MessageBox: QDialog
	{
	public:

		/**
		 * Protected constructor, so this class can not be instantiated.
		 * @param pParent Instance of the Window class that will parent this dialog.
		 */
		MessageBox(Window *pParent);

		/**
		 * Class destructor.
		 */
		virtual ~MessageBox();

		/**
		 * Asks an yes-no question with the given title and message.
		 * @param sMessage String with the message for the dialog.
		 * @return Boolean with true if the user clicked "Yes", and false if the user clicked "No".
		 */
		bool yesNoQuestion(QString sMessage);

		/**
		 * Static version of the method yesNoQuestion.
		 * 
		 * IMPORTANT: If more than one call of this or the other display methods are to be
		 * called in a sequence, DO NOT USE THEIR STATIC VERSIONS. Instead, create a local
		 * instance of the MessageBox class and call all regular methods from it. This is
		 * required to avoid crashes due to chained fade effects between displays.
		 * 
		 * @param pParent Instance of the class that will parent this dialog.
		 * @param sMessage String with the message for the dialog.
		 * @return Boolean with true if the user clicked "Yes", and false if the user clicked "No".
		 */
		static bool yesNoQuestion(BasePage *pParent, QString sMessage);

		/**
		 * Shows a message dialog with an Ok button.
		 * @param sMessage String with the message for the dialog.
		 */
		void infoMessage(QString sMessage);

		/**
		 * Static version of the method infoMessage.
		 *
		 * IMPORTANT: If more than one call of this or the other display methods are to be
		 * called in a sequence, DO NOT USE THEIR STATIC VERSIONS. Instead, create a local
		 * instance of the MessageBox class and call all regular methods from it. This is
		 * required to avoid crashes due to chained fade effects between displays.
		 *
		 * @param pParent Instance of the class that will parent this dialog.
		 * @param sMessage String with the message for the dialog.
		 */
		static void infoMessage(BasePage *pParent, QString sMessage);

	protected:

		/**
		 * Fades out the main window and all its children.
		 */
		void fadeOut();

		/**
		 * Fades in the main window and all its children.
		 */
		void fadeIn();

	private:

		/** Label to hold the message displayed by the dialog. */
		QLabel *m_pMessage;

		/** Group of buttons to be displayed for user interaction. */
		QDialogButtonBox *m_pButtons;

		/** Main window of the application. */
		Window *m_pMainWindow;

		/** The page that is faded out/in. */
		QWidget *m_pPage;
	};
}

#endif // MESSAGE_BOX_H
