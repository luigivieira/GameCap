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

#include "videoreviewer.h"
#include <QFrame>
#include <QStyle>
#include <QTime>

// +-----------------------------------------------------------
gc::VideoReviewer::VideoReviewer(QWidget *pParent) : QWidget(pParent)
{
	// Main widget layout
	QVBoxLayout *pLayout = new QVBoxLayout();
	setLayout(pLayout);

	// Create the video widget with a "TV" frame around it
	QFrame *pFrame = new QFrame(this);
	pFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
	pFrame->setLineWidth(1);
	pFrame->setMidLineWidth(10);
	pFrame->setLayout(new QHBoxLayout());
	pFrame->layout()->setMargin(0);
	pFrame->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	pLayout->addWidget(pFrame);

	m_pVideoWidget = new QVideoWidget(this);
	pFrame->layout()->addWidget(m_pVideoWidget);

	// Create the video controls
	QHBoxLayout *pControlsLayout = new QHBoxLayout();
	pLayout->addLayout(pControlsLayout);

	QSize oBtSize(24, 24);
	QPushButton *pPlayPauseButton = new QPushButton(this);
	pPlayPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	pPlayPauseButton->setIconSize(oBtSize);
	pPlayPauseButton->setFixedSize(oBtSize);
	pControlsLayout->addWidget(pPlayPauseButton);

	QPushButton *pVolumeButton = new QPushButton(this);
	pVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
	pVolumeButton->setIconSize(oBtSize);
	pVolumeButton->setFixedSize(oBtSize);
	pControlsLayout->addWidget(pVolumeButton);

	m_pElapsedTime = new QLabel("00:00", this);
	pControlsLayout->addWidget(m_pElapsedTime);

	m_pProgressSlider = new ProgressSlider(this);
	pControlsLayout->addWidget(m_pProgressSlider);
	connect(m_pProgressSlider, &QSlider::sliderMoved, this, &VideoReviewer::onSliderMoved);

	m_pRemainingTime = new QLabel("00:00", this);
	pControlsLayout->addWidget(m_pRemainingTime);

	// Create the media player
	m_pMediaPlayer = new QMediaPlayer(this);
	m_pMediaPlayer->setVideoOutput(m_pVideoWidget);

	connect(m_pMediaPlayer, &QMediaPlayer::bufferStatusChanged, this, &VideoReviewer::onBufferStatusChanged);
	connect(m_pMediaPlayer, &QMediaPlayer::durationChanged, this, &VideoReviewer::onDurationChanged);
	connect(m_pMediaPlayer, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error), this, &VideoReviewer::onError);
	connect(m_pMediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &VideoReviewer::onMediaStatusChanged);
	connect(m_pMediaPlayer, &QMediaPlayer::positionChanged, this, &VideoReviewer::onPositionChanged);
}

// +-----------------------------------------------------------
void gc::VideoReviewer::playVideo(QString sFileName)
{
	QUrl oUrl = QUrl::fromLocalFile(sFileName);
	QMediaContent oMedia = QMediaContent(oUrl);
	m_pMediaPlayer->setMedia(oMedia);
	m_pMediaPlayer->play();
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onBufferStatusChanged(int iPercentFilled)
{
	qDebug() << "onBufferStatusChanged(" << iPercentFilled << ")";
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onDurationChanged(qint64 iDuration)
{
	qDebug() << "onDurationChanged(" << iDuration << ")";
	m_pProgressSlider->setMaximum(iDuration / 1000);
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onPositionChanged(qint64 iPosition)
{
	m_pProgressSlider->setValue(iPosition / 1000);

	QTime oElapsed = QTime(0, 0, 0).addSecs(iPosition / 1000);
	QTime oRemaining = QTime(0, 0, 0).addSecs((m_pMediaPlayer->duration() / 1000) - (iPosition / 1000));

	m_pElapsedTime->setText(oElapsed.hour() > 0 ? oElapsed.toString("HH:mm:ss") : oElapsed.toString("mm:ss"));
	m_pRemainingTime->setText(oRemaining.hour() > 0 ? oRemaining.toString("HH:mm:ss") : oRemaining.toString("mm:ss"));
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onError(QMediaPlayer::Error eError)
{
	qDebug() << "onError(" << eError << ")";
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onMediaStatusChanged(QMediaPlayer::MediaStatus eStatus)
{
	qDebug() << "onMediaStatusChanged(" << eStatus << ")";

	/*switch(eStatus)
	{
		case QMediaPlayer::UnknownMediaStatus:
		case QMediaPlayer::InvalidMedia:

		case QMediaPlayer::NoMedia:
		case QMediaPlayer::LoadingMedia:
		case QMediaPlayer::LoadedMedia:
		case QMediaPlayer::StalledMedia:
		case QMediaPlayer::BufferingMedia:
		case QMediaPlayer::BufferedMedia:
		case QMediaPlayer::EndOfMedia:
	}*/
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onSliderMoved(int iNewPosition)
{
	m_pMediaPlayer->setPosition(iNewPosition * 1000);
}