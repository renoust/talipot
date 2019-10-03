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

namespace tlp {

class ColorScalesManager {
%TypeHeaderCode
#include <talipot/ColorScalesManager.h>
%End

%Docstring
That static class aims to facilitate working with Talipot color scales.
It allows to easily retrieve Talipot predefined ones but also to register new ones
in a persistent database for further reuse.
%End

  public:

//==========================================================================================

    static std::list<std::string> getColorScalesList();
%Docstring
tlpgui.ColorScalesManager.getColorScalesList()

Returns a list of the registered color scales names.

:rtype: list of strings
%End

//==========================================================================================

    static tlp::ColorScale getColorScale(const std::string &colorScaleName);
%Docstring
tlpgui.ColorScalesManager.getColorScale(colorScaleName)

Returns the color scale registered with the provided name.

.. warning:: If there is no such colorscale, the returned one will be empty.

:rtype: :class:`talipot.tlp.ColorScale`
%End

%MethodCode
    tlp::ColorScale cs = tlp::ColorScalesManager::getColorScale(*a0);
    if (cs.getStopsCount() == 0) {
      sipIsErr = 1;
      std::string errMsg = std::string("No registered color scale named '") + *a0 + "'.";
      PyErr_SetString(PyExc_Exception, errMsg.c_str());
    } else {
      sipRes = new tlp::ColorScale(cs);
    }
%End

//==========================================================================================

    static void registerColorScale(const std::string &colorScaleName, const tlp::ColorScale &colorScale);
%Docstring
tlpgui.ColorScalesManager.registerColorScale(colorScaleName, colorScale)

Registers a color scale in the persistent database for further reuse.

.. warning:: If there is already a color scale registered with such a name, it will not be overwritten.

:param colorScaleName: the name of the color scale to register
:type colorScaleName: string
:param colorScale: the color scale to set
:type colorScale: a :class:`tlp.ColorScale` instance, a list of :class:`tlp.Color` or a dictionary with float keys (between 0.0 and 1.0) and :class:`tlp.Color` values
%End

//==========================================================================================

    static void removeColorScale(const std::string &colorScaleName);
%Docstring
tlpgui.ColorScalesManager.removeColorScale(colorScaleName)

Removes the color scale registered with the provided name from the persistent database.

:param colorScaleName: the name of the color scale to remove
:type colorScaleName: string
%End

};

};
