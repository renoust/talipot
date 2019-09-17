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

#ifndef TALIPOT_PYTHON_CPP_TYPES_CONVERTER_H
#define TALIPOT_PYTHON_CPP_TYPES_CONVERTER_H

#include <talipot/Coord.h>
#include <talipot/Color.h>
#include <talipot/ColorScale.h>
#include <talipot/StringCollection.h>
#include <talipot/Size.h>
#include <talipot/Node.h>
#include <talipot/Edge.h>
#include <talipot/BooleanProperty.h>
#include <talipot/ColorProperty.h>
#include <talipot/DoubleProperty.h>
#include <talipot/IntegerProperty.h>
#include <talipot/LayoutProperty.h>
#include <talipot/SizeProperty.h>
#include <talipot/StringProperty.h>
#include <talipot/NumericProperty.h>
#include <talipot/TlpTools.h>

TLP_PYTHON_SCOPE void *convertSipWrapperToCppType(PyObject *sipWrapper,
                                                  const std::string &cppTypename,
                                                  const bool transferTo = false);
TLP_PYTHON_SCOPE PyObject *convertCppTypeToSipWrapper(void *cppObj, const std::string &cppTypename,
                                                      bool fromNew = false);

TLP_PYTHON_SCOPE bool convertPyObjectToBool(PyObject *pyObject, bool &cppObject);
TLP_PYTHON_SCOPE PyObject *convertBoolToPyObject(bool cppObject);

TLP_PYTHON_SCOPE bool convertPyObjectToDouble(PyObject *pyObject, double &cppObject);
TLP_PYTHON_SCOPE PyObject *convertDoubleToPyObject(double cppObject);

TLP_PYTHON_SCOPE bool convertPyObjectToLong(PyObject *pyObject, long &cppObject);
TLP_PYTHON_SCOPE PyObject *convertLongToPyObject(long cppObject);

TLP_PYTHON_SCOPE bool convertPyObjectToUnsignedLong(PyObject *pyObject, unsigned long &cppObject);
TLP_PYTHON_SCOPE PyObject *convertUnsignedLongToPyObject(unsigned long cppObject);

class TLP_PYTHON_SCOPE ValueSetter {

public:
  ValueSetter(tlp::DataSet *dataSet, const std::string &key)
      : dataSet(dataSet), graph(nullptr), key(key) {}

  ValueSetter(tlp::Graph *graph, const std::string &key)
      : dataSet(nullptr), graph(graph), key(key) {}

  template <typename T>
  void setValue(const T &value) {
    if (dataSet) {
      dataSet->set(key, value);
    } else if (graph) {
      graph->setAttribute(key, value);
    }
  }

private:
  tlp::DataSet *dataSet;
  tlp::Graph *graph;
  std::string key;
};

TLP_PYTHON_SCOPE PyObject *getPyObjectFromDataType(const tlp::DataType *dataType,
                                                   bool noCopy = false);

TLP_PYTHON_SCOPE bool setCppValueFromPyObject(PyObject *pyObj, ValueSetter &valSetter,
                                              tlp::DataType *dataType = nullptr);

template <typename T>
class PyObjectToCppObjectConvertor {

public:
  bool convert(PyObject *pyObject, T &cppObject) {
    std::string className = tlp::demangleClassName(typeid(T).name());

    void *cppObjPointer = convertSipWrapperToCppType(pyObject, className);

    if (cppObjPointer) {
      cppObject = *static_cast<T *>(cppObjPointer);
      return true;
    }

    return false;
  }
};

template <typename T>
class PyObjectToCppObjectConvertor<T *> {

public:
  bool convert(PyObject *pyObject, T *&cppObject) {
    std::string className = tlp::demangleClassName(typeid(T).name());

    void *cppObjPointer = convertSipWrapperToCppType(pyObject, className, true);

    if (cppObjPointer) {
      cppObject = static_cast<T *>(cppObjPointer);
      return true;
    }

    return false;
  }
};

template <>
class PyObjectToCppObjectConvertor<PyObject *> {
public:
  bool convert(PyObject *pyObject, PyObject *&cppObject) {
    cppObject = pyObject;
    return true;
  }
};

template <>
class PyObjectToCppObjectConvertor<bool> {
public:
  bool convert(PyObject *pyObject, bool &cppObject) {
    return convertPyObjectToBool(pyObject, cppObject);
  }
};

template <>
class PyObjectToCppObjectConvertor<double> {
public:
  bool convert(PyObject *pyObject, double &cppObject) {
    return convertPyObjectToDouble(pyObject, cppObject);
  }
};

template <>
class PyObjectToCppObjectConvertor<float> {
public:
  bool convert(PyObject *pyObject, float &cppObject) {
    double val = 0;
    PyObjectToCppObjectConvertor<double> convertor;
    bool ok = convertor.convert(pyObject, val);
    cppObject = val;
    return ok;
  }
};

template <>
class PyObjectToCppObjectConvertor<long> {

public:
  bool convert(PyObject *pyObject, long &cppObject) {
    return convertPyObjectToLong(pyObject, cppObject);
  }
};

template <>
class PyObjectToCppObjectConvertor<int> {
public:
  bool convert(PyObject *pyObject, int &cppObject) {
    long val = 0;
    PyObjectToCppObjectConvertor<long> convertor;
    bool ok = convertor.convert(pyObject, val);
    cppObject = val;
    return ok;
  }
};

template <>
class PyObjectToCppObjectConvertor<unsigned long> {
public:
  bool convert(PyObject *pyObject, unsigned long &cppObject) {
    return convertPyObjectToUnsignedLong(pyObject, cppObject);
  }
};

template <>
class PyObjectToCppObjectConvertor<unsigned int> {
public:
  bool convert(PyObject *pyObject, unsigned int &cppObject) {
    unsigned long val = 0;
    PyObjectToCppObjectConvertor<unsigned long> convertor;
    bool ok = convertor.convert(pyObject, val);
    cppObject = val;
    return ok;
  }
};

template <typename T>
class CppObjectToPyObjectConvertor {

public:
  bool convert(const T &cppObject, PyObject *&pyObject) {
    std::string className = tlp::demangleClassName(typeid(T).name());

    T *objCopy = new T(cppObject);
    PyObject *pyObj = convertCppTypeToSipWrapper(objCopy, className, true);

    if (pyObj) {
      pyObject = pyObj;
      return true;
    } else {
      delete objCopy;
    }

    return false;
  }
};

template <typename T>
class CppObjectToPyObjectConvertor<T *> {

public:
  bool convert(T *cppObject, PyObject *&pyObject) {
    std::string className = tlp::demangleClassName(typeid(T).name());

    PyObject *pyObj = convertCppTypeToSipWrapper(cppObject, className);

    if (pyObj) {
      pyObject = pyObj;
      return true;
    }

    return false;
  }
};

template <>
class CppObjectToPyObjectConvertor<PyObject *> {
public:
  bool convert(const PyObject *&cppObject, PyObject *&pyObject) {
    pyObject = const_cast<PyObject *>(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<bool> {
public:
  bool convert(const bool &cppObject, PyObject *&pyObject) {
    pyObject = convertBoolToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<long> {
public:
  bool convert(const long &cppObject, PyObject *&pyObject) {
    pyObject = convertLongToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<int> {
public:
  bool convert(const int &cppObject, PyObject *&pyObject) {
    pyObject = convertLongToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<unsigned int> {
public:
  bool convert(const unsigned int &cppObject, PyObject *&pyObject) {
    pyObject = convertUnsignedLongToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<unsigned long> {
public:
  bool convert(const unsigned long &cppObject, PyObject *&pyObject) {
    pyObject = convertUnsignedLongToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<double> {
public:
  bool convert(const double &cppObject, PyObject *&pyObject) {
    pyObject = convertDoubleToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<float> {
public:
  bool convert(const float &cppObject, PyObject *&pyObject) {
    pyObject = convertDoubleToPyObject(cppObject);
    return true;
  }
};

#endif // TALIPOT_PYTHON_CPP_TYPES_CONVERTER_H
