// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMARKERMGR_H
#define MOAIMARKERMGR_H

#if MOAI_WITH_ARTOOLKIT

#import <AR/ar.h>
#import <AR/video.h>
#import <AR/sys/CameraVideo.h>

class MOAIVideoTexture;

//================================================================//
// MOAIMarker
//================================================================//
//class MOAIMarker {
//private:
//
//public:
//
//};

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
    
    // Video acquisition
    AR2VideoParamT*     mVideoParam;
    
    // Marker detection
    ARHandle*           mARHandle;
    ARPattHandle*       mARPatternHandle;
    
    // Transformation matrix retrieval
    AR3DHandle*         mAR3DHandle;
    ARdouble            mPatternWidth;                // Per-marker, but we are using only 1 marker.
    ARdouble            mPatternTrans [ 3 ][ 4 ];     // Per-marker, but we are using only 1 marker.
    int                 mPatternFound;                // Per-marker, but we are using only 1 marker.
    int                 mPatternID;                   // Per-marker, but we are using only 1 marker.
    BOOL                mUseContPoseEstimation;

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
	
	GET ( AR2VideoBufferT*, VideoBuffer, mVideoBuffer )
	
	//----------------------------------------------------------------//
	static int			_getVideoDeck			( lua_State* L );
	static int			_start					( lua_State* L );
	static int			_stop					( lua_State* L );

	//----------------------------------------------------------------//
	void				AffirmDeck				();
	static void			StartCallback			( void* userData );
	void				VideoDidStart			();

public:

	enum {
		EVENT_UPDATE_FRAME,
		EVENT_VIDEO_START,
	};

	DECL_LUA_SINGLETON ( MOAIMarkerMgr )

	GET ( AR_PIXEL_FORMAT, VideoPixelFormat, mVideoPixelFormat )
	GET ( u32, VideoWidth, mVideoWidth )
	GET ( u32, VideoHeight, mVideoHeight )

	//----------------------------------------------------------------//
						MOAIMarkerMgr			();
						~MOAIMarkerMgr			();
	void				Pause					( bool pause );
	void				ProcessFrame			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				Start					();
	void				Stop					();
};

//================================================================//
// MOAIArVideoListener
//================================================================//
@interface MOAIArVideoListener : NSObject < CameraVideoTookPictureDelegate > {
}
@end

#endif
#endif
