;_______________________________________________________________________________________

SetCompressor /FINAL /SOLID lzma
SetCompressorDictSize 64
;RequestExecutionLevel user

!define DISPLAY_NAME "@@DISPLAY_NAME@@"
!define PROGRAM_FOLDER "Moai SDK"
!define INSTALLER_NAME "moai-sdk-installer.exe"
!define LICENSE_TEXT "license.txt"
!define ALL_USERS

!include "winmessages.nsh"
!define env_hklm 'HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"'
!define env_hkcu 'HKCU "Environment"'

;_______________________________________________________________________________________

!define INSTDIR_REG_ROOT "HKLM"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${DISPLAY_NAME}"
!define MOAI_BINARIES "..\vs2008\bin\Win32\Release"
!define MOAI_SRC "..\src"
!define MUI_HEADERIMAGE_BITMAP MUI_HEADERIMAGE_BITMAP.bmp
!define MUI_WELCOMEFINISHPAGE_BITMAP MUI_WELCOMEFINISHPAGE_BITMAP.bmp

!include MUI.nsh
!include AdvUninstLog.nsh
!include RegisterExtension.nsh
!include AddToPath.nsh

Name "${DISPLAY_NAME}"
OutFile "${INSTALLER_NAME}"
ShowInstDetails show
ShowUninstDetails show
InstallDir "$PROGRAMFILES\${PROGRAM_FOLDER}"
InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir"

!insertmacro UNATTENDED_UNINSTALL ;!insertmacro INTERACTIVE_UNINSTALL

!insertmacro MUI_PAGE_WELCOME
;!insertmacro MUI_PAGE_LICENSE "${LICENSE_TEXT}"
;!insertmacro MUI_PAGE_COMPONENTS 
!insertmacro MUI_PAGE_DIRECTORY
;Page custom nsDialogsPage
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_TEXT "Run Hello Moai!"
!define MUI_FINISHPAGE_RUN_FUNCTION launchHelloMoai
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

; !include LogicLib.nsh
; Var Dialog
;_______________________________________________________________________________________
; Function nsDialogsPage

	; nsDialogs::Create 1018
	; Pop $Dialog

	; ${If} $Dialog == error
		; Abort
	; ${EndIf}
	
	; nsDialogs::Show
; FunctionEnd

;_______________________________________________________________________________________
Function launchHelloMoai

	SetOutPath "$INSTDIR\samples\hello-moai"
	Exec "$INSTDIR\samples\hello-moai\run.bat"
FunctionEnd

;_______________________________________________________________________________________

Section "Moai"

	SetOutPath "$INSTDIR"

	;installation files
	!insertmacro UNINSTALL.LOG_OPEN_INSTALL
	File /r /x ".gitignore" /x "*.ncb" /x "*.suo" /x "*.user" "release\*.*"
	!insertmacro UNINSTALL.LOG_CLOSE_INSTALL
	
	;start menu shortcuts
	SetShellVarContext all
	CreateDirectory "$SMPROGRAMS\${DISPLAY_NAME}"
	CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Samples.lnk" "$INSTDIR\samples\lua"
	CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Reference.lnk" "$INSTDIR\docs\html\index.html"
	CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Uninstall.lnk" "${UNINST_EXE}"

	;desktop shortcut
	CreateShortCut "$DESKTOP\Moai Samples.lnk" "$INSTDIR\samples\lua"
	
	;system add/remove programs setup
	WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir" "$INSTDIR"
	WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName" "${DISPLAY_NAME}"
	WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString" "${UNINST_EXE}"

	;register extensions
	;${RegisterExtension} "$INSTDIR\moai.exe" ".moai" "Moai File"
	
	;add MOAI_BIN variable
	WriteRegExpandStr ${env_hklm} MOAI_BIN "$INSTDIR\bin\"
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
	StrCpy $R0 "$INSTDIR\bin\"
	System::Call 'Kernel32::SetEnvironmentVariableA(t, t) i("MOAI_BIN", R0).r0'
	
	;add MOAI_CONFIG variable
	WriteRegExpandStr ${env_hklm} MOAI_CONFIG "$INSTDIR\samples\config\"
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
	StrCpy $R0 "$INSTDIR\samples\config\"
	System::Call 'Kernel32::SetEnvironmentVariableA(t, t) i("MOAI_CONFIG", R0).r0'
	
SectionEnd

Function .onInit
	
	ClearErrors
	ReadRegStr $0 ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName"
	IfErrors init.normal
	MessageBox MB_YESNO|MB_ICONEXCLAMATION "There is an existing install of Moai SDK on your system.$\nYou must uninstall it before continuing. Run uninstaller now?" IDYES init.uninst
	Quit

init.uninst:
	ClearErrors
	ReadRegStr $0 ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString"
	IfErrors init.done
	ExecWait '$0 _?=$INSTDIR'
	Goto init.normal

init.done:
	Quit

init.normal:
	!insertmacro UNINSTALL.LOG_PREPARE_INSTALL
	
FunctionEnd

Function .onInstSuccess
	!insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

;_______________________________________________________________________________________

Section UnInstall

	; DeleteRegValue ${env_hklm} MOAI_BIN
	; SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000

	; DeleteRegValue ${env_hklm} MOAI_CONFIG
	; SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
   
	;${UnregisterExtension} ".moai" "Moai File"
	
	!insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
	!insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR"
	!insertmacro UNINSTALL.LOG_END_UNINSTALL
	
	SetShellVarContext all
	Delete "$SMPROGRAMS\${DISPLAY_NAME}\Samples.lnk"
	Delete "$SMPROGRAMS\${DISPLAY_NAME}\Reference.lnk"
	Delete "$SMPROGRAMS\${DISPLAY_NAME}\Uninstall.lnk"
	RMDir /r "$SMPROGRAMS\${DISPLAY_NAME}"

	Delete "$DESKTOP\Moai Samples.lnk"
	
	DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"

SectionEnd
