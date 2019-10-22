@echo off
set NSIS_PATH=%1
set TALIPOT_DIR=%2
set DEST_DIR=%3
if [%4] NEQ [] (
  if "%4" == "TRUE" (
    set DEBUG_MODE=%4
  ) else if NOT "%4" == "FALSE" (
    set OUT_FILE=%4
  )
)
SET MINGW=%5
set SRC_DIR=%cd%

echo 'Wiping destination directory'
if EXIST "%DEST_DIR%\\files" (
  del /s /f /q "%DEST_DIR%\\files\\*.*" >nul 2>&1
  for /f %%f in ('dir /ad /b "%DEST_DIR%\\files"') do rd /s /q "%DEST_DIR%\\files\\%%f"
) else ( mkdir "%DEST_DIR%\\files" )

echo 'Copying Talipot files'
xcopy "%TALIPOT_DIR%" "%DEST_DIR%\\files" /E /Q

if NOT "%DEBUG_MODE%" == "TRUE" (
echo 'Removing debug libs'
del /Q /F /S "%DEST_DIR%\\files\\bin\\Qt5*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d.pdb" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\qdirect2dd.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\qminimald.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\qoffscreend.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\qwebgld.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\qwindowsd.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\q*d.pdb" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\styles\\q*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\styles\\q*d.pdb" >nul 2>&1
)

echo 'Removing not needed dlls copied by fixup_bundle'
del /Q /F /S "%DEST_DIR%\\files\\bin\\python*.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\opengl*.dll" >nul 2>&1
if "%MINGW%" == "TRUE" (
  del /Q /F /S "%DEST_DIR%\\files\\bin\\vc*.dll" >nul 2>&1
)

echo 'Removing non dll files from lib directory'
del /Q /F /S "%DEST_DIR%\\files\\lib\\*.dll.a" >nul 2>&1

echo 'Copying NSIS script and licence'

copy "%SRC_DIR%\\Talipot.nsi" "%DEST_DIR%\\"
copy "%SRC_DIR%\\LICENSE" "%DEST_DIR%\\"

echo 'Running NSIS installer generator'
cd "%DEST_DIR%"
set PATH=%NSIS_PATH%;%PATH%
if "%OUT_FILE%" == "" (
  makensis /V4 Talipot.nsi
) else (
  makensis /V4 /DOUT_FILE=%OUT_FILE% Talipot.nsi
)

echo 'Cleaning up files'
del /s /f /q "%DEST_DIR%\\files\\*.*" >nul 2>&1
for /f %%f in ('dir /ad /b "%DEST_DIR%\\files"') do rd /s /q "%DEST_DIR%\\files\\%%f"

echo 'NSIS installer generator completed !'
