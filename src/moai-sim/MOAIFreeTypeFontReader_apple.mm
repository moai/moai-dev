// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#if MOAI_WITH_FREETYPE
#ifdef __APPLE__

#include <moai-sim/MOAIFreeTypeFontReader.h>

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

//----------------------------------------------------------------//
static u32 CalcTableCheckSum ( const u32* table, u32 numberOfBytesInTable ) {
    u32 sum = 0;
    u32 nLongs = ( numberOfBytesInTable + 3 ) / 4;
    while ( nLongs-- > 0 ) {
       sum += CFSwapInt32HostToBig ( *table++ );
    }
    return sum;
}

//================================================================//
// FontHeader
//================================================================//
class FontHeader {
public:

	s32		mVersion;
	u16		mNumTables;
	u16		mSearchRange;
	u16		mEntrySelector;
	u16		mRangeShift;
};

//================================================================//
// TableEntry
//================================================================//
class TableEntry {
public:

	u32		mTag;
	u32		mCheckSum;
	u32		mOffset;
	u32		mLength;
};

//================================================================//
// MOAIFreeTypeFontReader
//================================================================//

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::ExtractSystemFont ( cc8* fontName, ZLStream& stream ) {
	
	CFStringRef cfFontName = CFStringCreateWithCString ( 0, fontName, kCFStringEncodingUTF8 );
	CGFontRef cgFont = CGFontCreateWithFontName ( cfFontName );
	CFRelease ( cfFontName );
	
	if ( !cgFont ) return;
	
//    CFRetain(cgFont);

	CFArrayRef tags = CGFontCopyTableTags ( cgFont );
	int tableCount = CFArrayGetCount ( tags );

	size_t* tableSizes = ( size_t* )malloc ( sizeof ( size_t ) * tableCount );
	memset ( tableSizes, 0, sizeof ( size_t ) * tableCount );

	BOOL containsCFFTable = NO;

	size_t totalSize = sizeof ( FontHeader ) + sizeof ( TableEntry ) * tableCount;

	for ( int index = 0; index < tableCount; ++index ) {

		//get size
		size_t tableSize = 0;
		u32 tag = ( u32 )( size_t )CFArrayGetValueAtIndex ( tags, index );

		if ( tag == 'CFF ' && !containsCFFTable ) {
			containsCFFTable = YES;
		}

		CFDataRef tableDataRef = CGFontCopyTableForTag ( cgFont, tag );
		if ( tableDataRef != NULL ) {
			tableSize = CFDataGetLength ( tableDataRef );
			CFRelease ( tableDataRef );
		}
		totalSize += ( tableSize + 3 ) & ~3;

		tableSizes [ index ] = tableSize;
	}

	void* buffer = malloc ( totalSize );

	memset (( void* )buffer, 0, totalSize );
	char* dataStart = ( char* )buffer;
	char* dataPtr = dataStart;

	// compute font header entries
	u16 entrySelector = 0;
	u16 searchRange = 1;

	while ( searchRange < tableCount >> 1 ) {
		entrySelector++;
		searchRange <<= 1;
	}
	searchRange <<= 4;

	u16 rangeShift = ( tableCount << 4 ) - searchRange;

	// write font header (also called sfnt header, offset subtable)
	FontHeader* offsetTable = ( FontHeader* )dataPtr;

	//OpenType Font contains CFF Table use 'OTTO' as version, and with .otf extension
	//otherwise 0001 0000
	offsetTable->mVersion			= containsCFFTable ? 'OTTO' : CFSwapInt16HostToBig ( 1 );
	offsetTable->mNumTables			= CFSwapInt16HostToBig (( u16 )tableCount );
	offsetTable->mSearchRange		= CFSwapInt16HostToBig (( u16 )searchRange );
	offsetTable->mEntrySelector		= CFSwapInt16HostToBig (( u16 )entrySelector );
	offsetTable->mRangeShift		= CFSwapInt16HostToBig (( u16 )rangeShift );

	dataPtr += sizeof(FontHeader);

	// write tables
	TableEntry* entry = ( TableEntry* )dataPtr;
	dataPtr += sizeof ( TableEntry ) * tableCount;

	for ( int index = 0; index < tableCount; ++index ) {

		u32 tag						= ( u32 )( size_t )CFArrayGetValueAtIndex ( tags, index );
		CFDataRef tableDataRef		= CGFontCopyTableForTag ( cgFont, tag );
		size_t tableSize			= CFDataGetLength ( tableDataRef );
		u32 offset					= dataPtr - dataStart;

		memcpy ( dataPtr, CFDataGetBytePtr ( tableDataRef ), tableSize );

		entry->mTag			= CFSwapInt32HostToBig (( u32 )tag );
		entry->mCheckSum	= CFSwapInt32HostToBig ( CalcTableCheckSum (( u32* )dataPtr, tableSize ));
		entry->mOffset		= CFSwapInt32HostToBig (( u32 )offset );
		entry->mLength		= CFSwapInt32HostToBig (( u32 )tableSize );
		dataPtr += (tableSize + 3) & ~3;
		++entry;
		CFRelease ( tableDataRef );
	}

	stream.WriteBytes ( buffer, totalSize );

	CGFontRelease ( cgFont );
	CFRelease ( tags );
	
	free ( tableSizes );
	free ( buffer );
}

#endif
#endif
