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

/*
 * Note for developers
 *
 * When writing the doxygen documentation for a class, one should follow those steps:
 *
 * 1/ Remove the /// @cond DOXYGEN_HIDDEN condition on top of the header and the /// @endcond tag at the bottom.
 *   If you are adding a new class to Talipot and didn't write the documentation yet, you should add those tags until the documentation is written.
 * 2/ Every class should belong to a doxygen group. This means that a @ingroup declaration must be added before the class @brief tag.
 * 3/ A doxygen group is identified by a single-word identifier (first word after the @defgroup declaration). What follows in the  @defgroup declaration is the pretty name that will appear into the final HTML document.
 * 3/ Definitions for doxygen groups are found in this file.
 * 4/ Code examples and tutorials belong in this directory too. There should be one header per tutorial to ease documentation process.
 * 5/ Please remember that code comments should NEVER begin with /** (use /* instead) in order to exclude them from doxygen.
 */

#define DOXYGEN // Used to separate licence from groups definitions

/**
 * @mainpage Welcome to Talipot's documentation.
 * You can jump right in with \ref graph_display/graph_display.cpp "one of our code examples", or browse the documentation by module.
 *
 * See for instance the documentation for the @ref Graph.
 */

/**
 * @defgroup Checks Model Checking
 * @brief Graph model checking
 * Classes in this group allow to perform various checks on a graph data structure.
 */

/**
 * @defgroup Structures Data structures
 * @brief Talipot basic data structures
 */

/**
 * @defgroup Graph Graph model
 * @brief Talipot graphs manipulation system
 * Classes in this group are related to the core system of Graph manipulation.
 */

/**
 * @defgroup Observation Observation system
 * @brief Talipot observer pattern implementation
 * Classes in this group are involved in the observation system.
 */

/**
 * @defgroup OpenGL 3D graph rendering
 * @brief OpenGL graph rendering
 * Classes in this group are related to the OpenGL rendering of graphs and to the OpenGL augmented display
 */

/**
 * @defgroup Plugins Plugins system
 * @brief Talipot plugins interfaces
 * Classes in this group are interfaces for the plugins system. You will subclass one of them when creating your own Talipot plugins.
 */
