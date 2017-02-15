// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMARKERMGR_H
#define MOAIMARKERMGR_H

#if MOAI_WITH_ARTOOLKIT

#import <AR/ar.h>
#import <AR/video.h>
//#import <AR/gsub_es2.h>
#import <AR/sys/CameraVideo.h>

//================================================================//
// MOAIMarkerMgr
//================================================================//
class MOAIMarkerMgr :
	public ZLContextClass < MOAIMarkerMgr, MOAILuaObject > {
private:

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
	
	//id < CameraVideoTookPictureDelegate > mCameraListener;

	//----------------------------------------------------------------//
	static int			_start					( lua_State* L );
	static int			_stop					( lua_State* L );

	//----------------------------------------------------------------//
	static void			StartCallback			( void* userData );
	void				VideoDidStart			();

public:

	DECL_LUA_SINGLETON ( MOAIMarkerMgr )

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
