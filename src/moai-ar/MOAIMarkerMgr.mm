// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-ar/MOAIMarker.h>
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
int MOAIMarkerMgr::_getMarkerMatrix ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMarkerMgr, "NU" )

	u32 markerID			= state.GetValue < u32 >( 1, 1 ) - 1;
	MOAIMatrix* matrix		= state.GetLuaObject < MOAIMatrix >( 2, true );
	
	if ( matrix ) {
		self->GetMarkerMatrix ( markerID, *matrix );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMarkerMgr::_getMarkerPosition2D ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMarkerMgr, "N" )

	u32 markerID			= state.GetValue < u32 >( 1, 1 ) - 1;
	
	ZLVec2D position;
	if ( self->GetMarkerPosition ( markerID, position )) {
		state.Push ( position.mX );
		state.Push ( position.mY );
		return 2;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMarkerMgr::_getVideoCamera ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMarkerMgr, "" )

	if ( self->mVideoCamera ) {
		self->mVideoCamera.PushRef ( state );
		return 1;
	}
	return 0;
}

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
int MOAIMarkerMgr::_getVideoSize ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMarkerMgr, "" )
	
	state.Push ( self->mVideoWidth );
	state.Push ( self->mVideoHeight );

	return 2;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMarkerMgr::_loadPattern ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMarkerMgr, "SN" )
	
	cc8* filename		= state.GetValue < cc8* >( 1, "" );
	double width		= state.GetValue < double >( 2, 0.0 );
	
	int patternID = self->LoadPattern ( filename, width );
	
	if ( patternID >= 0 ) {
		state.Push ( patternID );
		return 1;
	}
	return 0;
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
void MOAIMarkerMgr::AffirmMarker ( ARMarkerInfo* markerInfo ) {

	int patternID = markerInfo->id;
	ZLVec2D position (( float )markerInfo->pos [ 0 ], ( float )markerInfo->pos [ 1 ]);

	float bestDist = 0.0f;
	MOAIMarker* bestMarker = 0;

	MarkerIt cursor = this->mStaleMarkers.Head ();
	for ( ; cursor; cursor = cursor->Next ()) {
	
		MOAIMarker* marker = cursor->Data ();
	
		if ( marker->mPattern->mPatternID == patternID ) {
		
			float distSquared = position.DistSqrd ( marker->mPosition );
		
			if ( !bestMarker || ( distSquared < bestDist )) {
				bestMarker = marker;
				bestDist = distSquared;
			}
		}
	}

	MOAIMarker* marker = bestMarker;

	if ( marker ) {
		
		this->mStaleMarkers.Remove ( marker->mLink );
	}
	else {
	
		if ( this->mFreeMarkers ) {
			marker = this->mFreeMarkers;
			this->mFreeMarkers = marker->mNext;
		}
		else {
			u32 idx = ( u32 )this->mMarkers.Size ();
			marker = new MOAIMarker ( idx );
			this->mMarkers.Grow ( idx + 1 );
			this->mMarkers [ idx ] = marker;
		}
	}
	
	assert ( marker );
	
	MOAIPattern& pattern = this->mPatterns [ patternID ];

	if ( marker->mPattern ) {
		arGetTransMatSquareCont ( this->mAR3DHandle, markerInfo, marker->mPatternTrans, pattern.mPatternWidth, marker->mPatternTrans );
	}
	else {
		arGetTransMatSquare ( this->mAR3DHandle, markerInfo, pattern.mPatternWidth, marker->mPatternTrans );
	}

	marker->mPattern = &pattern;
	marker->mPosition = position;
	marker->Update ();
	marker->mNext = this->mActiveMarkers;
	this->mActiveMarkers = marker;
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( marker ) {
	
		if ( !bestMarker ) {
			this->InvokeMarkerEvent ( *marker, EVENT_MARKER_BEGIN );
		}
		this->InvokeMarkerEvent ( *marker, EVENT_MARKER_UPDATE );
	}
}

//----------------------------------------------------------------//
bool MOAIMarkerMgr::GetMarkerMatrix ( u32 markerID, MOAIMatrix& matrix ) {

	if ( markerID < this->mMarkers.Size ()) {
	
		MOAIMarker* marker = this->mMarkers [ markerID ];
		matrix.Init ( marker->mTransform );
		matrix.ScheduleUpdate ();
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIMarkerMgr::GetMarkerPosition ( u32 markerID, ZLVec2D& position ) {

	if ( markerID < this->mMarkers.Size ()) {
	
		MOAIMarker* marker = this->mMarkers [ markerID ];
		
		//printf ( "marker: %g %g\n", marker->mPosition.mX, marker->mPosition.mY );
		
		// TODO: figure out right way to deal with portrait mode; integrate with Moai's screen model for this
		
		position.mX = ( this->mVideoHeight / 2.0f ) - marker->mPosition.mY;
		position.mY = ( this->mVideoWidth / 2.0f ) - marker->mPosition.mX;
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::InvokeMarkerEvent ( MOAIMarker& marker, u32 event ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	if ( this->PushListener ( event, state )) {
		state.Push ( marker.mPattern->mPatternID + 1 );
		state.Push ( marker.mMarkerID + 1 );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
int MOAIMarkerMgr::LoadPattern ( cc8* filename, double width ) {

	if (( this->mARHandle && this->mARPatternHandle ) &&  ZLFileSys::CheckFileExists ( filename )) {
	
		int patternID = arPattLoad ( this->mARPatternHandle, filename );
		
		if ( patternID < 0 ) {
			DEBUG_LOG ( "Error loading pattern file %s.\n", filename );
		}
		else {
		
			MOAIPattern& pattern = this->mPatterns [ patternID ];
			pattern.mPatternID = patternID;
			pattern.mPatternWidth = width;
			
			return patternID;
		}
	}
	return -1;
}

//----------------------------------------------------------------//
MOAIMarkerMgr::MOAIMarkerMgr () :
	mVideoPaused ( FALSE ),
	mVideoParam ( 0 ),
	mARHandle ( 0 ),
	mARPatternHandle ( 0 ),
	mAR3DHandle ( 0 ),
	mCameraParam ( 0 ),
	mVideoBuffer ( 0 ),
	mVideoWidth ( 0 ),
	mVideoHeight ( 0 ),
	mActiveMarkers ( 0 ),
	mFreeMarkers ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMarkerMgr::~MOAIMarkerMgr () {

	this->mVideoDeck.Set ( *this, 0 );
	this->mVideoShader.Set ( *this, 0 );
	this->mVideoShaderProgram.Set ( *this, 0 );
	this->mVideoPlane0.Set ( *this, 0 );
	this->mVideoPlane1.Set ( *this, 0 );
	this->mVideoCamera.Set ( *this, 0 );
	
	this->mStaleMarkers.Clear ();
	
	for ( size_t i = 0; i < this->mMarkers.Size (); ++i ) {
		delete ( this->mMarkers [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::ProcessFrame () {

	MOAIMarker*	cursor = this->mActiveMarkers;
	for ( ; cursor; cursor = cursor->mNext ) {
		this->mStaleMarkers.PushBack ( cursor->mLink );
	}
	this->mActiveMarkers = 0;

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

		// Detect the markers in the video frame.
		if ( arDetectMarker ( mARHandle, this->mVideoBuffer->buff ) < 0 ) return;
		DEBUG_LOG ( "found %d marker(s).\n", mARHandle->marker_num );

		// Check through the marker_info array for highest confidence
		// visible marker matching our preferred pattern.
		
		for ( int j = 0; j < mARHandle->marker_num; j++ ) {
		
			DEBUG_LOG ( "   pattern: %d confidence: %g\n", mARHandle->markerInfo [ j ].id, mARHandle->markerInfo [ j ].cf );
		
			ARMarkerInfo* markerInfo = &mARHandle->markerInfo [ j ];
			if ( markerInfo->cf > 0.8f ) {
				this->AffirmMarker ( markerInfo );
			}
		}
		
		this->UpdateVideoProjMtx ();
		this->mVideoCamera->SetProjMtx ( this->mProjMtx );
		this->mVideoCamera->ScheduleUpdate ();
		
		this->InvokeListener ( EVENT_UPDATE_FRAME );
	}
	
	while ( this->mStaleMarkers.Count ()) {
		MOAIMarker* marker = *this->mStaleMarkers.PopFront ();
		this->InvokeMarkerEvent ( *marker, EVENT_MARKER_END );
		marker->mNext = this->mFreeMarkers;
		this->mFreeMarkers = marker;
	}
}

//----------------------------------------------------------------//
void MOAIMarkerMgr::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );

	state.SetField ( -1, "EVENT_MARKER_BEGIN",	( u32 )EVENT_MARKER_BEGIN );
	state.SetField ( -1, "EVENT_MARKER_END",	( u32 )EVENT_MARKER_END );
	state.SetField ( -1, "EVENT_MARKER_UPDATE",	( u32 )EVENT_MARKER_UPDATE );
	state.SetField ( -1, "EVENT_UPDATE_FRAME",	( u32 )EVENT_UPDATE_FRAME );
	state.SetField ( -1, "EVENT_VIDEO_START",	( u32 )EVENT_VIDEO_START );

	luaL_Reg regTable [] = {
		{ "getMarkerMatrix",		_getMarkerMatrix },
		{ "getMarkerPosition2D",	_getMarkerPosition2D },
		{ "getListener",			&MOAIGlobalEventSource::_getListener < MOAIMarkerMgr > },
		{ "getVideoCamera",			_getVideoCamera },
		{ "getVideoDeck",			_getVideoDeck },
		{ "getVideoSize",			_getVideoSize },
		{ "loadPattern",			_loadPattern },
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
void MOAIMarkerMgr::UpdateVideoProjMtx () {

	assert ( this->mVideoCamera );
	assert ( this->mCameraParam );
	
	float near = this->mVideoCamera->GetNearPlane ();
	float far = this->mVideoCamera->GetFarPlane ();
	
	const ARParam& cparam = this->mCameraParam->param;

	float	icpara [ 3 ] [ 4];
	float	trans [ 3 ][ 4 ];
	float	p [ 3 ][ 3 ], q [ 4 ][ 4] ;
	float	widthm1, heightm1;
	int		i, j;

	widthm1  = ( float )( cparam.xsize - 1 );
	heightm1 = ( float )( cparam.ysize - 1 );

	if ( arParamDecompMatf ( cparam.mat, icpara, trans ) < 0 ) {
		printf("arglCameraFrustum(): arParamDecompMat() indicated parameter error.\n"); // Windows bug: when running multi-threaded, can't write to stderr!
		return;
	}
	
	for ( i = 0; i < 4; i++ ) {
		icpara [ 1 ][ i ] = heightm1 * ( icpara [ 2 ][ i ]) - icpara [ 1 ][ i ];
	}

	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			p [ i ][ j ] = icpara [ i ][ j ] / icpara [ 2 ][ 2 ];
		}
	}
	
	q [ 0 ][ 0 ] = ( 2.0f * p [ 0 ][ 0 ] / widthm1 );
	q [ 0 ][ 1 ] = ( 2.0f * p [ 0 ][ 1 ] / widthm1 );
	q [ 0 ][ 2 ] = -(( 2.0f * p [ 0 ][ 2 ] / widthm1 )  - 1.0f );
	q [ 0 ][ 3 ] = 0.0f;

	q [ 1 ][ 0 ] = 0.0f;
	q [ 1 ][ 1 ] = -( 2.0f * p [ 1 ][ 1 ] / heightm1 );
	q [ 1 ][ 2 ] = -(( 2.0f * p [ 1 ][ 2 ] / heightm1 ) - 1.0f );
	q [ 1 ][ 3 ] = 0.0f;

	q [ 2 ][ 0 ] = 0.0f;
	q [ 2 ][ 1 ] = 0.0f;
	q [ 2 ][ 2 ] = ( far + near ) / ( near - far );
	q [ 2 ][ 3 ] = 2.0f * far * near / ( near - far );

	q [ 3 ][ 0 ] = 0.0f;
	q [ 3 ][ 1 ] = 0.0f;
	q [ 3 ][ 2 ] = -1.0f;
	q [ 3 ][ 3 ] = 0.0f;

	ZLMatrix4x4& proj = this->mProjMtx;

	for ( i = 0; i < 4; i++ ) { // Row.
	
		// First 3 columns of the current row.
		for ( j = 0; j < 3; j++ ) { // Column.
			proj.m [ i + ( j * 4 )] =
				q [ i ][ 0 ] * trans [ 0 ][ j ] +
				q [ i ][ 1 ] * trans [ 1 ][ j ] +
				q [ i ][ 2 ] * trans [ 2 ][ j ];
		}
		// Fourth column of the current row.
		proj.m [ i + ( 3 * 4 )] =
			q [ i ][ 0 ] * trans [ 0 ][ 3 ] +
			q [ i ][ 1 ] * trans [ 1 ][ 3 ] +
			q [ i ][ 2 ] * trans [ 2 ][ 3 ] +
			q [ i ][ 3 ];
	}
	
	ZLMatrix4x4 ir90;
	
	ir90.m [ ZLMatrix4x4::C0_R0 ] = 0.0f;
	ir90.m [ ZLMatrix4x4::C0_R1 ] = -1.0f;
	ir90.m [ ZLMatrix4x4::C0_R2 ] = 0.0f;
	ir90.m [ ZLMatrix4x4::C0_R3 ] = 0.0f;
	
	ir90.m [ ZLMatrix4x4::C1_R0 ] = 1.0f;
	ir90.m [ ZLMatrix4x4::C1_R1 ] = 0.0f;
	ir90.m [ ZLMatrix4x4::C1_R2 ] = 0.0f;
	ir90.m [ ZLMatrix4x4::C1_R3 ] = 0.0f;
	
	ir90.m [ ZLMatrix4x4::C2_R0 ] = 0.0f;
	ir90.m [ ZLMatrix4x4::C2_R1 ] = 0.0f;
	ir90.m [ ZLMatrix4x4::C2_R2 ] = 1.0f;
	ir90.m [ ZLMatrix4x4::C2_R3 ] = 0.0f;
	
	ir90.m [ ZLMatrix4x4::C3_R0 ] = 0.0f;
	ir90.m [ ZLMatrix4x4::C3_R1 ] = 0.0f;
	ir90.m [ ZLMatrix4x4::C3_R2 ] = 0.0f;
	ir90.m [ ZLMatrix4x4::C3_R3 ] = 1.0f;
	
	proj.Append ( ir90 );
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

	if ( ar2VideoCapStart ( mVideoParam ) != 0 ) {
		DEBUG_LOG ( "Error: Unable to begin camera data capture.\n" );
		this->Stop ();
		return;
	}

	//this->mMarker.Set ( *this, new MOAIMatrix ());

	this->mVideoCamera.Set ( *this, new MOAICamera ());
	this->UpdateVideoProjMtx ();
	
	//GLfloat frustum [ 16 ];
	//arglCameraFrustumRHf ( &this->mCameraParam->param, 1.0f, 5000.0f, frustum );
    //[ glView setCameraLens:frustum ];
    //glView.contentFlipV = flipV;

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
