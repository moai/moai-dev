// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MATHCONSTS_H
#define	MATHCONSTS_H


#define PI			M_PI
#define TWOPI		( M_PI * 2 )
#define HALFPI		M_PI_2
#define FORTPI		M_PI_4
#define D2R			( M_PI / 180.0 )
#define R2D			( 180.0 / M_PI )
#define LOG2E		M_LOG2E
#define LOG10E		M_LOG10E

#ifndef MIN
	#define MIN(a,b) (a < b ? a : b)
#endif

#ifndef MAX
	#define MAX(a,b) (a > b ? a : b)
#endif

#endif
