// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/STLString.h>
#include <zl-util/ZLBitBuffer.h>

//================================================================//
// helpers
//================================================================//

bool	check_bin	( const void* buffer, cc8* str );
bool	check_hex	( const void* buffer, cc8* str );
void	decode_bin	( void* buffer, cc8* str );

//----------------------------------------------------------------//
bool check_bin ( const void* buffer, cc8* str ) {

	STLString bin;
	
	char byteStr [ 9 ];
	byteStr [ 8 ] = 0;
	
	const u8* bytes = ( const u8* )buffer;
	
	size_t len = strlen ( str ) >> 3;
	for ( size_t i = 0; i < len; ++i ) {
	
		u8 byte = *( bytes++ );
	
		for ( u32 j = 0; j < 8; ++j ) {
			byteStr [ j ] = '0' + (( byte >> j ) & 0x01 );
		}
		bin.append ( byteStr );
	}
	return ( bin.compare ( str ) == 0 );
}

//----------------------------------------------------------------//
bool check_hex ( const void* buffer, cc8* str ) {

	STLString hex;
	
	cc8* encode = "0123456789abcdef";
	
	const u8* bytes = ( const u8* )buffer;
	
	size_t len = strlen ( str ) >> 1;
	for ( size_t i = 0; i < len; ++i ) {
	
		u8 byte = *( bytes++ );
		
		hex.push_back ( encode [ byte & 0x0f ]);
		hex.push_back ( encode [( byte >> 4 ) & 0x0f ]);
	}
	return ( hex.compare ( str ) == 0 );
}

//----------------------------------------------------------------//
void decode_bin ( void* buffer, cc8* str ) {
	
	u8* bytes = ( u8* )buffer;
	for ( size_t i = 0; str [ i ]; ++i ) {
	
		u32 bit = str [ i ] == '1' ? 1 : 0;
	
		u32 byte = i >> 3;
		u32 shift = i & 0x07;
		
		bytes [ byte ] = ( bytes [ byte ] & ~( 1 << shift )) | ( bit << shift );
	}
}

//================================================================//
// ZLBitBuffer
//================================================================//

//----------------------------------------------------------------//
void ZLBitBuffer::Blit ( void* dest, size_t destX, const void* src, size_t srcX, size_t n, u32 bitDepth ) {

	assert ( ZLBitBuffer::IsLegalBitDepth ( bitDepth ));
	
	if ( bitDepth < 8 ) {
	
		// not 8-bit, so need to deal with sub-byte end caps and possible shift
	
		// just treat everything as 1-bit resolution
		if ( bitDepth == 4 ) {
			bitDepth = 8;
			
			// 4 bits to every nybble
			destX *= 4;
			srcX *= 4;
			n *= 4;
		}
	
		// 8-bit cursors for copying
		const u8* srcBytes = ( const u8* )src + ( srcX >> 3 );
		u8* destBytes = ( u8* )dest + ( destX >> 3 );
	
		// both will be 0 if byte aligned
		u32 srcMod = srcX & 0x07;
		u32 destMod = destX & 0x07;
		
		if ( srcMod == destMod ) {
		
			// we don't have to bitshift; just handle the two edges (if any) and memcopy the rest
		
			size_t mod = srcMod;
			
			if ( mod ) {
				u32 b = 8 - mod;
				u32 mask = (( 1 << b ) - 1 ) << mod;
				*( destBytes++ ) = (( *destBytes ) & ~mask ) | *( srcBytes++ ) & mask;
				n -= b;
			}
			
			size_t nBytes = n >> 3;
			if ( nBytes ) {
				memcpy ( destBytes, srcBytes, nBytes );
				destBytes += nBytes;
				srcBytes += nBytes;
			}
			
			u32 left = n & 0x07;
			if ( left ) {
				u32 mask = ( 1 << left ) - 1;
				*( destBytes ) = (( *destBytes ) & ~mask ) | *( srcBytes ) & mask;
			}
		}
		else {
		
			// have to shift and copy every byte, so we iterate two bytes at a time
		
			// not a big deal, but can save a few operations
			for ( ; n > 8; n -= 8 ) {
				
				u32 srcValue = (( srcBytes [ 0 ] >> srcMod ) + ( srcBytes [ 1 ] << ( 8 - srcMod ))) & 0xff;
				u32 destValue = (( destBytes [ 0 ] + ( destBytes [ 1 ] << 8 )) & ~( 0xff << destMod )) | ( srcValue << destMod );
				
				destBytes [ 0 ] = destValue & 0x000000ff;
				destBytes [ 1 ] = ( destValue >> 8 ) & 0x000000ff;
				
				srcBytes++;
				destBytes++;
			}
			
			// copy the last few bits
			if ( n > 0 ) {
				
				u32 mask = ( 1 << n ) - 1;
				
				u32 srcValue = ( *( srcBytes++ ) >> srcMod ) & mask;
				
				if (( srcMod + n ) > 8 ) {
					srcValue = ( srcValue + ( *srcBytes << ( 8 - srcMod ))) & mask;
				}
				
				*( destBytes++ ) = ( *destBytes & ~( mask << destMod )) | ( srcValue << destMod );
				
				if (( destMod + n ) > 8 ) {
					*destBytes = ( *destBytes & ~( mask >> ( 8 - destMod ))) | ( srcValue >> ( 8 - destMod ));
				}
			}
		}
	}
	else {
	
		// lucky, can just use memcpy
	
		size_t sizeInBytes = bitDepth >> 3;
		
		memcpy (
			( void* )(( size_t )dest + ( destX * sizeInBytes )),
			( const void* )(( size_t )src + ( srcX * sizeInBytes )),
			n * sizeInBytes
		);
	}
}

//----------------------------------------------------------------//
size_t ZLBitBuffer::CalculateSize ( u32 bitDepth, size_t n ) {

	assert ( ZLBitBuffer::IsLegalBitDepth ( bitDepth ));
	
	if ( bitDepth == 1 ) {
		return ( n >> 3 ) + (( n & 0x07 ) ? 1 : 0 );
	}
	else if ( bitDepth == 4 ) {
		return ( n >> 1 ) + ( n & 0x01 );
	}

	return n * ( bitDepth >> 3 );
}

//----------------------------------------------------------------//
void ZLBitBuffer::Clear ( void* buffer, u32 bitDepth, size_t x, size_t n ) {

	if ( n == 0 ) return;

	assert ( ZLBitBuffer::IsLegalBitDepth ( bitDepth ));

	if ( bitDepth < 8 ) {
		
		// not 8-bit, so need to deal with sub-byte end caps
	
		// just treat everything as 1-bit resolution
		if ( bitDepth == 4 ) {
			bitDepth = 8;
			
			// 4 bits to every nybble
			x *= 4;
			n *= 4;
		}
		
		// base address
		u8* bytes = ( u8* )(( size_t )buffer + ( x >> 3 ));

		// handle the lower boundary if not byte aligned
		size_t mod = x & 0x07; // will be 0 if byte aligned
		if ( mod > 0 ) {
			size_t bits = 8 - mod; // bits until end of byte
			bits = bits > n ? n : bits; // n is smaller than remainder of byte
			size_t mask = ( 1 << bits ) - 1; // mask for number of bytes to clear
			*bytes = ( *bytes ) & ~( mask << mod ); // have to shift mask up by mod to clear
			
			++bytes; // done with first byte
			n -= bits; // done with first bits of n
		}
		
		size_t size = n >> 3; // size in whole bytes
		if ( size ) {
			memset ( bytes, 0, size );
			bytes += size; // done with 'size' in whole bytes
		}
		
		// bits of end cap
		size_t bits = n & 0x07; // 'n' will have been advanced earlier if not byte aligned
		if ( bits > 0 ) {
			size_t mask = ( 1 << bits ) - 1; // get the mask
			*bytes = ( *bytes ) & ~mask; // clear out the mask
		}
	}
	else {
	
		size_t sizeInBytes = bitDepth >> 3;
		memset (( void* )(( size_t )buffer + ( x * sizeInBytes )), 0, n * sizeInBytes );
	}
}

//----------------------------------------------------------------//
u32 ZLBitBuffer::GetMask ( u32 bitDepth ) {

	return ( u32 )(( u64 )1 << bitDepth ) - 1;
}

//----------------------------------------------------------------//
u32 ZLBitBuffer::GetValue ( const void* buffer, size_t x, u32 bitDepth ) {

	assert ( ZLBitBuffer::IsLegalBitDepth ( bitDepth ));

	if ( bitDepth == 1 ) {
	
		const u8* bytes = ( const u8* )(( size_t )buffer + ( x >> 3 ));
		return ( *bytes >> ( x & 0x07 )) & 0x01;
	}
	else if ( bitDepth == 4 ) {

		const u8* bytes = ( const u8* )(( size_t )buffer + ( x >> 1 ));
		return ( *bytes >> (( x & 0x01 ) * 4 )) & 0x0F;
	}
	else {
		
		const u8* bytes = ( const u8* )(( size_t )buffer + ( x * ( bitDepth >> 3 )));
		u32 shift = 0;
		u32 value = 0;
		
		switch ( bitDepth ) {
		
			case 32:
				value += ( *( bytes++ )) << shift;
				shift += 0x08;
			case 24:
				value += ( *( bytes++ )) << shift;
				shift += 0x08;
			case 16:
				value += ( *( bytes++ )) << shift;
				shift += 0x08;
			case 8:
				value += ( *( bytes++ )) << shift;
		}
		return value;
	}
	return 0;
}

//----------------------------------------------------------------//
bool ZLBitBuffer::IsLegalBitDepth ( u32 bitDepth ) {

	switch ( bitDepth ) {
		case 1:
		case 4:
		case 8:
		case 16:
		case 24:
		case 32:
			return true;
	}
	return false;
}

//----------------------------------------------------------------//
void ZLBitBuffer::SetValue ( void* buffer, u32 value, size_t x, u32 bitDepth ) {

	assert ( ZLBitBuffer::IsLegalBitDepth ( bitDepth ));
	
	if ( bitDepth == 1 ) {
	
		u8* bytes = ( u8* )(( size_t )buffer + ( x >> 3 ));
		u8 shift = ( x & 0x07 );
		( *bytes ) = (( *bytes ) & ~( 0x01 << shift )) | (( value & 0x01 ) << shift );
	}
	else if ( bitDepth == 4 ) {
	
		u8* bytes = ( u8* )(( size_t )buffer + ( x >> 1 ));
		u8 shift = ( x & 0x01 ) * 4;
		( *bytes ) = (( *bytes ) & ~( 0x0f << shift )) | (( value & 0x0f ) << shift );
	}
	else {

		u8* bytes = ( u8* )(( size_t )buffer + ( x * ( bitDepth >> 3 )));
		u32 shift = 0;

		switch ( bitDepth ) {

			case 32:
				*( bytes++ ) = ( value >> shift ) & 0xff;
				shift += 0x08;
			case 24:
				*( bytes++ ) = ( value >> shift ) & 0xff;
				shift += 0x08;
			case 16:
				*( bytes++ ) = ( value >> shift ) & 0xff;
				shift += 0x08;
			case 8:
				*( bytes++ ) = ( value >> shift ) & 0xff;
				break;
		}
	}
}

//----------------------------------------------------------------//
// TODO: move this to tests
void ZLBitBuffer::Test () {

	assert ( GetMask ( 1 ) == 0x00000001 );
	assert ( GetMask ( 4 ) == 0x0000000f );
	assert ( GetMask ( 8 ) == 0x000000ff );
	assert ( GetMask ( 16 ) == 0x0000ffff );
	assert ( GetMask ( 24 ) == 0x00ffffff );
	assert ( GetMask ( 32 ) == 0xffffffff );
	
	static const size_t BUFF_SIZE = 32;
	u8 buffer [ BUFF_SIZE ];
	
	memset ( buffer, 0, BUFF_SIZE );
	SetValue ( buffer, 1, 5, 1 );
	SetValue ( buffer, 1, 13, 1 );
	SetValue ( buffer, 1, 20, 1 );
	assert ( check_bin ( buffer, "00000100000001000000100000000000" ));
	
	memset ( buffer, 0, BUFF_SIZE );
	SetValue ( buffer, 0xf, 1, 4 );
	SetValue ( buffer, 0x4, 3, 4 );
	SetValue ( buffer, 0xa, 8, 4 );
	SetValue ( buffer, 0x1, 12, 4 );
	SetValue ( buffer, 0x7, 15, 4 );
	assert ( check_hex ( buffer, "0f040000a0001007" ));
	
	memset ( buffer, 0, BUFF_SIZE );
	SetValue ( buffer, 0xfedcba, 1, 24 );
	SetValue ( buffer, 0x654321, 3, 24 );
	assert ( check_hex ( buffer, "000000abcdef000000123456" ));
	
	assert ( GetValue ( buffer, 7, 4 ) == 0xb );
	assert ( GetValue ( buffer, 22, 4 ) == 0x5 );
	assert ( GetValue ( buffer, 2, 16 ) == 0xfedc );
	assert ( GetValue ( buffer, 5, 16 ) == 0x6543 );
	
	memset ( buffer, -1L, BUFF_SIZE );
	Clear ( buffer, 1, 3, 22 );
	assert ( check_bin ( buffer, "11100000000000000000000001111111" ));
	
	memset ( buffer, -1L, BUFF_SIZE );
	Clear ( buffer, 4, 1, 6 );
	assert ( check_bin ( buffer, "11110000000000000000000000001111" ));
	
	memset ( buffer, -1L, BUFF_SIZE );
	Clear ( buffer, 1, 3, 4 );
	assert ( check_bin ( buffer, "11100001" ));
	
	u8 srcBuffer [ BUFF_SIZE ];
	
	decode_bin ( srcBuffer, "00000000100110011100110000001111" );
	
	memset ( buffer, 0, BUFF_SIZE );
	Blit ( buffer, 0, srcBuffer, 8, 24, 1 );
	assert ( check_bin ( buffer, "10011001110011000000111100000000" ));
	
	assert ( GetValue ( buffer, 2, 1 ) == 0 );
	assert ( GetValue ( buffer, 3, 1 ) == 1 );
	assert ( GetValue ( buffer, 4, 1 ) == 1 );
	assert ( GetValue ( buffer, 5, 1 ) == 0 );
	
	memset ( buffer, 0, BUFF_SIZE );
	Blit ( buffer, 3, srcBuffer, 8, 23, 1 );
	assert ( check_bin ( buffer, "00010011001110011000000111000000" ));
	
	memset ( buffer, -1L, BUFF_SIZE );
	Blit ( buffer, 3, srcBuffer, 8, 23, 1 );
	assert ( check_bin ( buffer, "11110011001110011000000111111111" ));
	
	decode_bin ( srcBuffer, "00000101000000001111111110100000" );
	
	memset ( buffer, -1L, BUFF_SIZE );
	Blit ( buffer, 5, srcBuffer, 5, 22, 1 );
	assert ( check_bin ( buffer, "11111101000000001111111110111111" ));
	
	memset ( buffer, -1L, BUFF_SIZE );
	Blit ( buffer, 7, srcBuffer, 5, 22, 1 );
	assert ( check_bin ( buffer, "11111111010000000011111111101111" ));
	
	memset ( buffer, 0, BUFF_SIZE );
	Blit ( buffer, 7, srcBuffer, 5, 17, 1 );
	assert ( check_bin ( buffer, "00000001010000000011111100000000" ));
	
	decode_bin ( srcBuffer, "00001111000011110000111100001111" );
	
	memset ( buffer, 0, BUFF_SIZE );
	Blit ( buffer, 0, srcBuffer, 1, 7, 4 );
	assert ( check_bin ( buffer, "11110000111100001111000011110000" ));
	
	decode_bin ( srcBuffer, "00001001" );
	memset ( buffer, 0, BUFF_SIZE );
	Blit ( buffer, 2, srcBuffer, 4, 4, 1 );
	assert ( check_bin ( buffer, "00100100" ));
	
	decode_bin ( srcBuffer, "01011101" );
	memset ( buffer, 0, BUFF_SIZE );
	Blit ( buffer, 4, srcBuffer, 1, 7, 1 );
	assert ( check_bin ( buffer, "0000101110100000" ));
	
	decode_bin ( srcBuffer, "0000010111010000" );
	
	memset ( buffer, 0, BUFF_SIZE );
	Blit ( buffer, 1, srcBuffer, 5, 7, 1 );
	assert ( check_bin ( buffer, "01011101" ));
	
	memset ( buffer, 0, BUFF_SIZE );
	Blit ( buffer, 4, srcBuffer, 5, 7, 1 );
	assert ( check_bin ( buffer, "0000101110100000" ));
}
