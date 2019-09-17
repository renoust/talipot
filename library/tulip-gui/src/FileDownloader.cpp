/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */
#include "tulip/FileDownloader.h"

#include <QEventLoop>

using namespace tlp;

FileDownloader::FileDownloader() : QObject() {
  connect(&_webCtrl, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(fileDownloaded(QNetworkReply *)));
}

void FileDownloader::fileDownloaded(QNetworkReply *pReply) {
  _downloadedData = pReply->readAll();
  pReply->deleteLater();
  emit downloaded();
}

const QByteArray &FileDownloader::download(const QUrl &url) {
  QNetworkRequest request(url);
  _webCtrl.get(request);
  QEventLoop loop;
  connect(this, SIGNAL(downloaded()), &loop, SLOT(quit()));
  loop.exec();
  return _downloadedData;
}
