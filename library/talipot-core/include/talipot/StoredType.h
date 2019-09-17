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

#ifndef TALIPOT_STORED_TYPE_H
#define TALIPOT_STORED_TYPE_H

namespace tlp {

// the template below defines how are returned and stored
// the types of value a MutableContainer can managed
// basically a type is returned and stored by value
template <typename TYPE>
struct StoredType {
  // the type of the stored value
  typedef TYPE Value;
  // the type of the returned value
  typedef TYPE ReturnedValue;
  // the type of a returned const value
  typedef TYPE ReturnedConstValue;
  // indicates if a pointer to the value is stored
  enum { isPointer = 0 };
  // simply get
  inline static TYPE &get(const TYPE &val) {
    return const_cast<TYPE &>(val);
  }
  // equallity test
  inline static bool equal(const TYPE &val1, const TYPE &val2) {
    return val2 == val1;
  }
  // cloning before storage
  inline static Value clone(const TYPE &val) {
    return val;
  }
  // destruction of stored value
  inline static void destroy(Value) {}
  // the default value of that type
  inline static Value defaultValue() {
    return static_cast<Value>(0);
  }
};

#define DECL_STORED_PTR(T)                                                                         \
  template <>                                                                                      \
  struct StoredType<T> {                                                                           \
    typedef T Value;                                                                               \
    typedef T ReturnedValue;                                                                       \
    typedef const T ReturnedConstValue;                                                            \
                                                                                                   \
    enum { isPointer = 1 };                                                                        \
                                                                                                   \
    inline static T get(T val) {                                                                   \
      return val;                                                                                  \
    }                                                                                              \
                                                                                                   \
    inline static bool equal(const T val1, const T val2) {                                         \
      return val2 == val1;                                                                         \
    }                                                                                              \
                                                                                                   \
    inline static T clone(T val) {                                                                 \
      return val;                                                                                  \
    }                                                                                              \
                                                                                                   \
    inline static void destroy(T val) {                                                            \
      delete val;                                                                                  \
    }                                                                                              \
    inline static T defaultValue() {                                                               \
      return nullptr;                                                                              \
    }                                                                                              \
  }

// non basic types are returned by reference
// and stored by pointer
// This last point enables a better management of the default value
// which can simply be flagged in storing a null pointer
// the macro below must be used to enable thies type of management
#ifdef TLP_NO_CONST_STORED_TYPE
#define DECL_STORED_STRUCT(T)                                                                      \
  template <>                                                                                      \
  struct StoredType<T> {                                                                           \
    typedef T *Value;                                                                              \
    typedef T &ReturnedValue;                                                                      \
    typedef T ReturnedConstValue;                                                                  \
                                                                                                   \
    enum { isPointer = 1 };                                                                        \
                                                                                                   \
    inline static T &get(const Value &val) {                                                       \
      return *val;                                                                                 \
    }                                                                                              \
                                                                                                   \
    inline static bool equal(Value val1, const T &val2) {                                          \
      return val2 == *val1;                                                                        \
    }                                                                                              \
                                                                                                   \
    inline static bool equal(const T &val2, Value val1) {                                          \
      return val2 == *val1;                                                                        \
    }                                                                                              \
                                                                                                   \
    inline static Value clone(const T &val) {                                                      \
      return new T(val);                                                                           \
    }                                                                                              \
                                                                                                   \
    inline static void destroy(Value val) {                                                        \
      delete val;                                                                                  \
    }                                                                                              \
    inline static Value defaultValue() {                                                           \
      return new T();                                                                              \
    }                                                                                              \
  };
#else
#define DECL_STORED_STRUCT(T)                                                                      \
  template <>                                                                                      \
  struct StoredType<T> {                                                                           \
    typedef T *Value;                                                                              \
    typedef T &ReturnedValue;                                                                      \
    typedef const T &ReturnedConstValue;                                                           \
                                                                                                   \
    enum { isPointer = 1 };                                                                        \
                                                                                                   \
    inline static T &get(const Value &val) {                                                       \
      return *val;                                                                                 \
    }                                                                                              \
                                                                                                   \
    inline static bool equal(Value val1, const T &val2) {                                          \
      return val2 == *val1;                                                                        \
    }                                                                                              \
                                                                                                   \
    inline static bool equal(const T &val2, Value val1) {                                          \
      return val2 == *val1;                                                                        \
    }                                                                                              \
                                                                                                   \
    inline static Value clone(const T &val) {                                                      \
      return new T(val);                                                                           \
    }                                                                                              \
                                                                                                   \
    inline static void destroy(Value val) {                                                        \
      delete val;                                                                                  \
    }                                                                                              \
    inline static Value defaultValue() {                                                           \
      return new T();                                                                              \
    }                                                                                              \
  };
#endif
} // namespace tlp
#endif // TALIPOT_STORED_TYPE_H
///@endcond
