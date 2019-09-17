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

#ifndef Tulip_FOREACH_H
#define Tulip_FOREACH_H

#include <cassert>
#include <tulip/Iterator.h>
#include <tulip/StableIterator.h>
#include <tulip/tulipconf.h>

///@cond DOXYGEN_HIDDEN
namespace tlp {
/**
 * @brief Encapsulation of a Tulip Iterator intended to be allocated on the stack instead of the
 *heap,
 * so it gets deleted when out of scope.
 *
 **/
template <typename TYPE>
struct _TLP_IT {
  _TLP_IT(Iterator<TYPE> *_it) : _it(_it) {}
  ~_TLP_IT() {
    delete _it;
  }
  Iterator<TYPE> *_it;
};

/**
 * @brief
 **/
template <typename TYPE>
inline bool _tlp_if_test(TYPE &n, _TLP_IT<TYPE> &_it) {
  assert(_it._it != nullptr);

  if (_it._it->hasNext()) {
    n = _it._it->next();
    return true;
  } else {
    return false;
  }
}
} // namespace tlp
///@endcond

/**
 * @brief Allows to iterate the nodes or edges of a Graph in a clear and concise way.
 * It also avoid having to manage a tulip Iterator, whose deletion is often forgotten.
 *
 * This code shows how forEach can be used instead of an Iterator to iterate a Graph's nodes
 * @code
 * node n;
 * forEach(n, graph->getNodes()) {
 *   // Do something with node n
 * }
 * @endcode
 *
 * This macro can be used with any Iterator subclass as it's based on the existence of the next()
 * and hasNext() methods
 */
#define forEach(A, B)                                                                              \
  for (tlp::_TLP_IT<decltype(A)> _it_foreach(B); tlp::_tlp_if_test(A, _it_foreach);)

/**
 * @brief Allows to iterate the nodes or edges of a copy of a Graph in a clear and concise way.
 * The stable Iterator creates a copy of the Graph, and iterates on the said copy.
 * It allows deletion operations to be performed without invalidating the iterator.
 * It also avoid having to manage a tulip Iterator, whose deletion is often forgotten.
 */
#define stableForEach(A, B)                                                                        \
  for (tlp::_TLP_IT<decltype(A)> _it_foreach(new tlp::StableIterator<decltype(A)>(B));             \
       tlp::_tlp_if_test(A, _it_foreach);)

#endif
