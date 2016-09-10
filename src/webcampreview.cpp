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

#include "webcampreview.h"
#include <QCameraInfo>
#include <QBoxLayout>

// +-----------------------------------------------------------
gc::WebcamPreview::WebcamPreview(QWidget *pParent) : QDialog(pParent)
{
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_DeleteOnClose);

	setLayout(new QVBoxLayout());

	m_pVideoOutput = new WebcamWidget(1.77f, this);
	layout()->addWidget(m_pVideoOutput);

	static_cast<QVBoxLayout*>(layout())->addSpacing(10);

	QHBoxLayout *pButtonLayout = new QHBoxLayout();
	static_cast<QVBoxLayout*>(layout())->addLayout(pButtonLayout);
	pButtonLayout->setAlignment(Qt::AlignCenter);

	m_pCamera = new QCamera(QCameraInfo::defaultCamera(), this);
	m_pCamera->setViewfinder(m_pVideoOutput);
	m_pCamera->setCaptureMode(QCamera::CaptureViewfinder);
}

// +-----------------------------------------------------------
void gc::WebcamPreview::showFullScreen()
{
	QWidget::showFullScreen();
	m_pCamera->start();
}

// +-----------------------------------------------------------
void gc::WebcamPreview::close()
{
	m_pCamera->stop();
	QWidget::close();
}