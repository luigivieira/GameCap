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

#define STYLE_SHEET "\
	QSlider::groove:horizontal {\
		border: 1px solid #bbb;\
		background: white;\
		height: 15px;\
		border-radius: 4px;\
	}\
	\
	QSlider::sub-page:horizontal {\
		background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #66e, stop: 1 #bbf);\
		background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1, stop: 0 #bbf, stop: 1 #55f);\
		border: 1px solid #777;\
		height: 10px;\
		border-radius: 4px;\
	}\
	\
	QSlider::add-page:horizontal {\
		background: #fff;\
		border: 1px solid #777;\
		height: 10px;\
		border-radius: 4px;\
	}\
	\
	QSlider::handle:horizontal {\
		background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc);\
		border: 1px solid #777;\
		width: 17px;\
		margin-top: -2px;\
		margin-bottom: -2px;\
		border-radius: 8px;\
	}\
	\
	QSlider::handle:horizontal:hover {\
		background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\
			stop:0 #fff, stop:1 #ddd);\
		border: 1px solid #444;\
		border-radius: 8px;\
	}\
	\
	QSlider::sub-page:horizontal:disabled {\
		background: #bbb;\
		border-color: #999;\
	}\
	\
	QSlider::add-page:horizontal:disabled {\
		background: #eee;\
		border-color: #999;\
	}\
	\
	QSlider::handle:horizontal:disabled {\
		background: #eee;\
		border: 1px solid #aaa;\
		border-radius: 4px;\
	}"

// +-----------------------------------------------------------
gc::ProgressSlider::ProgressSlider(QWidget *pParent) : QSlider(Qt::Horizontal, pParent)
{
	setStyleSheet(STYLE_SHEET);
	setTickInterval(30);
}

// +-----------------------------------------------------------
void gc::ProgressSlider::setTickPosition(TickPosition ePosition)
{
	QSlider::setTickPosition(QSlider::NoTicks); // avoid the original tick drawing mechanism
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
	int iInterval = tickInterval();
	if(iInterval == 0)
		iInterval = pageStep();

	int M = maximum();
	int m = minimum();
	int iStart = (maximum() - minimum()) / 2;

	QPen oPen(QColor("#a5a294"));
	oPen.setWidth(4);
	oPainter.setPen(oPen);
	for (int i = iStart; i <= maximum(); i += iInterval)
	{
		int x = round((double)((double)((double)(i - this->minimum()) / (double)(this->maximum() - this->minimum())) * (double)(this->width() - oHandle.width()) + (double)(oHandle.width() / 2.0))) - 1;
		int h = height();
		int y = rect().top() + height() / 2;
		oPainter.drawLine(x, y - h, x, y + h);
	}
}
