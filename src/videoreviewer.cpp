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
#include "application.h"
#include <QFrame>
#include <QStyle>
#include <QTime>

// +-----------------------------------------------------------
gc::VideoReviewer::VideoReviewer(QWidget *pParent) : QWidget(pParent)
{
	// Main widget layout
	QVBoxLayout *pLayout = new QVBoxLayout();
	setLayout(pLayout);

	// Create the video widget with a nice frame around it
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

	// Create the media player
	m_pMediaPlayer = new QMediaPlayer(this);
	m_pMediaPlayer->setVideoOutput(m_pVideoWidget);

	connect(m_pMediaPlayer, &QMediaPlayer::bufferStatusChanged, this, &VideoReviewer::onBufferStatusChanged);
	connect(m_pMediaPlayer, &QMediaPlayer::durationChanged, this, &VideoReviewer::onDurationChanged);
	connect(m_pMediaPlayer, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error), this, &VideoReviewer::onError);
	connect(m_pMediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &VideoReviewer::onMediaStatusChanged);
	connect(m_pMediaPlayer, &QMediaPlayer::positionChanged, this, &VideoReviewer::onPositionChanged);

	// Create the video controls
	QHBoxLayout *pControlsLayout = new QHBoxLayout();
	pLayout->addLayout(pControlsLayout);

	m_pPlayPauseButton = new QPushButton(this);
	m_pPlayPauseButton->setCursor(Qt::PointingHandCursor);
	m_pPlayPauseButton->setObjectName("playPauseButton");
	m_pPlayPauseButton->setProperty("state", "playing");
	pControlsLayout->addWidget(m_pPlayPauseButton);
	connect(m_pPlayPauseButton, &QPushButton::clicked, this, &VideoReviewer::onPlayPauseClicked);

	m_pVolumeButton = new VolumeButton(this);
	m_pVolumeButton->setObjectName("volumeButton");
	pControlsLayout->addWidget(m_pVolumeButton);
	connect(m_pVolumeButton, &VolumeButton::volumeChanged, m_pMediaPlayer, &QMediaPlayer::setVolume);

	m_pElapsedTime = new QLabel("00:00", this);
	pControlsLayout->addWidget(m_pElapsedTime);
	
	GameplayData *pData = static_cast<Application*>(qApp)->getGameplayData();
	QVector<uint> vTimeStamps = pData->getReviewTimestamps();
	m_pProgressSlider = new ProgressSlider(vTimeStamps, this);
	pControlsLayout->addWidget(m_pProgressSlider);
	connect(m_pProgressSlider, &QSlider::actionTriggered, this, &VideoReviewer::onActionTriggered);

	m_pRemainingTime = new QLabel("00:00", this);
	pControlsLayout->addWidget(m_pRemainingTime);

	// Create the sample questionnaire
	QString sTitle = tr("How were you feeling at that time?");
	QString sDescription = tr("Please indicate how you were feeling while playing the game at the time the video is paused, for each of the following items.");
	m_pQuestionnaire = new Questionnaire(sTitle, sDescription, this);
	pLayout->addSpacing(20);
	pLayout->addWidget(m_pQuestionnaire);

	m_pQuestionnaire->addQuestion(Questionnaire::Likert, tr("1. I was feeling frustrated."));
	m_pQuestionnaire->addQuestion(Questionnaire::Likert, tr("2. I was feeling involved."));
	m_pQuestionnaire->addQuestion(Questionnaire::Likert, tr("3. I was having fun."));

	connect(m_pQuestionnaire, &Questionnaire::questionChanged, this, &VideoReviewer::onQuestionChanged);
	connect(m_pQuestionnaire, &Questionnaire::completed, this, &VideoReviewer::onQuestionnaireCompleted);

	m_pQuestionnaire->hide();
	m_bInQuestionnaire = false;
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
	uint iPos = iPosition / 1000;
	m_pProgressSlider->setValue(iPos);

	QTime oElapsed = QTime(0, 0, 0).addSecs(iPos);
	QTime oRemaining = QTime(0, 0, 0).addSecs((m_pMediaPlayer->duration() / 1000) - (iPos));

	m_pElapsedTime->setText(oElapsed.hour() > 0 ? oElapsed.toString("HH:mm:ss") : oElapsed.toString("mm:ss"));
	m_pRemainingTime->setText(oRemaining.hour() > 0 ? oRemaining.toString("HH:mm:ss") : oRemaining.toString("mm:ss"));

	if(!m_bInQuestionnaire)
	{
		GameplayData *pData = static_cast<Application*>(qApp)->getGameplayData();
		QVector<uint> vTimeStamps = pData->getReviewTimestamps();
		if(vTimeStamps.contains(iPos))
			QTimer::singleShot(10, this, &VideoReviewer::showQuestionnaire);
	}
}

// +-----------------------------------------------------------
void gc::VideoReviewer::showQuestionnaire()
{
	GameplayData *pData = static_cast<Application*>(qApp)->getGameplayData();
	uint iPos = m_pProgressSlider->sliderPosition();

	GameplayData::AnswerValue eAnswer = pData->getReviewAnswer(GameplayData::Frustation, iPos);
	m_pQuestionnaire->setQuestionValue(GameplayData::Frustation, eAnswer);

	eAnswer = pData->getReviewAnswer(GameplayData::Involvement, iPos);
	m_pQuestionnaire->setQuestionValue(GameplayData::Involvement, eAnswer);

	eAnswer = pData->getReviewAnswer(GameplayData::Fun, iPos);
	m_pQuestionnaire->setQuestionValue(GameplayData::Fun, eAnswer);

	m_pQuestionnaire->show();
	m_pPlayPauseButton->setEnabled(false);
	m_pVolumeButton->setEnabled(false);
	m_pElapsedTime->setEnabled(false);
	m_pProgressSlider->setEnabled(false);
	m_pRemainingTime->setEnabled(false);

	m_bInQuestionnaire = true;
	VideoReviewer::onPlayPauseClicked();
}

// +-----------------------------------------------------------
void gc::VideoReviewer::hideQuestionnaire()
{
	m_pPlayPauseButton->setEnabled(true);
	m_pVolumeButton->setEnabled(true);
	m_pElapsedTime->setEnabled(true);
	m_pProgressSlider->setEnabled(true);
	m_pRemainingTime->setEnabled(true);
	m_pQuestionnaire->hide();

	m_pMediaPlayer->setPosition(m_pMediaPlayer->position() + 1000);
	m_bInQuestionnaire = false;
	VideoReviewer::onPlayPauseClicked();
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

	switch(eStatus)
	{
		case QMediaPlayer::UnknownMediaStatus:
		case QMediaPlayer::InvalidMedia:
			break;

		case QMediaPlayer::NoMedia:
		case QMediaPlayer::LoadingMedia:
		case QMediaPlayer::LoadedMedia:
		case QMediaPlayer::StalledMedia:
		case QMediaPlayer::BufferingMedia:
		case QMediaPlayer::BufferedMedia:
			break;

		case QMediaPlayer::EndOfMedia:
			m_pPlayPauseButton->setProperty("state", "paused");
			m_pPlayPauseButton->style()->unpolish(m_pPlayPauseButton);
			m_pPlayPauseButton->style()->polish(m_pPlayPauseButton);
			break;
	}
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onActionTriggered(int iAction)
{
	Q_UNUSED(iAction);
	m_pMediaPlayer->setPosition(m_pProgressSlider->sliderPosition() * 1000);
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onPlayPauseClicked()
{
	if (m_pPlayPauseButton->property("state") == "paused")
	{
		m_pMediaPlayer->play();
		m_pPlayPauseButton->setProperty("state", "playing");
	}
	else
	{
		m_pMediaPlayer->pause();
		m_pPlayPauseButton->setProperty("state", "paused");
	}
	m_pPlayPauseButton->style()->unpolish(m_pPlayPauseButton);
	m_pPlayPauseButton->style()->polish(m_pPlayPauseButton);
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onQuestionChanged(const uint iIndex, const Questionnaire::QuestionType eType, const QVariant oValue)
{
	GameplayData *pData = static_cast<Application*>(qApp)->getGameplayData();
	uint iPos = m_pProgressSlider->sliderPosition();

	GameplayData::ReviewQuestion eQuestion = static_cast<GameplayData::ReviewQuestion>(iIndex);
	GameplayData::AnswerValue eAnswer = static_cast<GameplayData::AnswerValue>(oValue.toInt());
	pData->setReviewAnswer(eQuestion, iPos, eAnswer);
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onQuestionnaireCompleted()
{
	hideQuestionnaire();
}