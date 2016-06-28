// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIImageFormatMgr.h>

//================================================================//
// MOAIImageFormatMgr
//================================================================//

//----------------------------------------------------------------//
MOAIImageFormat* MOAIImageFormatMgr::FindFormat ( cc8* name ) {

	if ( name ) {
		u32 hash = this->HashName ( name );
		return this->mFormats.contains ( hash ) ? this->mFormats [ hash ] : 0;
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIImageFormat* MOAIImageFormatMgr::FindFormat ( ZLStream& stream, cc8* name ) {

	size_t cursor = stream.GetCursor ();

	FormatIt formatIt = this->mFormats.begin ();
	for ( ; formatIt != this->mFormats.end (); formatIt++ ) {
		MOAIImageFormat& format = *formatIt->second;
		
		size_t headerSize = format.GetHeaderSize ();
		if ( headerSize > 0 ) {
		
			void* buffer = alloca ( headerSize );
			size_t size = stream.ReadBytes ( buffer, headerSize );
			stream.SetCursor ( cursor );
		
			if (( size == headerSize ) && format.CheckHeader ( buffer )) return &format;
		}
	}
	
	formatIt = this->mFormats.begin ();
	for ( ; formatIt != this->mFormats.end (); formatIt++ ) {
		MOAIImageFormat& format = *formatIt->second;
		
		size_t headerSize = format.GetHeaderGuessSize ();
		if ( headerSize > 0 ) {
		
			void* buffer = alloca ( headerSize );
			size_t size = stream.ReadBytes ( buffer, headerSize );
			stream.SetCursor ( cursor );
		
			if (( size == headerSize ) && format.GuessHeader ( buffer )) return &format;
		}
	}
	
	return this->FindFormat ( name );
}

//----------------------------------------------------------------//
u32 MOAIImageFormatMgr::HashName ( cc8* name ) {

	u32 hash = 0;
	int shift = 0;
	
	for ( int i = 0; i < 6; ++i ) {
	
		char c = name [ i ];
		if ( c && isalnum ( c )) {
			
			if ( isalpha ( c )) {
				c = ( char )toupper ( c );
				c = ( c - 'A' ) + 10;
			}
			else {
				c = c - '0';
			}
			
			hash += (( c & 0x1f ) << shift );
			shift += 5;
		}
		else {
			break;
		}
	}
	return hash;
}

//----------------------------------------------------------------//
MOAIImageFormatMgr::MOAIImageFormatMgr () {
}

//----------------------------------------------------------------//
MOAIImageFormatMgr::~MOAIImageFormatMgr () {

	FormatIt formatIt = this->mFormats.begin ();
	for ( ; formatIt != this->mFormats.end (); formatIt++ ) {
		delete formatIt->second;
	}
}

