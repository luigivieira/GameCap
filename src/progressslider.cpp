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

#include "progressslider.h"
#include <QStyle>
#include <QStylePainter>
#include <QStyleOptionSlider>

// +-----------------------------------------------------------
gc::ProgressSlider::ProgressSlider(const QVector<uint> vTicks, QWidget *pParent) : QSlider(Qt::Horizontal, pParent)
{
	setCursor(Qt::PointingHandCursor);
	m_vTicks = vTicks;

	// Force horizontal orientation and no default ticks (the ticks will be drawn
	// by the custom painting of this class)
	setOrientation(Qt::Horizontal);
	setTickPosition(QSlider::NoTicks);
}

// +-----------------------------------------------------------
void gc::ProgressSlider::setTickPosition(TickPosition ePosition)
{
	QSlider::setTickPosition(ePosition);
}

// +-----------------------------------------------------------
void gc::ProgressSlider::setOrientation(Qt::Orientation eOrientation)
{
	QSlider::setOrientation(eOrientation);
}

// +-----------------------------------------------------------
void gc::ProgressSlider::paintEvent(QPaintEvent *pEvent)
{
	QSlider::paintEvent(pEvent);

	QStylePainter oPainter(this);
	QStyleOptionSlider oOpt;
	initStyleOption(&oOpt);

	QRect oHandle = style()->subControlRect(QStyle::CC_Slider, &oOpt, QStyle::SC_SliderHandle, this);

	// draw tick marks manually
	QPen oPen(QColor("#a5a294"));
	oPen.setWidth(4);
	oPainter.setPen(oPen);

	double dRange = static_cast<double>(maximum() - minimum());
	foreach(uint iTick, m_vTicks)
	{
		double dPos = ((iTick - minimum()) / dRange) * (width() - oHandle.width()) + (oHandle.width() / 2.0);
		int x = round(dPos);
		int y = rect().top() + height() / 2;
		oPainter.drawLine(x, y - height(), x, y + height());
	}
}

// +-----------------------------------------------------------
void gc::ProgressSlider::mousePressEvent(QMouseEvent *pEvent)
{
	// Implements the "jump click" in the progress slider
	QStyleOptionSlider oOpt;
	initStyleOption(&oOpt);
	QRect oHandleRect = style()->subControlRect(QStyle::CC_Slider, &oOpt, QStyle::SC_SliderHandle, this);

	if (pEvent->button() == Qt::LeftButton && oHandleRect.contains(pEvent->pos()) == false)
	{
		double dHalfHandleWidth = (0.5 * oHandleRect.width()) + 0.5;
		int iAdaptedPosX = pEvent->x();
		if (iAdaptedPosX < dHalfHandleWidth)
			iAdaptedPosX = dHalfHandleWidth;
		if (iAdaptedPosX > width() - dHalfHandleWidth)
			iAdaptedPosX = width() - dHalfHandleWidth;
		
		double dNewWidth = (width() - dHalfHandleWidth) - dHalfHandleWidth;
		double normalizedPosition = (iAdaptedPosX - dHalfHandleWidth) / dNewWidth;

		int iNewVal = minimum() + ((maximum() - minimum()) * normalizedPosition);
		if (invertedAppearance() == true)
			setValue(maximum() - iNewVal);
		else
			setValue(iNewVal);

		pEvent->accept();
		emit actionTriggered(QSlider::SliderMove);
	}
	QSlider::mousePressEvent(pEvent);
}
