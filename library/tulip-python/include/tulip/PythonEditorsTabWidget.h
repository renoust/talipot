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

#ifndef PYTHONEDITORSTABWIDGET_H
#define PYTHONEDITORSTABWIDGET_H

#include <QMap>
#include <QTabWidget>
#include <QTabBar>

#include <tulip/tulipconf.h>

namespace tlp {

class PythonCodeEditor;

class TLP_PYTHON_SCOPE PythonEditorsTabWidget : public QTabWidget {

  Q_OBJECT

  int _fontZoom;
  bool _dontTreatFocusIn;
  bool reloadCodeInEditorIfNeeded(int index);

public:
  explicit PythonEditorsTabWidget(QWidget *parent = nullptr);

  int addEditor(const QString &fileName = "");

  PythonCodeEditor *getCurrentEditor() const;

  PythonCodeEditor *getEditor(int) const;

  void indicateErrors(const QMap<QString, QVector<int>> &errorLines);

  void clearErrorIndicators();

  bool eventFilter(QObject *, QEvent *) override;

  void saveCurrentEditorContentToFile();

  void saveEditorContentToFile(int);

  void increaseFontSize();

  void decreaseFontSize();

  QTabBar *tabBar() const;

signals:

  void tabAboutToBeDeleted(int);

  void fileSaved(int);

  void filesReloaded();

public slots:

  void scriptTextChanged();

  void reloadCodeInEditorsIfNeeded();

  void closeTabRequested(int tab);
};
} // namespace tlp

#endif // PYTHONEDITORSTABWIDGET_H
