Name "ValidationServer"
OutFile "ValidationServerSetup.exe"
InstallDir $PROGRAMFILES\ValidationServer
RequestExecutionLevel admin

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Section "Core files (required)"
  SectionIn RO
  
  SetOutPath "$INSTDIR\Bin"
  File "..\ValidationServerOutput\*.exe"
  File "$%QTDIR%\bin\Qt5Core.dll"
  File "$%QTDIR%\bin\Qt5Network.dll"
  File "$%QTDIR%\bin\libGLESv2.dll"
  File "$%QTDIR%\bin\libEGL.dll"
  File "$%QTDIR%\bin\icuuc51.dll"
  File "$%QTDIR%\bin\icuin51.dll"
  File "$%QTDIR%\bin\icudt51.dll"
  
  SetOutPath "$INSTDIR\Bin\platforms"
  File "$%QTDIR%\plugins\platforms\qwindows.dll"
  File "$%QTDIR%\plugins\platforms\qminimal.dll"
  File "$%QTDIR%\plugins\platforms\qoffscreen.dll"
  
  SetOutPath "$INSTDIR\Temp"
  File ".\Resources\vcredist_x86.exe"
  ExecWait "$INSTDIR\Temp\vcredist_x86.exe"
  Delete "$INSTDIR\Temp\vcredist_x86.exe"
  RMDir "$INSTDIR\Temp"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\ValidationServer "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ValidationServer" "DisplayName" "ValidationServer"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ValidationServer" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ValidationServer" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ValidationServer" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  SetOutPath "$INSTDIR\Bin"
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\ValidationServer"
  CreateShortCut "$SMPROGRAMS\ValidationServer\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\ValidationServer\ValidationServer.lnk" "$INSTDIR\Bin\ValidationServer.exe" "-setup_mode" "$INSTDIR\Bin\ValidationServer.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ValidationServer"
  DeleteRegKey HKLM SOFTWARE\ValidationServer

  ; Remove files and uninstaller
 ; Delete "$INSTDIR\*.*"
  ;Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\ValidationServer\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\ValidationServer"
  RMDir /r "$INSTDIR"

SectionEnd
