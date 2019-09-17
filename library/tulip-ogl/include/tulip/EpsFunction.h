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
///@cond DOXYGEN_HIDDEN

#ifndef EPSFUNCTION
#define EPSFUNCTION
#ifndef DOXYGEN_NOTFOR_DEVEL
//====================================================

#include <cstdio>

#include <tulip/OpenGlIncludes.h>

namespace tlp {

class Color;

void Line(float x1, float y1, float z1, float x2, float y2, float z2, Color &C1, Color &C2);
void Rect(float x, float y, float z, float w, float h);
GLfloat *spewPrimitiveEPS(FILE *file, GLfloat *loc);
void spewUnsortedFeedback(FILE *file, GLint size, GLfloat *buffer);
int compare(const void *a, const void *b);
void spewSortedFeedback(FILE *file, GLint size, GLfloat *buffer);
void spewWireFrameEPS(FILE *file, int doSort, GLint size, GLfloat *buffer, char *creator);
void print3DcolorVertex(GLint size, GLint *count, GLfloat *buffer);
void printBuffer(GLint size, GLfloat *buffer);
} // namespace tlp
#endif // DOXYGEN_NOTFOR_DEVEL
#endif
///@endcond
