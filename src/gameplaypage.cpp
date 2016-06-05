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

#include "gameplaypage.h"
#include "window.h"
#include <QBoxLayout>
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
#include "messagebox.h"

// +-----------------------------------------------------------
gc::GamePlayPage::GamePlayPage(QWidget *pParent) : QWizardPage(pParent)
{
	// Page layout and components
	QVBoxLayout *pLayout = new QVBoxLayout(this);
	pLayout->setMargin(50);
	pLayout->setAlignment(Qt::AlignCenter);

	pLayout->addStretch();

	m_pMessage = new QLabel(this);
	m_pMessage->setAlignment(Qt::AlignCenter);
	m_pMessage->setWordWrap(true);
	pLayout->addWidget(m_pMessage);

	pLayout->addSpacing(20);

	m_pRemainingTime = new QLabel(this);
	m_pRemainingTime->setAlignment(Qt::AlignCenter);
	m_pRemainingTime->setWordWrap(true);
	pLayout->addWidget(m_pRemainingTime);

	pLayout->addStretch();

	// Connect to the gameplay for the remaining time and game ended signals
	connect(((Application*) qApp)->gameControl(), &GameControl::gameRemainingTime, this, &GamePlayPage::onGameRemainingTime);
	connect(((Application*) qApp)->gameControl(), &GameControl::gameplayEnded, this, &GamePlayPage::onGameplayEnded);
}

// +-----------------------------------------------------------
void gc::GamePlayPage::initializePage()
{
	Game *pGame = ((Application*) qApp)->gameControl()->currentGame();
	m_pMessage->setText(tr("You are now playing %1. If you wish to quit the experiment, please quit from the game first.").arg(pGame->name()));
	m_pRemainingTime->setText("");
}

// +-----------------------------------------------------------
void gc::GamePlayPage::onGameRemainingTime(int iSeconds)
{
	QString sTime = QDateTime::fromTime_t(iSeconds).toUTC().toString("mm:ss");
	m_pRemainingTime->setText(tr("Remaining time playing: %1").arg(sTime));
}

// +-----------------------------------------------------------
void gc::GamePlayPage::onGameplayEnded(GameControl::GameplaySessionResult eResult)
{
	if (eResult == GameControl::Success)
		wizard()->next();
	else
	{
		Game *pGame = ((Application*)qApp)->gameControl()->currentGame();
		if (eResult == GameControl::Error)
			MessageBox::infoMessage(this, tr("The game %1 seems not to be working, so the experiment can not be continued. Please, inform the researcher in charge. Nevertheless, thank you very much for your time.").arg(pGame->name()));
		else if (eResult == GameControl::Cancelled)
			MessageBox::infoMessage(this, tr("You quit the game %1 before the required play time, hence quitting the experiment. Nevertheless, thank you very much for your time.").arg(pGame->name()));
		wizard()->reject();
	}
}