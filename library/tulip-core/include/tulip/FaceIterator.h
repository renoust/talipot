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

#ifndef FACEITERATOR2_H
#define FACEITERATOR2_H

#include <vector>

#include <tulip/Iterator.h>
#include <tulip/Face.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

//============================================================
/// Face iterator for PlanarConMap
namespace tlp {

class PlanarConMap;

class TLP_SCOPE FaceIterator : public Iterator<Face> {
public:
  FaceIterator(PlanarConMap *m);
  ~FaceIterator() override {}

  Face next() override;
  bool hasNext() override;

private:
  unsigned int i;
  PlanarConMap *mgraph;
};

//============================================================
/// Adjacente Face iterator for PlanarConMap
class TLP_SCOPE FaceAdjIterator : public Iterator<Face> {
public:
  FaceAdjIterator(PlanarConMap *m, const node n);
  ~FaceAdjIterator() override {}

  Face next() override;
  bool hasNext() override;

private:
  std::vector<Face> facesAdj;
  unsigned int i;
};

//============================================================
/// Node of face iterator for PlanarConMap
class TLP_SCOPE NodeFaceIterator : public Iterator<node> {
public:
  NodeFaceIterator(PlanarConMap *m, const Face);
  ~NodeFaceIterator() override {}

  node next() override;
  bool hasNext() override;

private:
  /** contains the set of computed nodes */
  std::vector<node> nodes;
  unsigned int i;
};

//============================================================
/// Edge of face iterator for PlanarConMap
class TLP_SCOPE EdgeFaceIterator : public Iterator<edge> {
public:
  EdgeFaceIterator(PlanarConMap *m, const Face);
  ~EdgeFaceIterator() override {}

  edge next() override;
  bool hasNext() override;

private:
  /** reference on the current face */
  std::vector<edge> ve;
  unsigned int i;
};
} // namespace tlp
#endif

///@endcond
