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

template<TYPE>
%MappedType std::set<TYPE> /TypeHint="Set[TYPE]"/{
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
  PyObject *l;

  const sipTypeDef* kpTypeDef = getSipType("TYPE");

  if (!kpTypeDef) {
    return NULL;
  }

  // Create an empty Python set
  if ((l = PySet_New(NULL)) == NULL) {
    return NULL;
  }

  // Go through each element in the C++ instance and convert it to a
  // wrapper object.
  for (const TYPE &v : *sipCpp) {
    TYPE *cpp = new TYPE(v);
    PyObject *pobj;

    // Get the Python wrapper for the Type instance, creating a new
    // one if necessary, and handle any ownership transfer.
    if ((pobj = sipConvertFromNewType(cpp, kpTypeDef, sipTransferObj)) == NULL) {
      // There was an error so garbage collect the Python set.
      Py_XDECREF(l);
      return NULL;
    }

    // Add the wrapper to the set.
    PySet_Add(l, pobj);
  }

  // Return the Python set.
  return l;
%End

%ConvertToTypeCode

  const sipTypeDef* kpTypeDef = getSipType("TYPE");

  if (!kpTypeDef) {
    return 0;
  }

  // Check if type is compatible
  if (sipIsErr == NULL) {
    if (!PySet_Check(sipPy)) {
      return 0;
    }
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item = NULL;

    while (item = PyIter_Next(iterator)) {
      if (!sipCanConvertToType(item, kpTypeDef, SIP_NOT_NONE)) {
        //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to TYPE");
        Py_DECREF(item);
        Py_DECREF(iterator);
        return 0;
      }
      Py_DECREF(item);
    }
    Py_DECREF(iterator);
    return 1;
  }

  // Convert Python set of TYPE to  std::set<TYPE>
  std::set<TYPE> *s = new std::set<TYPE>();

  PyObject *iterator = PyObject_GetIter(sipPy);
  PyObject *item = NULL;
  while (item = PyIter_Next(iterator)) {
    int state;
    TYPE* p = static_cast<TYPE*>(sipConvertToType(item, kpTypeDef, sipTransferObj, SIP_NOT_NONE, &state, sipIsErr));

    if (*sipIsErr) {
      Py_DECREF(item);
      Py_DECREF(iterator);
      sipReleaseType(p, kpTypeDef, state);
      delete s;
      return 0;
    }

    s->insert(*p);
    Py_DECREF(item);
    sipReleaseType(p, kpTypeDef, state);
  }
  Py_DECREF(iterator);
  *sipCppPtr = s;
  return sipGetState(sipTransferObj);
%End

};

template<TYPE*>
%MappedType std::set<TYPE*> /TypeHint="Set[TYPE]"/{
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
  PyObject *l;

  const sipTypeDef* kpTypeDef = getSipType("TYPE");

  if (!kpTypeDef) {
    return NULL;
  }

  // Create an empty Python set.
  if ((l = PySet_New(NULL)) == NULL) {
    return NULL;
  }

  // Go through each element in the C++ instance and convert it to a
  // wrapper object.
  for (TYPE *cpp : *sipCpp) {
    PyObject *pobj;

    // Get the Python wrapper for the Type instance, creating a new
    // one if necessary, and handle any ownership transfer.
    if ((pobj = sipConvertFromType(cpp, kpTypeDef, sipTransferObj)) == NULL) {
      // There was an error so garbage collect the Python set.
      Py_XDECREF(l);
      return NULL;
    }

    // Add the wrapper to the set.
    PySet_Add(l, pobj);
  }

  // Return the Python set.
  return l;
%End

%ConvertToTypeCode

  const sipTypeDef* kpTypeDef = getSipType("TYPE");

  if (!kpTypeDef) {
    return 0;
  }

  // Check if type is compatible
  if (sipIsErr == NULL) {
    if (!PySet_Check(sipPy)) {
      return 0;
    }
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item = NULL;
    while (item = PyIter_Next(iterator)) {
      if (!sipCanConvertToType(item, kpTypeDef, SIP_NOT_NONE)) {
        //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to TYPE");
        Py_DECREF(item);
        Py_DECREF(iterator);
        return 0;
      }
      Py_DECREF(item);
    }
    Py_DECREF(iterator);
    return 1;
  }

  // Convert Python list of TYPE to  std::set<TYPE>
  std::set<TYPE*> *s = new std::set<TYPE*>();

  PyObject *iterator = PyObject_GetIter(sipPy);
  PyObject *item = NULL;
  while (item = PyIter_Next(iterator)) {
    int state;
    TYPE* p = static_cast<TYPE*>(sipConvertToType(item, kpTypeDef, NULL, SIP_NOT_NONE, &state, sipIsErr));

    if (*sipIsErr) {
      Py_DECREF(item);
      Py_DECREF(iterator);
      sipReleaseType(p, kpTypeDef, state);
      delete s;
      return 0;
    }

    sipTransferTo(item, item);

    s->insert(p);
    Py_DECREF(item);
    sipReleaseType(p, kpTypeDef, state);
  }
  Py_DECREF(iterator);
  *sipCppPtr = s;
  return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<double> /TypeHint="Set[float]"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
  PyObject *l;

  // Create an empty Python set.
  if ((l = PySet_New(NULL)) == NULL) {
    return NULL;
  }

  // Go through each element in the C++ instance and convert it to a
  // wrapper object.
  for (double d : *sipCpp) {
    // Add the wrapper to the list.
    PySet_Add(l, PyFloat_FromDouble(d));
  }

  // Return the Python set.
  return l;
%End

%ConvertToTypeCode
  // Check if type is compatible
  if (sipIsErr == NULL) {
    if (!PySet_Check(sipPy)) {
      return 0;
    }
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item = NULL;
    while (item = PyIter_Next(iterator)) {
      if (!PyNumber_Check(item)) {
        //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to double");
        Py_DECREF(item);
        Py_DECREF(iterator);
        return 0;
      }
      Py_DECREF(item);
    }
    Py_DECREF(iterator);
    return 1;
  }

  // Convert Python set of numbers to a std::set<double>
  std::set<double> *s = new std::set<double>();

  PyObject *iterator = PyObject_GetIter(sipPy);
  PyObject *item = NULL;
  while (item = PyIter_Next(iterator)) {
    PyObject *f = PyNumber_Float(item);
    s->insert(PyFloat_AsDouble(f));
    Py_XDECREF(f);
    Py_DECREF(item);
  }
  Py_DECREF(iterator);

  *sipCppPtr = s;
  return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<float> /TypeHint="Set[float]"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
  PyObject *l;

  // Create an empty Python set.
  if ((l = PySet_New(NULL)) == NULL) {
    return NULL;
  }

  // Go through each element in the C++ instance and convert it to a
  // wrapper object.
  for (float f : *sipCpp) {
    // Add the wrapper to the list.
    PySet_Add(l, PyFloat_FromDouble(f));
  }

  // Return the Python set.
  return l;
%End

%ConvertToTypeCode
  // Check if type is compatible
  if (sipIsErr == NULL) {
    if (!PySet_Check(sipPy)) {
      return 0;
    }
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item = NULL;
    while (item = PyIter_Next(iterator)) {
      if (!PyNumber_Check(item)) {
        //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to float");
        Py_DECREF(item);
        Py_DECREF(iterator);
        return 0;
      }
      Py_DECREF(item);
    }
    Py_DECREF(iterator);
    return 1;
  }

  // Convert Python set of numbers to a std::set<float>
  std::set<float> *s = new std::set<float>();

  PyObject *iterator = PyObject_GetIter(sipPy);
  PyObject *item = NULL;
  while (item = PyIter_Next(iterator)) {
    PyObject *f = PyNumber_Float(item);
    s->insert(float(PyFloat_AsDouble(f)));
    Py_XDECREF(f);
    Py_DECREF(item);
  }
  Py_DECREF(iterator);

  *sipCppPtr = s;
  return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<int> /TypeHint="Set[int]"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
  PyObject *l;

  // Create an empty Python set.
  if ((l = PySet_New(NULL)) == NULL) {
    return NULL;
  }

  // Go through each element in the C++ instance and convert it to a
  // wrapper object.
  for (int n : *sipCpp) {
    // Add the wrapper to the list.
    PySet_Add(l, PyLong_FromLong(n));
  }

  // Return the Python set.
  return l;
%End

%ConvertToTypeCode
  // Check if type is compatible
  if (sipIsErr == NULL) {
    if (!PySet_Check(sipPy)) {
      return 0;
    }
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item = NULL;
    while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
      if (!PyLong_Check(item)) {
#else
      if (!PyLong_Check(item) && !PyInt_Check(item)) {
#endif
        //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to int");
        Py_DECREF(item);
        Py_DECREF(iterator);
        return 0;
      }
      Py_DECREF(item);
    }
    Py_DECREF(iterator);
    return 1;
  }

  // Convert Python set of integers to a std::set<int>
  std::set<int> *s = new std::set<int>();

  PyObject *iterator = PyObject_GetIter(sipPy);
  PyObject *item = NULL;
  while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
    s->insert(int(PyLong_AsLong(item)));
#else
    if (PyLong_Check(item)) {
      s->insert(int(PyLong_AsLong(item)));
    } else {
      s->insert(int(PyInt_AsLong(item)));
    }
#endif
    Py_DECREF(item);
  }

  Py_DECREF(iterator);
  *sipCppPtr = s;
  return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<long> /TypeHint="Set[int]"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
  PyObject *l;

  // Create an empty Python set.
  if ((l = PySet_New(NULL)) == NULL) {
    return NULL;
  }

  // Go through each element in the C++ instance and convert it to a
  // wrapper object.
  for (long n : *sipCpp) {
    // Add the wrapper to the list.
    PySet_Add(l, PyLong_FromLong(n));
  }

  // Return the Python set.
  return l;
%End

%ConvertToTypeCode
  // Check if type is compatible
  if (sipIsErr == NULL) {
    if (!PySet_Check(sipPy)) {
      return 0;
    }
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item = NULL;
    while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
      if (!PyLong_Check(item)) {
#else
      if (!PyLong_Check(item) && !PyInt_Check(item)) {
#endif
        //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to long");
        Py_DECREF(item);
        Py_DECREF(iterator);
        return 0;
      }
      Py_DECREF(item);
    }
    Py_DECREF(iterator);
    return 1;
  }

  // Convert Python set of integers to a std::set<long>
  std::set<long> *s = new std::set<long>();

  PyObject *iterator = PyObject_GetIter(sipPy);
  PyObject *item = NULL;
  while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
    s->insert(PyLong_AsLong(item));
#else
    if (PyLong_Check(item)) {
      s->insert(PyLong_AsLong(item));
    } else {
      s->insert(PyInt_AsLong(item));
    }
#endif
    Py_DECREF(item);
  }

  Py_DECREF(iterator);
  *sipCppPtr = s;
  return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<unsigned int> /TypeHint="Set[int]"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
  PyObject *l;

  // Create an empty Python set.
  if ((l = PySet_New(NULL)) == NULL) {
    return NULL;
  }

  // Go through each element in the C++ instance and convert it to a
  // wrapper object.
  for (unsigned int n : *sipCpp) {
    // Add the wrapper to the list.
    PySet_Add(l, PyLong_FromUnsignedLong(n));
  }

  // Return the Python set.
  return l;
%End

%ConvertToTypeCode
  // Check if type is compatible
  if (sipIsErr == NULL) {
    if (!PySet_Check(sipPy)) {
      return 0;
    }
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item = NULL;
    while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
      if (!PyLong_Check(item)) {
#else
      if (!PyLong_Check(item) && !PyInt_Check(item)) {
#endif
        //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to unsigned int");
        Py_DECREF(item);
        Py_DECREF(iterator);
        return 0;
      }
      Py_DECREF(item);
    }
    Py_DECREF(iterator);
    return 1;
  }

  // Convert Python set of integers to a std::set<unsigned int>
  std::set<unsigned int> *s = new std::set<unsigned int>();

  PyObject *iterator = PyObject_GetIter(sipPy);
  PyObject *item = NULL;
  while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
    s->insert(uint(PyLong_AsUnsignedLong(item)));
#else
    if (PyLong_Check(item)) {
      s->insert(uint(PyLong_AsUnsignedLong(item)));
    } else {
      s->insert(uint(PyInt_AsUnsignedLongMask(item)));
    }
#endif
    Py_DECREF(item);
  }
  Py_DECREF(iterator);
  *sipCppPtr = s;
  return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<unsigned long> /TypeHint="Set[int]"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
  PyObject *l;

  // Create an empty Python set.
  if ((l = PySet_New(NULL)) == NULL) {
    return NULL;
  }

  // Go through each element in the C++ instance and convert it to a
  // wrapper object.
  for (unsigned long n : *sipCpp) {
    // Add the wrapper to the list.
    PySet_Add(l, PyLong_FromUnsignedLong(n));
  }

  // Return the Python set.
  return l;
%End

%ConvertToTypeCode
  // Check if type is compatible
  if (sipIsErr == NULL) {
    if (!PySet_Check(sipPy)) {
      return 0;
    }
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item = NULL;
    while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
      if (!PyLong_Check(item)) {
#else
      if (!PyLong_Check(item) && !PyInt_Check(item)) {
#endif
        //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to unsigned long");
        Py_DECREF(item);
        Py_DECREF(iterator);
        return 0;
      }
      Py_DECREF(item);
    }
    Py_DECREF(iterator);
    return 1;
  }

  // Convert Python set of integers to a std::set<unsigned long>
  std::set<unsigned long> *s = new std::set<unsigned long>();

  PyObject *iterator = PyObject_GetIter(sipPy);
  PyObject *item = NULL;
  while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
    s->insert(PyLong_AsUnsignedLong(item));
#else
    if (PyLong_Check(item)) {
      s->insert(PyLong_AsUnsignedLong(item));
    } else {
      s->insert(PyInt_AsUnsignedLongMask(item));
    }
#endif
    Py_DECREF(item);
  }
  Py_DECREF(iterator);
  *sipCppPtr = s;
  return sipGetState(sipTransferObj);
%End

};
