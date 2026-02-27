@echo off
:: Check for admin rights
net session >nul 2>&1
if %errorlevel% NEQ 0 (
    echo Requesting administrative privileges...
    powershell -Command "Start-Process '%~f0' -Verb runAs"
    exit /b
)

echo Running as administrator!

setlocal

REM === TARGET FOLDER ===
set "SOURCE_DIR=C:\Program Files\QSPICE"

REM === TIMESTAMP ===
for /f %%i in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set TS=%%i

REM === 7-ZIP PATH (DEFAULT INSTALL LOCATION) ===
set "ZIP=C:\Program Files\7-Zip\7z.exe"

REM === OUTPUT ZIP FILE ===
set "ZIPFILE=C:\Program Files\QSPICE\QSPICE_BACKUP_%TS%.zip"
REM === ALTERNATE TARGET DIRECTORY FOR ZIP FILE TO AVOID PERMISSION ===
REM set "ZIPFILE=%USERPROFILE%\Desktop\QSPICE_BACKUP_%TS%.zip"

REM === SAFETY CHECK ===
if not exist "%ZIP%" (
    echo ERROR: 7-Zip not found at "%ZIP%"
    pause
    exit /b 1
)

if not exist "%SOURCE_DIR%" (
    echo ERROR: Folder "%SOURCE_DIR%" not found
    pause
    exit /b 1
)

REM === COMPRESS (exclude previous QSPICE_BACKUP zip files) ===
"%ZIP%" a -tzip "%ZIPFILE%" "%SOURCE_DIR%\*" -x!QSPICE_BACKUP*.zip

echo.
echo Created: %ZIPFILE%
endlocal
pause