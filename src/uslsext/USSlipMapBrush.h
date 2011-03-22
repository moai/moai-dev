// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USSLIPPMAPBRUSH_H
#define USSLIPPMAPBRUSH_H

#include <uslsext/USFileCache.h>
#include <uslsext/USSlipMapTileSource.h>
#include <uslsext/USTexture.h>

class USSlipMapTile;
class USQuadCoord;

//================================================================//
// USSlipMapRenderHint
//================================================================//
namespace USSlipMapRenderHint {
	enum Type {
		DRAW_SPINNER		= 1 << 0x00,
		DRAW_STANDINS		= 1 << 0x01,
	};
};

//================================================================//
// USSlipMapBrush
//================================================================//
class USSlipMapBrush {
private:

	enum kTexID {
		LOADING_TEX,
		LOADING_BG_TEX,
	};

	USRect			mFrame;

	u32				mSourceID;

	float			mTime;
	u32				mQueueSize;

	typedef STLMap < USQuadCoord, USSlipMapTile >::iterator QuadIt;
	STLMap < USQuadCoord, USSlipMapTile > mQuads;

	u32				mZoom;

	USTexture		mLoadingTex;
	USTexture		mLoadingBGTex;
	
	u32				mExtraLevels;
	u32				mPrecacheLevels;
	float			mViewPad;
	u32				mMaxTiles;

	USFileCache*	mFileCache;
	int				mCachePriority;

	//----------------------------------------------------------------//
	USSlipMapTile&		AffirmQuad					( USQuadCoord quadID, bool zoomed );
	bool				BindTex						( kTexID texID );
	void				UpdateQuads					( USRect& bounds );

public:
	
	friend class USSlipMapTile;
	
	GET ( u32, SourceID, mSourceID )
	GET ( USFileCache*, FileCache, mFileCache )
	GET ( int, CachePriority, mCachePriority )
	
	GET_SET ( USRect, Frame, mFrame )
	GET_SET ( u32, ExtraLevels, mExtraLevels )
	GET_SET ( u32, PrecacheLevels, mPrecacheLevels )
	GET_SET ( float, ViewPad, mViewPad )
	GET_SET ( u32, MaxTiles, mMaxTiles )
	
	//----------------------------------------------------------------//
	u32						ClampZoom					( u32 zoom );
	void					Clear						();
	u32						ComputeZoom					();
	void					Draw						( u32 hint, bool zoomed );
	float					GetCamScaleFromZoom			( u32 zoom );
	virtual u32				GetMaxZoom					();
	virtual u32				GetMinZoom					();
	virtual u32				GetTileSize					();
	virtual STLString		GetTileStoreKey				( USQuadCoord& quadID );
	virtual STLString		GetTileURL					( USQuadCoord& quadID );
	float					GetWidth					();
	u32						GetZoom						();
	u32						GetZoomFromCamScale			( float scale );
	USVec2D64				ProjectMapToPolar			( double x, double y );
	USVec2D					ProjectPolarToMap			( double lon, double lat );
	void					PurgeQuads					();
	void					SetFileCache				( USFileCache* fileCache, int cachePriority );
	void					SetLoadingTiles				( cc8* loadingTile, cc8* bgTile );
	void					SetSourceID					( u32 sourceID );
	void					Update						( float step );
	void					SetZoom						( u32 zoom );
							USSlipMapBrush				();
	virtual					~USSlipMapBrush				();
};

#endif
