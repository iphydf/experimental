/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "videosettings.h"

#include <QComboBox>
#include <QDebug>
#include <QMediaRecorder>
#include <QMediaService>

#include "ui_videosettings.h"

VideoSettings::VideoSettings(QMediaRecorder *mediaRecorder, QWidget *parent)
    : QDialog(parent), ui_(new Ui::VideoSettingsUi), mediaRecorder_(mediaRecorder) {
  ui_->setupUi(this);

  // audio codecs
  ui_->audioCodecBox->addItem(tr("Default audio codec"), QVariant(QString()));
  foreach (const QString &codecName, mediaRecorder_->supportedAudioCodecs()) {
    QString description = mediaRecorder_->audioCodecDescription(codecName);
    ui_->audioCodecBox->addItem(codecName + ": " + description, QVariant(codecName));
  }

  // sample rate:
  foreach (int sampleRate, mediaRecorder_->supportedAudioSampleRates()) {
    ui_->audioSampleRateBox->addItem(QString::number(sampleRate), QVariant(sampleRate));
  }

  ui_->audioQualitySlider->setRange(0, int(QMultimedia::VeryHighQuality));

  // video codecs
  ui_->videoCodecBox->addItem(tr("Default video codec"), QVariant(QString()));
  foreach (const QString &codecName, mediaRecorder_->supportedVideoCodecs()) {
    QString description = mediaRecorder_->videoCodecDescription(codecName);
    ui_->videoCodecBox->addItem(codecName + ": " + description, QVariant(codecName));
  }

  ui_->videoQualitySlider->setRange(0, int(QMultimedia::VeryHighQuality));

  ui_->videoResolutionBox->addItem(tr("Default"));
  QList<QSize> supportedResolutions = mediaRecorder_->supportedResolutions();
  foreach (const QSize &resolution, supportedResolutions) {
    ui_->videoResolutionBox->addItem(
        QString("%1x%2").arg(resolution.width()).arg(resolution.height()), QVariant(resolution));
  }

  ui_->videoFramerateBox->addItem(tr("Default"));
  QList<qreal> supportedFrameRates = mediaRecorder_->supportedFrameRates();
  qreal rate;
  foreach (rate, supportedFrameRates) {
    QString rateString = QString("%1").arg(rate, 0, 'f', 2);
    ui_->videoFramerateBox->addItem(rateString, QVariant(rate));
  }

  // containers
  ui_->containerFormatBox->addItem(tr("Default container"), QVariant(QString()));
  foreach (const QString &format, mediaRecorder_->supportedContainers()) {
    ui_->containerFormatBox->addItem(format + ":" + mediaRecorder_->containerDescription(format),
                                     QVariant(format));
  }
}

VideoSettings::~VideoSettings() { delete ui_; }

void VideoSettings::changeEvent(QEvent *e) {
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui_->retranslateUi(this);
      break;
    default:
      break;
  }
}

QAudioEncoderSettings VideoSettings::audioSettings() const {
  QAudioEncoderSettings settings = mediaRecorder_->audioSettings();
  settings.setCodec(boxValue(ui_->audioCodecBox).toString());
  settings.setQuality(QMultimedia::EncodingQuality(ui_->audioQualitySlider->value()));
  settings.setSampleRate(boxValue(ui_->audioSampleRateBox).toInt());
  return settings;
}

void VideoSettings::setAudioSettings(const QAudioEncoderSettings &audioSettings) {
  selectComboBoxItem(ui_->audioCodecBox, QVariant(audioSettings.codec()));
  selectComboBoxItem(ui_->audioSampleRateBox, QVariant(audioSettings.sampleRate()));
  ui_->audioQualitySlider->setValue(audioSettings.quality());
}

QVideoEncoderSettings VideoSettings::videoSettings() const {
  QVideoEncoderSettings settings = mediaRecorder_->videoSettings();
  settings.setCodec(boxValue(ui_->videoCodecBox).toString());
  settings.setQuality(QMultimedia::EncodingQuality(ui_->videoQualitySlider->value()));
  settings.setResolution(boxValue(ui_->videoResolutionBox).toSize());
  settings.setFrameRate(boxValue(ui_->videoFramerateBox).value<qreal>());

  return settings;
}

void VideoSettings::setVideoSettings(const QVideoEncoderSettings &videoSettings) {
  selectComboBoxItem(ui_->videoCodecBox, QVariant(videoSettings.codec()));
  selectComboBoxItem(ui_->videoResolutionBox, QVariant(videoSettings.resolution()));
  ui_->videoQualitySlider->setValue(videoSettings.quality());

  // special case for frame rate
  for (int i = 0; i < ui_->videoFramerateBox->count(); ++i) {
    qreal itemRate = ui_->videoFramerateBox->itemData(i).value<qreal>();
    if (qFuzzyCompare(itemRate, videoSettings.frameRate())) {
      ui_->videoFramerateBox->setCurrentIndex(i);
      break;
    }
  }
}

QString VideoSettings::format() const { return boxValue(ui_->containerFormatBox).toString(); }

void VideoSettings::setFormat(const QString &format) {
  selectComboBoxItem(ui_->containerFormatBox, QVariant(format));
}

QVariant VideoSettings::boxValue(const QComboBox *box) const {
  int idx = box->currentIndex();
  if (idx == -1) return QVariant();

  return box->itemData(idx);
}

void VideoSettings::selectComboBoxItem(QComboBox *box, const QVariant &value) {
  for (int i = 0; i < box->count(); ++i) {
    if (box->itemData(i) == value) {
      box->setCurrentIndex(i);
      break;
    }
  }
}
