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

#ifndef TALIPOT_STL_ITERATOR_H
#define TALIPOT_STL_ITERATOR_H

#include <map>
#include <type_traits>

#include <talipot/Iterator.h>
#include <talipot/MemoryPool.h>

namespace tlp {

/**
 * @class StlIterator
 * @ingroup Iterators
 *
 * @brief StlIterator wraps a stl iterator.
 *
 * Creates a Talipot iterator from two stl iterators.
 *
 * @param startIt a stl iterator from which to begin the iteration
 * @param endIt a stl iterator on which to end the iteration
 *
 **/
template <typename T, typename ITERATOR>
struct StlIterator : public Iterator<T> {
  StlIterator(const ITERATOR &startIt, const ITERATOR &endIt) : it(startIt), itEnd(endIt) {}
  T next() {
    return *it++;
  }
  bool hasNext() {
    return (itEnd != it);
  }

private:
  ITERATOR it, itEnd;
};
//=================================================
template <typename T, typename ITERATOR>
struct MPStlIterator : public StlIterator<T, ITERATOR>,
                       public MemoryPool<MPStlIterator<T, ITERATOR>> {
  MPStlIterator(const ITERATOR &startIt, const ITERATOR &endIt)
      : StlIterator<T, ITERATOR>(startIt, endIt) {}
};
//=================================================

// Helper to determine whether there's a const_iterator for T.
template <typename T>
struct has_const_iterator {
private:
  template <typename C>
  static char test(typename C::const_iterator *);
  template <typename C>
  static int test(...);

public:
  enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

/**
 * @brief Convenient function for creating a StlIterator from a stl container.
 * @ingroup Iterators
 *
 * Creates a StlIterator from a STL container (std::list, std::vector, std::set, std::map, ...).
 *
 * @param stlContainer any STL container
 * @return a StlIterator
 **/
template <typename Container>
typename std::enable_if<
    has_const_iterator<Container>::value,
    StlIterator<typename Container::value_type, typename Container::const_iterator>
        *>::type inline stlIterator(const Container &stlContainer) {
  return new MPStlIterator<typename Container::value_type, typename Container::const_iterator>(
      stlContainer.begin(), stlContainer.end());
}

//=================================================
template <typename Map>
struct StlMapIterator
    : public Iterator<std::pair<typename Map::key_type, typename Map::mapped_type>> {
  StlMapIterator(typename Map::const_iterator startIt, typename Map::const_iterator endIt)
      : it(startIt), itEnd(endIt) {}

  std::pair<typename Map::key_type, typename Map::mapped_type> next() {
    return *it++;
  }

  bool hasNext() {
    return (itEnd != it);
  }

private:
  typename Map::const_iterator it, itEnd;
};
//=================================================
template <typename Map>
typename std::enable_if<has_const_iterator<Map>::value,
                        StlMapIterator<Map> *>::type inline stlMapIterator(const Map &map) {
  return new StlMapIterator<Map>(map.begin(), map.end());
}
//=================================================
template <typename Map>
struct StlMapKeyIterator : public tlp::Iterator<typename Map::key_type> {
  StlMapKeyIterator(typename Map::const_iterator startIt, typename Map::const_iterator endIt)
      : it(startIt), itEnd(endIt) {}

  typename Map::key_type next() {
    auto tmp = it->first;
    ++it;
    return tmp;
  }

  bool hasNext() {
    return it != itEnd;
  }

private:
  typename Map::const_iterator it, itEnd;
};
//=================================================
template <typename Map>
typename std::enable_if<has_const_iterator<Map>::value,
                        StlMapKeyIterator<Map> *>::type inline stlMapKeyIterator(const Map &map) {
  return new StlMapKeyIterator<Map>(map.begin(), map.end());
}
}

#endif // TALIPOT_STL_ITERATOR_H
