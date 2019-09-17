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

#ifndef TALIPOT_OPEN_GL_INCLUDES_H
#define TALIPOT_OPEN_GL_INCLUDES_H

#if defined(_MSC_VER)
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/*Taken from
 * https://gitlab.freedesktop.org/gstreamer/gst-plugins-good/commit/3d708a5bfa8961cc37671bc3226976dfc9ba50ad*/
/* The glext.h guard was renamed in 2018, but some software which
 * includes their own copy of the GL headers (such as qt (at least version 5.11.3)) might have
 * older version which use the old guard. This would result in the
 * header being included again (and symbols redefined).
 *
 * To avoid this, we define the "old" guard if the "new" guard is
 * defined.*/
#ifdef __gl_glext_h_
#ifndef __glext_h_
#define __glext_h_ 1
#endif
#endif

#endif // TALIPOT_OPEN_GL_INCLUDES_H
