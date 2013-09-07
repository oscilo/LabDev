Name "LaboratoryDevices"
OutFile "LaboratoryDevicesSetup.exe"
InstallDir $PROGRAMFILES\LaboratoryDevices
RequestExecutionLevel admin

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Section "Core files (required)"
  SectionIn RO
  
  SetOutPath "$INSTDIR\Bin"
  File "..\Output\*.dll"
  File "..\Output\*.exe"
  File /r "..\Output\*.png"
  File "$%QTDIR%\bin\Qt5Core.dll"
  File "$%QTDIR%\bin\Qt5Gui.dll"
  File "$%QTDIR%\bin\Qt5Sql.dll"
  File "$%QTDIR%\bin\Qt5Svg.dll"
  File "$%QTDIR%\bin\Qt5Xml.dll"
  File "$%QTDIR%\bin\Qt5Widgets.dll"
  File "$%QTDIR%\bin\libGLESv2.dll"
  File "$%QTDIR%\bin\libEGL.dll"
  File "$%QTDIR%\bin\icuuc51.dll"
  File "$%QTDIR%\bin\icuin51.dll"
  File "$%QTDIR%\bin\icudt51.dll"
  
  SetOutPath "$INSTDIR\Bin\platforms"
  File "$%QTDIR%\plugins\platforms\qwindows.dll"
  File "$%QTDIR%\plugins\platforms\qminimal.dll"
  File "$%QTDIR%\plugins\platforms\qoffscreen.dll"
  
  SetOutPath "$INSTDIR\Bin\sqldrivers"
  File "$%QTDIR%\plugins\sqldrivers\qsqlite.dll"
  
  SetOutPath "$INSTDIR\BlackBoxValues"
  File "..\BlackBoxValues\*.val"
  
  SetOutPath "$INSTDIR\DB"
  File "..\DB\LaboratoryFacilities.db3"
  
  SetOutPath "$INSTDIR\Documents"
  File /r "..\Documents\*.*"
  
  SetOutPath "$INSTDIR\Temp"
  File ".\Resources\vcredist_x86.exe"
  ExecWait "$INSTDIR\Temp\vcredist_x86.exe"
  Delete "$INSTDIR\Temp\vcredist_x86.exe"
  RMDir "$INSTDIR\Temp"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\LaboratoryDevices "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LaboratoryDevices" "DisplayName" "LaboratoryDevices"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LaboratoryDevices" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LaboratoryDevices" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LaboratoryDevices" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  SetOutPath "$INSTDIR\Bin"
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\LaboratoryDevices"
  CreateShortCut "$SMPROGRAMS\LaboratoryDevices\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\LaboratoryDevices\LaboratoryDevices.lnk" "$INSTDIR\Bin\LaboratoryDevices.exe" "" "$INSTDIR\Bin\LaboratoryDevices.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LaboratoryDevices"
  DeleteRegKey HKLM SOFTWARE\LaboratoryDevices

  ; Remove files and uninstaller
 ; Delete "$INSTDIR\*.*"
  ;Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\LaboratoryDevices\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\LaboratoryDevices"
  RMDir /r "$INSTDIR"

SectionEnd
