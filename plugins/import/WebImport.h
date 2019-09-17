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

#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

#include <string>

class HttpContext : public QObject {
  Q_OBJECT
public:
  bool status;
  int code;
  QNetworkReply *reply;
  bool processed;
  bool redirected;
  bool isHtml;
  std::string data;

  HttpContext();
  ~HttpContext() override;
  void setTimer(QTimer *timer);
  void request(const std::string &url, bool header = false);

public slots:
  void finished();
  void headerReceived();
  void timeout();
};
