// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFSSTRING_H
#define ZIPFSSTRING_H

#define MOAIO_STRING_BLOCK_SIZE 256

//================================================================//
// ZIPFSString
//================================================================//
typedef struct ZIPFSString {

	char*	mMem;
	size_t	mSize;
	size_t	mStrLen;
	int		mCleanup;

} ZIPFSString;

//----------------------------------------------------------------//
extern char*			ZIPFSString_Append		( ZIPFSString* self, const char* str );
extern void				ZIPFSString_Clear		( ZIPFSString* self );
extern void				ZIPFSString_Delete		( ZIPFSString* self );
extern char*			ZIPFSString_Grow		( ZIPFSString* self, size_t size );
extern ZIPFSString*		ZIPFSString_New			();
extern char				ZIPFSString_PopChar		( ZIPFSString* self );
extern void				ZIPFSString_PushChar	( ZIPFSString* self, char c );
extern char*			ZIPFSString_Set			( ZIPFSString* self, const char* str );
extern void				ZIPFSString_SetBuffer	( ZIPFSString* self, char* buffer, size_t size );
extern char*			ZIPFSString_Shift		( ZIPFSString* self, size_t base, size_t length, size_t newbase );

#endif