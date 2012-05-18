#ifndef NACLFILE_H
#define NACLFILE_H

#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>

#undef FILE
#undef fopen 
#undef fclose
#undef fread 
#undef fwrite 
#undef feof 
#undef ferror
#undef clearerr
#undef fgetc
#undef ungetc
#undef fseek
#undef ftell
#undef getcwd

#ifdef __cplusplus
extern "C" {
#endif

FILE * nacl_fopen ( const char * path, const char *mode );
int nacl_fclose ( FILE *file );
int nacl_fread ( void *ptr, int size_of_elements, int number_of_elements, FILE *file );
int nacl_fwrite ( const void * ptr, int size, int count, FILE *file );
int nacl_feof ( FILE * file );
int nacl_ferror ( FILE * file );
void nacl_clearerr ( FILE * stream );
int nacl_fgetc ( FILE * stream );
int nacl_ungetc ( int c, FILE *void_file );
int nacl_fseek ( FILE * stream, long int offset, int origin );
long int nacl_ftell ( FILE * stream );
int nacl_stat ( const char *path, struct stat *buf );
char *nacl_getcwd( char *buf, size_t size );

#ifdef __cplusplus
}
#endif

#ifndef NACLFILE_CPP
#define fopen nacl_fopen
#define fclose nacl_fclose
#define fread nacl_fread
#define fwrite nacl_fwrite
#define feof nacl_feof
#define ferror nacl_ferror
#define clearerr nacl_clearerr
#define fgetc nacl_fgetc
#define ungetc nacl_ungetc
#define fseek nacl_fseek
#define ftell nacl_ftell
#define stat nacl_stat
#define getcwd nacl_getcwd

#endif

#endif