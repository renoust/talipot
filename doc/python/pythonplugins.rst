.. py:currentmodule:: talipot

.. _talipotpythonplugins:

Writing Talipot plugins in Python
=================================

The Talipot Python bindings offer the possibility to write Talipot plugins in pure Python.
You can easily develop and test Python plugins from the Talipot GUI through the "Plugins editor" tab
of the Talipot plugins editor accessible through de *Develop* button (see :ref:`Using the bindings from the Talipot Software GUI <usingBindingsInTalipotGUI>`).

Below is the list of plugins that can be implemented in Python :

	* General algorithms
	* Selection algorithms
	* Color algorithms
	* Measure algorithms
	* Integer algorithms
	* Layout algorithms
	* Size algorithms
	* Export modules
	* Import modules

Developing a plugin consists in writing a Python module (in a specific .py file) that contains the implementation
of a Python class deriving from another one specific to the plugin type.
The plugin is then registered in the plugin database through the use of
specific functions from the :mod:`talipotlugins` module (see :ref:`talipotplugins module API <talipotpluginsref>`).
Once the plugin written and functional, you can copy the source file(s) to the following directory :
<talipot_install_dir>/lib/talipot/python and it will be automatically loaded at Talipot startup.
The remaining of this section illustrates how to write each type of plugin.

Writing a general algorithm in Python
--------------------------------------

A general algorithm must implement the :class:`tlp.Algorithm` interface as illustrated below::

  from talipot import tlp
  import talipotplugins

  class GeneralAlgorithmExample(tlp.Algorithm):
    def __init__(self, context):
      tlp.Algorithm.__init__(self, context)
      # you can add parameters to the plugin here through the following syntax
      # self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
      # (see documentation of class tlp.WithParameter to see what types of parameters are supported)

    def check(self):
      # This method is called before applying the algorithm on the input graph.
      # You can perform some precondition checks here.
      # See comments in the run method to know how to access to the input graph.

      # Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
      # and the second one can be used to provide an error message
      return (True, "Ok")

    def run(self):
      # This method is the entry point of the algorithm when it is called
      # and must contain its implementation.

      # The graph on which the algorithm is applied can be accessed through
      # the "graph" class attribute (see documentation of class tlp.Graph).

      # The parameters provided by the user are stored in a dictionnary
      # that can be accessed through the "dataSet" class attribute.

      # The method must return a boolean indicating if the algorithm
      # has been successfully applied on the input graph.
      return True

  # The line below does the magic to register the plugin to the plugin database
  # and updates the GUI to make it accessible through the menus.
  talipotplugins.registerPluginOfGroup("GeneralAlgorithmExample", "General Algorithm Demo", "author", "26/07/2011", "info", "1.0", "Python")

Writing a selection algorithm in Python
----------------------------------------

A selection algorithm must implement the :class:`tlp.BooleanAlgorithm` interface as illustrated below::

  from talipot import tlp
  import talipotplugins

  class SelectionAlgorithmExample(tlp.BooleanAlgorithm):
    def __init__(self, context):
      tlp.BooleanAlgorithm.__init__(self, context)
      # you can add parameters to the plugin here through the following syntax
      # self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
      # (see documentation of class tlp.WithParameter to see what types of parameters are supported)

    def check(self):
      # This method is called before applying the algorithm on the input graph.
      # You can perform some precondition checks here.
      # See comments in the run method to know how to access to the input graph.

      # Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
      # and the second one can be used to provide an error message
      return (True, "Ok")

    def run(self):
      # This method is the entry point of the algorithm when it is called
      # and must contain its implementation.

      # The graph on which the algorithm is applied can be accessed through
      # the "graph" class attribute (see documentation of class tlp.Graph).

      # The parameters provided by the user are stored in a dictionnary
      # that can be accessed through the "dataSet" class attribute.

      # The result of this selection algorithm must be stored in the
      # boolean property accessible through the "result" class attribute
      # (see documentation to know how to work with graph properties).

      # The method must return a boolean indicating if the algorithm
      # has been successfully applied on the input graph.
      return True

  # The line below does the magic to register the plugin to the plugin database
  # and updates the GUI to make it accessible through the menus.
  talipotplugins.registerPluginOfGroup("SelectionAlgorithmExample", "Selection Algorithm Demo", "author", "27/07/2011", "info", "1.0", "Python")

Writing a color algorithm in Python
-----------------------------------

A color algorithm must implement the :class:`tlp.ColorAlgorithm` interface as illustrated below::

  from talipot import tlp
  import talipotplugins

  class ColorAlgorithmExample(tlp.ColorAlgorithm):
    def __init__(self, context):
      tlp.ColorAlgorithm.__init__(self, context)
      # you can add parameters to the plugin here through the following syntax
      # self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
      # (see documentation of class tlp.WithParameter to see what types of parameters are supported)

    def check(self):
      # This method is called before applying the algorithm on the input graph.
      # You can perform some precondition checks here.
      # See comments in the run method to know how to access to the input graph.

      # Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
      # and the second one can be used to provide an error message
      return (True, "Ok")

    def run(self):
      # This method is the entry point of the algorithm when it is called
      # and must contain its implementation.

      # The graph on which the algorithm is applied can be accessed through
      # the "graph" class attribute (see documentation of class tlp.Graph).

      # The parameters provided by the user are stored in a dictionnary
      # that can be accessed through the "dataSet" class attribute.

      # The result of this color algorithm must be stored in the
      # color property accessible through the "result" class attribute
      # (see documentation to know how to work with graph properties).

      # The method must return a boolean indicating if the algorithm
      # has been successfully applied on the input graph.
      return True

  # The line below does the magic to register the plugin to the plugin database
  # and updates the GUI to make it accessible through the menus.
  talipotplugins.registerPluginOfGroup("ColorAlgorithmExample", "Color Algorithm Demo", "author", "27/07/2011", "info", "1.0", "Python")

Writing a measure algorithm in Python
-------------------------------------

A measure algorithm must implement the :class:`tlp.DoubleAlgorithm` interface as illustrated below::

  from talipot import tlp
  import talipotplugins

  class MeasureAlgorithmExample(tlp.DoubleAlgorithm):
    def __init__(self, context):
      tlp.DoubleAlgorithm.__init__(self, context)
      # you can add parameters to the plugin here through the following syntax
      # self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
      # (see documentation of class tlp.WithParameter to see what types of parameters are supported)

    def check(self):
      # This method is called before applying the algorithm on the input graph.
      # You can perform some precondition checks here.
      # See comments in the run method to know how to access to the input graph.

      # Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
      # and the second one can be used to provide an error message
      return (True, "Ok")

    def run(self):
      # This method is the entry point of the algorithm when it is called
      # and must contain its implementation.

      # The graph on which the algorithm is applied can be accessed through
      # the "graph" class attribute (see documentation of class tlp.Graph).

      # The parameters provided by the user are stored in a dictionnary
      # that can be accessed through the "dataSet" class attribute.

      # The result of this measure algorithm must be stored in the
      # double property accessible through the "result" class attribute
      # (see documentation to know how to work with graph properties).

      # The method must return a boolean indicating if the algorithm
      # has been successfully applied on the input graph.
      return True

  # The line below does the magic to register the plugin to the plugin database
  # and updates the GUI to make it accessible through the menus.
  talipotplugins.registerPluginOfGroup("MeasureAlgorithmExample", "Measure Algorithm Demo", "author", "27/07/2011", "info", "1.0", "Python")

Writing an integer algorithm in Python
--------------------------------------

An integer algorithm must implement the :class:`tlp.IntegerAlgorithm` interface as illustrated below::

  from talipot import tlp
  import talipotplugins

  class IntegerAlgorithmExample(tlp.IntegerAlgorithm):
    def __init__(self, context):
      tlp.IntegerAlgorithm.__init__(self, context)
      # you can add parameters to the plugin here through the following syntax
      # self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
      # (see documentation of class tlp.WithParameter to see what types of parameters are supported)

    def check(self):
      # This method is called before applying the algorithm on the input graph.
      # You can perform some precondition checks here.
      # See comments in the run method to know how to access to the input graph.

      # Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
      # and the second one can be used to provide an error message
      return (True, "Ok")

    def run(self):
      # This method is the entry point of the algorithm when it is called
      # and must contain its implementation.

      # The graph on which the algorithm is applied can be accessed through
      # the "graph" class attribute (see documentation of class tlp.Graph).

      # The parameters provided by the user are stored in a dictionnary
      # that can be accessed through the "dataSet" class attribute.

      # The result of this integer algorithm must be stored in the
      # integer property accessible through the "result" class attribute
      # (see documentation to know how to work with graph properties).

      # The method must return a boolean indicating if the algorithm
      # has been successfully applied on the input graph.
      return True

  # The line below does the magic to register the plugin to the plugin database
  # and updates the GUI to make it accessible through the menus.
  talipotplugins.registerPluginOfGroup("IntegerAlgorithmExample", "Integer Algorithm Demo", "author", "27/07/2011", "info", "1.0", "Python")

Writing a layout algorithm in Python
------------------------------------

A layout algorithm must implement the :class:`tlp.LayoutAlgorithm` interface as illustrated below::

  from talipot import tlp
  import talipotplugins

  class LayoutAlgorithmExample(tlp.LayoutAlgorithm):
    def __init__(self, context):
      tlp.LayoutAlgorithm.__init__(self, context)
      # you can add parameters to the plugin here through the following syntax
      # self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
      # (see documentation of class tlp.WithParameter to see what types of parameters are supported)

    def check(self):
      # This method is called before applying the algorithm on the input graph.
      # You can perform some precondition checks here.
      # See comments in the run method to know how to access to the input graph.

      # Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
      # and the second one can be used to provide an error message
      return (True, "Ok")

    def run(self):
      # This method is the entry point of the algorithm when it is called
      # and must contain its implementation.

      # The graph on which the algorithm is applied can be accessed through
      # the "graph" class attribute (see documentation of class tlp.Graph).

      # The parameters provided by the user are stored in a dictionnary
      # that can be accessed through the "dataSet" class attribute.

      # The result of this layout algorithm must be stored in the
      # layout property accessible through the "result" class attribute
      # (see documentation to know how to work with graph properties).

      # The method must return a boolean indicating if the algorithm
      # has been successfully applied on the input graph.
      return True

  # The line below does the magic to register the plugin to the plugin database
  # and updates the GUI to make it accessible through the menus.
  talipotplugins.registerPluginOfGroup("LayoutAlgorithmExample", "Layout Algorithm Demo", "author", "27/07/2011", "info", "1.0", "Python")

Writing a size algorithm in Python
----------------------------------

A size algorithm must implement the :class:`tlp.SizeAlgorithm` interface as illustrated below::

  from talipot import tlp
  import talipotplugins

  class SizeAlgorithmExample(tlp.SizeAlgorithm):
    def __init__(self, context):
      tlp.SizeAlgorithm.__init__(self, context)
      # you can add parameters to the plugin here through the following syntax
      # self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
      # (see documentation of class tlp.WithParameter to see what types of parameters are supported)

    def check(self):
      # This method is called before applying the algorithm on the input graph.
      # You can perform some precondition checks here.
      # See comments in the run method to know how to access to the input graph.

      # Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
      # and the second one can be used to provide an error message
      return (True, "Ok")

    def run(self):
      # This method is the entry point of the algorithm when it is called
      # and must contain its implementation.

      # The graph on which the algorithm is applied can be accessed through
      # the "graph" class attribute (see documentation of class tlp.Graph).

      # The parameters provided by the user are stored in a dictionnary
      # that can be accessed through the "dataSet" class attribute.

      # The result of this size algorithm must be stored in the
      # size property accessible through the "result" class attribute
      # (see documentation to know how to work with graph properties).

      # The method must return a boolean indicating if the algorithm
      # has been successfully applied on the input graph.
      return True

  # The line below does the magic to register the plugin to the plugin database
  # and updates the GUI to make it accessible through the menus.
  talipotplugins.registerPluginOfGroup("SizeAlgorithmExample", "Size Algorithm Demo", "author", "27/07/2011", "info", "1.0", "Python")

Writing an export module in Python
-----------------------------------

An export module must implement the :class:`tlp.ExportModule` interface as illustrated below::

  from talipot import tlp
  import talipotplugins

  class ExportModuleExample(tlp.ExportModule):
    def __init__(self, context):
      tlp.ExportModule.__init__(self, context)
      # you can add parameters to the plugin here through the following syntax
      # self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
      # (see documentation of class tlp.WithParameter to see what types of parameters are supported)

    def exportGraph(self, os):
      # This method is called to export a graph.
      # The graph to export is accessible through the "graph" class attribute
      # (see documentation of class tlp.Graph).

      # The parameters provided by the user are stored in a dictionnary
      # that can be accessed through the "dataSet" class attribute.

      # The os parameter is an output file stream (initialized by the Talipot GUI
      # or by the tlp.exportGraph function.).
      # To write data to the file, you have to use the following syntax :

      # write the number of nodes and edges to the file
      os << self.graph.numberOfNodes() << "\n"
      os << self.graph.numberOfEdges() << "\n"

      # The method must return a boolean indicating if the
      # graph has been successfully exported.
      return True

  # The line below does the magic to register the plugin to the plugin database
  # and updates the GUI to make it accessible through the menus.
  talipotplugins.registerPluginOfGroup("ExportModuleExample", "Export Module Demo", "author", "27/07/2011", "info", "1.0", "Python")

Writing an import module in Python
-----------------------------------

An import module must implement the :class:`tlp.ImportModule` interface as illustrated below::

  from talipot import tlp
  import talipotplugins

  class ImportModuleExample(tlp.ImportModule):
    def __init__(self, context):
      tlp.ImportModule.__init__(self, context)
      # you can add parameters to the plugin here through the following syntax
      # self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
      # (see documentation of class tlp.WithParameter to see what types of parameters are supported)

    def importGraph(self):
      # This method is called to import a new graph.
      # An empty graph to populate is accessible through the "graph" class attribute
      # (see documentation of class tlp.Graph).

      # The parameters provided by the user are stored in a dictionnary
      # that can be accessed through the "dataSet" class attribute

      # The method must return a boolean indicating if the
      # graph has been successfully imported.
      return True

  # The line below does the magic to register the plugin to the plugin database
  # and updates the GUI to make it accessible through the menus.
  talipotplugins.registerPluginOfGroup("ImportModuleExample", "Import Module Demo", "author", "27/07/2011", "info", "1.0", "Python")

