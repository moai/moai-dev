// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USSLIPMAPTILESOURCE_H
#define USSLIPMAPTILESOURCE_H

#include <uslsext/USTask.h>

class USFileCache;
class USHttpTask;
class USSlipMapBrush;
class USQuadCoord;

//================================================================//
// USSlipMapTileSource
//================================================================//
namespace USSlipMapTileSource {

	enum Enum {
		OSM_STREET,
		VE_STREET,
		VE_SAT,
		OCM_TOPO,
		USGS_TOPO,
		GOOGLE_STREET,
		GOOGLE_SAT,
		GOOGLE_HYBRID,
		UNAVAILABLE_STREET,
		UNAVAILABLE_SAT,
	};
	
	//----------------------------------------------------------------//
	u32					GetMaxZoom				( u32 sourceID );
	u32					GetMinZoom				( u32 sourceID );
	u32					GetTileSize				( u32 sourceID );
	STLString			GetTileStoreKey			( u32 sourceID, USQuadCoord& quadID );
	STLString			GetTileURL				( u32 sourceID, USQuadCoord& quadID );
	STLString			GetVEToken				();
	void				SetVEToken				( cc8* token );
};

#endif
