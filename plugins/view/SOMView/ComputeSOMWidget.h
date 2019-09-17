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

#ifndef COMPUTESOMWIDGET_H_
#define COMPUTESOMWIDGET_H_

#include <QWidget>

#include <vector>
#include <string>

namespace Ui {
class SOMComputeWidget;
}

namespace tlp {

class Graph;
/**
 * @brief Class configuring properties selected and number of iteration for computing SOM.
 */
class ComputeSOMWidget : public QWidget {
  Q_OBJECT
  Ui::SOMComputeWidget *_ui;

public:
  ComputeSOMWidget(QWidget *parent = nullptr);
  ~ComputeSOMWidget() override;
  unsigned number() const;
  void setNumber(unsigned uintValue);
  void clearLists();
  std::vector<std::string> getSelectedProperties();
  void setWidgetParameters(tlp::Graph *g, std::vector<std::string> &propertyFilterType);
  void setOutputPropertiesList(std::vector<std::string> &properties);
};
} // namespace tlp
#endif /* COMPUTESOMWIDGET_H_ */
