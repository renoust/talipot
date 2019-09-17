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

#ifndef LINUXSIGNALINTERPOSER_H_
#define LINUXSIGNALINTERPOSER_H_

#include <csignal>

typedef void SignalHandlerFunc(int);
typedef void SigactionHandlerFunc(int, siginfo_t *, void *);

extern void installSignalHandler(int sig, SignalHandlerFunc *handler);

extern void installSignalHandler(int sig, SigactionHandlerFunc *handler);

#endif /* LINUXSIGNALINTERPOSER_H_ */
