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

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include "talipot/PythonIncludes.h"
#include "talipot/PythonInterpreter.h"
#include "ConsoleHandlers.h"

#include <sstream>

#include <QMessageBox>
#include <QApplication>
#include <QLibrary>
#include <QTime>
#include <QTextStream>
#include <QMutex>
#include <QWaitCondition>
#include <QDir>
#if defined(__MINGW32__)
#include <QSslSocket>
#endif

#include <talipot/Release.h>
#include <talipot/TlpTools.h>
#include <talipot/PythonVersionChecker.h>
#include <talipot/TlpQtTools.h>

#include <cstdio>
#ifndef WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

using namespace tlp;

static ConsoleOutputHandler *consoleOuputHandler = nullptr;
static ConsoleOutputEmitter *consoleOuputEmitter = nullptr;

extern QString consoleOuputString;
extern QString consoleErrorOuputString;
extern QString mainScriptFileName;

PyMODINIT_FUNC initconsoleutils();
PyMODINIT_FUNC inittalipotutils();

static PyThreadState *mainThreadState;

static PyGILState_STATE gilState;

static const QString printObjectDictFunction =
    "def printObjectDict(obj):\n"
    "  if hasattr(obj, \"__dict__\"):\n"
    "    for k in obj.__dict__.keys():\n"
#if PY_MAJOR_VERSION >= 3
    "      print(k)\n"
#else
    "      print k\n"
#endif
    "  if hasattr(obj, \"__bases__\"):\n"
    "    for k in obj.__bases__:\n"
    "      printObjectDict(k)\n"
    "  if hasattr(obj, \"__class__\") and obj.__class__ != type(type):\n"
    "    printObjectDict(obj.__class__)\n"
    "";

static const QString printObjectClassFunction = "def printObjectClass(obj):\n"
                                                "  type = \"\"\n"
                                                "  if obj and hasattr(obj, \"__class__\"):\n"
                                                "    if hasattr(obj.__class__, \"__module__\"):\n"
                                                "      mod = obj.__class__.__module__\n"
                                                "      if mod == \"_talipot\":"
                                                "        mod = \"tlp\"\n"
                                                "      type = mod + \".\"\n"
                                                "    if hasattr(obj.__class__, \"__name__\"):\n"
                                                "      type = type + obj.__class__.__name__\n"
#if PY_MAJOR_VERSION >= 3
                                                "    print(type)\n"
#else
                                                "    print type\n"
#endif
                                                "";

#if PY_MAJOR_VERSION >= 3
static QString convertPythonUnicodeObjectToQString(PyObject *pyUnicodeObj) {
  PyObject *utf8Str = PyUnicode_AsUTF8String(pyUnicodeObj);
  QString ret = QString::fromUtf8(PyBytes_AsString(utf8Str));
  decrefPyObject(utf8Str);
  return ret;
}
#endif

static bool scriptPaused = false;
static bool processQtEvents = false;

QTime timer;

class SleepSimulator {
  QMutex localMutex;
  QWaitCondition sleepSimulator;

public:
  SleepSimulator() {
    localMutex.lock();
  }
  ~SleepSimulator() {
    localMutex.unlock();
  }

  void sleep(unsigned long sleepMS) {
    sleepSimulator.wait(&localMutex, sleepMS);
  }
};

static SleepSimulator ss;

void tlp::decrefPyObject(PyObject *obj) {
  Py_XDECREF(obj);
}

int tracefunc(PyObject *, PyFrameObject *, int what, PyObject *) {

  if (what == PyTrace_LINE) {
    if (processQtEvents && !scriptPaused && timer.elapsed() >= 50) {
      QApplication::processEvents();

      timer.start();
    }

    while (scriptPaused) {
      if (processQtEvents)
        QApplication::processEvents();

      ss.sleep(30);
    }
  }

  return 0;
}

const QString PythonInterpreter::pythonPluginsPath(tlpStringToQString(tlp::TalipotLibDir) +
                                                   "talipot/python/");

const QString PythonInterpreter::pythonPluginsPathHome(QDir::homePath() + "/.Talipot-" +
                                                       TALIPOT_MM_VERSION + "/plugins/python");

const char PythonInterpreter::pythonReservedCharacters[] = {
    '#', '%',  '/',  '+', '-', '&', '*', '<', '>', '|', '~', '^', '=',
    '!', '\'', '\"', '{', '}', '(', ')', '[', ']', '.', ':', '@', 0};

const std::vector<QString> PythonInterpreter::pythonAccentuatedCharacters = {"é", "è", "ù", "à",
                                                                             "ç"};

const std::vector<QString> PythonInterpreter::pythonAccentuatedCharactersReplace = {"e", "e", "u",
                                                                                    "a", "c"};

const char *PythonInterpreter::pythonKeywords[] = {
    "def",     "class",    "from",  "in",     "and",  "or",    "not",    "is",
    "with",    "assert",   "for",   "while",  "if",   "elif",  "import", "True",
    "False",   "pass",     "exec",  "else",   "None", "print", "global", "return",
    "break",   "continue", "as",    "lambda", "del",  "try",   "except", "raise",
    "finally", "yield",    "async", "await",  nullptr};

#ifdef _MSC_VER
extern "C" {
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:

    if (QApplication::instance()) {
      PythonInterpreter::getInstance()->initConsoleOutput();
    }

    break;

  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;

  case DLL_PROCESS_DETACH:
    break;
  }

  return TRUE;
}
}
#endif

std::unique_ptr<PythonInterpreter> PythonInterpreter::_instance;
std::once_flag PythonInterpreter::_onceFlag;

PythonInterpreter::PythonInterpreter()
    : _wasInit(false), _runningScript(false), _defaultConsoleWidget(nullptr), _outputEnabled(true),
      _errorOutputEnabled(true) {

  if (Py_IsInitialized()) {
    _wasInit = true;
  }

  if (!_wasInit) {

#if defined(__MINGW32__)
    // When using MSYS2 platform to compile Tulip, force the dynamic loading of
    // OpenSSL libraries Qt was compiled against before Python initialization to
    // avoid a DLL Hell on windows
    QSslSocket::supportsSsl();
#endif

    int argc = 1;
#if PY_MAJOR_VERSION >= 3
    static const std::wstring argv0 = L"talipot";
    wchar_t *argv[1];
    argv[0] = const_cast<wchar_t *>(argv0.c_str());
#else
    static const std::string argv0 = "talipot";
    char *argv[1];
    argv[0] = const_cast<char *>(argv0.c_str());
#endif

    Py_OptimizeFlag = 1;
    Py_NoSiteFlag = 1;

// Fix for GDB debugging on windows when compiling with MinGW.
// GDB contains an embedded Python interpreter that messes up Python Home value.
// When Talipot is compiled with a version of Python different from the one embedded in GDB,
// it crashes at startup when running it through GDB.
// So reset correct one to be able to debug it.
#ifdef __MINGW32__
    QString pythonHome = PythonVersionChecker::getPythonHome();

    if (!pythonHome.isEmpty()) {
#if PY_MAJOR_VERSION >= 3
      static std::wstring pythonHomeWString = pythonHome.toStdWString();
      Py_SetPythonHome(const_cast<wchar_t *>(pythonHomeWString.c_str()));
#else
      static std::string pythonHomeString = QStringToTlpString(pythonHome);
      Py_SetPythonHome(const_cast<char *>(pythonHomeString.c_str()));
#endif
    }

#endif

    // register Talipot builtin Python modules
    PyImport_AppendInittab("consoleutils", initconsoleutils);
    PyImport_AppendInittab("talipotutils", inittalipotutils);

    Py_InitializeEx(0);

    PySys_SetArgv(argc, argv);

    PyEval_InitThreads();
    mainThreadState = PyEval_SaveThread();
  }

  holdGIL();

  importModule("sys");

#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString("__main__");
#else
  PyObject *pName = PyString_FromString("__main__");
#endif
  PyObject *pMainModule = PyImport_Import(pName);
  decrefPyObject(pName);
  PyObject *pMainDict = PyModule_GetDict(pMainModule);
  PyObject *pVersion = PyRun_String("str(sys.version_info[0])+\".\"+str(sys.version_info[1])",
                                    Py_eval_input, pMainDict, pMainDict);

#if PY_MAJOR_VERSION >= 3
  _pythonVersion = convertPythonUnicodeObjectToQString(pVersion);
#else
  _pythonVersion = QString(PyString_AsString(pVersion));
#endif

  if (!_wasInit) {

#if PY_MAJOR_VERSION < 3
    reloadModule("sys");
    runString("sys.setdefaultencoding('utf-8')");
#endif

// hack for linux in order to be able to load dynamic python modules installed on the system (like
// numpy, matplotlib and other cool stuffs)
#ifndef WIN32
    QString libPythonName = QString("libpython") + _pythonVersion;
#ifdef __APPLE__
    libPythonName += QString(".dylib");
#else
    libPythonName += QString(".so.1.0");
#endif

    if (!dlopen(QStringToTlpString(libPythonName).c_str(), RTLD_LAZY | RTLD_GLOBAL)) {

      // for Python 3.2
      libPythonName = QString("libpython") + _pythonVersion + QString("mu");
#ifdef __APPLE__
      libPythonName += QString(".dylib");
#else
      libPythonName += QString(".so.1.0");
#endif

      if (!dlopen(QStringToTlpString(libPythonName).c_str(), RTLD_LAZY | RTLD_GLOBAL)) {
        // for Python 3.3
        libPythonName = QString("libpython") + _pythonVersion + QString("m");
#ifdef __APPLE__
        libPythonName += QString(".dylib");
#else
        libPythonName += QString(".so.1.0");
#endif
        dlopen(QStringToTlpString(libPythonName).c_str(), RTLD_LAZY | RTLD_GLOBAL);
      }
    }

#endif

#if !defined(_MSC_VER)
    initConsoleOutput();
#endif

    if (interpreterInit()) {

      addModuleSearchPath(pythonPluginsPath, true);
      addModuleSearchPath(pythonPluginsPathHome);

#if defined(__APPLE__)
      addModuleSearchPath(tlpStringToQString(tlp::TalipotLibDir) + "../lib/talipot/python", true);
#else
      addModuleSearchPath(tlpStringToQString(tlp::TalipotLibDir) + "/talipot/python", true);
#endif

      // Try to import site package manually otherwise Py_InitializeEx can crash if Py_NoSiteFlag is
      // not set
      // and if the site module is not present on the host system
      // Disable output while trying to import the module to not confuse the user
      runString("import site");
#if PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION >= 3
      runString("site.main()");
#endif
      runString("from talipot import tlp");
      runString("from talipotgui import tlpgui");

      // When importing the talipot module, Talipot Python plugins and
      // startup scripts will be possibly loaded and other Python modules can be loaded as a side
      // effect.
      // Some external modules (like numpy) overrides the SIGINT handler at import,
      // so reinstall the default one, otherwise Talipot can not be interrupted by hitting Ctrl-C in
      // a console
      setDefaultSIGINTHandler();

      runString(printObjectDictFunction);
      runString(printObjectClassFunction);

      runString("import sys;"
                "import talipotutils;"
                "import consoleutils;"
                "sys.stdout = consoleutils.ConsoleOutput(False);"
                "sys.stderr = consoleutils.ConsoleOutput(True);"
                "sys.stdin = consoleutils.ConsoleInput()\n");
    }

    PyEval_SetTrace(tracefunc, nullptr);

// disable exit and quit functions
#if PY_MAJOR_VERSION >= 3
    runString("import builtins;"
              "builtins.exit = lambda *args: None;"
              "builtins.quit= lambda *args: None;");
#else
    runString("import __builtin__;"
              "__builtin__.exit = lambda *args: None;"
              "__builtin__.quit = lambda *args: None;");
#endif

    runString("import sys;"
              "sys.exit = lambda *args: None");
  }

  releaseGIL();
}

PythonInterpreter::~PythonInterpreter() {
  processQtEvents = false;

  if (!_wasInit && interpreterInit()) {
    consoleOuputString = "";
    runString(
        "sys.stdout = sys.__stdout__; sys.stderr = sys.__stderr__; sys.stdin = sys.__stdin__\n");
    PyEval_ReleaseLock();
    PyEval_RestoreThread(mainThreadState);

    holdGIL();
    Py_Finalize();
  }

  delete consoleOuputEmitter;
  consoleOuputEmitter = nullptr;
  delete consoleOuputHandler;
  consoleOuputHandler = nullptr;
}

PythonInterpreter *PythonInterpreter::getInstance() {
  std::call_once(PythonInterpreter::_onceFlag, []() { _instance.reset(new PythonInterpreter); });
  return _instance.get();
}

void PythonInterpreter::initConsoleOutput() {
  consoleOuputHandler = new ConsoleOutputHandler();
  consoleOuputEmitter = new ConsoleOutputEmitter();
  QObject::connect(
      consoleOuputEmitter, SIGNAL(consoleOutput(QAbstractScrollArea *, const QString &, bool)),
      consoleOuputHandler, SLOT(writeToConsole(QAbstractScrollArea *, const QString &, bool)));
}

bool PythonInterpreter::interpreterInit() {
  holdGIL();
  bool ret = Py_IsInitialized();
  releaseGIL();
  return ret;
}

bool PythonInterpreter::importModule(const QString &moduleName) {
  return runString(QString("import ") + moduleName);
}

bool PythonInterpreter::registerNewModuleFromString(const QString &moduleName,
                                                    const QString &moduleSrcCode) {
  bool ret = true;
  holdGIL();
  PyObject *pycomp =
      Py_CompileString(QStringToTlpString(moduleSrcCode).c_str(),
                       QStringToTlpString(moduleName + ".py").c_str(), Py_file_input);

  if (pycomp == nullptr) {
    PyErr_Print();
    PyErr_Clear();
    ret = false;
  } else {

    PyObject *pmod =
        PyImport_ExecCodeModule(const_cast<char *>(QStringToTlpString(moduleName).c_str()), pycomp);

    if (pmod == nullptr) {
      PyErr_Print();
      PyErr_Clear();
      ret = false;
    }
  }

  releaseGIL();
  return ret;
}

bool PythonInterpreter::functionExists(const QString &moduleName, const QString &functionName) {
  holdGIL();
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString(QStringToTlpString(moduleName).c_str());
#else
  PyObject *pName = PyString_FromString(QStringToTlpString(moduleName).c_str());
#endif
  PyObject *pModule = PyImport_Import(pName);
  decrefPyObject(pName);
  PyObject *pDict = PyModule_GetDict(pModule);
  PyObject *pFunc = PyDict_GetItemString(pDict, QStringToTlpString(functionName).c_str());
  bool ret = (pFunc != nullptr && PyCallable_Check(pFunc));
  releaseGIL();
  return ret;
}

bool PythonInterpreter::runString(const QString &pythonCode, const QString &scriptFilePath) {
  if (!scriptFilePath.isEmpty())
    mainScriptFileName = scriptFilePath;

  int ret = 0;
  holdGIL();
  ret = PyRun_SimpleString(QStringToTlpString(pythonCode).c_str());

  if (PyErr_Occurred()) {
    PyErr_Print();
    PyErr_Clear();
  }

  releaseGIL();

  if (!scriptFilePath.isEmpty())
    mainScriptFileName = "";

  return ret != -1;
}

PyObject *PythonInterpreter::evalPythonStatement(const QString &pythonStatement, bool singleInput) {
  holdGIL();
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString("__main__");
#else
  PyObject *pName = PyString_FromString("__main__");
#endif
  PyObject *pMainModule = PyImport_Import(pName);
  decrefPyObject(pName);
  PyObject *pMainDict = PyModule_GetDict(pMainModule);

  PyObject *ret = PyRun_String(QStringToTlpString(pythonStatement).c_str(),
                               singleInput ? Py_single_input : Py_eval_input, pMainDict, pMainDict);

  if (PyErr_Occurred()) {
    PyErr_Print();
    PyErr_Clear();
  }

  releaseGIL();

  return ret;
}

PyObject *PythonInterpreter::callPythonFunction(const QString &module, const QString &function,
                                                const tlp::DataSet &parameters) {
  holdGIL();
  PyObject *ret = nullptr;
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString(QStringToTlpString(module).c_str());
#else
  PyObject *pName = PyString_FromString(QStringToTlpString(module).c_str());
#endif

  PyObject *pModule = PyImport_Import(pName);
  decrefPyObject(pName);

  PyObject *pDict = PyModule_GetDict(pModule);
  decrefPyObject(pModule);

  PyObject *pFunc = PyDict_GetItemString(pDict, QStringToTlpString(function).c_str());

  if (PyCallable_Check(pFunc)) {
    PyObject *argTup = PyTuple_New(parameters.size());
    int idx = 0;
    bool paramError = false;
    for (const std::pair<std::string, DataType *> &param : parameters.getValues()) {
      PyObject *pyParam = getPyObjectFromDataType(param.second);

      if (!pyParam) {
        paramError = true;
        break;
      }

      PyTuple_SetItem(argTup, idx++, pyParam);
    }

    if (!paramError) {
      ret = PyObject_CallObject(pFunc, argTup);

      if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
      }
    }

    decrefPyObject(argTup);
  }

  releaseGIL();
  return ret;
}

bool PythonInterpreter::callFunction(const QString &module, const QString &function,
                                     const tlp::DataSet &parameters) {
  holdGIL();
  bool ok = false;
  PyObject *ret = callPythonFunction(module, function, parameters);

  if (ret) {
    ok = true;
  }

  decrefPyObject(ret);
  releaseGIL();
  return ok;
}

void PythonInterpreter::addModuleSearchPath(const QString &path, const bool beforeOtherPaths) {
  if (_currentImportPaths.find(path) == _currentImportPaths.end()) {
    QString pythonCode;
    QTextStream oss(&pythonCode);
    oss << "import sys" << endl;

    if (beforeOtherPaths) {
      oss << "sys.path.insert(0, \"" << path << "\")" << endl;
    } else {
      oss << "sys.path.append(\"" << path << "\")" << endl;
    }

    runString(pythonCode);
    _currentImportPaths.insert(path);
  }
}

bool PythonInterpreter::runGraphScript(const QString &module, const QString &function,
                                       tlp::Graph *graph, const QString &scriptFilePath) {

  if (!scriptFilePath.isEmpty())
    mainScriptFileName = scriptFilePath;

  timer.start();

  holdGIL();

  clearTracebacks();

  // ensure to reset the trace function in order to be able to pause a script (need that call for
  // that feature to work on windows platform)
  PyEval_SetTrace(tracefunc, nullptr);

  bool ret = true;
  scriptPaused = false;

// Build the name object
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString(QStringToTlpString(module).c_str());
#else
  PyObject *pName = PyString_FromString(QStringToTlpString(module).c_str());
#endif
  // Load the module object
  PyObject *pModule = PyImport_Import(pName);
  decrefPyObject(pName);

  // If the module is not __main__, reload it (needed to avoid errors when calling the
  // runGraphScript
  // function from Python)
  if (module != "__main__") {
    PyObject *pModuleReloaded = PyImport_ReloadModule(pModule);
    decrefPyObject(pModule);
    pModule = pModuleReloaded;
  }

  // pDict is a borrowed reference
  PyObject *pDict = PyModule_GetDict(pModule);

  // pFunc is also a borrowed reference
  PyObject *pFunc = PyDict_GetItemString(pDict, QStringToTlpString(function).c_str());

  if (PyCallable_Check(pFunc)) {

    if (sipAPI() == nullptr) {
      QMessageBox::critical(nullptr, "Failed to initialize Python",
                            "SIP could not be initialized (sipApi is null)");
      return false;
    }

    // Getting proper sipWrapperType
    const sipTypeDef *kpTypeDef = sipFindType("tlp::Graph");

    if (!kpTypeDef) {
      return false;
    }

    // Wrapping up C++ instance
    PyObject *pArgs = sipConvertFromType(graph, kpTypeDef, nullptr);

    // Finally calling 'process'
    PyObject *argTup = Py_BuildValue("(O)", pArgs);
    _runningScript = true;

    try {
      PyObject_CallObject(pFunc, argTup);
    } catch (std::exception &e) {
      std::ostringstream oss;
      oss << "A C++ exception (" << e.what() << ") has been thrown while executing the script";
      PyErr_SetString(PyExc_Exception, oss.str().c_str());
    } catch (...) {
      PyErr_SetString(PyExc_Exception,
                      "A C++ exception has been thrown while executing the script");
    }

    _runningScript = false;
    decrefPyObject(argTup);
    decrefPyObject(pArgs);
    decrefPyObject(pModule);

    if (PyErr_Occurred()) {
      PyErr_Print();
      PyErr_Clear();
      ret = false;
    }

  } else {
    PyErr_Print();
    PyErr_Clear();
    ret = false;
  }

  releaseGIL();

  if (!scriptFilePath.isEmpty())
    mainScriptFileName = "";

  return ret;
}

int stopScript(void *) {
  PyErr_SetString(PyExc_Exception, "Script execution terminated by user");
  return -1;
}

void PythonInterpreter::stopCurrentScript() {
  if (_runningScript) {
    holdGIL();
    Py_AddPendingCall(&stopScript, nullptr);
    releaseGIL();
    scriptPaused = false;
  }
}

void PythonInterpreter::deleteModule(const QString &moduleName) {
  QString pythonCode;
  QTextStream oss(&pythonCode);
  oss << "import sys" << endl;
  oss << "if \"" << moduleName << "\" in sys.modules:" << endl;
  oss << "  del sys.modules[\"" << moduleName << "\"]" << endl;
  runString(pythonCode);
}

bool PythonInterpreter::reloadModule(const QString &moduleName) {
  QString pythonCode;
  QTextStream oss(&pythonCode);
  oss << "import sys" << endl;
  oss << "if sys.version_info[0] == 3:" << endl;
  oss << "  from imp import reload" << endl;
  oss << "import " << moduleName << endl;
  oss << "reload(" << moduleName << ")" << endl;
  return runString(pythonCode);
}

void PythonInterpreter::setDefaultConsoleWidget(QPlainTextEdit *console) {
  setDefaultConsoleWidget(static_cast<QAbstractScrollArea *>(console));
}

void PythonInterpreter::setConsoleWidget(QPlainTextEdit *console) {
  setConsoleWidget(static_cast<QAbstractScrollArea *>(console));
}

void PythonInterpreter::setDefaultConsoleWidget(QTextBrowser *console) {
  setDefaultConsoleWidget(static_cast<QAbstractScrollArea *>(console));
}

void PythonInterpreter::setConsoleWidget(QTextBrowser *console) {
  setConsoleWidget(static_cast<QAbstractScrollArea *>(console));
}

void PythonInterpreter::setDefaultConsoleWidget(QAbstractScrollArea *console) {
  _defaultConsoleWidget = console;
}

void PythonInterpreter::setConsoleWidget(QAbstractScrollArea *console) {
  setOutputEnabled(true);
  setErrorOutputEnabled(true);

  if (consoleOuputEmitter) {
    consoleOuputEmitter->setConsoleWidget(console);
  }
}

void PythonInterpreter::resetConsoleWidget() {
  setOutputEnabled(true);
  setErrorOutputEnabled(true);

  if (consoleOuputEmitter) {
    consoleOuputEmitter->setConsoleWidget(nullptr);
  }
}

void PythonInterpreter::setDefaultSIGINTHandler() {
  setOutputEnabled(false);
  setErrorOutputEnabled(false);

  if (runString("import signal")) {
    runString("signal.signal(signal.SIGINT, signal.SIG_DFL)");
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
}

QString PythonInterpreter::getPythonShellBanner() {
  holdGIL();
  QString ret =
      QString("Python ") + QString(Py_GetVersion()) + QString(" on ") + QString(Py_GetPlatform());
  releaseGIL();
  return ret;
}

QVector<QString> PythonInterpreter::getGlobalDictEntries(const QString &prefixFilter) {
  QVector<QString> ret;
  QSet<QString> publicMembersSorted;
  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  consoleOuputString = "";
  runString("import __main__;printObjectDict(__main__)");
  QStringList objectDictList = consoleOuputString.split("\n");

  for (int i = 0; i < objectDictList.count(); ++i) {
    if (!objectDictList[i].isEmpty()) {
      if (objectDictList[i].startsWith("_")) {
        continue;
      } else {
        if (!prefixFilter.isEmpty()) {
          if (objectDictList[i].startsWith(prefixFilter)) {
            publicMembersSorted.insert(objectDictList[i]);
          }
        } else {
          publicMembersSorted.insert(objectDictList[i]);
        }
      }
    }
  }

  for (const auto &it : publicMembersSorted) {
    ret.push_back(it);
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
  return ret;
}

QString PythonInterpreter::getVariableType(const QString &varName) {
  // avoid any function call
  if (varName.contains('('))
    return "";

  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  consoleOuputString = "";
  bool ok = runString(QString("printObjectClass(") + varName + ")");
  setOutputEnabled(true);
  setErrorOutputEnabled(true);

  if (ok)
    return consoleOuputString.mid(0, consoleOuputString.size() - 1);
  else
    return "";
}

QVector<QString> PythonInterpreter::getObjectDictEntries(const QString &objectName,
                                                         const QString &prefixFilter) {
  QVector<QString> ret;
  QSet<QString> publicMembersSorted;
  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  consoleOuputString = "";

  if (runString(objectName)) {
    runString(QString("printObjectDict(") + objectName + ")");
    QStringList objectDictList = consoleOuputString.split("\n");

    for (int i = 0; i < objectDictList.count(); ++i) {
      if (!objectDictList[i].isEmpty()) {
        if (objectDictList[i].startsWith("_")) {
          continue;
        } else {
          if (!prefixFilter.isEmpty()) {
            if (objectDictList[i].startsWith(prefixFilter)) {
              publicMembersSorted.insert(objectDictList[i]);
            }
          } else {
            publicMembersSorted.insert(objectDictList[i]);
          }
        }
      }
    }

    for (const auto &it : publicMembersSorted) {
      ret.push_back(it);
    }
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
  return ret;
}

QVector<QString> PythonInterpreter::getImportedModulesList() {
  QVector<QString> ret;
  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  consoleOuputString = "";
#if PY_MAJOR_VERSION >= 3

  if (runString("import sys\nfor mod in sorted(sys.modules.keys()): print(mod)")) {
#else

  if (runString("import sys\nfor mod in sorted(sys.modules.keys()): print mod")) {
#endif
    QStringList modulesList = consoleOuputString.split("\n");

    for (int i = 0; i < modulesList.count(); ++i) {
      if (!modulesList[i].isEmpty()) {
        if (modulesList[i].startsWith("_")) {
          continue;
        } else {
          ret.push_back(modulesList[i]);
        }
      }
    }
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
  return ret;
}

QVector<QString> PythonInterpreter::getBaseTypesForType(const QString &typeName) {
  QVector<QString> ret;
  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  QStringList modules = typeName.split(".");
  QString curModule = "";

  for (int i = 0; i < modules.size() - 1; ++i) {
    curModule += modules[i];
    runString(QString("import ") + curModule);
    curModule += ".";
  }

  consoleOuputString = "";

  QString pythonCode;

#if PY_MAJOR_VERSION >= 3
  pythonCode = QString("for base in ") + typeName + ".__bases__ : print(base)";
#else
  pythonCode = QString("for base in ") + typeName + ".__bases__ : print base";
#endif

  if (runString(pythonCode)) {
    QStringList basesList = consoleOuputString.split("\n");

    for (int i = 0; i < basesList.count(); ++i) {
      int pos = basesList[i].indexOf('\'');
      int pos2 = basesList[i].lastIndexOf('\'');
      ret.push_back(basesList[i].mid(pos + 1, pos2 - pos - 1));
    }
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
  return ret;
}

void PythonInterpreter::holdGIL() {
  if (!_wasInit)
    gilState = PyGILState_Ensure();
}

void PythonInterpreter::releaseGIL() {
  if (!_wasInit)
    PyGILState_Release(gilState);
}

QString PythonInterpreter::getStandardOutput() const {
  return consoleOuputString;
}

QString PythonInterpreter::getStandardErrorOutput() const {
  return consoleErrorOuputString;
}

void PythonInterpreter::clearOutputBuffers() {
  consoleOuputString = "";
  consoleErrorOuputString = "";
}

void PythonInterpreter::pauseCurrentScript(const bool pause) {
  bool wasPaused = scriptPaused;
  scriptPaused = pause;

  if (!wasPaused && scriptPaused) {
    emit scriptExecutionPaused();
  }
}

bool PythonInterpreter::isScriptPaused() const {
  return scriptPaused;
}

void PythonInterpreter::setProcessQtEventsDuringScriptExecution(bool processEvents) {
  processQtEvents = processEvents;
}

void PythonInterpreter::setOutputEnabled(const bool enableOutput) {
  _outputEnabled = enableOutput;
}

bool PythonInterpreter::outputEnabled() const {
  return _outputEnabled;
}

void PythonInterpreter::setErrorOutputEnabled(const bool enableOutput) {
  _errorOutputEnabled = enableOutput;
}

bool PythonInterpreter::errorOutputEnabled() const {
  return _errorOutputEnabled;
}

double PythonInterpreter::getPythonVersion() const {
  return atof(QStringToTlpString(_pythonVersion).c_str());
}

void PythonInterpreter::sendOutputToConsole(const QString &output, bool stdErr) {
  bool textOutput = false;

  if (consoleOuputEmitter) {
    if ((outputEnabled() && !stdErr) || (errorOutputEnabled() && stdErr)) {
      consoleOuputEmitter->sendOutputToConsole(output, stdErr);
    }
  } else {
    textOutput = true;
  }

  if (textOutput) {
    if (errorOutputEnabled() && stdErr) {
      std::cerr << QStringToTlpString(output);
    } else if (outputEnabled() && !stdErr) {
      std::cout << QStringToTlpString(output);
    }
  }
}

QString PythonInterpreter::readLineFromConsole() {
  if (consoleOuputEmitter && consoleOuputEmitter->consoleWidget()) {
    ConsoleInputHandler cih;
    cih.setConsoleWidget(consoleOuputEmitter->consoleWidget());
    cih.startReadLine();

    while (!cih.lineRead()) {
      QApplication::processEvents();
      ss.sleep(30);
    }

    return cih.line();
  }

  return "";
}

void PythonInterpreter::clearTracebacks() {
  QString pythonCode = "import sys\n";
#if PY_MAJOR_VERSION < 3
  pythonCode += "sys.exc_clear()\n";
  pythonCode += "sys.exc_traceback = None\n";
#endif
  pythonCode += "sys.last_traceback = None\n";
  runString(pythonCode);
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
