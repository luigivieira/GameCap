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
	
	m_pProgressSlider = new ProgressSlider(this);
	pControlsLayout->addWidget(m_pProgressSlider);
	connect(m_pProgressSlider, &QSlider::actionTriggered, this, &VideoReviewer::onActionTriggered);

	m_pRemainingTime = new QLabel("00:00", this);
	pControlsLayout->addWidget(m_pRemainingTime);

	// Create the sample questionnaire
	m_pQuestionnaire = new Questionnaire(this);
	pLayout->addWidget(m_pQuestionnaire);

	m_pQuestionnaire->addQuestion(Questionnaire::Likert, 5);
	m_pQuestionnaire->addQuestion(Questionnaire::Likert, 5);
	m_pQuestionnaire->addQuestion(Questionnaire::Likert, 5);

	connect(m_pQuestionnaire, &Questionnaire::questionChanged, this, &VideoReviewer::onQuestionChanged);
	connect(m_pQuestionnaire, &Questionnaire::completed, this, &VideoReviewer::onQuestionnaireCompleted);

	m_pQuestionnaire->hide();
	m_pData = static_cast<Application*>(qApp)->getGameplayData();
}

// +-----------------------------------------------------------
void gc::VideoReviewer::initializeQuestionnaire()
{
	m_pQuestionnaire->setTitle(tr("How were you feeling at that time?"));
	m_pQuestionnaire->setDescription(tr("Please indicate how you were feeling while playing the game at the time the video is paused, for each of the following items."));

	m_pQuestionnaire->setQuestionTitle(0, tr("1. I was feeling frustrated"));
	m_pQuestionnaire->setQuestionTitle(1, tr("2. I was feeling involved"));
	m_pQuestionnaire->setQuestionTitle(2, tr("3. I was having fun"));

	QStringList lOptions = {
		tr("not at all"),
		tr("slightly"),
		tr("moderately"),
		tr("fairly"),
		tr("extremely")
	};
	for(uint i = 0; i < m_pQuestionnaire->getNumberOfQuestions(); i++)
		m_pQuestionnaire->setLikertOptionTitles(i, lOptions);
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
void gc::VideoReviewer::stopVideo()
{
	m_pMediaPlayer->stop();
	m_pMediaPlayer->setMedia(QMediaContent());
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
	
	if(m_pData->getReviewTimestamps().contains(iPos))
	{
		if(!m_pQuestionnaire->isVisible())
			QTimer::singleShot(10, this, &VideoReviewer::showQuestionnaire);
	}
	else
	{
		if(m_pQuestionnaire->isVisible())
			m_pQuestionnaire->hide();
	}

}

// +-----------------------------------------------------------
void gc::VideoReviewer::showQuestionnaire()
{
	m_ePreviousState = m_pMediaPlayer->state();
	pauseVideo();

	uint iPos = m_pProgressSlider->sliderPosition();

	GameplayData::AnswerValue eAnswer = m_pData->getReviewAnswer(GameplayData::Frustation, iPos);
	m_pQuestionnaire->setQuestionValue(GameplayData::Frustation, (eAnswer == GameplayData::Undefined) ? -1 : eAnswer + 2);

	eAnswer = m_pData->getReviewAnswer(GameplayData::Involvement, iPos);
	m_pQuestionnaire->setQuestionValue(GameplayData::Involvement, (eAnswer == GameplayData::Undefined) ? -1 : eAnswer + 2);

	eAnswer = m_pData->getReviewAnswer(GameplayData::Fun, iPos);
	m_pQuestionnaire->setQuestionValue(GameplayData::Fun, (eAnswer == GameplayData::Undefined) ? -1 : eAnswer + 2);

	m_pQuestionnaire->show();
	m_pQuestionnaire->setFocus();
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onError(QMediaPlayer::Error eError)
{
	char *sMsg;
	switch(eError)
	{
		case QMediaPlayer::ResourceError:
			sMsg = "Resource Error";
			break;

		case QMediaPlayer::FormatError:
			sMsg = "Format Error";
			break;

		case QMediaPlayer::NetworkError:
			sMsg = "Network Error";
			break;

		case QMediaPlayer::AccessDeniedError:
			sMsg = "Access Denied";
			break;

		case QMediaPlayer::ServiceMissingError:
			sMsg = "Service Missing";
			break;

		case QMediaPlayer::MediaIsPlaylist:
			sMsg = "Media Is Playlist";
			break;

		default:
			sMsg = "Unknown Error";
			break;
	}
	qFatal("Media error: %s", sMsg);
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onMediaStatusChanged(QMediaPlayer::MediaStatus eStatus)
{
	qDebug() << "onMediaStatusChanged(" << eStatus << ")";

	switch(eStatus)
	{
		case QMediaPlayer::UnknownMediaStatus:
		case QMediaPlayer::InvalidMedia:
			qFatal("Media error: Invalid Media");
			break;

		case QMediaPlayer::EndOfMedia:
			pauseVideo();
			break;

		default:
			break;
	}
}

// +-----------------------------------------------------------
void gc::VideoReviewer::playVideo()
{
	if(m_pMediaPlayer->state() != QMediaPlayer::PlayingState)
	{
		m_pMediaPlayer->play();
		m_pPlayPauseButton->setProperty("state", "playing");
		m_pPlayPauseButton->style()->unpolish(m_pPlayPauseButton);
		m_pPlayPauseButton->style()->polish(m_pPlayPauseButton);
	}
}

// +-----------------------------------------------------------
void gc::VideoReviewer::pauseVideo()
{
	if(m_pMediaPlayer->state() == QMediaPlayer::PlayingState)
	{
		m_pMediaPlayer->pause();
		m_pPlayPauseButton->setProperty("state", "paused");
		m_pPlayPauseButton->style()->unpolish(m_pPlayPauseButton);
		m_pPlayPauseButton->style()->polish(m_pPlayPauseButton);
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
	if(m_pMediaPlayer->state() != QMediaPlayer::PlayingState)
		playVideo();
	else
		pauseVideo();
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onQuestionChanged(const uint iIndex, const Questionnaire::QuestionType eType, const QVariant oValue)
{
	uint iPos = m_pProgressSlider->sliderPosition();

	GameplayData::ReviewQuestion eQuestion = static_cast<GameplayData::ReviewQuestion>(iIndex);
	GameplayData::AnswerValue eAnswer = static_cast<GameplayData::AnswerValue>(oValue.toInt() - 2);
	m_pData->setReviewAnswer(eQuestion, iPos, eAnswer);
}

// +-----------------------------------------------------------
void gc::VideoReviewer::onQuestionnaireCompleted()
{
	if(m_pData->isReviewCompleted())
		emit reviewCompleted();

	if(m_ePreviousState == QMediaPlayer::PlayingState)
		playVideo();
}