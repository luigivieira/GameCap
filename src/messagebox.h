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

#include <QMessageBox>

namespace gc
{
	/**
	 * Provides custom MessageBox methods to avoid having to use native Qt translations.
	 */
	class MessageBox
	{
	protected:

		/**
		 * Protected constructor, so this class can not be instantiated.
		 */
		MessageBox() {};

	public:

		/**
		 * Asks an yes-no question with the given title and message.
		 * @param sTitle String with the title for the dialog.
		 * @param sMessage String with the message for the dialog.
		 * @return Boolean with true if the user clicked "Yes", and false if the user clicked "No".
		 */
		static bool yesNoQuestion(QString sTitle, QString sMessage);
	};
}

#endif // MESSAGE_BOX_H
