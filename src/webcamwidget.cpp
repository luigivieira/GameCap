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

#include "webcamwidget.h"

// +-----------------------------------------------------------
gc::WebcamWidget::WebcamWidget(float fAspectRatio, QWidget *pParent) : QCameraViewfinder(pParent)
{
	m_fAspectRatio = fAspectRatio != 0.0f ? fAspectRatio : 1.0f;
	setAspectRatioMode(Qt::IgnoreAspectRatio);
}

// +-----------------------------------------------------------
bool gc::WebcamWidget::hasHeightForWidth() const
{
	return m_fAspectRatio != 1.0 ? true : false;
}

// +-----------------------------------------------------------
int gc::WebcamWidget::heightForWidth(int iWidth) const
{
	float fHeight = iWidth / m_fAspectRatio;
	qDebug("Altura: %f, Largura: %d", fHeight, iWidth);
	return fHeight;
}
