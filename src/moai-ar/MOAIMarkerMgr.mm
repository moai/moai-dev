// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-ar/MOAIMarkerMgr.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_WITH_ARTOOLKIT

// uncomment me to debug log
#define MOAIMARKERMGR_DEBUG

#ifdef MOAIMARKERMGR_DEBUG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

#define VIEW_SCALEFACTOR        1.0f

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMarkerMgr::_start ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMarkerMgr, "" )
	
	MOAIMarkerMgr::Get ().Start ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMarkerMgr::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMarkerMgr, "" )
	
	MOAIMarkerMgr::Get ().Stop ();
	return 0;
}

//================================================================//
// MOAIMarkerMgr
//================================================================//

//----------------------------------------------------------------//
MOAIMarkerMgr::MOAIMarkerMgr () :
	mVideoPaused ( FALSE ),
	mVideoParam ( 0 ),
	mARHandle ( 0 ),
	mARPatternHandle ( 0 ),
	mAR3DHandle ( 0 ),
	mUseContPoseEstimation ( FALSE ),
	mCameraParam ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMarkerMgr::~MOAIMarkerMgr () {
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::Start () {

	char* vconf = ""; // See http://www.artoolworks.com/support/library/Configuring_video_capture_in_ARToolKit_Professional#AR_VIDEO_DEVICE_IPHONE
	if ( !( mVideoParam = ar2VideoOpenAsync ( vconf, StartCallback, 0 ))) {
		DEBUG_LOG ( "Error: Unable to open connection to camera.\n" );
		this->Stop ();
		return;
	}
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::StartCallback ( void* userData ) {
	UNUSED ( userData );

	MOAIMarkerMgr::Get ().VideoDidStart ();
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::Stop () {

	if ( !mVideoPaused ) {
        ar2VideoCapStop ( mVideoParam );
    }
    
    if ( mARHandle ) {
        arPattDetach ( mARHandle );
    }
    
    if ( mARPatternHandle ) {
        arPattDeleteHandle ( mARPatternHandle );
        mARPatternHandle = NULL;
    }
    
    if ( mAR3DHandle ) {
        ar3DDeleteHandle ( &mAR3DHandle );
    }
    
    if ( mARHandle ) {
        arDeleteHandle ( mARHandle );
        mARHandle = NULL;
    }
    
    if ( mCameraParam ) {
        arParamLTFree ( &mCameraParam );
        mCameraParam = NULL;
    }
    
    if ( mVideoParam ) {
        ar2VideoClose ( mVideoParam );
        mVideoParam = NULL;
    }
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::ProcessFrame () {

	AR2VideoBufferT* buffer = ar2VideoGetImage ( mVideoParam );

	if ( buffer ) {

		ARdouble err;
		int j, k;

		// Detect the markers in the video frame.
		if ( arDetectMarker ( mARHandle, buffer->buff ) < 0 ) return;
		DEBUG_LOG ( "found %d marker(s).\n", mARHandle->marker_num );

		// Check through the marker_info array for highest confidence
		// visible marker matching our preferred pattern.
		k = -1;
		for ( j = 0; j < mARHandle->marker_num; j++ ) {
			if ( mARHandle->markerInfo [ j ].id == mPatternID ) {
				if ( k == -1 ) {
					k = j;
				}
				else if ( mARHandle->markerInfo [ j ].cf > mARHandle->markerInfo [ k ].cf ) {
					k = j;
				}
			}
		}

		float modelView [ 16 ];

		if ( k != -1 ) {

			DEBUG_LOG ( "marker %d matched pattern %d.\n", k, mPatternID );

			// Get the transformation between the marker and the real camera into mPatternTrans.
			if ( mPatternFound && mUseContPoseEstimation ) {
				err = arGetTransMatSquareCont ( mAR3DHandle, &( mARHandle->markerInfo [ k ]), mPatternTrans, mPatternWidth, mPatternTrans );
			}
			else {
				err = arGetTransMatSquare ( mAR3DHandle, &( mARHandle->markerInfo [ k ]), mPatternWidth, mPatternTrans );
			}
			//arglCameraViewRHf ( mPatternTrans, modelView, VIEW_SCALEFACTOR );
			mPatternFound = TRUE;
		}
		else {
			mPatternFound = FALSE;
		}

		//if ( mDelegate ) {
		//	[ mDelegate cameraVideoTookPicture:self :buffer :( mPatternFound ? modelView : NULL )];
		//}
	}
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );

	//state.SetField ( -1, "EVENT_FINALIZE",	( u32 )EVENT_FINALIZE );

	luaL_Reg regTable [] = {
		{ "start",					_start },
		{ "stop",					_stop },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::VideoDidStart () {

	// Find the size of the window.
	int xsize, ysize;
	if ( ar2VideoGetSize ( mVideoParam, &xsize, &ysize ) < 0 ) {
		DEBUG_LOG ( "Error: ar2VideoGetSize.\n" );
		this->Stop ();
		return;
	}

	// Get the format in which the camera is returning pixels.
	AR_PIXEL_FORMAT pixFormat = ar2VideoGetPixelFormat ( mVideoParam );
	if ( pixFormat == AR_PIXEL_FORMAT_INVALID ) {
		DEBUG_LOG ( "Error: Camera is using unsupported pixel format.\n" );
		//[ self stop ] ;
		return;
	}

	// Work out if the front camera is being used. If it is, flip the viewing frustum for
	// 3D drawing.
	BOOL flipV = FALSE;
	int frontCamera;
	if ( ar2VideoGetParami ( mVideoParam, AR_VIDEO_PARAM_IOS_CAMERA_POSITION, &frontCamera ) >= 0 ) {
		if ( frontCamera == AR_VIDEO_IOS_CAMERA_POSITION_FRONT ) flipV = TRUE;
	}

	// Tell arVideo what the typical focal distance will be. Note that this does NOT
	// change the actual focus, but on devices with non-fixed focus, it lets arVideo
	// choose a better set of camera parameters.
	// Default is 0.3 metres. See <AR/sys/videoiPhone.h> for allowable values.
	ar2VideoSetParami ( mVideoParam, AR_VIDEO_PARAM_IOS_FOCUS, AR_VIDEO_IOS_FOCUS_0_3M );

	DEBUG_LOG ( "%s\n", ZLFileSys::GetCurrentPath ().c_str ());

	// Load the camera parameters, resize for the window and init.
	ARParam cparam;
	if ( ar2VideoGetCParam ( mVideoParam, &cparam ) < 0 ) {
		char cparam_name [] = "camera_para.dat";
		DEBUG_LOG ( "Unable to automatically determine camera parameters. Using default.\n" );
		if ( arParamLoad ( cparam_name, 1, &cparam ) < 0 ) {
			DEBUG_LOG ( "Error: Unable to load parameter file %s for camera.\n", cparam_name );
			this->Stop ();
			return;
		}
	}

	if ( cparam.xsize != xsize || cparam.ysize != ysize ) {
		DEBUG_LOG ( "*** Camera Parameter resized from %d, %d. ***\n", cparam.xsize, cparam.ysize );
		arParamChangeSize ( &cparam, xsize, ysize, &cparam );
	}

	DEBUG_LOG ( "*** Camera Parameter ***\n" );
	arParamDisp ( &cparam );

	if (( mCameraParam = arParamLTCreate ( &cparam, AR_PARAM_LT_DEFAULT_OFFSET )) == NULL ) {
		DEBUG_LOG ( "Error: arParamLTCreate.\n" );
		this->Stop ();
		return;
	}

	// AR init.
	if (( mARHandle = arCreateHandle ( mCameraParam )) == NULL ) {
		DEBUG_LOG ( "Error: arCreateHandle.\n" );
		this->Stop ();
		return;
	}

	if ( arSetPixelFormat ( mARHandle, pixFormat ) < 0 ) {
		DEBUG_LOG ( "Error: arSetPixelFormat.\n" );
		this->Stop ();
		return;
	}

	if (( mAR3DHandle = ar3DCreateHandle ( &mCameraParam->param )) == NULL ) {
		DEBUG_LOG ( "Error: ar3DCreateHandle.\n" );
		this->Stop ();
		return;
	}

	// libARvideo on iPhone uses an underlying class called CameraVideo. Here, we
	// access the instance of this class to get/set some special types of information.
	CameraVideo* cameraVideo = ar2VideoGetNativeVideoInstanceiPhone ( mVideoParam->device.iPhone );
	if ( !cameraVideo ) {
		DEBUG_LOG ( "Error: Unable to set up AR camera: missing CameraVideo instance.\n" );
		this->Stop ();
		return;
	}

	// The camera will be started by -startRunLoop.
	//videoDelegate = [[ ARVideoDelegate alloc ] init ];
	MOAIArVideoListener* videoLIstener = [[ MOAIArVideoListener alloc ] init ];
	[ cameraVideo setTookPictureDelegate:videoLIstener ];
	//[ cameraVideo setTookPictureDelegateUserData:NULL ];

	// Other ARToolKit setup. 
	arSetMarkerExtractionMode ( mARHandle, AR_USE_TRACKING_HISTORY_V2 );
	//arSetMarkerExtractionMode ( mARHandle, AR_NOUSE_TRACKING_HISTORY );
	//arSetLabelingThreshMode ( mARHandle, AR_LABELING_THRESH_MODE_MANUAL );

	// Prepare ARToolKit to load patterns.
	if ( !( mARPatternHandle = arPattCreateHandle ())) {
		DEBUG_LOG ( "Error: arPattCreateHandle.\n" );
		this->Stop ();
		return;
	}
	arPattAttach ( mARHandle, mARPatternHandle );

	// Load marker(s).
	// Loading only 1 pattern in this example.
	char *patt_name  = "hiro.patt";
	if (( mPatternID = arPattLoad ( mARPatternHandle, patt_name )) < 0 ) {
		DEBUG_LOG ( "Error loading pattern file %s.\n", patt_name );
		this->Stop ();
		return;
	}
	mPatternWidth = 40.0f;
	mPatternFound = FALSE;

	if ( ar2VideoCapStart ( mVideoParam ) != 0 ) {
		DEBUG_LOG ( "Error: Unable to begin camera data capture.\n" );
		this->Stop ();
		return;
	}

	//[ mDelegate videoInitialized:self :pixFormat :flipV ];
}

//================================================================//
// MOAIArVideoListener
//================================================================//
@implementation MOAIArVideoListener {
}

//----------------------------------------------------------------//
-( void )cameraVideoTookPicture:( id )sender userData:( void* )data {

	MOAIMarkerMgr::Get ().ProcessFrame ();
}

@end

#endif
