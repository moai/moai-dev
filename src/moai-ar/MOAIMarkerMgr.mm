// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-ar/MOAIMarkerMgr.h>
#include <moai-ar/MOAIVideoTexture.h>

#include <moai-sim/shaders/MOAIDeck2DShader-fsh.h>
#include <moai-sim/shaders/MOAIDeck2DShader-vsh.h>

#include <moai-ar/shaders/MOAIVideoShaderYCbCrITURec601FullRangeBiPlanar-fsh.h>
#include <moai-ar/shaders/MOAIVideoShaderYCbCrITURec601FullRangeBiPlanar-vsh.h>
#include <moai-ar/shaders/MOAIVideoShaderYCbCrITURec601VideoRangeBiPlanar-fsh.h>
#include <moai-ar/shaders/MOAIVideoShaderYCbCrITURec601VideoRangeBiPlanar-vsh.h>
#include <moai-ar/shaders/MOAIVideoShaderYCrCbITURec601FullRangeBiPlanar-fsh.h>
#include <moai-ar/shaders/MOAIVideoShaderYCrCbITURec601FullRangeBiPlanar-vsh.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_WITH_ARTOOLKIT

// uncomment me to debug log
//#define MOAIMARKERMGR_DEBUG

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
int MOAIMarkerMgr::_getVideoDeck ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMarkerMgr, "" )
	
	self->AffirmDeck ();
	self->mVideoDeck.PushRef ( state );
	return 1;
}

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
void MOAIMarkerMgr::AffirmDeck () {

	if (( this->mVideoPixelFormat != AR_PIXEL_FORMAT_INVALID ) && !this->mVideoDeck ) {

		cc8* fsh = _deck2DShaderFSH;
		cc8* vsh = _deck2DShaderVSH;
		
		switch ( this->mVideoPixelFormat ) {
		
			case AR_PIXEL_FORMAT_420f:
				fsh = _videoShaderYCbCrITURec601FullRangeBiPlanarFSH;
				vsh = _videoShaderYCbCrITURec601FullRangeBiPlanarVSH;
				break;
			
			case AR_PIXEL_FORMAT_420v:
				fsh = _videoShaderYCbCrITURec601VideoRangeBiPlanarFSH;
				vsh = _videoShaderYCbCrITURec601VideoRangeBiPlanarVSH;
				break;
			
			case AR_PIXEL_FORMAT_NV21:
				fsh = _videoShaderYCrCbITURec601FullRangeBiPlanarFSH;
				vsh = _videoShaderYCrCbITURec601FullRangeBiPlanarVSH;
				break;
				
			default:
				break;
		}
		
		MOAIVideoTexture* texture0 = new MOAIVideoTexture ();
		MOAIVideoTexture* texture1 = new MOAIVideoTexture ();
		
		texture0->Init ( MOAIVideoTexture::VIDEO_TEXTURE_PLANE_0, this->mVideoPixelFormat );
		texture1->Init ( MOAIVideoTexture::VIDEO_TEXTURE_PLANE_1, this->mVideoPixelFormat );

		MOAIShaderProgram* program = new MOAIShaderProgram ();
		program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
		program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
		program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
		program->Load ( vsh, fsh );

		program->ReserveUniforms ( 2 );
		program->DeclareUniform ( 0, "texture0", MOAIShaderUniformFormatter::UNIFORM_TYPE_INT, 1, 1 );
		program->DeclareUniform ( 1, "texture1", MOAIShaderUniformFormatter::UNIFORM_TYPE_INT, 1, 1 );

		program->ReserveTextures ( 2 );
		program->SetTexture ( 0, texture0, 0 );
		program->SetTexture ( 1, texture1, 1 );

		MOAIShader* shader = new MOAIShader ();
		shader->SetProgram ( program );
		
		shader->Bless ();
		shader->SetUniform ( 0, 0 );
		shader->SetUniform ( 1, 1 );
		
		int halfWidth = ( int )this->mVideoWidth / 2;
		int halfHeight = ( int )this->mVideoHeight / 2;
		
		ZLRect frame;
		frame.Init ( -halfWidth, -halfHeight, halfWidth, halfHeight );
		
		MOAISpriteDeck2D* deck = new MOAISpriteDeck2D ();
		deck->ReserveQuads ( 1 );
		deck->SetRect ( 0, frame );
		deck->AffirmMaterialBatch ()->Reserve ( 1 );
		
		MOAIMaterial* material = deck->GetMaterial ( 0 );
		material->SetShader ( shader );
		
		this->mVideoDeck.Set ( *this, deck );
		this->mVideoShaderProgram.Set ( *this, program );
		this->mVideoShader.Set ( *this, shader );
		this->mVideoPlane0.Set ( *this, texture0 );
		this->mVideoPlane1.Set ( *this, texture1 );
	}
}

//----------------------------------------------------------------//
MOAIMarkerMgr::MOAIMarkerMgr () :
	mVideoPaused ( FALSE ),
	mVideoParam ( 0 ),
	mARHandle ( 0 ),
	mARPatternHandle ( 0 ),
	mAR3DHandle ( 0 ),
	mUseContPoseEstimation ( FALSE ),
	mCameraParam ( 0 ),
	mVideoBuffer ( 0 ),
	mVideoWidth ( 0 ),
	mVideoHeight ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMarkerMgr::~MOAIMarkerMgr () {

	this->mVideoDeck.Set ( *this, 0 );
	this->mVideoShader.Set ( *this, 0 );
	this->mVideoShaderProgram.Set ( *this, 0 );
	this->mVideoPlane0.Set ( *this, 0 );
	this->mVideoPlane1.Set ( *this, 0 );
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

	//this->FinishInit ();
	//this->ScheduleForGPUUpdate ();

	this->mVideoBuffer = ar2VideoGetImage ( mVideoParam );

	if ( this->mVideoBuffer ) {

		if ( this->mVideoBuffer->bufPlaneCount == 2 ) {
			if ( this->mVideoPlane0 ) {
				this->mVideoPlane0->UpdateBuffer ( this->mVideoBuffer->bufPlanes [ 0 ]);
			}
			if ( this->mVideoPlane1 ) {
				this->mVideoPlane1->UpdateBuffer ( this->mVideoBuffer->bufPlanes [ 1 ]);
			}
		}
		else {
			this->mVideoPlane0->UpdateBuffer ( this->mVideoBuffer->buff );
		}

		ARdouble err;
		int j, k;

		// Detect the markers in the video frame.
		if ( arDetectMarker ( mARHandle, this->mVideoBuffer->buff ) < 0 ) return;
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
		
		this->InvokeListener ( EVENT_UPDATE_FRAME );
	}
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );

	state.SetField ( -1, "EVENT_UPDATE_FRAME",	( u32 )EVENT_UPDATE_FRAME );
	state.SetField ( -1, "EVENT_VIDEO_START",	( u32 )EVENT_VIDEO_START );

	luaL_Reg regTable [] = {
		{ "getListener",			&MOAIGlobalEventSource::_getListener < MOAIMarkerMgr > },
		{ "getVideoDeck",			_getVideoDeck },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAIMarkerMgr > },
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

	// Get the format in which the camera is returning pixels.
	this->mVideoPixelFormat = ar2VideoGetPixelFormat ( mVideoParam );
	if ( this->mVideoPixelFormat == AR_PIXEL_FORMAT_INVALID ) {
		DEBUG_LOG ( "Error: Camera is using unsupported pixel format.\n" );
		//[ self stop ] ;
		return;
	}

	// Find the size of the window.
	int xsize, ysize;
	if ( ar2VideoGetSize ( mVideoParam, &xsize, &ysize ) < 0 ) {
		DEBUG_LOG ( "Error: ar2VideoGetSize.\n" );
		this->Stop ();
		return;
	}
	
	this->mVideoWidth = ( u32 )xsize;
	this->mVideoHeight = ( u32 )ysize;

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

	if ( arSetPixelFormat ( mARHandle, this->mVideoPixelFormat ) < 0 ) {
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
	[ cameraVideo setTookPictureDelegate:[[ MOAIArVideoListener alloc ] init ]];

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

	this->InvokeListener ( EVENT_VIDEO_START );
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
