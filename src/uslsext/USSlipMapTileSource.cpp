// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USFileCache.h>
#include <uslsext/USSlipMapTileSource.h>
#include <uslsext/USQuadCoord.h>

// TODO: move to Lua
#define CLOUDMADE_API_KEY "1d586f1b57ea51238741390130b237d2"

static STLString sVEToken;

//================================================================//
// local
//================================================================//

STLString GetVEQuadKey ( USQuadCoord& quadID );

//----------------------------------------------------------------//
STLString GetVEQuadKey ( USQuadCoord& quadID ) {

	u32 cursor = 0;
	char quadKey [ 32 ];
	
	for ( int i = quadID.mLevel; i > 0; i-- ) {
	
		int mask = 1 << (i - 1);
		int cell = 0;
		
		if (( quadID.mXTile & mask ) != 0) {
			cell++;
		}
		
		if (( quadID.mYTile & mask ) != 0) {
			cell += 2;
		}
		
		sprintf ( &quadKey [ cursor++ ], "%d", cell );
	}
	
	quadKey [ cursor ] = 0;
	
	STLString result ( quadKey );
	return result;
}

//================================================================//
// USSlipMapTileSource
//================================================================//

//----------------------------------------------------------------//
u32 USSlipMapTileSource::GetMaxZoom ( u32 sourceID ) {

	u32 zoom = 0;

	switch ( sourceID ) {
	
		case VE_STREET: {
			zoom = 19;
			break;
		}
		case VE_SAT: {
			zoom = 19;
			break;
		}
		case OCM_TOPO: {
			zoom = 18;
			break;
		}
		default:
		case OSM_STREET: {
			zoom = 18;
			break;
		}
	}
	
	return zoom;
}

//----------------------------------------------------------------//
u32 USSlipMapTileSource::GetMinZoom ( u32 sourceID ) {

	u32 zoom = 0;

	switch ( sourceID ) {
		case VE_STREET: {
			zoom = 1;
			break;
		}
		case VE_SAT: {
			zoom = 1;
			break;
		}
		case OCM_TOPO: {
			zoom = 1;
			break;
		}
		default:
		case OSM_STREET: {
			zoom = 0;
			break;
		}
	}
	
	return zoom;
}

//----------------------------------------------------------------//
u32 USSlipMapTileSource::GetTileSize ( u32 sourceID ) {
	UNUSED ( sourceID );
	return 256;
}

//----------------------------------------------------------------//
STLString USSlipMapTileSource::GetTileStoreKey ( u32 sourceID, USQuadCoord& quadID ) {

	STLString storeKey;

	switch ( sourceID ) {
		case VE_STREET: {
			storeKey.write ( "VE_MAP_%ld_%ld_%ld", quadID.mLevel, quadID.mXTile, quadID.mYTile );
			break;
		}
		case VE_SAT: {
			storeKey.write ( "VE_SAT_%ld_%ld_%ld", quadID.mLevel, quadID.mXTile, quadID.mYTile );
			break;
		}
		case OCM_TOPO: {
			storeKey.write ( "OCM_TOPO_%ld_%ld_%ld", quadID.mLevel, quadID.mXTile, quadID.mYTile );
			break;
		}
		default:
		case OSM_STREET: {
			storeKey.write ( "OSM_MAP_%ld_%ld_%ld", quadID.mLevel, quadID.mXTile, quadID.mYTile );
			break;
		}
	}
	
	return storeKey;
}

//----------------------------------------------------------------//
STLString USSlipMapTileSource::GetTileURL ( u32 sourceID, USQuadCoord& quadID ) {

	STLString url;

	switch ( sourceID ) {
		case VE_STREET: {
			STLString quadKey = GetVEQuadKey ( quadID );
			url.write ( "http://r3.ortho.tiles.virtualearth.net/tiles/r%s.png?g=15&token=%s", quadKey.str (), sVEToken.str());
			break;
		}
		case VE_SAT: {
			STLString quadKey = GetVEQuadKey ( quadID );
			url.write ( "http://h3.ortho.tiles.virtualearth.net/tiles/h%s.png?g=15&token=%s", quadKey.str (), sVEToken.str());
			break;
		}
		case OCM_TOPO: {
			url.write ( "http://andy.sandbox.cloudmade.com/tiles/cycle/%ld/%ld/%ld.png",quadID.mLevel, quadID.mXTile, quadID.mYTile);
			break;
		}
		default:
		case OSM_STREET: {
			url.write ( "http://tile.cloudmade.com/%s/1/256/%ld/%ld/%ld.png", CLOUDMADE_API_KEY, quadID.mLevel, quadID.mXTile, quadID.mYTile );
			break;
		}
	}
	
	return url;
}

//----------------------------------------------------------------//
STLString USSlipMapTileSource::GetVEToken () {

	return sVEToken;
}

//----------------------------------------------------------------//
void USSlipMapTileSource::SetVEToken ( cc8* token ) {

	sVEToken = token;
}

