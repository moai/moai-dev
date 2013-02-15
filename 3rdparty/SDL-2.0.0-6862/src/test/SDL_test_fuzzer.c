/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2012 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/* 

  Data generators for fuzzing test data in a reproducible way.
 
*/

#include "SDL_config.h"

/* Visual Studio 2008 doesn't have stdint.h */
#if defined(_MSC_VER) && _MSC_VER <= 1500
#define UINT8_MAX   ~(Uint8)0
#define UINT16_MAX  ~(Uint16)0
#define UINT32_MAX  ~(Uint32)0
#define UINT64_MAX  ~(Uint64)0
#else
#include <stdint.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#include "SDL_test.h"

/** 
 *Counter for fuzzer invocations
 */
static int fuzzerInvocationCounter = 0;

/**
 * Context for shared random number generator
 */
static SDLTest_RandomContext rndContext;

/*
 * Note: doxygen documentation markup for functions is in the header file.
 */

void
SDLTest_FuzzerInit(Uint64 execKey)
{
	Uint32 a = (execKey >> 32) & 0x00000000FFFFFFFF;
	Uint32 b = execKey & 0x00000000FFFFFFFF;
	SDL_memset((void *)&rndContext, 0, sizeof(SDLTest_RandomContext));
	SDLTest_RandomInit(&rndContext, a, b);
	fuzzerInvocationCounter = 0;
}

int
SDLTest_GetFuzzerInvocationCount()
{
	return fuzzerInvocationCounter;
}

Uint8
SDLTest_RandomUint8()
{
	fuzzerInvocationCounter++;

	return (Uint8) SDLTest_RandomInt(&rndContext) & 0x000000FF;
}

Sint8
SDLTest_RandomSint8()
{
	fuzzerInvocationCounter++;

	return (Sint8) SDLTest_RandomInt(&rndContext) & 0x000000FF;
}

Uint16
SDLTest_RandomUint16()
{
	fuzzerInvocationCounter++;

	return (Uint16) SDLTest_RandomInt(&rndContext) & 0x0000FFFF;
}

Sint16
SDLTest_RandomSint16()
{
	fuzzerInvocationCounter++;

	return (Sint16) SDLTest_RandomInt(&rndContext) & 0x0000FFFF;
}

Sint32
SDLTest_RandomSint32()
{
	fuzzerInvocationCounter++;

	return (Sint32) SDLTest_RandomInt(&rndContext);
}

Uint32
SDLTest_RandomUint32()
{
	fuzzerInvocationCounter++;

	return (Uint32) SDLTest_RandomInt(&rndContext);
}

Uint64
SDLTest_RandomUint64()
{
	Uint64 value = 0;
	Uint32 *vp = (void *)&value;

	fuzzerInvocationCounter++;

	vp[0] = SDLTest_RandomSint32();
	vp[1] = SDLTest_RandomSint32();

	return value;
}

Sint64
SDLTest_RandomSint64()
{
	Uint64 value = 0;
	Uint32 *vp = (void *)&value;

	fuzzerInvocationCounter++;

	vp[0] = SDLTest_RandomSint32();
	vp[1] = SDLTest_RandomSint32();

	return value;
}



Sint32
SDLTest_RandomIntegerInRange(Sint32 pMin, Sint32 pMax)
{
	Sint64 min = pMin;
	Sint64 max = pMax;
	Sint64 temp;
	Sint64 number;

	if(pMin > pMax) {
		temp = min;
		min = max;
		max = temp;
	} else if(pMin == pMax) {
		return (Sint32)min;
	}

	number = SDLTest_RandomUint32(); // invocation count increment in there

	return (Sint32)((number % ((max + 1) - min)) + min);
}

/*!
 * Generates boundary values between the given boundaries.
 * Boundary values are inclusive. See the examples below.
 * If boundary2 < boundary1, the values are swapped.
 * If boundary1 == boundary2, value of boundary1 will be returned
 *
 * Generating boundary values for Uint8:
 * BoundaryValues(sizeof(Uint8), 10, 20, True) -> [10,11,19,20]
 * BoundaryValues(sizeof(Uint8), 10, 20, False) -> [9,21]
 * BoundaryValues(sizeof(Uint8), 0, 15, True) -> [0, 1, 14, 15]
 * BoundaryValues(sizeof(Uint8), 0, 15, False) -> [16]
 * BoundaryValues(sizeof(Uint8), 0, 255, False) -> NULL
 *
 * Generator works the same for other types of unsigned integers.
 *
 * Note: outBuffer will be allocated and needs to be freed later.
 * If outbuffer != NULL, it'll be freed.
 *
 * \param maxValue The biggest value that is acceptable for this data type.
 * 					For instance, for Uint8 -> 255, Uint16 -> 65536 etc.
 * \param pBoundary1 defines lower boundary
 * \param pBoundary2 defines upper boundary
 * \param validDomain Generate only for valid domain (for the data type)
 *
 * \param outBuffer The generated boundary values are put here
 *
 * \returns Returns the number of elements in outBuffer or -1 in case of error
 */
Uint32
SDLTest_GenerateUnsignedBoundaryValues(const Uint64 maxValue,
					Uint64 pBoundary1, Uint64 pBoundary2, SDL_bool validDomain,
					Uint64 *outBuffer)
{
	Uint64 boundary1 = pBoundary1, boundary2 = pBoundary2;
	Uint64 temp;
	Uint64 tempBuf[4];
	int index;

	if(outBuffer != NULL) {
		SDL_free(outBuffer);
	}

	if(boundary1 > boundary2) {
		temp = boundary1;
		boundary1 = boundary2;
		boundary2 = temp;
	}

	index = 0;
	if(boundary1 == boundary2) {
		tempBuf[index++] = boundary1;
	}
	else if(validDomain) {
		tempBuf[index++] = boundary1;

		if(boundary1 < UINT64_MAX)
			tempBuf[index++] = boundary1 + 1;

		tempBuf[index++] = boundary2 - 1;
		tempBuf[index++] = boundary2;
	}
	else {
		if(boundary1 > 0) {
			tempBuf[index++] = boundary1 - 1;
		}

		if(boundary2 < maxValue && boundary2 < UINT64_MAX) {
			tempBuf[index++] = boundary2 + 1;
		}
	}

	if(index == 0) {
		// There are no valid boundaries
		return 0;
	}

	// Create the return buffer
	outBuffer = (Uint64 *)SDL_malloc(index * sizeof(Uint64));
	if(outBuffer == NULL) {
		return 0;
	}

	SDL_memcpy(outBuffer, tempBuf, index * sizeof(Uint64));

	return index;
}

Uint8
SDLTest_RandomUint8BoundaryValue(Uint8 boundary1, Uint8 boundary2, SDL_bool validDomain)
{
	Uint64 *buffer = NULL;
	Uint32 size;
	Uint32 index;
	Uint8 retVal;

	// max value for Uint8
	const Uint64 maxValue = UINT8_MAX;

	size = SDLTest_GenerateUnsignedBoundaryValues(maxValue,
				(Uint64) boundary1, (Uint64) boundary2,
				validDomain, buffer);
	if (buffer == NULL || size == 0) {
		return 0;
	}

	index = SDLTest_RandomSint32() % size;
	retVal = (Uint8)buffer[index];

	SDL_free(buffer);

	fuzzerInvocationCounter++;

	return retVal;
}

Uint16
SDLTest_RandomUint16BoundaryValue(Uint16 boundary1, Uint16 boundary2, SDL_bool validDomain)
{
	Uint64 *buffer = NULL;
	Uint32 size;
	Uint32 index;
	Uint16 retVal;

	// max value for Uint16
	const Uint64 maxValue = UINT16_MAX;

	size = SDLTest_GenerateUnsignedBoundaryValues(maxValue,
				(Uint64) boundary1, (Uint64) boundary2,
				validDomain, buffer);
	if (buffer == NULL || size == 0) {
		return 0;
	}

	index = SDLTest_RandomSint32() % size;
	retVal = (Uint16) buffer[index];

	SDL_free(buffer);

	fuzzerInvocationCounter++;

	return retVal;
}

Uint32
SDLTest_RandomUint32BoundaryValue(Uint32 boundary1, Uint32 boundary2, SDL_bool validDomain)
{
	Uint64 *buffer = NULL;
	Uint32 size;
	Uint32 index;
	Uint32 retVal;

	// max value for Uint32
	const Uint64 maxValue = UINT32_MAX;

	size = SDLTest_GenerateUnsignedBoundaryValues(maxValue,
				(Uint64) boundary1, (Uint64) boundary2,
				validDomain, buffer);
	if (buffer == NULL || size == 0) {
		return 0;
	}

	index = SDLTest_RandomSint32() % size;
	retVal = (Uint32) buffer[index];

	SDL_free(buffer);

	fuzzerInvocationCounter++;

	return retVal;
}

Uint64
SDLTest_RandomUint64BoundaryValue(Uint64 boundary1, Uint64 boundary2, SDL_bool validDomain)
{
	Uint64 *buffer = NULL;
	Uint32 size;
	Uint32 index;
	Uint64 retVal;

	// max value for Uint64
	const Uint64 maxValue = UINT64_MAX;

	size = SDLTest_GenerateUnsignedBoundaryValues(maxValue,
				(Uint64) boundary1, (Uint64) boundary2,
				validDomain, buffer);
	if (buffer == NULL || size == 0) {
		return 0;
	}

	index = SDLTest_RandomSint32() % size;
	retVal = (Uint64) buffer[index];

	SDL_free(buffer);

	fuzzerInvocationCounter++;

	return retVal;
}

/*!
 * Generates boundary values between the given boundaries.
 * Boundary values are inclusive. See the examples below.
 * If boundary2 < boundary1, the values are swapped.
 * If boundary1 == boundary2, value of boundary1 will be returned
 *
 * Generating boundary values for Sint8:
 * SignedBoundaryValues(sizeof(Sint8), -10, 20, True) -> [-11,-10,19,20]
 * SignedBoundaryValues(sizeof(Sint8), -10, 20, False) -> [-11,21]
 * SignedBoundaryValues(sizeof(Sint8), -30, -15, True) -> [-30, -29, -16, -15]
 * SignedBoundaryValues(sizeof(Sint8), -128, 15, False) -> [16]
 * SignedBoundaryValues(sizeof(Sint8), -128, 127, False) -> NULL
 *
 * Generator works the same for other types of signed integers.
 *
 * Note: outBuffer will be allocated and needs to be freed later.
 * If outbuffer != NULL, it'll be freed.
 *
 *
 * \param minValue The smallest value  that is acceptable for this data type.
 *					For instance, for Uint8 -> -128, Uint16 -> -32,768 etc.
 * \param maxValue The biggest value that is acceptable for this data type.
 * 					For instance, for Uint8 -> 127, Uint16 -> 32767 etc.
 * \param pBoundary1 defines lower boundary
 * \param pBoundary2 defines upper boundary
 * \param validDomain Generate only for valid domain (for the data type)
 *
 * \param outBuffer The generated boundary values are put here
 *
 * \returns Returns the number of elements in outBuffer or -1 in case of error
 */
Uint32
SDLTest_GenerateSignedBoundaryValues(const Sint64 minValue, const Sint64 maxValue,
					Sint64 pBoundary1, Sint64 pBoundary2, SDL_bool validDomain,
					Sint64 *outBuffer)
{
	int index;
	Sint64 tempBuf[4];
	Sint64 boundary1 = pBoundary1, boundary2 = pBoundary2;

	if(outBuffer != NULL) {
		SDL_free(outBuffer);
	}

	if(boundary1 > boundary2) {
		Sint64 temp = boundary1;
		boundary1 = boundary2;
		boundary2 = temp;
	}

	index = 0;
	if(boundary1 == boundary2) {
		tempBuf[index++] = boundary1;
	}
	else if(validDomain) {
		tempBuf[index++] = boundary1;

		if(boundary1 < LLONG_MAX)
			tempBuf[index++] = boundary1 + 1;

		if(boundary2 > LLONG_MIN)
			tempBuf[index++] = boundary2 - 1;

		tempBuf[index++] = boundary2;
	}
	else {
		if(boundary1 > minValue &&  boundary1 > LLONG_MIN) {
			tempBuf[index++] = boundary1 - 1;
		}

		if(boundary2 < maxValue && boundary2 < UINT64_MAX) {
			tempBuf[index++] = boundary2 + 1;
		}
	}

	if(index == 0) {
		// There are no valid boundaries
		return 0;
	}

	// Create the return buffer
	outBuffer = (Sint64 *)SDL_malloc(index * sizeof(Sint64));
	if(outBuffer == NULL) {
		return 0;
	}

	SDL_memcpy((void *)outBuffer, (void *)tempBuf, index * sizeof(Sint64));

	return (Uint32)index;
}

Sint8
SDLTest_RandomSint8BoundaryValue(Sint8 boundary1, Sint8 boundary2, SDL_bool validDomain)
{
	// min & max values for Sint8
	const Sint64 maxValue = CHAR_MAX;
	const Sint64 minValue = CHAR_MIN;

	Sint64 *buffer = NULL;
	Uint32 size;
	Uint32 index;
	Sint8 retVal;

	size = SDLTest_GenerateSignedBoundaryValues(minValue, maxValue,
				(Sint64) boundary1, (Sint64) boundary2,
				validDomain, buffer);
	if (buffer == NULL || size == 0) {
		return CHAR_MIN;
	}

	index = SDLTest_RandomSint32() % size;
	retVal = (Sint8) buffer[index];

	SDL_free(buffer);

	fuzzerInvocationCounter++;

	return retVal;
}

Sint16
SDLTest_RandomSint16BoundaryValue(Sint16 boundary1, Sint16 boundary2, SDL_bool validDomain)
{
	// min & max values for Sint16
	const Sint64 maxValue = SHRT_MAX;
	const Sint64 minValue = SHRT_MIN;
	Sint64 *buffer = NULL;
	Uint32 size;
	Uint32 index;
	Sint16 retVal;

	size = SDLTest_GenerateSignedBoundaryValues(minValue, maxValue,
					(Sint64) boundary1, (Sint64) boundary2,
					validDomain, buffer);
	if (buffer == NULL || size == 0) {
		return SHRT_MIN;
	}

	index = SDLTest_RandomSint32() % size;
	retVal = (Sint16) buffer[index];

	SDL_free(buffer);

	fuzzerInvocationCounter++;

	return retVal;
}

Sint32
SDLTest_RandomSint32BoundaryValue(Sint32 boundary1, Sint32 boundary2, SDL_bool validDomain)
{
	// min & max values for Sint32
	const Sint64 maxValue = INT_MAX;
	const Sint64 minValue = INT_MIN;

	Sint64 *buffer = NULL;
	Uint32 size;
	Uint32 index;
	Sint32 retVal;

	size = SDLTest_GenerateSignedBoundaryValues(minValue, maxValue,
				(Sint64) boundary1, (Sint64) boundary2,
				validDomain, buffer);
	if (buffer == NULL || size == 0) {
		return INT_MIN;
	}

	index = SDLTest_RandomSint32() % size;
	retVal = (Sint32) buffer[index];

	SDL_free(buffer);

	fuzzerInvocationCounter++;

	return retVal;
}

Sint64
SDLTest_RandomSint64BoundaryValue(Sint64 boundary1, Sint64 boundary2, SDL_bool validDomain)
{
	Sint64 *buffer = NULL;
	Uint32 size;
	Uint32 index;
	Sint64 retVal;

	// min & max values for Sint64
	const Sint64 maxValue = LLONG_MAX;
	const Sint64 minValue = LLONG_MIN;

	size = SDLTest_GenerateSignedBoundaryValues(minValue, maxValue,
				(Sint64) boundary1, (Sint64) boundary2,
				validDomain, buffer);
	if (buffer == NULL || size == 0) {
		return LLONG_MIN;
	}

	index = SDLTest_RandomSint32() % size;
	retVal = (Sint64) buffer[index];

	SDL_free(buffer);

	fuzzerInvocationCounter++;

	return retVal;
}

float
SDLTest_RandomUnitFloat()
{
	return (float) SDLTest_RandomUint32() / UINT_MAX;
}

float
SDLTest_RandomFloat()
{
        return (float) (SDLTest_RandomUnitDouble() * (double)2.0 * (double)FLT_MAX - (double)(FLT_MAX));
}

double
SDLTest_RandomUnitDouble()
{
	return (double) (SDLTest_RandomUint64() >> 11) * (1.0/9007199254740992.0);
}

double
SDLTest_RandomDouble()
{
	double r = 0.0;
	double s = 1.0;
	do {
	  s /= UINT_MAX + 1.0;
	  r += (double)SDLTest_RandomInt(&rndContext) * s;
	} while (s > DBL_EPSILON);
	  
	fuzzerInvocationCounter++;
	
	return r;
}


char *
SDLTest_RandomAsciiString()
{
	return SDLTest_RandomAsciiStringWithMaximumLength(255);
}

char *
SDLTest_RandomAsciiStringWithMaximumLength(int maxLength)
{
	int size;

	if(maxLength < 1) {
                SDL_InvalidParamError("maxLength");
		return NULL;
	}

	size = (SDLTest_RandomUint32() % (maxLength + 1));
	
	return SDLTest_RandomAsciiStringOfSize(size);	
}

char *
SDLTest_RandomAsciiStringOfSize(int size)
{
	char *string;
	int counter;


	if(size < 1) {
                SDL_InvalidParamError("size");
		return NULL;
	}

	string = (char *)SDL_malloc((size + 1) * sizeof(char));
	if (string==NULL) {
	  return NULL;
        }

	for(counter = 0; counter < size; ++counter) {
		string[counter] = (char)SDLTest_RandomIntegerInRange(32, 126);
	}

	string[counter] = '\0';

	fuzzerInvocationCounter++;

	return string;
}
