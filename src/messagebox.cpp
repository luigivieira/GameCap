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

#include "messagebox.h"
#include <QApplication>

 // +-----------------------------------------------------------
bool gc::MessageBox::yesNoQuestion(QString sTitle, QString sMessage)
{
	QMessageBox oMsgBox(QMessageBox::Question, sTitle, sMessage, QMessageBox::Yes | QMessageBox::No);
	oMsgBox.setButtonText(QMessageBox::Yes, QApplication::translate("MessageBox", "Yes"));
	oMsgBox.setButtonText(QMessageBox::No, QApplication::translate("MessageBox", "No"));
	return (oMsgBox.exec() == QMessageBox::Yes);
}