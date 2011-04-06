;_______________________________________________________________________________________

SetCompressor /FINAL /SOLID lzma
SetCompressorDictSize 64
RequestExecutionLevel user

!define DISPLAY_NAME "@@DISPLAY_NAME@@"
!define VERSION_PATH "@@VERSION_PATH@@"
!define PARENT_FOLDER_NAME "Moai SDK"
!define INSTALLER_NAME "moai-sdk-installer.exe"
!define LICENSE_TEXT "license.txt"
!define ALL_USERS
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
InstallDir "$PROGRAMFILES\${PARENT_FOLDER_NAME}\${VERSION_PATH}"
InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir"

!insertmacro UNATTENDED_UNINSTALL ;!insertmacro INTERACTIVE_UNINSTALL

!insertmacro MUI_PAGE_WELCOME
;!insertmacro MUI_PAGE_LICENSE "${LICENSE_TEXT}"
;!insertmacro MUI_PAGE_COMPONENTS 
!insertmacro MUI_PAGE_DIRECTORY
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
	
	;add install dir to path environment variable
    Push "$INSTDIR\bin"
	Call AddToPath
	
	;add config to path
	; Push "MOAI_CONFIG"
	; Push $INSTDIR\samples\config
	; Call AddToEnvVar
	ReadEnvStr $R0 "PATH"
	StrCpy $R0 "$R0;$INSTDIR\bin"
	System::Call 'Kernel32::SetEnvironmentVariableA(t, t) i("PATH", R0).r0'
	
SectionEnd

Function .onInit
	!insertmacro UNINSTALL.LOG_PREPARE_INSTALL
FunctionEnd

Function .onInstSuccess
	!insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

;_______________________________________________________________________________________

Section UnInstall

 	Push "$INSTDIR\bin"
	Call un.RemoveFromPath

	Push "MOAI_CONFIG"
	Push $INSTDIR\samples\config
	Call un.RemoveFromEnvVar
	
	;${UnregisterExtension} ".moai" "Moai File"
	
	!insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
	!insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR"
	!insertmacro UNINSTALL.LOG_END_UNINSTALL
	
	Delete "$SMPROGRAMS\${DISPLAY_NAME}\Samples.lnk"
	Delete "$SMPROGRAMS\${DISPLAY_NAME}\Reference.lnk"
	Delete "$SMPROGRAMS\${DISPLAY_NAME}\Uninstall.lnk"
	RMDir /r "$SMPROGRAMS\${DISPLAY_NAME}"

	Delete "$DESKTOP\Moai Samples.lnk"
	
	DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"

SectionEnd
