# Microsoft Developer Studio Project File - Name="luasqlsqlite_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=luasqlsqlite_dll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "luasqlsqlite_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "luasqlsqlite_dll.mak" CFG="luasqlsqlite_dll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "luasqlsqlite_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "luasqlsqlite_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "luasqlsqlite_dll"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "luasqlsqlite_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../lib/vc6/luasql"
# PROP Intermediate_Dir "luasqlsqlite_dll/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "luasqlsqlite_dll_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../external-src/lua50/include" /I "../../external-src/sqlite-2.8.17" /I "../../compat/src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "luasqlsqlite_dll_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /machine:I386
# ADD LINK32 lua50.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /machine:I386 /out:"../bin/vc6/luasql/sqlite.dll" /libpath:"../../external-src/lua50/lib/dll" /libpath:"../../external-src/sqlite-2.8.15/lib/opt"

!ELSEIF  "$(CFG)" == "luasqlsqlite_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib/vc6/luasql"
# PROP Intermediate_Dir "luasqlsqlite_dll/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "luasqlsqlite_dll_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../external-src/lua50/include" /I "../../external-src/sqlite-2.8.17" /I "../../compat/src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "luasqlsqlite_dll_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 lua50.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /debug /machine:I386 /out:"../bin/vc6/luasql/sqlited.dll" /pdbtype:sept /libpath:"../../external-src/lua50/lib/dll" /libpath:"../../external-src/sqlite-2.8.17/lib/debug"

!ENDIF 

# Begin Target

# Name "luasqlsqlite_dll - Win32 Release"
# Name "luasqlsqlite_dll - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="..\..\compat\src\compat-5.1.c"
# End Source File
# Begin Source File

SOURCE=..\src\ls_sqlite.c
# End Source File
# Begin Source File

SOURCE=..\src\luasql.c
# End Source File
# Begin Source File

SOURCE=.\sqlite.def
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\..\compat\src\compat-5.1.h"
# End Source File
# Begin Source File

SOURCE=..\src\luasql.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "sqllite Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\attach.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\auth.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\btree.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\btree.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\btree_rb.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\build.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\config.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\copy.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\date.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\delete.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\encode.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\expr.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\func.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\hash.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\hash.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\insert.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\main.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\opcodes.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\opcodes.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\os.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\os.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\pager.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\pager.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\parse.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\parse.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\pragma.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\printf.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\random.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\select.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\shell.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\sqlite.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\sqliteInt.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\table.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\tokenize.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\trigger.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\update.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\util.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\vacuum.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\vdbe.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\vdbe.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\vdbeaux.c"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\vdbeInt.h"
# End Source File
# Begin Source File

SOURCE="..\..\external-src\sqlite-2.8.17\where.c"
# End Source File
# End Group
# End Target
# End Project
