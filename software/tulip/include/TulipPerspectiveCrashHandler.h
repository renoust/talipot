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
#ifndef TULIPPERSPECTIVECRASHHANDLER_H
#define TULIPPERSPECTIVECRASHHANDLER_H

#include <QDialog>
#include "TulipPerspectiveProcessHandler.h"

class FormPost;

namespace Ui {
class TulipPerspectiveCrashHandlerData;
}

class TulipPerspectiveCrashHandler : public QDialog {
  Q_OBJECT

  Ui::TulipPerspectiveCrashHandlerData *_ui;
  PerspectiveProcessInfo _perspectiveInfo;
  bool _isDetailedView;
  FormPost *_poster;

public:
  explicit TulipPerspectiveCrashHandler(QWidget *parent = nullptr);
  ~TulipPerspectiveCrashHandler() override;

protected slots:
  void reportPosted();

signals:

public slots:
  void setDetailedView(bool);
  void toggleDetailedView();
  void sendReport();
  //  void saveData();
  void setEnvData(const QString &plateform, const QString &arch, const QString &compiler,
                  const QString &version, const QString &stackTrace);
  void setPerspectiveData(const PerspectiveProcessInfo &info);
};

#endif // TULIPPERSPECTIVECRASHHANDLER_H
