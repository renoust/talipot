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

#ifndef Tulip_GLMETANODERENDERER_H
#define Tulip_GLMETANODERENDERER_H

#include <tulip/tulipconf.h>
#include <tulip/Observable.h>
#include <unordered_map>

namespace tlp {

class GlGraphInputData;
class Camera;
class Graph;
class GlScene;

/**
 * Class used to render a meta node
 */
class TLP_GL_SCOPE GlMetaNodeRenderer : public Observable {

public:
  GlMetaNodeRenderer(GlGraphInputData *inputData);

  ~GlMetaNodeRenderer() override;

  virtual void render(node, float, Camera *);

  virtual void setInputData(GlGraphInputData *inputData);

  virtual GlGraphInputData *getInputData() const;

  GlScene *getSceneForMetaGraph(Graph *g) const;

protected:
  void clearScenes();

  void treatEvent(const Event &) override;

  virtual GlScene *createScene(Graph *) const;

private:
  GlGraphInputData *_inputData;
  std::unordered_map<Graph *, GlScene *> _metaGraphToSceneMap;
};
} // namespace tlp

#endif // Tulip_GLMETANODERENDERER_H
///@endcond
