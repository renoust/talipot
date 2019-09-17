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

#ifndef COLORSCALEPREVIEW_H_
#define COLORSCALEPREVIEW_H_

#include <QLabel>

class QResizeEvent;
class QMouseEvent;

/**
 * @brief Label used to preview ColorScale object in Qt widget.
 * Label used to preview ColorScale object in Qt widget. This object display the render of a
 * ColorScale in a label.
 * When user click on this object he can edit the ColorScale.
 */

namespace tlp {

class ColorScale;

class ColorScalePreview : public QLabel {
  Q_OBJECT
public:
  ColorScalePreview(ColorScale *colorScale);
  ~ColorScalePreview() override;
  void setColorScale(ColorScale *colorScale);
  ColorScale *getColorScale() {
    return currentColorScale;
  }

  void resizeEvent(QResizeEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

protected:
  void fillLabel();

  ColorScale *currentColorScale;
};
} // namespace tlp
#endif /* COLORSCALEPREVIEW_H_ */
