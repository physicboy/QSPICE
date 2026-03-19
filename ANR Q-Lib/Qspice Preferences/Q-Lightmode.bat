@echo off
REM ==============================
REM QSPICE Light Mode Preferences
REM ==============================

REM --- Schematic editor ---
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v AutoDeleteRawFiles /t REG_SZ /d true /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v BUSwidthMultiplier /t REG_SZ /d 2.0 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADBackgroundColor /t REG_SZ /d 0xffffff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADbomberSight /t REG_SZ /d 0x000000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADbusTapError /t REG_SZ /d 0x0000ff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADcommentText /t REG_SZ /d 0xff00ff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADgrapicAnnotation /t REG_SZ /d 0xff0000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADgrapicText /t REG_SZ /d 0xff0000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADhierarchalBlock /t REG_SZ /d 0xff0000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADhierarchalFill /t REG_SZ /d 0xc8c8c8 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADhighlightWireColor /t REG_SZ /d 0x64e1e1 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADlineWidth /t REG_SZ /d 1.5 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADnetLabelText /t REG_SZ /d 0x0000ff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADnormalSchText /t REG_SZ /d 0x000000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADnormalText /t REG_SZ /d 0x000000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADpinColor /t REG_SZ /d 0x000000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADpinDotColor /t REG_SZ /d 0x000000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADportBkColor /t REG_SZ /d 0xffffff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADsolidAnnotation /t REG_SZ /d 0x00ffff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADspritedAnchor /t REG_SZ /d 0x00ffff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADunconnectedPin /t REG_SZ /d 0xffffff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CADwireColor /t REG_SZ /d 0xff0000 /f

REM --- Misc preferences ---
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DefaultTextSize /t REG_SZ /d 1.0 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DoNoShowQuickTips /t REG_SZ /d true /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v EnableDotOpHints /t REG_SZ /d true /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v FastMath /t REG_SZ /d true /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v IgnoreVerilogWarnings /t REG_SZ /d true /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v JunctionSize /t REG_SZ /d 2.0 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v NoInternetEpoch /t REG_SZ /d 0 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v NotDisturbEpoch /t REG_SZ /d 0 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v OverlayPriorSimulation /t REG_SZ /d false /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v ReverseMouseWheelSense /t REG_SZ /d false /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v SavePowers /t REG_SZ /d false /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v ShowDeviceAndIPbrowser /t REG_SZ /d false /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v ShowMacStyleStart /t REG_SZ /d true /f

REM --- Waveform viewer ---
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v BackgroundColor /t REG_SZ /d 0xffffff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CursorBackFore /t REG_SZ /d 0xffffff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CursorBackGnd /t REG_SZ /d 0x0000ff /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v CursorLineColor /t REG_SZ /d 0xc8c8c8 /f

reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor1 /t REG_SZ /d 0xb80000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor2 /t REG_SZ /d 0x0058b8 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor3 /t REG_SZ /d 0x00a800 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor4 /t REG_SZ /d 0xb85c00 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor5 /t REG_SZ /d 0x8000b8 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor6 /t REG_SZ /d 0x008080 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor7 /t REG_SZ /d 0xb8005c /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor8 /t REG_SZ /d 0x00b85c /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor9 /t REG_SZ /d 0x804000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor10 /t REG_SZ /d 0x004080 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor11 /t REG_SZ /d 0x408000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor12 /t REG_SZ /d 0x800040 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor13 /t REG_SZ /d 0x0080b8 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor14 /t REG_SZ /d 0xb88000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor15 /t REG_SZ /d 0x005c00 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor16 /t REG_SZ /d 0x580080 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor17 /t REG_SZ /d 0x808000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor18 /t REG_SZ /d 0x800080 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor19 /t REG_SZ /d 0x004040 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor20 /t REG_SZ /d 0xb84c4c /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor21 /t REG_SZ /d 0x0040b8 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor22 /t REG_SZ /d 0x40b840 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor23 /t REG_SZ /d 0x8040b8 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v DataColor24 /t REG_SZ /d 0x808080 /f

reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v FrameColor /t REG_SZ /d 0x000000 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v NumberDataTraceColors /t REG_SZ /d 24 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v WaveFormViewerFontPoints /t REG_SZ /d 80 /f
reg add "HKCU\Software\Marcus Aurelius Software LLC\QSPICE\[Preferences]" /v WaveformPlotLineWidth /t REG_SZ /d 2.0 /f

exit
