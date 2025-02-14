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

#ifndef GRIP_H
#define GRIP_H

#include <talipot/PluginHeaders.h>
#include <unordered_map>
#include "MISFiltering.h"

/*@{*/
/** \file
 *  \brief  This plugin is an implementation of the GRIP layout
 *
 *  An implementation of a force directed graph drawing
 *  algorithm first published as:
 *
 *  P. Gajer and S.G. Kobourov,
 *  "GRIP: Graph dRawing with Intelligent Placement",
 *  in Journal Graph Algorithm and Applications, vol. 6, no. 3,
 *  p. 203-224, 2002
 *
 *  <b>HISTORY</b>
 *
 *  - 2007 Version 1.0: Initial release
 *  by Romain Bourqui, LaBRI, University Bordeaux I, France
 *
 *
 */

class Grip : public tlp::LayoutAlgorithm {

public:
  PLUGININFORMATION("GRIP", "Romain Bourqui", "01/11/2010",
                    "Implements a force directed graph drawing algorithm first published as:<br/>"
                    "<b>GRIP: Graph dRawing with Intelligent Placement</b>, P. Gajer and S.G. "
                    "Kobourov, Journal Graph Algorithm and Applications, vol. 6, no. 3, pages "
                    "203--224, (2002).",
                    "1.1", "Force Directed")

  Grip(const tlp::PluginContext *);
  ~Grip() override;

  bool run() override;

private:
  void computeCurrentGraphLayout();
  void computeOrdering();
  void firstNodesPlacement();
  void placement();
  void initialPlacement(unsigned int, unsigned int);
  void kk_local_reffinement(tlp::node);
  void kk_reffinement(unsigned int, unsigned int);
  void fr_reffinement(unsigned int, unsigned int);
  void displace(tlp::node);
  void updateLocalTemp(tlp::node);
  void init();
  void init_heat(unsigned int);

  void seeLayout(unsigned int);

  unsigned int rounds(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
  void set_nbr_size();
  float sched(int, int, int, int, int);

  MISFiltering *misf;
  float edgeLength;
  int level;
  std::unordered_map<tlp::node, std::vector<unsigned int>> neighbors_dist;
  std::unordered_map<tlp::node, std::vector<tlp::node>> neighbors;
  std::unordered_map<unsigned int, unsigned int> levelToNbNeighbors;
  std::unordered_map<tlp::node, tlp::Coord> disp;
  std::unordered_map<tlp::node, tlp::Coord> oldDisp;
  std::unordered_map<tlp::node, double> heat;
  std::unordered_map<tlp::node, double> oldCos;

  tlp::Graph *currentGraph;
  int _dim;
};
/*@}*/
#endif // GRIP_H
