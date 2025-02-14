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

#include <QCheckBox>
#include <QLabel>
#include <QStylePainter>
#include <QApplication>
#include <QLinearGradient>
#include <QPaintEvent>
#include <QColorDialog>
#include <QMainWindow>
#include <QFileDialog>
#include <QHBoxLayout>

#include <talipot/TlpTools.h>
#include <talipot/ColorScaleButton.h>
#include <talipot/MetaTypes.h>
#include <talipot/CoordEditor.h>
#include <talipot/StringEditor.h>
#include <talipot/GlyphRenderer.h>
#include <talipot/EdgeExtremityGlyphManager.h>
#include <talipot/EdgeExtremityGlyph.h>
#include <talipot/FontDialog.h>
#include <talipot/GlyphManager.h>
#include <talipot/GraphPropertiesModel.h>
#include <talipot/ItemEditorCreators.h>
#include <talipot/FontAwesome.h>
#include <talipot/TextureFileDialog.h>
#include <talipot/FontIconDialog.h>
#include <talipot/FontIconManager.h>
#include <talipot/ShapeDialog.h>

using namespace tlp;

/*
 * Base class
 */
bool ItemEditorCreator::paint(QPainter *painter, const QStyleOptionViewItem &option,
                              const QVariant &, const QModelIndex &) const {
  if (option.state.testFlag(QStyle::State_Selected) && option.showDecorationSelected) {
    painter->setBrush(option.palette.highlight());
    painter->setPen(Qt::transparent);
    painter->drawRect(option.rect);
  }

  return false;
}

QSize ItemEditorCreator::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  QString line = displayText(data);
  QFontMetrics fontMetrics(option.font);
  QRect textBB = fontMetrics.boundingRect(line);
  return QSize(textBB.width() + 15, textBB.height() + 5);
}

// this class is defined to properly catch the return status
// of a QColorDialog. calling QDialog::result() instead does not work
class ColorDialog : public QColorDialog {
public:
  ColorDialog(QWidget *w) : QColorDialog(w), previousColor(), ok(QDialog::Rejected) {
    // we don't use native dialog to ensure alpha channel can be set
    // it may not be shown when using gnome
    setOptions(QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);
  }
  ~ColorDialog() override {}
  tlp::Color previousColor;
  int ok;
  void done(int res) override {
    ok = res;
    QColorDialog::done(res);
  }
  void showEvent(QShowEvent *ev) override {
    QDialog::showEvent(ev);

    if (parentWidget())
      move(parentWidget()->window()->frameGeometry().topLeft() +
           parentWidget()->window()->rect().center() - rect().center());
  }
};

/*
  ColorEditorCreator
*/
QWidget *ColorEditorCreator::createWidget(QWidget *parent) const {
  QMainWindow *mainWindow = getMainWindow();
  ColorDialog *colorDialog = new ColorDialog(mainWindow ? mainWindow : parent);
  colorDialog->setModal(true);
  return colorDialog;
}

bool ColorEditorCreator::paint(QPainter *painter, const QStyleOptionViewItem &option,
                               const QVariant &v, const QModelIndex &index) const {
  ItemEditorCreator::paint(painter, option, v, index);
  painter->setBrush(colorToQColor(v.value<tlp::Color>()));
  painter->setPen(Qt::black);
  painter->drawRect(option.rect.x() + 6, option.rect.y() + 6, option.rect.width() - 12,
                    option.rect.height() - 12);
  return true;
}

void ColorEditorCreator::setEditorData(QWidget *editor, const QVariant &data, bool, tlp::Graph *) {
  ColorDialog *dlg = static_cast<ColorDialog *>(editor);

  dlg->previousColor = data.value<tlp::Color>();
  dlg->setCurrentColor(colorToQColor(dlg->previousColor));
  dlg->move(QCursor::pos() - QPoint(dlg->width() / 2, dlg->height() / 2));
}

QVariant ColorEditorCreator::editorData(QWidget *editor, tlp::Graph *) {
  ColorDialog *dlg = static_cast<ColorDialog *>(editor);

  if (dlg->ok == QDialog::Rejected)
    // restore the previous color
    return QVariant::fromValue<tlp::Color>(dlg->previousColor);

  return QVariant::fromValue<tlp::Color>(QColorToColor(dlg->currentColor()));
}

/*
  BooleanEditorCreator
*/
QWidget *BooleanEditorCreator::createWidget(QWidget *parent) const {
  return new QComboBox(parent);
}

void BooleanEditorCreator::setEditorData(QWidget *editor, const QVariant &v, bool, tlp::Graph *) {
  QComboBox *cb = static_cast<QComboBox *>(editor);
  cb->addItem("false");
  cb->addItem("true");
  cb->setCurrentIndex(v.toBool() ? 1 : 0);
}

QVariant BooleanEditorCreator::editorData(QWidget *editor, tlp::Graph *) {
  return static_cast<QComboBox *>(editor)->currentIndex() == 1;
}

QString BooleanEditorCreator::displayText(const QVariant &v) const {
  return v.toBool() ? "true" : "false";
}

/*
  CoordEditorCreator
*/
QWidget *CoordEditorCreator::createWidget(QWidget *parent) const {
  QMainWindow *mainWindow = getMainWindow();
  return new CoordEditor(mainWindow ? mainWindow : parent, editSize);
}

void CoordEditorCreator::setEditorData(QWidget *w, const QVariant &v, bool, tlp::Graph *) {
  static_cast<CoordEditor *>(w)->setCoord(v.value<tlp::Coord>());
}

QVariant CoordEditorCreator::editorData(QWidget *w, tlp::Graph *) {
  return QVariant::fromValue<tlp::Coord>(static_cast<CoordEditor *>(w)->coord());
}

void CoordEditorCreator::setPropertyToEdit(tlp::PropertyInterface *prop) {
  editSize = (dynamic_cast<tlp::SizeProperty *>(prop) != nullptr);
}

/*
  PropertyInterfaceEditorCreator
*/
QWidget *PropertyInterfaceEditorCreator::createWidget(QWidget *parent) const {
  return new QComboBox(parent);
}

void PropertyInterfaceEditorCreator::setEditorData(QWidget *w, const QVariant &val,
                                                   bool isMandatory, tlp::Graph *g) {
  if (g == nullptr) {
    w->setEnabled(false);
    return;
  }

  PropertyInterface *prop = val.value<PropertyInterface *>();
  QComboBox *combo = static_cast<QComboBox *>(w);
  GraphPropertiesModel<PropertyInterface> *model = nullptr;

  if (isMandatory)
    model = new GraphPropertiesModel<PropertyInterface>(g, false, combo);
  else
    model = new GraphPropertiesModel<PropertyInterface>("Select a property", g, false, combo);

  combo->setModel(model);
  combo->setCurrentIndex(model->rowOf(prop));
}

QVariant PropertyInterfaceEditorCreator::editorData(QWidget *w, tlp::Graph *) {
  QComboBox *combo = static_cast<QComboBox *>(w);
  GraphPropertiesModel<PropertyInterface> *model =
      static_cast<GraphPropertiesModel<PropertyInterface> *>(combo->model());
  return model->data(model->index(combo->currentIndex(), 0), Model::PropertyRole);
}

QString PropertyInterfaceEditorCreator::displayText(const QVariant &v) const {
  PropertyInterface *prop = v.value<PropertyInterface *>();

  if (prop == nullptr)
    return "";

  return prop->getName().c_str();
}

/*
  NumericPropertyEditorCreator
*/
QWidget *NumericPropertyEditorCreator::createWidget(QWidget *parent) const {
  return new QComboBox(parent);
}

void NumericPropertyEditorCreator::setEditorData(QWidget *w, const QVariant &val, bool isMandatory,
                                                 tlp::Graph *g) {
  if (g == nullptr) {
    w->setEnabled(false);
    return;
  }

  NumericProperty *prop = val.value<NumericProperty *>();
  QComboBox *combo = static_cast<QComboBox *>(w);
  GraphPropertiesModel<NumericProperty> *model = nullptr;

  if (isMandatory)
    model = new GraphPropertiesModel<NumericProperty>(g, false, combo);
  else
    model = new GraphPropertiesModel<NumericProperty>("Select a property", g, false, combo);

  combo->setModel(model);
  combo->setCurrentIndex(model->rowOf(prop));
}

QVariant NumericPropertyEditorCreator::editorData(QWidget *w, tlp::Graph *) {
  QComboBox *combo = static_cast<QComboBox *>(w);
  GraphPropertiesModel<NumericProperty> *model =
      static_cast<GraphPropertiesModel<NumericProperty> *>(combo->model());
  return model->data(model->index(combo->currentIndex(), 0), Model::PropertyRole);
}

QString NumericPropertyEditorCreator::displayText(const QVariant &v) const {
  NumericProperty *prop = v.value<NumericProperty *>();

  if (prop == nullptr)
    return "";

  return prop->getName().c_str();
}

/*
  ColorScaleEditorCreator
*/

QWidget *ColorScaleEditorCreator::createWidget(QWidget *parent) const {
  return new ColorScaleButton(ColorScale(), parent);
}

bool ColorScaleEditorCreator::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QVariant &var, const QModelIndex &index) const {
  ItemEditorCreator::paint(painter, option, var, index);
  ColorScaleButton::paintScale(painter, option.rect, var.value<ColorScale>());
  return true;
}

void ColorScaleEditorCreator::setEditorData(QWidget *w, const QVariant &var, bool, tlp::Graph *) {
  static_cast<ColorScaleButton *>(w)->editColorScale(var.value<ColorScale>());
}

QVariant ColorScaleEditorCreator::editorData(QWidget *w, tlp::Graph *) {
  return QVariant::fromValue<ColorScale>(static_cast<ColorScaleButton *>(w)->colorScale());
}

/*
  StringCollectionEditorCreator
*/
QWidget *StringCollectionEditorCreator::createWidget(QWidget *parent) const {
  return new QComboBox(parent);
}

void StringCollectionEditorCreator::setEditorData(QWidget *widget, const QVariant &var, bool,
                                                  tlp::Graph *) {
  StringCollection col = var.value<StringCollection>();
  QComboBox *combo = static_cast<QComboBox *>(widget);

  for (unsigned int i = 0; i < col.size(); ++i)
    combo->addItem(tlp::tlpStringToQString(col[i]));

  combo->setCurrentIndex(col.getCurrent());
}

QVariant StringCollectionEditorCreator::editorData(QWidget *widget, tlp::Graph *) {
  QComboBox *combo = static_cast<QComboBox *>(widget);
  StringCollection col;

  for (int i = 0; i < combo->count(); ++i)
    col.push_back(QStringToTlpString(combo->itemText(i)));

  col.setCurrent(combo->currentIndex());
  return QVariant::fromValue<StringCollection>(col);
}

QString StringCollectionEditorCreator::displayText(const QVariant &var) const {
  StringCollection col = var.value<StringCollection>();
  return tlpStringToQString(col[col.getCurrent()]);
}

// this class is defined to properly catch the return status
// of a QFileDialog. calling QDialog::result() instead does not work
class FileDialog : public QFileDialog {

public:
  FileDialog(QWidget *w) : QFileDialog(w), ok(QDialog::Rejected) {}
  ~FileDialog() override {}
  int ok;
  FileDescriptor previousFileDescriptor;

  void done(int res) override {
    ok = res;
    QFileDialog::done(res);
  }

  void showEvent(QShowEvent *ev) override {
    QDialog::showEvent(ev);

    if (parentWidget())
      move(parentWidget()->window()->frameGeometry().topLeft() +
           parentWidget()->window()->rect().center() - rect().center());
  }
};

/*
  FileDescriptorEditorCreator
  */
QWidget *FileDescriptorEditorCreator::createWidget(QWidget *parent) const {
  QMainWindow *mainWindow = getMainWindow();
  QFileDialog *dlg = new FileDialog(mainWindow ? mainWindow : parent);
#if defined(__APPLE__)
  dlg->setOption(QFileDialog::DontUseNativeDialog, true);
#else
  dlg->setOption(QFileDialog::DontUseNativeDialog, false);
#endif
  dlg->setMinimumSize(300, 400);
  return dlg;
}

void FileDescriptorEditorCreator::setEditorData(QWidget *w, const QVariant &v, bool, tlp::Graph *) {
  FileDescriptor desc = v.value<FileDescriptor>();
  FileDialog *dlg = static_cast<FileDialog *>(w);
  dlg->previousFileDescriptor = desc;

  // force the dialog initial directory
  // only if there is a non empty absolute path
  if (!desc.absolutePath.isEmpty())
    dlg->setDirectory(QFileInfo(desc.absolutePath).absolutePath());

  if (desc.type == FileDescriptor::Directory) {
    dlg->setFileMode(QFileDialog::Directory);
    dlg->setOption(QFileDialog::ShowDirsOnly);
  } else
    dlg->setFileMode(desc.mustExist ? QFileDialog::ExistingFile : QFileDialog::AnyFile);

  dlg->setModal(true);
  dlg->move(QCursor::pos() - QPoint(150, 200));
}

QVariant FileDescriptorEditorCreator::editorData(QWidget *w, tlp::Graph *) {
  FileDialog *dlg = static_cast<FileDialog *>(w);

  int result = dlg->ok;

  if (result == QDialog::Rejected)
    return QVariant::fromValue<FileDescriptor>(dlg->previousFileDescriptor);

  if (!dlg->selectedFiles().empty()) {
    return QVariant::fromValue<FileDescriptor>(
        FileDescriptor(dlg->selectedFiles()[0], (dlg->fileMode() == QFileDialog::Directory)
                                                    ? FileDescriptor::Directory
                                                    : FileDescriptor::File));
  }

  return QVariant::fromValue<FileDescriptor>(FileDescriptor());
}

class QImageIconPool {

public:
  const QIcon &getIconForImageFile(const QString &file) {
    if (iconPool.contains(file)) {
      return iconPool[file];
    } else if (!file.isEmpty()) {
      QImage image;

      QFile imageFile(file);

      if (imageFile.open(QIODevice::ReadOnly)) {
        image.loadFromData(imageFile.readAll());
      }

      if (!image.isNull()) {
        iconPool[file] = QPixmap::fromImage(image.scaled(32, 32));
        return iconPool[file];
      }
    }

    return nullIcon;
  }

  QMap<QString, QIcon> iconPool;

private:
  QIcon nullIcon;
};

static QImageIconPool imageIconPool;

void tlp::addIconToPool(const QString &iconName, const QIcon &icon) {
  imageIconPool.iconPool[iconName] = icon;
}

bool FileDescriptorEditorCreator::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                        const QVariant &v, const QModelIndex &index) const {
  ItemEditorCreator::paint(painter, option, v, index);
  QRect rect = option.rect;
  FileDescriptor fileDesc = v.value<FileDescriptor>();
  QFileInfo fileInfo(fileDesc.absolutePath);
  QString imageFilePath = fileInfo.absoluteFilePath();

  QIcon icon;
  QString text;

  const QIcon &imageIcon = imageIconPool.getIconForImageFile(imageFilePath);

  if (!imageIcon.isNull()) {
    icon = imageIcon;
    text = fileInfo.fileName();
  } else if (fileInfo.isFile()) {
    icon = QApplication::style()->standardIcon(QStyle::SP_FileIcon);
    text = fileInfo.fileName();
  } else if (fileInfo.isDir()) {
    icon = QApplication::style()->standardIcon(QStyle::SP_DirIcon);
    QDir d1 = fileInfo.dir();
    d1.cdUp();
    text = fileInfo.absoluteFilePath().remove(0, d1.absolutePath().length() - 1);
  }

  int iconSize = rect.height() - 4;

  painter->drawPixmap(rect.x() + 2, rect.y() + 2, iconSize, iconSize, icon.pixmap(iconSize));

  int textXPos = rect.x() + iconSize + 5;

  if (option.state.testFlag(QStyle::State_Selected) && option.showDecorationSelected) {
    painter->setPen(option.palette.highlightedText().color());
    painter->setBrush(option.palette.highlightedText());
  } else {
    painter->setPen(option.palette.text().color());
    painter->setBrush(option.palette.text());
  }

  painter->drawText(textXPos, rect.y() + 2, rect.width() - (textXPos - rect.x()), rect.height() - 4,
                    Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap,
                    QFileInfo(fileDesc.absolutePath).fileName());

  return true;
}

QSize FileDescriptorEditorCreator::sizeHint(const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  FileDescriptor fileDesc = data.value<FileDescriptor>();
  QFileInfo fileInfo(fileDesc.absolutePath);
  QString text;

  if (fileInfo.isDir()) {
    QDir d1 = fileInfo.dir();
    d1.cdUp();
    text = fileInfo.absoluteFilePath().remove(0, d1.absolutePath().length() - 1);
  } else {
    text = fileInfo.fileName();
  }

  const int pixmapWidth = 32;

  QFontMetrics fontMetrics(option.font);

  return QSize(pixmapWidth + fontMetrics.boundingRect(text).width(), pixmapWidth);
}

/*
  TextureFileEditorCreator
  */
QWidget *TextureFileEditorCreator::createWidget(QWidget *parent) const {
  QMainWindow *mainWindow = getMainWindow();
  return new TextureFileDialog(mainWindow ? mainWindow : parent);
}

void TextureFileEditorCreator::setEditorData(QWidget *w, const QVariant &v, bool, tlp::Graph *) {
  TextureFile desc = v.value<TextureFile>();
  TextureFileDialog *dlg = static_cast<TextureFileDialog *>(w);
  dlg->setData(desc);
}

QVariant TextureFileEditorCreator::editorData(QWidget *w, tlp::Graph *) {
  TextureFileDialog *dlg = static_cast<TextureFileDialog *>(w);
  return QVariant::fromValue<TextureFile>(dlg->data());
}

bool TextureFileEditorCreator::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                     const QVariant &v, const QModelIndex &index) const {
  ItemEditorCreator::paint(painter, option, v, index);
  QRect rect = option.rect;
  TextureFile tf = v.value<TextureFile>();
  QFileInfo fileInfo(tf.texturePath);
  QString imageFilePath = fileInfo.absoluteFilePath();

  QIcon icon;
  QString text = fileInfo.fileName();

  if (tf.texturePath.startsWith("http")) {
    imageFilePath = tf.texturePath;
  }

  truncateText(text);

  const QIcon &imageIcon = imageIconPool.getIconForImageFile(imageFilePath);

  if (!imageIcon.isNull())
    icon = imageIcon;

  int iconSize = rect.height() - 4;

  painter->drawPixmap(rect.x() + 2, rect.y() + 2, iconSize, iconSize, icon.pixmap(iconSize));

  int textXPos = rect.x() + iconSize + 5;

  if (option.state.testFlag(QStyle::State_Selected) && option.showDecorationSelected) {
    painter->setPen(option.palette.highlightedText().color());
    painter->setBrush(option.palette.highlightedText());
  } else {
    painter->setPen(option.palette.text().color());
    painter->setBrush(option.palette.text());
  }

  painter->drawText(textXPos, rect.y() + 2, rect.width() - (textXPos - rect.x()), rect.height() - 4,
                    Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, text);

  return true;
}

QSize TextureFileEditorCreator::sizeHint(const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  TextureFile tf = data.value<TextureFile>();
  QFileInfo fileInfo(tf.texturePath);
  QString text = fileInfo.fileName();

  truncateText(text);

  const int pixmapWidth = 32;

  QFontMetrics fontMetrics(option.font);

  return QSize(pixmapWidth + fontMetrics.boundingRect(text).width() + 20, pixmapWidth);
}

/*
  FontIconCreator
  */
QWidget *FontIconCreator::createWidget(QWidget *parent) const {
  // Due to a Qt issue when embedding a combo box with a large amount
  // of items in a QGraphicsScene (popup has a too large height,
  // making the scrollbars unreachable ...), we use a native
  // dialog with the combo box inside
  QMainWindow *mainWindow = getMainWindow();
  return new FontIconDialog(mainWindow ? mainWindow : parent);
}

void FontIconCreator::setEditorData(QWidget *w, const QVariant &v, bool, tlp::Graph *) {
  FontIconDialog *tfid = static_cast<FontIconDialog *>(w);
  tfid->setSelectedIconName(v.value<FontIcon>().iconName);
}

QVariant FontIconCreator::editorData(QWidget *w, tlp::Graph *) {
  return QVariant::fromValue<FontIcon>(
      FontIcon(static_cast<FontIconDialog *>(w)->getSelectedIconName()));
}

QString FontIconCreator::displayText(const QVariant &data) const {
  return data.value<FontIcon>().iconName;
}

bool FontIconCreator::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QVariant &v, const QModelIndex &index) const {
  ItemEditorCreator::paint(painter, option, v, index);

  QString iconName = v.value<FontIcon>().iconName;

  if (iconName.isEmpty()) {
    return true;
  }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
  QStyleOptionViewItem opt = option;
  opt.features |= QStyleOptionViewItem::HasDecoration;
  opt.features |= QStyleOptionViewItem::HasDisplay;
#else
  QStyleOptionViewItemV4 opt = option;
  opt.features |= QStyleOptionViewItemV2::HasDecoration;
  opt.features |= QStyleOptionViewItemV2::HasDisplay;
#endif

  opt.icon = FontIconManager::icon(iconName);

  opt.decorationSize = opt.icon.actualSize(QSize(16, 16));

  opt.text = displayText(v);

  QStyle *style = QApplication::style();
  style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, nullptr);
  return true;
}

QSize FontIconCreator::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  static QSize iconSize(16, 16);
  QFontMetrics fontMetrics(option.font);
  return QSize(iconSize.width() + fontMetrics.boundingRect(displayText(data)).width() + 20,
               iconSize.height());
}

/// NodeShapeEditorCreator
QWidget *NodeShapeEditorCreator::createWidget(QWidget *parent) const {
  // Due to a Qt issue when embedding a combo box with a large amount
  // of items in a QGraphicsScene (popup has a too large height,
  // making the scrollbars unreachable ...), we use a native
  // dialog with a QListWidget inside
  std::list<std::pair<QString, QPixmap>> shapes;
  auto glyphs = PluginsManager::availablePlugins<Glyph>();

  for (const auto &glyph : glyphs) {
    QString shapeName = tlpStringToQString(glyph);
    QPixmap pixmap = GlyphRenderer::render(GlyphManager::glyphId(glyph));
    shapes.push_back({shapeName, pixmap});
  }

  QMainWindow *mainWindow = getMainWindow();
  return new ShapeDialog(shapes, mainWindow ? mainWindow : parent);
}

void NodeShapeEditorCreator::setEditorData(QWidget *w, const QVariant &data, bool, tlp::Graph *) {
  ShapeDialog *nsd = static_cast<ShapeDialog *>(w);
  nsd->setSelectedShapeName(
      tlpStringToQString(GlyphManager::glyphName(data.value<NodeShape::NodeShapes>())));
}

QVariant NodeShapeEditorCreator::editorData(QWidget *w, tlp::Graph *) {
  ShapeDialog *nsd = static_cast<ShapeDialog *>(w);
  return QVariant::fromValue<NodeShape::NodeShapes>(static_cast<NodeShape::NodeShapes>(
      GlyphManager::glyphId(QStringToTlpString(nsd->getSelectedShapeName()))));
}

QString NodeShapeEditorCreator::displayText(const QVariant &data) const {
  return tlpStringToQString(GlyphManager::glyphName(data.value<NodeShape::NodeShapes>()));
}

QSize NodeShapeEditorCreator::sizeHint(const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  static QPixmap pixmap = GlyphRenderer::render(data.value<NodeShape::NodeShapes>());
  QFontMetrics fontMetrics(option.font);
  return QSize(pixmap.width() + fontMetrics.boundingRect(displayText(data)).width() + 20,
               pixmap.height());
}

bool NodeShapeEditorCreator::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                   const QVariant &data, const QModelIndex &index) const {
  ItemEditorCreator::paint(painter, option, data, index);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
  QStyleOptionViewItem opt = option;
  opt.features |= QStyleOptionViewItem::HasDecoration;
  opt.features |= QStyleOptionViewItem::HasDisplay;
#else
  QStyleOptionViewItemV4 opt = option;
  opt.features |= QStyleOptionViewItemV2::HasDecoration;
  opt.features |= QStyleOptionViewItemV2::HasDisplay;
#endif

  QPixmap pixmap = GlyphRenderer::render(data.value<NodeShape::NodeShapes>());
  opt.icon = QIcon(pixmap);
  opt.decorationSize = pixmap.size();

  opt.text = displayText(data);

  QStyle *style = QApplication::style();
  style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, nullptr);
  return true;
}

/// EdgeExtremityShapeEditorCreator
QWidget *EdgeExtremityShapeEditorCreator::createWidget(QWidget *parent) const {
  // Due to a Qt issue when embedding a combo box with a large amount
  // of items in a QGraphicsScene (popup has a too large height,
  // making the scrollbars unreachable ...), we use a native
  // dialog with a QListWidget inside
  std::list<std::pair<QString, QPixmap>> shapes;
  shapes.push_back({"NONE", QPixmap()});

  auto glyphs = PluginsManager::availablePlugins<EdgeExtremityGlyph>();

  for (const auto &glyph : glyphs) {
    QString shapeName = tlpStringToQString(glyph);
    QPixmap pixmap = EdgeExtremityGlyphRenderer::render(EdgeExtremityGlyphManager::glyphId(glyph));
    shapes.push_back({shapeName, pixmap});
  }

  QMainWindow *mainWindow = getMainWindow();
  ShapeDialog *shapeDialog = new ShapeDialog(shapes, mainWindow ? mainWindow : parent);
  shapeDialog->setWindowTitle("Select an edge extremity shape");
  return shapeDialog;
}

void EdgeExtremityShapeEditorCreator::setEditorData(QWidget *w, const QVariant &data, bool,
                                                    tlp::Graph *) {
  ShapeDialog *nsd = static_cast<ShapeDialog *>(w);
  nsd->setSelectedShapeName(tlpStringToQString(
      EdgeExtremityGlyphManager::glyphName(data.value<EdgeExtremityShape::EdgeExtremityShapes>())));
}

QVariant EdgeExtremityShapeEditorCreator::editorData(QWidget *w, tlp::Graph *) {
  ShapeDialog *nsd = static_cast<ShapeDialog *>(w);
  return QVariant::fromValue<EdgeExtremityShape::EdgeExtremityShapes>(
      static_cast<EdgeExtremityShape::EdgeExtremityShapes>(
          EdgeExtremityGlyphManager::glyphId(QStringToTlpString(nsd->getSelectedShapeName()))));
}

QString EdgeExtremityShapeEditorCreator::displayText(const QVariant &data) const {
  return tlpStringToQString(
      EdgeExtremityGlyphManager::glyphName(data.value<EdgeExtremityShape::EdgeExtremityShapes>()));
}

bool EdgeExtremityShapeEditorCreator::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                            const QVariant &data, const QModelIndex &index) const {
  ItemEditorCreator::paint(painter, option, data, index);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
  QStyleOptionViewItem opt = option;
  opt.features |= QStyleOptionViewItem::HasDecoration;
  opt.features |= QStyleOptionViewItem::HasDisplay;
#else
  QStyleOptionViewItemV4 opt = option;
  opt.features |= QStyleOptionViewItemV2::HasDecoration;
  opt.features |= QStyleOptionViewItemV2::HasDisplay;
#endif

  QPixmap pixmap =
      EdgeExtremityGlyphRenderer::render(data.value<EdgeExtremityShape::EdgeExtremityShapes>());
  opt.icon = QIcon(pixmap);
  opt.decorationSize = pixmap.size();

  opt.text = displayText(data);

  QStyle *style = QApplication::style();
  style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, nullptr);
  return true;
}

QSize EdgeExtremityShapeEditorCreator::sizeHint(const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  static QPixmap pixmap =
      EdgeExtremityGlyphRenderer::render(data.value<EdgeExtremityShape::EdgeExtremityShapes>());
  QFontMetrics fontMetrics(option.font);
  return QSize(pixmap.width() + fontMetrics.boundingRect(displayText(data)).width() + 40,
               pixmap.height());
}

/// EdgeShapeEditorCreator
QWidget *EdgeShapeEditorCreator::createWidget(QWidget *parent) const {
  QComboBox *combobox = new QComboBox(parent);

  for (int i = 0; i < GlGraphStaticData::edgeShapesCount; i++) {
    combobox->addItem(
        tlpStringToQString(GlGraphStaticData::edgeShapeName(GlGraphStaticData::edgeShapeIds[i])),
        QVariant(GlGraphStaticData::edgeShapeIds[i]));
  }

  return combobox;
}
void EdgeShapeEditorCreator::setEditorData(QWidget *editor, const QVariant &data, bool, Graph *) {
  QComboBox *combobox = static_cast<QComboBox *>(editor);
  combobox->setCurrentIndex(combobox->findData(int(data.value<EdgeShape::EdgeShapes>())));
}

QVariant EdgeShapeEditorCreator::editorData(QWidget *editor, Graph *) {
  QComboBox *combobox = static_cast<QComboBox *>(editor);
  return QVariant::fromValue<EdgeShape::EdgeShapes>(
      static_cast<EdgeShape::EdgeShapes>(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString EdgeShapeEditorCreator::displayText(const QVariant &data) const {
  return tlpStringToQString(GlGraphStaticData::edgeShapeName(data.value<EdgeShape::EdgeShapes>()));
}

// FontEditorCreator
QWidget *FontEditorCreator::createWidget(QWidget *parent) const {
  QMainWindow *mainWindow = getMainWindow();
  return new FontDialog(mainWindow ? mainWindow : parent);
}
void FontEditorCreator::setEditorData(QWidget *editor, const QVariant &data, bool, tlp::Graph *) {
  Font font = data.value<Font>();
  FontDialog *fontWidget = static_cast<FontDialog *>(editor);
  fontWidget->selectFont(font);
  fontWidget->move(QCursor::pos() - QPoint(fontWidget->width() / 2, fontWidget->height() / 2));
}

QVariant FontEditorCreator::editorData(QWidget *editor, tlp::Graph *) {
  FontDialog *fontWidget = static_cast<FontDialog *>(editor);
  return QVariant::fromValue<Font>(fontWidget->getSelectedFont());
}

QString FontEditorCreator::displayText(const QVariant &data) const {
  Font font = data.value<Font>();
  QString text(font.fontName());

  if (font.isBold())
    text += " bold";

  if (font.isItalic())
    text += " italic";

  return text;
}

QWidget *LabelPositionEditorCreator::createWidget(QWidget *parent) const {
  QComboBox *result = new QComboBox(parent);

  for (const auto &i : ViewSettings::POSITION_LABEL_MAP) {
    result->addItem(tlp::tlpStringToQString(i.second),
                    QVariant::fromValue<LabelPosition::LabelPositions>(i.first));
  }

  return result;
}
void LabelPositionEditorCreator::setEditorData(QWidget *w, const QVariant &var, bool,
                                               tlp::Graph *) {
  QComboBox *comboBox = static_cast<QComboBox *>(w);
  comboBox->setCurrentIndex(int(var.value<LabelPosition::LabelPositions>()));
}
QVariant LabelPositionEditorCreator::editorData(QWidget *w, tlp::Graph *) {
  return QVariant::fromValue<LabelPosition::LabelPositions>(
      static_cast<LabelPosition::LabelPositions>(static_cast<QComboBox *>(w)->currentIndex()));
}
QString LabelPositionEditorCreator::displayText(const QVariant &v) const {
  return tlp::tlpStringToQString(
      ViewSettings::POSITION_LABEL_MAP[v.value<LabelPosition::LabelPositions>()]);
}

// GraphEditorCreator
QWidget *GraphEditorCreator::createWidget(QWidget *parent) const {
  return new QLabel(parent);
}

void GraphEditorCreator::setEditorData(QWidget *w, const QVariant &var, bool, tlp::Graph *) {
  Graph *g = var.value<Graph *>();

  if (g != nullptr) {
    std::string name;
    g->getAttribute<std::string>("name", name);
    static_cast<QLabel *>(w)->setText(name.c_str());
  }
}

QVariant GraphEditorCreator::editorData(QWidget *, tlp::Graph *) {
  return QVariant();
}

QString GraphEditorCreator::displayText(const QVariant &var) const {
  Graph *g = var.value<Graph *>();

  if (g == nullptr)
    return QString();

  std::string name;
  g->getAttribute<std::string>("name", name);
  return name.c_str();
}

// EdgeSetEditorCreator
QWidget *EdgeSetEditorCreator::createWidget(QWidget *parent) const {
  return new QLabel(parent);
}

void EdgeSetEditorCreator::setEditorData(QWidget *w, const QVariant &var, bool, tlp::Graph *) {
  std::set<tlp::edge> eset = var.value<std::set<tlp::edge>>();

  std::stringstream ss;
  tlp::EdgeSetType::write(ss, eset);
  static_cast<QLabel *>(w)->setText(ss.str().c_str());
}

QVariant EdgeSetEditorCreator::editorData(QWidget *, tlp::Graph *) {
  return QVariant();
}

QString EdgeSetEditorCreator::displayText(const QVariant &var) const {
  std::set<tlp::edge> eset = var.value<std::set<tlp::edge>>();

  std::stringstream ss;
  tlp::EdgeSetType::write(ss, eset);

  return ss.str().c_str();
}

QWidget *QVectorBoolEditorCreator::createWidget(QWidget *parent) const {
  QMainWindow *mainWindow = getMainWindow();
  VectorEditor *w = new VectorEditor(mainWindow ? mainWindow : parent);
  w->setWindowFlags(Qt::Dialog);
  w->setWindowModality(Qt::ApplicationModal);
  return w;
}

void QVectorBoolEditorCreator::setEditorData(QWidget *editor, const QVariant &v, bool,
                                             tlp::Graph *) {
  QVector<QVariant> editorData;
  QVector<bool> vect = v.value<QVector<bool>>();

  for (int i = 0; i < vect.size(); ++i) {
    editorData.push_back(QVariant::fromValue<bool>(vect[i]));
  }

  static_cast<VectorEditor *>(editor)->setVector(editorData, qMetaTypeId<bool>());

  static_cast<VectorEditor *>(editor)->move(QCursor::pos());
}

QVariant QVectorBoolEditorCreator::editorData(QWidget *editor, tlp::Graph *) {
  QVector<bool> result;
  QVector<QVariant> editorData = static_cast<VectorEditor *>(editor)->vector();

  for (const QVariant &v : editorData)
    result.push_back(v.value<bool>());

  return QVariant::fromValue<QVector<bool>>(result);
}

QString QVectorBoolEditorCreator::displayText(const QVariant &data) const {
  std::vector<bool> v = data.value<QVector<bool>>().toStdVector();

  if (v.empty())
    return QString();

  // use a DataTypeSerializer if any
  DataTypeSerializer *dts = DataSet::typenameToSerializer(std::string(typeid(v).name()));

  if (dts) {
    DisplayVectorDataType<bool> dt(&v);

    std::stringstream sstr;
    dts->writeData(sstr, &dt);

    QString str = tlpStringToQString(sstr.str());

    return truncateText(str, " ...)");
  }

  if (v.size() == 1)
    return QString("1 element");

  return QString::number(v.size()) + QString(" elements");
}

// QStringEditorCreator
QWidget *QStringEditorCreator::createWidget(QWidget *parent) const {
  QMainWindow *mainWindow = getMainWindow();
  StringEditor *editor = new StringEditor(mainWindow ? mainWindow : parent);
  editor->setWindowTitle(QString("Set ") + propName.c_str() + " value");
  editor->setMinimumSize(QSize(250, 250));
  return editor;
}

void QStringEditorCreator::setEditorData(QWidget *editor, const QVariant &var, bool, tlp::Graph *) {
  static_cast<StringEditor *>(editor)->setString(var.toString());
}

QVariant QStringEditorCreator::editorData(QWidget *editor, tlp::Graph *) {
  return static_cast<StringEditor *>(editor)->getString();
}

QString QStringEditorCreator::displayText(const QVariant &var) const {
  QString qstr = var.toString();
  return truncateText(qstr);
}

void QStringEditorCreator::setPropertyToEdit(tlp::PropertyInterface *prop) {
  // we should have a property
  // but it cannot be the case when editing a string vector element
  if (prop)
    propName = prop->getName();
}

// StdStringEditorCreator
void StdStringEditorCreator::setEditorData(QWidget *editor, const QVariant &var, bool,
                                           tlp::Graph *) {
  static_cast<StringEditor *>(editor)->setString(tlpStringToQString(var.value<std::string>()));
}

QVariant StdStringEditorCreator::editorData(QWidget *editor, tlp::Graph *) {
  return QVariant::fromValue<std::string>(
      QStringToTlpString(static_cast<StringEditor *>(editor)->getString()));
}

QString StdStringEditorCreator::displayText(const QVariant &var) const {
  QString qstr = tlpStringToQString(var.value<std::string>());
  return truncateText(qstr);
}

// QStringListEditorCreator
QWidget *QStringListEditorCreator::createWidget(QWidget *parent) const {
  QMainWindow *mainWindow = getMainWindow();
  VectorEditor *w = new VectorEditor(mainWindow ? mainWindow : parent);
  w->setWindowFlags(Qt::Dialog);
  w->setWindowModality(Qt::ApplicationModal);
  return w;
}

void QStringListEditorCreator::setEditorData(QWidget *w, const QVariant &var, bool, Graph *) {
  QStringList strList = var.toStringList();
  QVector<QVariant> vect(strList.length());
  int i = 0;

  for (const QString &s : strList) {
    vect[i++] = s;
  }

  static_cast<VectorEditor *>(w)->setVector(vect, qMetaTypeId<QString>());
}

QVariant QStringListEditorCreator::editorData(QWidget *w, Graph *) {
  auto vect = static_cast<VectorEditor *>(w)->vector();
  QStringList lst;

  for (const QVariant &v : vect)
    lst.push_back(v.toString());

  return lst;
}

QString QStringListEditorCreator::displayText(const QVariant &var) const {
  return QStringListType::toString(var.toStringList()).c_str();
}
