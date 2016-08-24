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

#include "gamereviewpage.h"
#include "application.h"
#include <QUrl>

// +-----------------------------------------------------------
gc::GameReviewPage::GameReviewPage(QWidget *pParent) : BasePage(pParent)
{
	m_pReviewer = new VideoReviewer(this);
	layout()->addWidget(m_pReviewer);

	connect(m_pReviewer, &VideoReviewer::reviewCompleted, this, &GameReviewPage::onReviewCompleted);
}

// +-----------------------------------------------------------
void gc::GameReviewPage::initializePage()
{
	m_bCompleted = false;
	emit completeChanged();
	QString sFile = static_cast<Application*>(qApp)->getGameplayFile();
	m_pReviewer->initializeQuestionnaire();
	m_pReviewer->playVideo(sFile);
}

// +-----------------------------------------------------------
bool gc::GameReviewPage::validatePage()
{
	m_pReviewer->stopVideo();
	return true;
}

// +-----------------------------------------------------------
bool gc::GameReviewPage::isComplete() const
{
	return m_bCompleted;
}

// +-----------------------------------------------------------
void gc::GameReviewPage::onReviewCompleted()
{
	m_bCompleted = true;
	emit completeChanged();
}
