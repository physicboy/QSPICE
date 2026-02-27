@echo off
setlocal

REM === TARGET FOLDER ===
set "SOURCE_DIR=C:\Program Files\QSPICE"

REM === TIMESTAMP ===
for /f %%i in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set TS=%%i

REM === 7-ZIP PATH (DEFAULT INSTALL LOCATION) ===
set "ZIP=C:\Program Files\7-Zip\7z.exe"

REM === OUTPUT ZIP FILE ===
set "ZIPFILE=C:\Program Files\QSPICE\QSPICE_BACKUP_%TS%.zip"

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

REM === COMPRESS ===
"%ZIP%" a -tzip "%ZIPFILE%" "%SOURCE_DIR%\*"

echo.
echo Created: %ZIPFILE%
endlocal
pause