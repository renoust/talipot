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
#include <QString>
#include <QFile>
#include <QTextStream>

#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>

namespace tlp {

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
QString getSipVersion() {
  return SIP_VERSION;
}
#endif

QString getTulipGitRevision() {
  QFile gitCommitFile(tlpStringToQString(TulipShareDir + "GIT_COMMIT"));

  if (gitCommitFile.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream in(&gitCommitFile);
    in.setCodec("UTF-8");
    return in.readAll().replace("\n", "");
  }
  return "";
}
} // namespace tlp
