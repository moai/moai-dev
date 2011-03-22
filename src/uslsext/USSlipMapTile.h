// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USSLIPQUAD_H
#define USSLIPQUAD_H

#include <uslsext/USHttpTask.h>
#include <uslsext/USTexture.h>
#include <uslsext/USQuadCoord.h>

class USHttpTask;
class USSlipMapBrush;
class USQuadCoord;

//================================================================//
// USSlipMapTile
//================================================================//
class USSlipMapTile {
private:

	enum {
		PRE_LOAD,
		LOADING,
		LOADED,
		FAILED,
		KILL_ME
	};

	USRef < USHttpTask > mImageFetcher;

	USQuadCoord			mQuadID;
	USSlipMapBrush*		mMap;

	USSlipMapTile*		mParent;
	USTexture			mTexture;

	u32					mLoadingState;

	bool				mFailed;
	bool				mStandIn;

	u32					mWaitCounter;

	//----------------------------------------------------------------//
	void				Clear				();
	void				DrawBackground		( USRect& rect );
	void				DrawSpinner			( USRect& rect, float time );
	void				DrawSubQuad			( USQuadCoord& subQuadID, float time, const USRect& frame, u32 hint );
	void				LoadTile			();
	void				LoadTileFinish		( USHttpTask* fetcher );

public:
	
	friend class USSlipMapBrush;
	
	//----------------------------------------------------------------//
	bool				BindTexture			();
	void				Draw				( float time, const USRect& frame, u32 hint );
	USSlipMapTile*		GetParent			();
	u32					GetState			();
	STLString			GetVEQuadKey		();
	void				Init				( USSlipMapBrush& map, USQuadCoord quadID, bool standIn );
						USSlipMapTile		();
						~USSlipMapTile		();
};

#endif
