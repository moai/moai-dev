#ifndef	_FOLDERWATCHER_H_
#define	_FOLDERWATCHER_H_
#ifdef WIN32
#include <tchar.h>
#endif

void winhostext_SetWorkingDirectory(const TCHAR* startupScript);
void winhostext_WatchFolder(const char* startupScript);
void winhostext_Query();
void winhostext_CleanUp();

void setStartupDir(const char * startupScript );

#endif //_FOLDERWATCHER_H_