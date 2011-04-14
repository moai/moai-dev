// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USTile.h>

//================================================================//
// USTile
//================================================================//

//----------------------------------------------------------------//
u32 USTile::FromString ( cc8* str ) {

	char tileStr [ TILE_STR_LEN + 1 ];

	strncpy ( tileStr, str, TILE_STR_LEN );
	tileStr [ TILE_STR_LEN ] = 0;

	uint tileID;
	sscanf ( tileStr, "0x%04X", &tileID );

	u32 code = tileID & CODE_MASK;

	code |= ( tileStr [ TILE_STR_XFLIP ] == '1' ) ? XFLIP : 0;
	code |= ( tileStr [ TILE_STR_YFLIP ] == '1' ) ? YFLIP : 0;
	code |= ( tileStr [ TILE_STR_HIDE ] == '1' ) ? HIDDEN : 0;
	
	return code;
}

//----------------------------------------------------------------//
STLString USTile::ToString ( u32 code ) {

	char tileStr [ TILE_STR_LEN + 1 ];

	uint tileID = code & CODE_MASK;
	int xFlip = ( code & XFLIP ) ? 1 : 0;
	int yFlip = ( code & YFLIP ) ? 1 : 0;
	int hidden = ( code & HIDDEN ) ? 1 : 0;

	sprintf ( tileStr, "0x%04X.%d%d.%d", tileID, xFlip, yFlip, hidden );

	return tileStr;
}
