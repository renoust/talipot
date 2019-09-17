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

#ifndef TALIPOT_NODE_LINK_DIAGRAM_COMPONENT_INTERACTOR_H
#define TALIPOT_NODE_LINK_DIAGRAM_COMPONENT_INTERACTOR_H

#include <talipot/GLInteractor.h>

class QLabel;

namespace tlp {

/** @brief Interactor abstract class for NodeLinkDiagramComponent
 *
 */
class TLP_QT_SCOPE NodeLinkDiagramComponentInteractor : public GLInteractorComposite {
  QLabel *_label;
  unsigned int _priority;

public:
  NodeLinkDiagramComponentInteractor(const QString &iconPath, const QString &text,
                                     unsigned int priority = 0);

  ~NodeLinkDiagramComponentInteractor() override;

  void setConfigurationWidgetText(const QString &text);

  QLabel *configurationDocWidget() const override;

  unsigned int priority() const override;

  /**
   * @brief setPriority
   * @param p The priority of the interactor (aka its place in the interactor bar)
   * @deprecated The priority should not be changed dynamically. Use the third argument of the
   * constructor to initialize the priority.
   */
  _DEPRECATED void setPriority(const unsigned int p);
};
} // namespace tlp

#endif // TALIPOT_NODE_LINK_DIAGRAM_COMPONENT_INTERACTOR_H
