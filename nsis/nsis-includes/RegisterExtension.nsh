/*
_____________________________________________________________________________
 
                       File Association
_____________________________________________________________________________
 
 Based on code taken from http://nsis.sourceforge.net/File_Association 
 
 Usage in script:
 1. !include "FileAssociation.nsh"
 2. [Section|Function]
      ${FileAssociationFunction} "Param1" "Param2" "..." $var
    [SectionEnd|FunctionEnd]
 
 FileAssociationFunction=[RegisterExtension|UnRegisterExtension]
 
_____________________________________________________________________________
 
 ${RegisterExtension} "[executable]" "[extension]" "[description]"
 
"[executable]"     ; executable which opens the file format
                   ;
"[extension]"      ; extension, which represents the file format to open
                   ;
"[description]"    ; description for the extension. This will be display in Windows Explorer.
                   ;
 
 
 ${UnRegisterExtension} "[extension]" "[description]"
 
"[extension]"      ; extension, which represents the file format to open
                   ;
"[description]"    ; description for the extension. This will be display in Windows Explorer.
                   ;
 
_____________________________________________________________________________
 
                         Macros
_____________________________________________________________________________
 
 Change log window verbosity (default: 3=no script)
 
 Example:
 !include "FileAssociation.nsh"
 !insertmacro RegisterExtension
 ${FileAssociation_VERBOSE} 4   # all verbosity
 !insertmacro UnRegisterExtension
 ${FileAssociation_VERBOSE} 3   # no script
*/

!define RegisterExtension "!insertmacro RegisterExtension"
!define UnregisterExtension "!insertmacro UnregisterExtension"

; usage:
; !include "RegisterExtension.nsh"
; ${RegisterExtension} "c:\myplayer.exe" ".mkv" "MKV File"
; ...
; ${UnregisterExtension} ".mkv" "MKV File"

!macro RegisterExtension executable extension description

	Push "${executable}"  ; "full path to my.exe"
	Push "${extension}"   ;  ".mkv"
	Push "${description}" ;  "MKV File"
	Call RegisterExtension
!macroend
 
; back up old value of .opt
Function RegisterExtension

!define Index "Line${__LINE__}"

	pop $R0 ; ext name
	pop $R1
	pop $R2
	push $1
	push $0
	ReadRegStr $1 HKCR $R1 ""
	StrCmp $1 "" "${Index}-NoBackup"
		StrCmp $1 "OptionsFile" "${Index}-NoBackup"
		WriteRegStr HKCR $R1 "backup_val" $1

"${Index}-NoBackup:"
	WriteRegStr HKCR $R1 "" $R0
	ReadRegStr $0 HKCR $R0 ""
	StrCmp $0 "" 0 "${Index}-Skip"
		WriteRegStr HKCR $R0 "" $R0
		WriteRegStr HKCR "$R0\shell" "" "open"
		WriteRegStr HKCR "$R0\DefaultIcon" "" "$R2,0"
		
"${Index}-Skip:"
	WriteRegStr HKCR "$R0\shell\open\command" "" '$R2 "%1"'
	WriteRegStr HKCR "$R0\shell\edit" "" "Edit $R0"
	WriteRegStr HKCR "$R0\shell\edit\command" "" '$R2 "%1"'
	pop $0
	pop $1
!undef Index
FunctionEnd
 
!macro UnregisterExtension extension description
	Push "${extension}"   ;  ".mkv"
	Push "${description}"   ;  "MKV File"
	Call un.UnregisterExtension
!macroend

Function un.UnregisterExtension

	pop $R1 ; description
	pop $R0 ; extension
	
!define Index "Line${__LINE__}"
	ReadRegStr $1 HKCR $R0 ""
	StrCmp $1 $R1 0 "${Index}-NoOwn" ; only do this if we own it
	ReadRegStr $1 HKCR $R0 "backup_val"
	StrCmp $1 "" 0 "${Index}-Restore" ; if backup="" then delete the whole key
	DeleteRegKey HKCR $R0
	Goto "${Index}-NoOwn"
	
"${Index}-Restore:"
	WriteRegStr HKCR $R0 "" $1
	DeleteRegValue HKCR $R0 "backup_val"
	DeleteRegKey HKCR $R1 ;Delete key with association name settings
	
"${Index}-NoOwn:"
!undef Index
FunctionEnd