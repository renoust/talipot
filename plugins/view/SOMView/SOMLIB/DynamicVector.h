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

#ifndef DYNAMIC_VECTOR_H
#define DYNAMIC_VECTOR_H

template <class T>
class DynamicVector {
public:
  DynamicVector() : array(nullptr), size(0) {}

  DynamicVector(const DynamicVector<T> &vect) {
    array = new T[vect.size];
    size = vect.size;

    for (unsigned int i = 0; i < vect.size; ++i) {
      array[i] = vect.array[i];
    }
  }

  DynamicVector(unsigned int size) : size(size) {
    array = new T[size];
  }
  virtual ~DynamicVector() {
    if (size != 0)
      delete[] array;
  }

  inline DynamicVector<T> &operator=(const DynamicVector<T> &);

  inline T operator[](const unsigned int i) const;
  inline T &operator[](const unsigned int i);

  inline DynamicVector<T> &operator*=(const T &);
  inline DynamicVector<T> &operator*=(const DynamicVector<T> &);
  inline DynamicVector<T> &operator/=(const T &);
  inline DynamicVector<T> &operator/=(const DynamicVector<T> &);
  inline DynamicVector<T> &operator+=(const T &);
  inline DynamicVector<T> &operator+=(const DynamicVector<T> &);
  inline DynamicVector<T> &operator-=(const T &);
  inline DynamicVector<T> &operator-=(const DynamicVector<T> &);
  inline DynamicVector<T> &operator^=(const DynamicVector<T> &);

  inline DynamicVector<T> operator+(const DynamicVector<T> &) const;
  inline DynamicVector<T> operator+(const T &) const;
  inline DynamicVector<T> operator-(const DynamicVector<T> &) const;
  inline DynamicVector<T> operator-(const T &) const;

  inline DynamicVector<T> operator/(const DynamicVector<T> &) const;
  inline DynamicVector<T> operator/(const T &) const;
  inline DynamicVector<T> operator^(const DynamicVector<T> &) const;

  inline bool operator!=(const DynamicVector<T> &) const;
  inline bool operator==(const DynamicVector<T> &) const;
  inline DynamicVector<T> &fill(const T &val);
  inline T norm() const;
  inline T dist(const DynamicVector<T> &) const;
  inline T dotProduct(const DynamicVector<T> &) const;

  inline unsigned int getSize() const {
    return size;
  }

protected:
  T *array;
  unsigned int size;
};

template <class T>
std::ostream &operator<<(std::ostream &os, const DynamicVector<T> &);
template <class T>
std::istream &operator>>(std::istream &is, DynamicVector<T> &);

#include "DynamicVector.cxx"
#endif // DYNAMIC_VECTOR_H
