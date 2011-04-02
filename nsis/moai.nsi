;_______________________________________________________________________________________

SetCompressor /FINAL /SOLID lzma
SetCompressorDictSize 64

!define APP_NAME "Moai"
!define INSTALLER_NAME "moai-sdk-installer.exe"
!define LICENSE_TEXT "license.txt"
;_______________________________________________________________________________________

!define INSTDIR_REG_ROOT "HKLM"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
!define MOAI_BINARIES "..\vs2008\bin\Win32\Release"
!define MOAI_SRC "..\src"

!include MUI.nsh
!include AdvUninstLog.nsh
!include RegisterExtension.nsh
!include AddToPath.nsh

Name "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
ShowInstDetails show
ShowUninstDetails show
InstallDir "$PROGRAMFILES\${APP_NAME}"
InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir"

!insertmacro UNATTENDED_UNINSTALL ;!insertmacro INTERACTIVE_UNINSTALL

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TEXT}"
;!insertmacro MUI_PAGE_COMPONENTS 
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

;_______________________________________________________________________________________

Section "Moai"

	SetOutPath "$INSTDIR"

	;installation files
	!insertmacro UNINSTALL.LOG_OPEN_INSTALL
	File /r /x ".gitignore" /x "*.ncb" /x "*.suo" /x "*.user" "release\*.*"
	!insertmacro UNINSTALL.LOG_CLOSE_INSTALL
	
	;start menu shortcuts
	CreateDirectory "$SMPROGRAMS\${APP_NAME}"
	CreateShortCut "$SMPROGRAMS\${APP_NAME}\Samples.lnk" "$INSTDIR\samples"
	CreateShortCut "$SMPROGRAMS\${APP_NAME}\Reference.lnk" "$INSTDIR\docs\html\index.html"
	CreateShortCut "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk" "${UNINST_EXE}"
	
	;system add/remove programs setup
	WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir" "$INSTDIR"
	WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName" "${APP_NAME}"
	WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString" "${UNINST_EXE}"

	;register extensions
	;${RegisterExtension} "$INSTDIR\moai.exe" ".moai" "Moai File"
	
	;add install dir to path environment variable
    Push "$INSTDIR\bin"
	Call AddToPath
	
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
	
	;${UnregisterExtension} ".moai" "Moai File"
	
	!insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
	!insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR"
	!insertmacro UNINSTALL.LOG_END_UNINSTALL
	
	RMDir /r "$SMPROGRAMS\${APP_NAME}"

	DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"

SectionEnd
