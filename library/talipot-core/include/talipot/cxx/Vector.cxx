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

#define VECTORTLP tlp::Vector<TYPE, SIZE, OTYPE, DTYPE>

//======================================================
TEMPLATEVECTOR
inline VECTORTLP &VECTORTLP::operator*=(const TYPE scalaire) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] *= scalaire;

  return (*this);
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP &VECTORTLP::operator*=(const VECTORTLP &vecto) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] *= vecto[i];

  return (*this);
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP &VECTORTLP::operator/=(const TYPE scalaire) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] /= scalaire;

  return (*this);
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP &VECTORTLP::operator/=(const VECTORTLP &vecto) {
  for (size_t i = 0; i < SIZE; ++i) {
    (*this)[i] /= vecto[i];
  }

  return (*this);
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP &VECTORTLP::operator+=(const TYPE scalaire) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] += scalaire;

  return (*this);
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP &VECTORTLP::operator+=(const VECTORTLP &vecto) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] += vecto[i];

  return (*this);
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP &VECTORTLP::operator-=(const TYPE scalaire) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] -= scalaire;

  return (*this);
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP &VECTORTLP::operator-=(const VECTORTLP &vecto) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] -= vecto[i];

  return (*this);
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP &VECTORTLP::operator^=(const VECTORTLP &v) {
  (*this) = (*this) ^ v;
  return (*this);
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator+(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) += v;
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator+(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u) += scalaire;
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator-(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) -= v;
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator-(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u) -= scalaire;
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator*(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) *= v;
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator*(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u) *= scalaire;
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator*(const TYPE scalaire, const VECTORTLP &u) {
  return VECTORTLP(u) *= scalaire;
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator/(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) /= v;
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator/(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u) /= scalaire;
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator^(const VECTORTLP &u, const VECTORTLP &v) {

  switch (SIZE) {
  case 3:
    return VECTORTLP(static_cast<TYPE>(static_cast<OTYPE>(u[1]) * static_cast<OTYPE>(v[2]) -
                                       static_cast<OTYPE>(u[2]) * static_cast<OTYPE>(v[1])),
                     static_cast<TYPE>(static_cast<OTYPE>(u[2]) * static_cast<OTYPE>(v[0]) -
                                       static_cast<OTYPE>(u[0]) * static_cast<OTYPE>(v[2])),
                     static_cast<TYPE>(static_cast<OTYPE>(u[0]) * static_cast<OTYPE>(v[1]) -
                                       static_cast<OTYPE>(u[1]) * static_cast<OTYPE>(v[0])));
    break;

  default:
    tlp::warning() << "cross product not implemented for dimension :" << SIZE << std::endl;
    VECTORTLP result;
    return result;
    break;
  }
}
//======================================================
TEMPLATEVECTOR
inline VECTORTLP tlp::operator-(const VECTORTLP &u) {
  return VECTORTLP(u) *= static_cast<TYPE>(-1);
}
//======================================================
TEMPLATEVECTOR
inline bool VECTORTLP::operator>(const VECTORTLP &vecto) const {
  return vecto < (*this);
}
//======================================================
TEMPLATEVECTOR
inline bool VECTORTLP::operator<(const VECTORTLP &v) const {
  if (std::numeric_limits<TYPE>::is_integer) {
    return std::memcmp(this->data(), v.data(), SIZE * sizeof(TYPE)) < 0;
  }

  for (size_t i = 0; i < SIZE; ++i) {
    OTYPE tmp = static_cast<OTYPE>((*this)[i]) - static_cast<OTYPE>(v[i]);

    if (tmp > std::sqrt(std::numeric_limits<TYPE>::epsilon()) ||
        tmp < -std::sqrt(std::numeric_limits<TYPE>::epsilon())) {
      if (tmp > 0)
        return false;

      if (tmp < 0)
        return true;
    }
  }

  return false;
}
//======================================================
TEMPLATEVECTOR
inline bool VECTORTLP::operator!=(const VECTORTLP &vecto) const {
  return (!((*this) == vecto));
}
//======================================================
TEMPLATEVECTOR
inline bool VECTORTLP::operator==(const VECTORTLP &v) const {
  if (std::numeric_limits<TYPE>::is_integer) {
    return std::memcmp(this->data(), v.data(), SIZE * sizeof(TYPE)) == 0;
  }

  for (size_t i = 0; i < SIZE; ++i) {
    OTYPE tmp = static_cast<OTYPE>((*this)[i]) - static_cast<OTYPE>(v[i]);

    if (tmp > std::sqrt(std::numeric_limits<TYPE>::epsilon()) ||
        tmp < -std::sqrt(std::numeric_limits<TYPE>::epsilon())) {
      return false;
    }
  }

  return true;
}
//======================================================
TEMPLATEVECTOR
inline TYPE VECTORTLP::dotProduct(const VECTORTLP &v) const {
  assert(SIZE > 0);
  OTYPE tmpO = static_cast<OTYPE>((*this)[0]) * static_cast<OTYPE>(v[0]);

  for (size_t i = 1; i < SIZE; ++i)
    tmpO += static_cast<OTYPE>((*this)[i]) * static_cast<OTYPE>(v[i]);

  return static_cast<TYPE>(tmpO);
}
//======================================================
TEMPLATEVECTOR
inline TYPE VECTORTLP::norm() const {
  switch (SIZE) {
  case 1:
    return (*this)[0];

  case 2:
    return tlpsqrt<TYPE, OTYPE>(tlpsqr<TYPE, OTYPE>((*this)[0]) + tlpsqr<TYPE, OTYPE>((*this)[1]));

  case 3:
    return tlpsqrt<TYPE, OTYPE>(tlpsqr<TYPE, OTYPE>((*this)[0]) + tlpsqr<TYPE, OTYPE>((*this)[1]) +
                                tlpsqr<TYPE, OTYPE>((*this)[2]));

  default:
    OTYPE tmp = tlpsqr<TYPE, OTYPE>((*this)[0]);

    for (size_t i = 1; i < SIZE; ++i)
      tmp += tlpsqr<TYPE, OTYPE>((*this)[i]);

    return (tlpsqrt<TYPE, OTYPE>(tmp));
  }
}
//======================================================
TEMPLATEVECTOR
inline DTYPE VECTORTLP::dist(const VECTOR &c) const {
  switch (SIZE) {
  case 1:
    return static_cast<TYPE>(fabs((*this)[0] - c[0]));

  case 2:
    return tlpsqrt<DTYPE, OTYPE>(tlpsqr<DTYPE, OTYPE>((*this)[0] - c[0]) +
                                 tlpsqr<DTYPE, OTYPE>((*this)[1] - c[1]));

  case 3:
    return tlpsqrt<DTYPE, OTYPE>(tlpsqr<DTYPE, OTYPE>((*this)[0] - c[0]) +
                                 tlpsqr<DTYPE, OTYPE>((*this)[1] - c[1]) +
                                 tlpsqr<DTYPE, OTYPE>((*this)[2] - c[2]));

  default:
    OTYPE tmp = tlpsqr<DTYPE, OTYPE>((*this)[0] - c[0]);

    for (size_t i = 1; i < SIZE; ++i)
      tmp += tlpsqr<DTYPE, OTYPE>((*this)[i] - c[i]);

    return (tlpsqrt<DTYPE, OTYPE>(tmp));
  }
}

#undef VECTORTLP
