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
};

#endif
