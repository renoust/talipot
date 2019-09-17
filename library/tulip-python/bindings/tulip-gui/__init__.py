# Copyright (C) 2019  The Talipot developers
#
# Talipot is a fork of Tulip, created by David Auber
# and the Tulip development Team from LaBRI, University of Bordeaux
#
# See the AUTHORS file at the top-level directory of this distribution
# License: GNU General Public License version 3, or any later version
# See top-level LICENSE file for more information

# -*- coding: utf-8 -*-

import os
import os.path
import sys
import platform

from tulip import tlp

# fix an ubuntu bug when using the nvidia driver,
# when the python interpreter dynamically loads the GL library,
# it ends up loading the mesa one instead of the nvidia one.
# The use of the tulipgui module through the classical python intrepreter
# was quite unusable because of that ...
# Apply the workaround proposed here :
# https://bugs.launchpad.net/ubuntu/+source/python-qt4/+bug/941826
if hasattr(os, 'uname'):
    if os.uname()[3].lower().find('ubuntu') != -1:
        import ctypes.util
        ctypes.CDLL(ctypes.util.find_library('GL'), ctypes.RTLD_GLOBAL)

_tulipGuiNativeLibsPath = '%s/native/' % os.path.dirname(__file__)
_tulipGuiNativePluginsPath = '%splugins' % _tulipGuiNativeLibsPath
_tulipGuiPluginsPath = '%s/plugins/' % os.path.dirname(__file__)

sys.path.append(_tulipGuiNativeLibsPath)

if platform.system() == 'Windows':
    os.environ['PATH'] = '%s;%s../../../;%s' % (
        _tulipGuiNativeLibsPath, _tulipGuiNativeLibsPath, os.environ['PATH'])

if not sys.argv[0] == 'tulip':
    # when the tulipgui module is installed from the Python Packaging Index
    # modify the TulipBitmapDir variable as it is different from
    # the default one
    bitmapDir = '%s/share/bitmaps/' % os.path.dirname(__file__)
    if os.path.isdir(bitmapDir):
        tlp.TulipBitmapDir = bitmapDir
        tlp.TulipPluginsPath = _tulipGuiNativePluginsPath
        tlp.TulipViewSettings.instance().setDefaultFontFile(
            '%sfont.ttf' % tlp.TulipBitmapDir)


import _tulipgui # noqa

sys.path.pop()


class tlpgui(_tulipgui.tlpgui):
    pass


def tulipguiExitFunc():
    import tulipgui
    tulipgui.tlpgui.runQtMainLoop()


# fix loading of Tulip plugins when the tulipgui module has been installed
# with the pip tool
if platform.system() == 'Linux' and os.path.exists(_tulipGuiNativePluginsPath):
    dlOpenFlagsBackup = sys.getdlopenflags()
    if sys.version_info < (3, 6):
        import DLFCN
        dlOpenFlags = DLFCN.RTLD_NOW | DLFCN.RTLD_GLOBAL
    else:
        dlOpenFlags = os.RTLD_NOW | os.RTLD_GLOBAL
    sys.setdlopenflags(dlOpenFlags)

tlp.loadTulipPluginsFromDir(_tulipGuiPluginsPath)

if (platform.system() == 'Linux'
        and os.path.exists(_tulipGuiNativePluginsPath)):
    sys.setdlopenflags(dlOpenFlagsBackup)

# Check if we are in script execution mode
# (sys.ps1 is not defined in that case)
# If so, register an exit callback that will run the Qt event loop
# if some widgets are still in a visible state
if not hasattr(sys, 'ps1') and not sys.flags.interactive:
    import atexit
    atexit.register(tulipguiExitFunc)

import signal # noqa
signal.signal(signal.SIGINT, signal.SIG_DFL)

__all__ = ['tlpgui']
__author__ = 'David Auber, and the Tulip development Team'
__license__ = 'LGPLv3'
__version__ = tlp.getTulipRelease()
__email__ = 'tulipdev@labri.fr'
__status__ = 'Production'
