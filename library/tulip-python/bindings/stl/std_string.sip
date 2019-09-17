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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%MappedType std::string /TypeHint="str"/
{
%TypeHeaderCode
// Include the library interface to the type being mapped.
#include <string>
%End

%ConvertToTypeCode
  if (sipIsErr == NULL) {
#if PY_MAJOR_VERSION >= 3
    return PyUnicode_Check(sipPy);
#else
    return PyString_Check(sipPy) || PyUnicode_Check(sipPy);
#endif
  }

  if (sipPy == Py_None) {
    *sipCppPtr = new std::string;
    return 1;
  }
#if PY_MAJOR_VERSION >= 3
  *sipCppPtr = new std::string(convertPythonUnicodeObjectToStdString(sipPy));
#else
  if (PyString_Check(sipPy)) {
    *sipCppPtr = new std::string(PyString_AS_STRING(sipPy));
  } else {
    *sipCppPtr = new std::string(convertPythonUnicodeObjectToStdString(sipPy));
  }
#endif
  return 1;
%End

%ConvertFromTypeCode
#if PY_MAJOR_VERSION >= 3
  PyObject *obj = PyUnicode_FromString(sipCpp->c_str());
#else
  PyObject *obj = PyString_FromString(sipCpp->c_str());
#endif
  return obj;
%End
};
