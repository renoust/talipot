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

#ifndef MOUSEADDEDGE_H
#define MOUSEADDEDGE_H

#include <vector>
#include <tulip/Coord.h>
#include <tulip/GLInteractor.h>
#include <tulip/Observable.h>

namespace tlp {
class Graph;
class LayoutProperty;
class GlMainWidget;

class TLP_QT_SCOPE MouseEdgeBuilder : public GLInteractorComponent, public Observable {
private:
  node _source;
  bool _started;
  Coord _startPos, _curPos;
  std::vector<Coord> _bends;
  Graph *_graph;
  LayoutProperty *_layoutProperty;
  GlMainWidget *glMainWidget;

protected:
  /**
   * @brief addLink Adds a link between a source and destination nodes. Adds an edge by default
   * @param source
   * @param dest
   */
  virtual void addLink(const node &source, const node &dest);
  /**
   * @brief source gets the source node.
   * @return
   */
  node source() const {
    return _source;
  }
  /**
   * @brief bends returns the bends list.
   * @return
   */
  const std::vector<Coord> &bends() const {
    return _bends;
  }
  /**
   * @brief setBends updates the bends list.
   * @param bends
   */
  void setBends(const std::vector<Coord> &bends) {
    _bends = bends;
  }

public:
  MouseEdgeBuilder();
  bool draw(GlMainWidget *) override;
  bool eventFilter(QObject *, QEvent *) override;
  void initObserver(Graph *);
  void clearObserver();

  void treatEvent(const Event &) override;

  void clear() override;
};
} // namespace tlp
#endif
///@endcond
