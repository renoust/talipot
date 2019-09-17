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

#ifndef TULIPEXCEPTION_H
#define TULIPEXCEPTION_H

#include <tulip/tulipconf.h>
#include <string>

namespace tlp {

//=======================================
/**
 * @class TulipException
 * @brief TulipException is a basic class to build exceptions from string
 **/
class TLP_SCOPE TulipException : public std::exception {
public:
  TulipException(const std::string &desc);
  ~TulipException() throw() override;
  const char *what() const throw() override;

private:
  std::string desc;
};
} // namespace tlp
#endif // TULIPEXCEPTION
///@endcond
