;_______________________________________________________________________________________

!include LogicLib.nsh
!include FileFunc.nsh
!insertmacro Locate
 
Var /GLOBAL switch_overwrite
!include MoveFileFolder.nsh

;_______________________________________________________________________________________

SetCompressor /FINAL /SOLID lzma
SetCompressorDictSize 64

!define DISPLAY_NAME "@@DISPLAY_NAME@@"
!define PROGRAM_FOLDER "Moai SDK"
!define INSTALLER_NAME "moai-sdk-windows.exe"
!define LICENSE_TEXT "license.txt"
!define ALL_USERS

!include "winmessages.nsh"
!define env_hklm 'HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"'
!define env_hkcu 'HKCU "Environment"'

;_______________________________________________________________________________________

!define INSTDIR_REG_ROOT "HKLM"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_FOLDER}"
!define MOAI_BINARIES "..\vs2008\bin\Win32\Release"
!define MOAI_SRC "..\src"
!define MUI_HEADERIMAGE_BITMAP MUI_HEADERIMAGE_BITMAP.bmp
!define MUI_WELCOMEFINISHPAGE_BITMAP MUI_WELCOMEFINISHPAGE_BITMAP.bmp
!define REG_ENVIRONMENT "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"
!define GETTING_STARTED_URL "http://getmoai.com/GettingStartedWithMoai.pdf"

!include MUI.nsh
!include AdvUninstLog.nsh
!include RegisterExtension.nsh
!include AddToPath.nsh
!include nsDialogs.nsh

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
!define MUI_FINISHPAGE_SHOWREADME ${GETTING_STARTED_URL}
!define MUI_FINISHPAGE_SHOWREADME_TEXT "View $\"Getting Started With Moai$\" Online"
!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

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

	;Call GetMyDocs
	;SetOutPath "$0\${PROGRAM_FOLDER}\samples\hello-moai"
	;Exec "$0\${PROGRAM_FOLDER}\samples\hello-moai\run.bat"	
	SetOutPath "$INSTDIR\samples\lua\hello-moai"
	Exec "$INSTDIR\samples\lua\hello-moai\run.bat"
FunctionEnd

;_______________________________________________________________________________________
; Function GetMyDocs
	; ReadRegStr $0 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" Personal
; FunctionEnd

; Function un.GetMyDocs
	; ReadRegStr $0 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" Personal
; FunctionEnd

Section "Moai"

	SetOutPath "$INSTDIR"

	; installation files
	!insertmacro UNINSTALL.LOG_OPEN_INSTALL
	File /r /x ".gitignore" /x "*.ncb" /x "*.suo" /x "*.user" "..\sdk\*.*"
	!insertmacro UNINSTALL.LOG_CLOSE_INSTALL

	; move samples to my documents path
	;StrCpy $switch_overwrite 0
	;Call GetMyDocs
	;!insertmacro MoveFolder "$INSTDIR\samples\" "$0\${PROGRAM_FOLDER}\samples\" "*.*"	
	
	; copy import text files too
	; StrCpy $3 "$INSTDIR\version.txt" ;Path of copy file from
	; StrCpy $4 "$0\${PROGRAM_FOLDER}\version.txt"   ;Path of copy file to
	; StrCpy $5 0 ; only 0 or 1, set 0 to overwrite file if it already exists
	; System::Call 'kernel32::CopyFile(t r3, t r4, b r5) ?e'

	; StrCpy $3 "$INSTDIR\legal.txt" ;Path of copy file from
	; StrCpy $4 "$0\${PROGRAM_FOLDER}\legal.txt"   ;Path of copy file to
	; StrCpy $5 0 ; only 0 or 1, set 0 to overwrite file if it already exists
	; System::Call 'kernel32::CopyFile(t r3, t r4, b r5) ?e'

	; StrCpy $3 "$INSTDIR\license.txt" ;Path of copy file from
	; StrCpy $4 "$0\${PROGRAM_FOLDER}\license.txt"   ;Path of copy file to
	; StrCpy $5 0 ; only 0 or 1, set 0 to overwrite file if it already exists
	; System::Call 'kernel32::CopyFile(t r3, t r4, b r5) ?e'
	
	; start menu shortcuts
	SetShellVarContext all
	CreateDirectory "$SMPROGRAMS\${DISPLAY_NAME}"
	;CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Getting Started.lnk" "${GETTING_STARTED_URL}"
	CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Samples.lnk" "$INSTDIR\samples\lua" 	;CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Samples.lnk" "$0\${PROGRAM_FOLDER}\samples\lua"
	CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Reference.lnk" "$INSTDIR\docs\html\index.html"
	CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Uninstall.lnk" "${UNINST_EXE}"

	; desktop shortcut
	;CreateShortCut "$DESKTOP\Moai Samples.lnk" "$0\${PROGRAM_FOLDER}\samples\lua"
	CreateShortCut "$DESKTOP\Moai Samples.lnk" "$INSTDIR\samples\lua"
	
	; documentation shortcut
	CreateShortCut "$INSTDIR\docs\Moai SDK Documentation.lnk" "$INSTDIR\docs\html\index.html"

	; system add/remove programs setup
	WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir" "$INSTDIR"
	WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName" "${DISPLAY_NAME}"
	WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString" "${UNINST_EXE}"

	; register extensions
	;${RegisterExtension} "$INSTDIR\moai.exe" ".moai" "Moai File"
	
	; add MOAI_BIN variable
	WriteRegExpandStr ${env_hklm} MOAI_BIN "$INSTDIR\bin\"
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
	StrCpy $R0 "$INSTDIR\bin\"
	System::Call 'Kernel32::SetEnvironmentVariableA(t, t) i("MOAI_BIN", R0).r0'
	
	; add MOAI_CONFIG variable
	;Call GetMyDocs
	;WriteRegExpandStr ${env_hklm} MOAI_CONFIG "$0\${PROGRAM_FOLDER}\samples\lua\config\"
	WriteRegExpandStr ${env_hklm} MOAI_CONFIG "$INSTDIR\samples\lua\config\"
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
	;StrCpy $R0 "$0\${PROGRAM_FOLDER}\samples\lua\config\"
	StrCpy $R0 "$INSTDIR\samples\lua\config\"
	System::Call 'Kernel32::SetEnvironmentVariableA(t, t) i("MOAI_CONFIG", R0).r0'
	
SectionEnd

Function .onInit
	
	; check for exisiting install
	ClearErrors
	ReadRegStr $0 ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName"
	IfErrors init.normal
	MessageBox MB_YESNO|MB_ICONQUESTION "Moai SDK has already been installed.$\nYou must uninstall it before installing a new version.$\nUninstall now?" IDYES init.uninst
	Quit

init.uninst:
	; if existing version is there, run its uninstaller before running installer
	ClearErrors
	ReadRegStr $0 ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString"
	IfErrors init.done
	ExecWait '$0 _?=$INSTDIR'
	Goto init.normal

init.done:
	Quit

init.normal:
	!insertmacro UNINSTALL.LOG_PREPARE_INSTALL
	
	; save existing values of MOAI_BIN and MOAI_CONFIG environment variables
	ReadRegStr $R0 HKLM "${REG_ENVIRONMENT}" "MOAI_BIN"
	${If} "$R0" != ""
		WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "OLD_MOAI_BIN" "$R0"
	${EndIf}

	ReadRegStr $R0 HKLM "${REG_ENVIRONMENT}" "MOAI_CONFIG"
	${If} "$R0" != ""
		WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "OLD_MOAI_CONFIG" "$R0"
	${EndIf}
	
FunctionEnd

Function .onInstSuccess
	!insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

;_______________________________________________________________________________________

Section UnInstall

	; restore old values of MOAI_BIN and MOAI_CONFIG environment variables
	ReadRegStr $R0 ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "OLD_MOAI_BIN"
	${If} $R0 != ""
		WriteRegExpandStr ${env_hklm} MOAI_BIN "$R0"
	${Else}
		DeleteRegValue ${env_hklm} MOAI_BIN
	${EndIf}
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000

	ReadRegStr $R0 ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "OLD_MOAI_CONFIG"
	${If} $R0 != ""
		WriteRegExpandStr ${env_hklm} MOAI_CONFIG "$R0"
	${Else}
		DeleteRegValue ${env_hklm} MOAI_CONFIG
	${EndIf}
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
	
	;${UnregisterExtension} ".moai" "Moai File"
	
	!insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
	!insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR"
	!insertmacro UNINSTALL.LOG_END_UNINSTALL

	;Call un.GetMyDocs
	;RMDir /r "$0\${PROGRAM_FOLDER}"
	
	SetShellVarContext all
	
	;Delete "$SMPROGRAMS\${DISPLAY_NAME}\Getting Started.lnk"
	Delete "$SMPROGRAMS\${DISPLAY_NAME}\Samples.lnk"
	Delete "$SMPROGRAMS\${DISPLAY_NAME}\Reference.lnk"
	Delete "$SMPROGRAMS\${DISPLAY_NAME}\Uninstall.lnk"
	RMDir /r "$SMPROGRAMS\${DISPLAY_NAME}"

	Delete "$DESKTOP\Moai Samples.lnk"
	
	DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"

SectionEnd
