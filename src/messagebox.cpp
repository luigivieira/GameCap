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
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QScreen>
#include <QDebug>
#include <QThread>

// +-----------------------------------------------------------
gc::MessageBox::MessageBox(gc::Window *pParent): QDialog(pParent)
{
	m_pMainWindow = pParent;
	m_pPage = NULL;

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

	QScreen *pScreen = QApplication::primaryScreen();
	QRect oDim = pScreen->availableGeometry();
	setFixedSize(oDim.width() * 0.4, oDim.height() * 0.4);

	fadeOut();
}

// +-----------------------------------------------------------
gc::MessageBox::~MessageBox()
{
	fadeIn();
}

// +-----------------------------------------------------------
void gc::MessageBox::fadeOut()
{
	m_pPage = m_pMainWindow->currentPage();

	if(!m_pPage->graphicsEffect())
	{
		QGraphicsOpacityEffect *pEffect = new QGraphicsOpacityEffect(m_pPage);
		m_pPage->setGraphicsEffect(pEffect);
	}

	QPropertyAnimation *pAnim = new QPropertyAnimation(m_pPage->graphicsEffect(), "opacity");
	pAnim->setDuration(350);
	pAnim->setStartValue(1);
	pAnim->setEndValue(0.1);
	pAnim->setEasingCurve(QEasingCurve::OutBack);
	pAnim->start(QPropertyAnimation::DeleteWhenStopped);
}

// +-----------------------------------------------------------
void gc::MessageBox::fadeIn()
{
	Q_ASSERT(m_pPage);
	if(!m_pPage->graphicsEffect())
	{
		QGraphicsOpacityEffect *pEffect = new QGraphicsOpacityEffect(m_pPage);
		m_pPage->setGraphicsEffect(pEffect);
	}

	QPropertyAnimation *pAnim = new QPropertyAnimation(m_pPage->graphicsEffect(), "opacity");
	pAnim->setDuration(350);
	pAnim->setStartValue(0.1);
	pAnim->setEndValue(1);
	pAnim->setEasingCurve(QEasingCurve::InBack);
	pAnim->start(QPropertyAnimation::DeleteWhenStopped);
}

// +-----------------------------------------------------------
bool gc::MessageBox::yesNoQuestion(BasePage *pParent, QString sMessage)
{
	MessageBox oBox(pParent->window());
	return oBox.yesNoQuestion(sMessage);
}

// +-----------------------------------------------------------
bool gc::MessageBox::yesNoQuestion(QString sMessage)
{
	m_pMessage->setText(sMessage);

	m_pButtons->clear();
	QPushButton *pYes = m_pButtons->addButton(QDialogButtonBox::Yes);
	QPushButton *pNo = m_pButtons->addButton(QDialogButtonBox::No);
	pYes->setText(QApplication::translate("MessageBox", "Yes"));
	pNo->setText(QApplication::translate("MessageBox", "No"));
	pYes->setCursor(Qt::PointingHandCursor);
	pNo->setCursor(Qt::PointingHandCursor);

	return (exec() == QDialog::Accepted);
}

// +-----------------------------------------------------------
void gc::MessageBox::infoMessage(BasePage *pParent, QString sMessage)
{
	MessageBox oBox(pParent->window());
	oBox.infoMessage(sMessage);
}

// +-----------------------------------------------------------
void gc::MessageBox::infoMessage(QString sMessage)
{
	m_pMessage->setText(sMessage);

	m_pButtons->clear();
	QPushButton *pOk = m_pButtons->addButton(QDialogButtonBox::Ok);
	pOk->setCursor(Qt::PointingHandCursor);

	exec();
}