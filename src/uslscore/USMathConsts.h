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

#ifndef ABS
	#define ABS(a) (( a ) < 0 ? -( a ) : ( a ))
#endif

#ifndef MIN
	#define MIN(a,b) (a < b ? a : b)
#endif

#ifndef MAX
	#define MAX(a,b) (a > b ? a : b)
#endif

#ifndef M_SQRT1_2
	#define M_SQRT1_2     0.70710678118654752440084436210
#endif

#ifndef M_SQRT2
	#define M_SQRT2       1.41421356237309504880168872421
#endif

#ifndef M_SQRT3
	#define M_SQRT3       1.73205080756887729352744634151
#endif

#ifndef M_SQRTPI
	#define M_SQRTPI      1.77245385090551602729816748334
#endif

#endif
