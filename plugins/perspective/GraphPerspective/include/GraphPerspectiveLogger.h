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

#ifndef GRAPHPERSPECTIVELOGGER_H
#define GRAPHPERSPECTIVELOGGER_H

#include <QPixmap>
#include <QDebug>
#include <QDialog>
#include <QMap>
#include <QByteArray>

namespace Ui {
class GraphPerspectiveLogger;
}

class QShowEvent;
class QHideEvent;

class GraphPerspectiveLogger : public QDialog {
  Q_OBJECT

  QtMsgType _logType;
  Ui::GraphPerspectiveLogger *_ui;
  bool _pythonOutput;
  QMap<int, int> _logCounts;
  QByteArray _windowGeometry;
  bool _anchored;

public:
  enum LogType { Info, Warning, Error, Python };

  GraphPerspectiveLogger(QWidget *parent = nullptr);
  ~GraphPerspectiveLogger() override;
  QPixmap icon(LogType logType) const;
  int count() const;
  int countByType(LogType logType) const;
  GraphPerspectiveLogger::LogType getLastLogType() const;
  void log(QtMsgType, const QMessageLogContext &, const QString &);

  bool eventFilter(QObject *, QEvent *) override;

  void showEvent(QShowEvent *) override;
  void hideEvent(QHideEvent *) override;

  bool anchored() const {
    return _anchored;
  }

  void setGeometry(int x, int y, int w, int h);

public slots:
  void setAnchored(bool anchored);

  void clear();

private slots:
  void copy();
  void showContextMenu(const QPoint &pos);

private:
  void logImpl(QtMsgType, const QString &);

signals:
  void cleared();
  void resetLoggerPosition();
};

#endif // GRAPHPERSPECTIVELOGGER_H
