// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_REPLACE_STDIO_H
#define ZL_REPLACE_STDIO_H

#include <zlcore/zlcore.h>

#ifdef  __cplusplus
	extern "C" {
#endif

//================================================================//
// stdio
//================================================================//

//----------------------------------------------------------------//
#undef FILE
#define FILE ZLFILE

//----------------------------------------------------------------//
#undef stderr
#define stderr zl_stderr

#undef stdin
#define stdin zl_stdin

#undef stdout
#define stdout zl_stdout

//----------------------------------------------------------------//
#undef clearerr
#define clearerr zl_clearerr

#undef fclose
#define fclose zl_fclose

#undef feof
#define feof zl_feof

#undef ferror
#define ferror zl_ferror

#undef fflush
#define fflush zl_fflush

#undef fgetc
#define fgetc zl_fgetc

#undef fgetpos
#define fgetpos zl_fgetpos

#undef fgets
#define fgets zl_fgets

#undef fileno
#define fileno zl_fileno

#undef fopen
#define fopen zl_fopen

#undef fopen_s
#define fopen_s zl_fopen_s

#undef fprintf
#define fprintf zl_fprintf

#undef fputc
#define fputc zl_fputc

#undef fputs
#define fputs zl_fputs

#undef fread
#define fread zl_fread

#undef freopen
#define freopen zl_freopen

#undef fscanf
#define fscanf zl_fscanf

#undef fseek
#define fseek zl_fseek

#undef _fseeki64
#define _fseeki64 zl_fseeki64

#undef fsetpos
#define fsetpos zl_fsetpos

#undef ftell
#define ftell zl_ftell

#undef fwrite
#define fwrite zl_fwrite

#undef getc
#define getc zl_getc

#undef getwc
#define getwc zl_getwc

#undef pclose
#define pclose zl_pclose

#undef _pclose
#define _pclose zl_pclose

#undef popen
#define popen zl_popen

#undef _popen
#define _popen zl_popen

#undef putc
#define putc zl_putc

#undef remove
#define remove zl_remove

#undef rename
#define rename zl_rename

#undef rewind
#define rewind zl_rewind

#undef setbuf
#define setbuf zl_setbuf

#undef setvbuf
#define setvbuf zl_setvbuf

#undef tmpfile
#define tmpfile zl_tmpfile

#undef tmpnam
#define tmpnam zl_tmpnam

#undef ungetc
#define ungetc zl_ungetc

#undef vfprintf
#define vfprintf zl_vfprintf

#undef vfscanf
#define vfscanf zl_vfscanf

#ifdef  __cplusplus
	}
#endif

#endif