
#ifndef _JPEG_DEBUG_H_
#define _JPEG_DEBUG_H_

#define JPEG_DEBUG(n, format...)	do{ \
	if((n)<=JPEG_DEBUG_LEVEL)jpeg_debug((n),format); \
}while(0)
#define JPEG_DEBUG_LEVEL 4

void jpeg_debug(int n, const char *format, ... );

#endif

