@echo off

echo "Starting Talipot ..."

rem Talipot needs Python but on windows it can be installed for current user only.
rem In that case, the Python dll is not located in system path but in the Python home directory.
rem So add the Python home directory in the Dll search paths in order to be able to start Talipot
set PATH=%~dp0;%PATH%
for /F "delims=" %%i in ('getpythonhome.exe') do set TALIPOT_PYTHON_HOME=%%i
set PATH=%TALIPOT_PYTHON_HOME%;%PATH%
start talipot.exe %*