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

namespace tlp {

template<TYPE>
class Iterator /Abstract/ {
%TypeHeaderCode
#include <tulip/Iterator.h>
%End

public:

  Iterator();

  TYPE next();
%MethodCode
  if (sipCpp->hasNext()) {
    sipRes = copyValue(sipCpp->next());
  } else {
    PyErr_SetString(PyExc_Exception, "Iterator : next() has been called but there is no more elements to iterate on.");
    sipIsErr = -1;
  }
%End

  bool hasNext();

  Iterator * __iter__();
%MethodCode
  sipRes = sipCpp;
%End

  TYPE __next__();
%MethodCode
  if (sipCpp->hasNext()) {
    sipRes = copyValue(sipCpp->next());
  } else {
    PyErr_SetNone(PyExc_StopIteration);
    return NULL;
  }
%End

};

};
