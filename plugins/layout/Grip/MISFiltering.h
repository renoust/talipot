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
#ifndef MISFILTERING_H
#define MISFILTERING_H

#include <unordered_map>
#include <unordered_set>

#include <tulip/TulipPluginHeaders.h>
#include <tulip/MutableContainer.h>

class MISFiltering {

public:
  MISFiltering(tlp::Graph *);
  ~MISFiltering();

  void computeFiltering();
  void getNearest(tlp::node, std::vector<tlp::node> &, std::vector<unsigned int> &, unsigned int,
                  unsigned int = 3);

  std::vector<tlp::node> ordering;
  std::vector<unsigned int> index;

private:
  unsigned int level;
  tlp::MutableContainer<bool> inLastVi;
  tlp::MutableContainer<bool> inCurVi;
  tlp::MutableContainer<bool> removed;
  std::vector<unsigned int> v_dist;

  std::unordered_map<unsigned int, std::unordered_set<tlp::node>> levelToNodes;

  void bfsDepth(tlp::node, unsigned int);
  void updateVectors();

  tlp::Graph *g_copy;
  tlp::MutableContainer<bool> removedVisit;
  tlp::MutableContainer<bool> attendedVisit;
  tlp::MutableContainer<bool> visited;

  std::vector<tlp::node> toVisit;
};
#endif
