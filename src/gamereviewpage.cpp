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
#include <QBoxLayout>
#include <QUrl>

// +-----------------------------------------------------------
gc::GameReviewPage::GameReviewPage(QWidget *pParent) : QWizardPage(pParent)
{
	// Main page layout
	QVBoxLayout *pLayout = new QVBoxLayout(this);
	pLayout->setMargin(50);

	m_pReviewer = new VideoReviewer(this);
	pLayout->addWidget(m_pReviewer);
}

// +-----------------------------------------------------------
void gc::GameReviewPage::initializePage()
{
	QString sFile = ((Application *) qApp)->getGameplayFile();
	
	m_pReviewer->playVideo("C:\\Temp\\Filme de teste.avi");
}
