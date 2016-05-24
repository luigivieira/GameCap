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
#include <QBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>

 // +-----------------------------------------------------------
gc::MessageBox::MessageBox(QWidget *pParent): QDialog(pParent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	QVBoxLayout *pLayout = new QVBoxLayout(this);
	QFrame *pFrame = new QFrame(this);
	pFrame->setAutoFillBackground(true);
	QPalette oWhite(palette());
	oWhite.setColor(QPalette::Background, Qt::white);
	pFrame->setPalette(oWhite);
	pFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
	pFrame->setLineWidth(2);
	pLayout->addWidget(pFrame);

	pLayout = new QVBoxLayout(pFrame);
	m_pMessage = new QLabel(this);
	m_pMessage->setWordWrap(true);
	m_pMessage->setAlignment(Qt::AlignCenter);
	pLayout->addWidget(m_pMessage);

	m_pMessage->setText("This is a test for a very long message box, weird indeed.");

	m_pButtons = new QDialogButtonBox(this);
	m_pButtons->setCenterButtons(true);
	pLayout->addWidget(m_pButtons);

	QGraphicsDropShadowEffect *pShadow = new QGraphicsDropShadowEffect();
	pShadow->setBlurRadius(9.0);
	pShadow->setColor(QColor(0, 0, 0, 160));
	pShadow->setOffset(10.0);
	pFrame->setGraphicsEffect(pShadow);

	connect(m_pButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(m_pButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

 // +-----------------------------------------------------------
bool gc::MessageBox::yesNoQuestion(QWidget *pParent, QString sMessage)
{
	MessageBox oBox(pParent);
	oBox.m_pMessage->setText(sMessage);
	QPushButton *pYes = oBox.m_pButtons->addButton(QDialogButtonBox::Yes);
	QPushButton *pNo = oBox.m_pButtons->addButton(QDialogButtonBox::No);
	pYes->setText(QApplication::translate("MessageBox", "Yes"));
	pNo->setText(QApplication::translate("MessageBox", "No"));
	pYes->setCursor(Qt::PointingHandCursor);
	pNo->setCursor(Qt::PointingHandCursor);

	return (oBox.exec() == QDialog::Accepted);
}

// +-----------------------------------------------------------
void gc::MessageBox::infoMessage(QWidget *pParent, QString sMessage)
{
	MessageBox oBox(pParent);
	oBox.m_pMessage->setText(sMessage);
	QPushButton *pOk = oBox.m_pButtons->addButton(QDialogButtonBox::Ok);
	pOk->setCursor(Qt::PointingHandCursor);
	oBox.exec();
}