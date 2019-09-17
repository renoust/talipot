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
namespace tlp {

template <typename T>
void TulipItemDelegate::registerCreator(tlp::TulipItemEditorCreator *c) {
  int id = qMetaTypeId<T>();

  if (_creators.contains(id))
    return;

  _creators[id] = c;
}

template <typename T>
void TulipItemDelegate::unregisterCreator() {
  int id = qMetaTypeId<T>();

  if (_creators.contains(id)) {
    delete _creators[id];
    _creators.remove(id);
  }
}

template <typename T>
tlp::TulipItemEditorCreator *TulipItemDelegate::creator() const {
  return creator(qMetaTypeId<T>());
}
} // namespace tlp
