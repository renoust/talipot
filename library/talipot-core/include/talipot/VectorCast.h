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

//@TLPGEOLICENCE#

#ifndef TALIPOT_VECTOR_CAST_H
#define TALIPOT_VECTOR_CAST_H

#include <cassert>
#include <talipot/Vector.h>

#define TEMPLATEVECTORCAST template <typename Obj, unsigned int SIZE, typename TYPE>

#define VECTORCAST tlp::VectorCast<Obj, SIZE, TYPE>

namespace tlp {
/**
 * @ingroup Structures
 * \brief class for mathematical vector
 *
 * Enables to create a Vector of Obj with a
 * fixed size and provides Mathematical operation. Mathematical
 * operators must be defined for Obj. Out of bound accesses are only checked
 * in debug mode.
 *
 * \author : David Auber auber@labri.fr
 * \version 0.0.1 24/01/2003
 */
TEMPLATEVECTORCAST
class TLP_SCOPE VectorCast : public Vector<Obj, SIZE> {
public:
  VectorCast() {}
  inline explicit VectorCast(const tlp::Vector<float, 3> &v) : Vector<Obj, SIZE>(v) {}

  inline TYPE &operator*=(const Obj obj) {
    return (TYPE &)this->Vector<Obj, SIZE>::operator*=(obj);
  }
  inline TYPE &operator*=(const Vector<Obj, SIZE> &vec) {
    return (TYPE &)this->Vector<Obj, SIZE>::operator*=(vec);
  }
  inline TYPE &operator/=(const Obj obj) {
    return (TYPE &)this->Vector<Obj, SIZE>::operator/=(obj);
  }
  inline TYPE &operator/=(const Vector<Obj, SIZE> &vec) {
    return (TYPE &)this->Vector<Obj, SIZE>::operator/=(vec);
  }
  inline TYPE &operator+=(const Obj obj) {
    return (TYPE &)this->Vector<Obj, SIZE>::operator+=(obj);
  }
  inline TYPE &operator+=(const Vector<Obj, SIZE> &vec) {
    return (TYPE &)this->Vector<Obj, SIZE>::operator+=(vec);
  }
  inline TYPE &operator-=(const Obj obj) {
    return (TYPE &)this->Vector<Obj, SIZE>::operator-=(obj);
  }
  inline TYPE &operator-=(const Vector<Obj, SIZE> &vec) {
    return (TYPE &)this->Vector<Obj, SIZE>::operator-=(vec);
  }
  inline TYPE &operator^=(const Vector<Obj, SIZE> &vec) {
    return (TYPE &)this->Vector<Obj, SIZE>::operator^=(vec);
  }
  inline TYPE &fill(const Obj obj) {
    return (TYPE &)this->Vector<Obj, SIZE>::fill(obj);
  }

  inline TYPE operator*(const Vector<Obj, SIZE> &v) const {
    return VECTORCAST(*this) *= v;
  }
  inline TYPE operator*(const Obj v) const {
    return VECTORCAST(*this) *= v;
  }
  inline TYPE operator+(const Vector<Obj, SIZE> &v) const {
    return VECTORCAST(*this) += v;
  }
  inline TYPE operator+(const Obj v) const {
    return VECTORCAST(*this) += v;
  }
  inline TYPE operator-(const Vector<Obj, SIZE> &v) const {
    return VECTORCAST(*this) -= v;
  }
  inline TYPE operator-(const Obj v) const {
    return VECTORCAST(*this) -= v;
  }
  inline TYPE operator/(const Vector<Obj, SIZE> &v) const {
    return VECTORCAST(*this) /= v;
  }
  inline TYPE operator/(const Obj v) const {
    return VECTORCAST(*this) /= v;
  }
  inline TYPE operator^(const Vector<Obj, SIZE> &v) const {
    return VECTORCAST(*this) ^= v;
  }
};
}

#endif // TALIPOT_VECTOR_CAST_H
///@endcond
