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

#ifndef EDIT_COLOR_SCALE_INTERACTOR_H
#define EDIT_COLOR_SCALE_INTERACTOR_H

#include <talipot/GLInteractor.h>
//#include <talipot/GlColorScale.h>
//#include <talipot/DoubleProperty.h>

//#include "SOMView.h"

namespace tlp {
class GlLayer;
class View;
class GlMainWidget;
class GlLabelledColorScale;
class NumericProperty;
class SOMView;

class EditColorScaleInteractor : public GLInteractorComponent {
public:
  EditColorScaleInteractor();
  ~EditColorScaleInteractor() override;
  bool eventFilter(QObject *, QEvent *) override;
  void viewChanged(View *view) override;
  bool compute(GlMainWidget *) override;
  bool draw(GlMainWidget *glMainWidget) override;

protected:
  virtual void propertyChanged(SOMView *somView, const std::string &propertyName,
                               NumericProperty *newProperty);
  virtual bool screenSizeChanged(SOMView *somView);
  NumericProperty *currentProperty;
  GlLabelledColorScale *colorScale;
  float widthPercent;
  float heightPercent;
  float heightPosition;
  int glMainWidgetWidth;
  int glMainWidgetHeight;

  GlLayer *selectionLayer;
};
} // namespace tlp
#endif // EDIT_COLOR_SCALE_INTERACTOR_H
