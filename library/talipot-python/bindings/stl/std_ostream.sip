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

namespace std {

class ostream /NoDefaultCtors/ {
%TypeHeaderCode
#include <iostream>
%End

public:

  std::ostream& operator<< (SIP_PYOBJECT val) /NoCopy/;
%MethodCode
  PyObject *objStr = PyObject_Str(a1);
  PyObject *objRepr = PyObject_Repr(a1);
  if (objStr) {
#if PY_MAJOR_VERSION >= 3
    sipRes = &(*a0 << convertPythonUnicodeObjectToStdString(objStr));
#else
    sipRes = &(*a0 << PyString_AS_STRING(objStr));
#endif
  } else if (objRepr) {
#if PY_MAJOR_VERSION >= 3
    sipRes = &(*a0 << convertPythonUnicodeObjectToStdString(objRepr));
#else
    sipRes = &(*a0 << PyString_AS_STRING(objRepr));
#endif
  } else {
    sipRes = a0;
  }

  Py_XDECREF(objStr);
  Py_XDECREF(objRepr);
%End

};

};
