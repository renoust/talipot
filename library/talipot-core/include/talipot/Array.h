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

#ifndef TALIPOT_ARRAY_H
#define TALIPOT_ARRAY_H

#include <array>
#include <iostream>

namespace tlp {

template <typename T, size_t N>
using Array = std::array<T, N>;

/**
 * @brief operator << stream operator to easily print an array, or save it to a file.
 * @param os The stream to output to.
 * @param array The array to output.
 * @return The stream to output to, to chain calls.
 */
template <typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const Array<T, N> &array);

/**
 * @brief operator >> stream operator to easily read an array
 * @param is The stream to read from.
 * @param array A reference to an array, that will be written to.
 * @return The stream to read from, to chain calls.
 */
template <typename T, size_t N>
std::istream &operator>>(std::istream &is, Array<T, N> &array);
}

#include "cxx/Array.cxx"

#endif // TALIPOT_ARRAY_H
