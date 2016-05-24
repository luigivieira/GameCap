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

#include <QDialog>
#include <QLabel>
#include <QDialogButtonBox>

namespace gc
{
	/**
	 * Provides custom MessageBox methods without title bars and locally translated
	 * buttons (i.e. avoiding to have to include all other translations from Qt).
	 */
	class MessageBox: QDialog
	{
	protected:

		/**
		 * Protected constructor, so this class can not be instantiated.
		 */
		MessageBox(QWidget *pParent);

	public:

		/**
		 * Asks an yes-no question with the given title and message.
		 * @param pParent Instance of the QWidget that will parent this dialog.
		 * @param sMessage String with the message for the dialog.
		 * @return Boolean with true if the user clicked "Yes", and false if the user clicked "No".
		 */
		static bool yesNoQuestion(QWidget *pParent, QString sMessage);

		/**
		* Shows a message dialog with an Ok button.
		* @param pParent Instance of the QWidget that will parent this dialog.
		* @param sMessage String with the message for the dialog.
		*/
		static void infoMessage(QWidget *pParent, QString sMessage);

	private:

		QLabel *m_pMessage;

		QDialogButtonBox *m_pButtons;
	};
}

#endif // MESSAGE_BOX_H
