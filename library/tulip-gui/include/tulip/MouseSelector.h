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
///@cond DOXYGEN_HIDDEN

#ifndef MOUSESELECTION_H
#define MOUSESELECTION_H

#include <tulip/GLInteractor.h>

namespace tlp {

class Graph;
class TLP_QT_SCOPE MouseSelector : public GLInteractorComponent {
public:
  enum SelectionMode { EdgesAndNodes = 0, EdgesOnly, NodesOnly };

  MouseSelector(Qt::MouseButton button = Qt::LeftButton,
                Qt::KeyboardModifier modifier = Qt::NoModifier, SelectionMode mode = EdgesAndNodes);
  ~MouseSelector() override {}
  bool draw(GlMainWidget *) override;
  bool eventFilter(QObject *, QEvent *) override;

protected:
  Qt::MouseButton mButton;
  Qt::KeyboardModifier kModifier;
  Qt::KeyboardModifiers mousePressModifier;
  unsigned int x, y;
  int w, h;
  bool started;
  Graph *graph;
  SelectionMode _mode;
};
} // namespace tlp
#endif
///@endcond
