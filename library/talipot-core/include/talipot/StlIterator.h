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
#include <talipot/memorypool.h>

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
    T tmp = *it;
    ++it;
    return tmp;
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
template <typename KEY, typename VALUE>
struct StlMapIterator : public Iterator<std::pair<KEY, VALUE>> {
  StlMapIterator(typename std::map<KEY, VALUE>::const_iterator startIt,
                 typename std::map<KEY, VALUE>::const_iterator endIt)
      : it(startIt), itEnd(endIt) {}

  std::pair<KEY, VALUE> next() {
    std::pair<KEY, VALUE> tmp = *it;
    ++it;
    return tmp;
  }

  bool hasNext() {
    return (itEnd != it);
  }

private:
  typename std::map<KEY, VALUE>::const_iterator it, itEnd;
};
//=================================================
template <typename KEY, typename VALUE>
struct StlMapKeyIterator : public tlp::Iterator<KEY> {
  StlMapKeyIterator(typename std::map<KEY, VALUE>::const_iterator startIt,
                    typename std::map<KEY, VALUE>::const_iterator endIt)
      : it(startIt), itEnd(endIt) {}

  KEY next() {
    const KEY tmp = it->first;
    ++it;
    return tmp;
  }

  bool hasNext() {
    return it != itEnd;
  }

private:
  typename std::map<KEY, VALUE>::const_iterator it, itEnd;
};
} // namespace tlp

#endif // TALIPOT_STL_ITERATOR_H
