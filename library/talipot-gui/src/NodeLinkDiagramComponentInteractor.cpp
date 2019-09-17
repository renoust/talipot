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

#include <talipot/NodeLinkDiagramComponentInteractor.h>
#include <talipot/NodeLinkDiagramComponent.h>

#include <QLabel>
#include <QIcon>

using namespace tlp;

NodeLinkDiagramComponentInteractor::NodeLinkDiagramComponentInteractor(const QString &iconPath,
                                                                       const QString &text,
                                                                       unsigned int priority)
    : GLInteractorComposite(QIcon(iconPath), text), _label(new QLabel), _priority(priority) {
  _label->setWordWrap(true);
  _label->setAlignment(Qt::AlignTop);
  _label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

NodeLinkDiagramComponentInteractor::~NodeLinkDiagramComponentInteractor() {
  delete _label;
}

void NodeLinkDiagramComponentInteractor::setConfigurationWidgetText(const QString &text) {
  _label->setText(text);
}

QLabel *NodeLinkDiagramComponentInteractor::configurationDocWidget() const {
  return _label->text().isEmpty() ? nullptr : _label;
}

unsigned int NodeLinkDiagramComponentInteractor::priority() const {
  return _priority;
}

void NodeLinkDiagramComponentInteractor::setPriority(const unsigned int p) {
  _priority = p;
}
