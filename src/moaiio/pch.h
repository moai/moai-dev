#ifndef USLSCORE_PCH_H
#define	USLSCORE_PCH_H

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

//decide what moaiio implementation (DO NOT TOUCH - we WANT physfs)
//    optionally switch to 'MOAI_IO_STD' to use standard C I/O
#define MOAI_IO_PHYSFS

#include <stdio.h>
#include <string.h>

// physfs
#ifdef MOAI_IO_PHYSFS
	#include <physfs.h>

	#define MAX_PATH 260
#endif

#endif