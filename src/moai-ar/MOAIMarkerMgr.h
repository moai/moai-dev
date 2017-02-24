// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMARKERMGR_H
#define MOAIMARKERMGR_H

#if MOAI_WITH_ARTOOLKIT

#import <AR/ar.h>
#import <AR/video.h>
#import <AR/sys/CameraVideo.h>

class MOAIMarker;
class MOAIVideoTexture;

//================================================================//
// MOAIPattern
//================================================================//
class MOAIPattern {
private:

	friend class MOAIMarkerMgr;
	
    ARdouble            mPatternWidth;
    int                 mPatternID;
};

//================================================================//
// MOAIMarkerMgr
//================================================================//
class MOAIMarkerMgr :
	public ZLContextClass < MOAIMarkerMgr, MOAIGlobalEventSource > {
private:

	friend class MOAIVideoTexture;

	enum {
		VIDEO_TEXTURE_PLANE_0,
		VIDEO_TEXTURE_PLANE_1,
	};

	BOOL                mVideoPaused;
    AR2VideoParamT*     mVideoParam;
    ARHandle*           mARHandle;
	AR3DHandle*         mAR3DHandle;
    ARPattHandle*       mARPatternHandle;
    
    // Transformation matrix retrieval
    //AR3DHandle*         mAR3DHandle;
    //ARdouble            mPatternTrans [ 3 ][ 4 ];     // Per-marker, but we are using only 1 marker.
    //int                 mPatternFound;                // Per-marker, but we are using only 1 marker.
    //BOOL                mUseContPoseEstimation;

	ARParamLT*          mCameraParam;
	
	AR_PIXEL_FORMAT		mVideoPixelFormat;
	u32					mVideoWidth;
	u32					mVideoHeight;
	
	AR2VideoBufferT*						mVideoBuffer;
	
	MOAILuaSharedPtr < MOAISpriteDeck2D >	mVideoDeck;
	MOAILuaSharedPtr < MOAIShader >			mVideoShader;
	MOAILuaSharedPtr < MOAIShaderProgram >	mVideoShaderProgram;
	MOAILuaSharedPtr < MOAIVideoTexture >	mVideoPlane0;
	MOAILuaSharedPtr < MOAIVideoTexture >	mVideoPlane1;
	MOAILuaSharedPtr < MOAICamera >			mVideoCamera;
	
	//MOAILuaSharedPtr < MOAIMatrix >			mMarker;
	
	typedef STLMap < int, MOAIPattern >::iterator	PatternIt;
	STLMap < int, MOAIPattern >						mPatterns;
	
	typedef ZLLeanList < MOAIMarker* >::Iterator	MarkerIt;
	
	ZLLeanArray < MOAIMarker* >				mMarkers;
	ZLLeanList < MOAIMarker* >				mStaleMarkers;
	MOAIMarker*								mActiveMarkers;
	MOAIMarker*								mFreeMarkers;
	
	ZLMatrix4x4 mProjMtx;
	
	GET ( AR2VideoBufferT*, VideoBuffer, mVideoBuffer )
	
	//----------------------------------------------------------------//
	static int			_getMarkerMatrix		( lua_State* L );
	static int			_getMarkerPosition2D	( lua_State* L );
	static int			_getVideoCamera			( lua_State* L );
	static int			_getVideoDeck			( lua_State* L );
	static int			_getVideoSize			( lua_State* L );
	static int			_loadPattern			( lua_State* L );
	static int			_start					( lua_State* L );
	static int			_stop					( lua_State* L );

	//----------------------------------------------------------------//
	void				AffirmDeck				();
	void				AffirmMarker			( ARMarkerInfo* markerInfo );
	void				InvokeMarkerEvent		( MOAIMarker& marker, u32 event );
	static void			StartCallback			( void* userData );
	void				UpdateVideoProjMtx		();
	void				VideoDidStart			();

public:

	enum {
		EVENT_MARKER_BEGIN,
		EVENT_MARKER_END,
		EVENT_MARKER_UPDATE,
		EVENT_UPDATE_FRAME,
		EVENT_VIDEO_START,
	};

	DECL_LUA_SINGLETON ( MOAIMarkerMgr )

	GET ( AR_PIXEL_FORMAT, VideoPixelFormat, mVideoPixelFormat )
	GET ( u32, VideoWidth, mVideoWidth )
	GET ( u32, VideoHeight, mVideoHeight )

	//----------------------------------------------------------------//
	bool			GetMarkerMatrix			( u32 markerID, MOAIMatrix& matrix );
	bool			GetMarkerPosition		( u32 markerID, ZLVec2D& position );
	int				LoadPattern				( cc8* filename, double width );
					MOAIMarkerMgr			();
					~MOAIMarkerMgr			();
	void			Pause					( bool pause );
	void			ProcessFrame			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Start					();
	void			Stop					();
};

//================================================================//
// MOAIArVideoListener
//================================================================//
@interface MOAIArVideoListener : NSObject < CameraVideoTookPictureDelegate > {
}
@end

#endif
#endif
