// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIOSTRING_H
#define MOAIOSTRING_H

#define MOAIO_STRING_BLOCK_SIZE 256

//================================================================//
// MOAIOString
//================================================================//
typedef struct MOAIOString {

	char*	mMem;
	size_t	mSize;
	size_t	mStrLen;

} MOAIOString;

//----------------------------------------------------------------//
extern char*			MOAIOString_Append		( MOAIOString* self, const char* str );
extern void				MOAIOString_Delete		( MOAIOString* self );
extern char*			MOAIOString_Grow		( MOAIOString* self, size_t size );
extern MOAIOString*		MOAIOString_New			();
extern char*			MOAIOString_Set			( MOAIOString* self, const char* str );
extern char*			MOAIOString_Shift		( MOAIOString* self, size_t base, size_t length, size_t newbase );

#endif