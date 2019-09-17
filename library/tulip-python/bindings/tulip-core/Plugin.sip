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
class Plugin : tlp::WithParameter, tlp::WithDependency {
%TypeHeaderCode
#include <tulip/Plugin.h>
%End

%Docstring
Bases: :class:`tlp.WithParameter`

This is the base class for all Tulip plugins.
%End

public:

  virtual ~Plugin();

  virtual std::string category() const = 0;
%Docstring
tlp.Plugin.category()

Returns the category of the plugin.

:rtype: string
%End

  virtual std::string name() const = 0;
%Docstring
tlp.Plugin.name()

Returns the name of the plugin.

:rtype: string
%End

  virtual std::string group() const = 0;
%Docstring
tlp.Plugin.group()

Returns the group of the plugin.

:rtype: string
%End

  virtual std::string author() const = 0;
%Docstring
tlp.Plugin.author()

Returns the author of the plugin.

:rtype: string
%End

  virtual std::string date() const = 0;
%Docstring
tlp.Plugin.category()

Returns the creation date of the plugin.

:rtype: string
%End

  virtual std::string info() const = 0;
%Docstring
tlp.Plugin.category()

Returns some information about the plugin.

:rtype: string
%End

  virtual std::string release() const = 0;
%Docstring
tlp.Plugin.category()

Returns the release number of the plugin.

:rtype: string
%End

  virtual std::string tulipRelease() const = 0;
%Docstring
tlp.Plugin.category()

Returns the Tulip release number.

:rtype: string
%End

  virtual std::string icon() const;
%Docstring
tlp.Plugin.icon()

.. versionadded:: 5.0

Returns the icon file path associated to the plugin.

:rtype: string
%End

  virtual std::string major() const;

  virtual std::string minor() const;

  virtual std::string tulipMajor() const;

  virtual std::string tulipMinor() const;

  virtual int id() const;

  virtual std::string programmingLanguage() const;

};

class FactoryInterface {
%TypeHeaderCode
#include <tulip/Plugin.h>
%End

public:

  virtual tlp::Plugin* createPluginObject(tlp::PluginContext* context) = 0 /Factory/;

  void registerPlugin();
%MethodCode
  tlp::PluginLister::registerPlugin(sipCpp);
%End

};

};
