// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USTILE_H
#define USTILE_H

//================================================================//
// USTile
//================================================================//
class USTile {
public:

	static const u32 TILE_STR_LEN		= 11;
	static const u32 TILE_STR_XFLIP		= 7;
	static const u32 TILE_STR_YFLIP		= 8;
	static const u32 TILE_STR_HIDE		= 10;

	static const u32 XFLIP				= 0x20000000;
	static const u32 YFLIP				= 0x40000000;
	static const u32 HIDDEN				= 0x80000000;
		
	static const u32 FLIP_MASK			= 0x60000000;
	static const u32 FLAGS_MASK			= 0xE0000000;
	static const u32 CODE_MASK			= 0x1fffffff;
		
	static const u32 NULL_MASK			= 0x00002000;
	
	//----------------------------------------------------------------//
	static u32			FromString			( cc8* str );
	static STLString	ToString			( u32 code );
	
	//----------------------------------------------------------------//
	inline static u32 GetBase ( u32 mask ) {
		
		return ( mask & 0xff );
	}
	
	//----------------------------------------------------------------//
	inline static u32 GetCodeMask ( u32 mask ) {
	
		u32 loSize	= GetLoSize ( mask );
		u32 hiSize	= GetHiSize ( mask );
		u32 base	= GetBase ( mask );
		
		return GetCodeMask ( loSize, hiSize, base );
	}
	
	//----------------------------------------------------------------//
	inline static u32 GetCodeMask ( u32 loSize, u32 hiSize, u32 base ) {
		
		u32 size = ( loSize + hiSize );
		return ( size < 32 ) ? ((( 1 << size ) - 1 ) << base ) : 0xffffffff;
	}
	
	//----------------------------------------------------------------//
	inline static u32 GetHiSize ( u32 mask ) {
		
		return ( mask >> 0x10 ) & 0xff;
	}
	
	//----------------------------------------------------------------//
	inline static u32 GetLoSize ( u32 mask ) {
		
		return ( mask >> 0x08 ) & 0xff;
	}
	
	//----------------------------------------------------------------//
	inline static u32 Init ( u32 code, bool xFlip, bool yFlip, bool hidden = false ) {
	
		//u32 t = code & CODE_MASK;
		
		if ( xFlip ) {
			code |= XFLIP;
		}
		
		if ( yFlip ) {
			code |= YFLIP;
		}
		
		if ( hidden ) {
			code |= HIDDEN;
		}
		
		return code;
	}
	
	//----------------------------------------------------------------//
	inline static u32 PackCode ( u32 code, u32 mask ) {
	
		u32 loSize	= GetLoSize ( mask );
		u32 hiSize	= GetHiSize ( mask );
		u32 base	= GetBase ( mask );
		
		return PackCode ( code, loSize, hiSize, base );
	}
	
	//----------------------------------------------------------------//
	inline static u32 PackCode ( u32 code, u32 loSize, u32 hiSize, u32 base ) {
	
		u32 loMask	= ( loSize < 32 ) ? (( 1 << loSize ) - 1 ) : 0xffffffff;
		u32 hiMask	= ( hiSize < 32 ) ? (( 1 << hiSize ) - 1 ) << ( 32 - hiSize ) : 0xffffffff;
		
		u32 lo	= ( code & loMask ) << base;
		u32 hi	= ( code & hiMask ) << ( 32 - base - loSize - hiSize );
		
		return ( lo | hi );
	}
	
	//----------------------------------------------------------------//
	inline static u32 PackMask ( u32 loSize, u32 hiSize, u32 base ) {
		
		return ( base & 0xff ) | (( loSize & 0xff ) << 0x08 ) | (( hiSize & 0xff ) << 0x10 );
	}
	
	//----------------------------------------------------------------//
	inline static u32 ToggleHidden ( u32 code ) {
	
		return ( code ^= HIDDEN );
	}
	
	//----------------------------------------------------------------//
	inline static u32 ToggleXFlip ( u32 code ) {
	
		return ( code ^= XFLIP );
	}
	
	//----------------------------------------------------------------//
	inline static u32 ToggleYFlip ( u32 code ) {
	
		return ( code ^= YFLIP );
	}
	
	//----------------------------------------------------------------//
	inline static u32 UnpackCode ( u32 code, u32 mask ) {
	
		u32 loSize	= GetLoSize ( mask );
		u32 hiSize	= GetHiSize ( mask );
		u32 base	= GetBase ( mask );
		
		return UnpackCode ( code, loSize, hiSize, base );
	}
	
	//----------------------------------------------------------------//
	inline static u32 UnpackCode ( u32 code, u32 loSize, u32 hiSize, u32 base ) {
	
		u32 loMask	= ( loSize < 32 ) ? (( 1 << loSize ) - 1 ) : 0xffffffff;
		u32 hiMask	= ( hiSize < 32 ) ? (( 1 << hiSize ) - 1 ) << ( 32 - hiSize ) : 0xffffffff;
		
		u32 lo	= ( code >> base ) & loMask;
		u32 hi	= ( code << ( 32 - base - loSize - hiSize )) & hiMask;
		
		return ( lo | hi );
	}
};

#endif
