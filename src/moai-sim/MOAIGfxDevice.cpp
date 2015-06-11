// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getFrameBuffer
	@text	Returns the frame buffer associated with the device.

	@out	MOAIFrameBuffer frameBuffer
*/
int MOAIGfxDevice::_getFrameBuffer ( lua_State* L ) {

	MOAILuaState state ( L );
	state.Push ( MOAIGfxDevice::Get ().GetDefaultBuffer ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureUnits
	@text	Returns the total number of texture units available on the device.

	@out	number maxTextureUnits
*/
int MOAIGfxDevice::_getMaxTextureUnits ( lua_State* L ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	lua_pushnumber ( L, gfxDevice.mTextureUnits.Size ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getViewSize
	@text	Returns the width and height of the view
	
	@out	number width
	@out	number height
*/
int MOAIGfxDevice::_getViewSize ( lua_State* L  ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	lua_pushnumber ( L, gfxDevice.GetWidth ());
	lua_pushnumber ( L, gfxDevice.GetHeight ());
	
	return 2;
}


//----------------------------------------------------------------//
/**	@lua	isProgrammable
	@text	Returns a boolean indicating whether or not Moai is running
			under the programmable pipeline.

	@out	boolean isProgrammable
*/
int MOAIGfxDevice::_isProgrammable ( lua_State* L ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	lua_pushboolean ( L, gfxDevice.IsProgrammable ());
	
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxDevice::_setDefaultTexture ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAIGfxDevice& device = MOAIGfxDevice::Get ();

	MOAITexture* texture = state.GetLuaObject < MOAITexture >( 1, false );
	
	if ( !texture ) {
		texture = new MOAITexture ();
		if ( !texture->Init ( state, 1 )) {
			// TODO: report error
			delete texture;
			texture = 0;
		}
	}

	device.mDefaultTexture.Set ( device, texture );

	if ( texture ) {
		texture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenColor

	@in		number r
	@in		number g
	@in		number b
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIGfxDevice::_setPenColor ( lua_State* L ) {

	MOAILuaState state ( L );

	float r = state.GetValue < float >( 1, 1.0f );
	float g = state.GetValue < float >( 2, 1.0f );
	float b = state.GetValue < float >( 3, 1.0f );
	float a = state.GetValue < float >( 4, 1.0f );

	MOAIGfxDevice::Get ().SetPenColor ( r, g, b, a );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenWidth

	@in		number width
	@out	nil
*/
int MOAIGfxDevice::_setPenWidth ( lua_State* L ) {

	MOAILuaState state ( L );

	float width = state.GetValue < float >( 1, 1.0f );
	MOAIGfxDevice::Get ().SetPenWidth ( width );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPointSize

	@in		number size
	@out	nil
*/
int MOAIGfxDevice::_setPointSize ( lua_State* L ) {
	MOAILuaState state ( L );

	float size = state.GetValue < float >( 1, 1.0f );
	MOAIGfxDevice::Get ().SetPointSize ( size );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	release

	@in		release textures scheduled to be released
	@out	nil
 */
// TODO: rename this to something more descriptive?
int MOAIGfxDevice::_release ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//================================================================//
// MOAIGfxDevice
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDevice::BeginPrim () {

	if ( this->mPrimSize ) {

		u32 primBytes = this->mPrimSize * this->mVertexFormat->GetVertexSize ();

		this->mMaxPrims = ( u32 )( this->mSize / primBytes );
		this->mPrimTop = this->mTop + primBytes;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::BeginPrim ( u32 primType ) {

	this->SetPrimType ( primType );
	this->BeginPrim ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::Clear () {

	this->mDefaultTexture.Set ( *this, 0 );

	if ( this->mBuffer ) {
		free ( this->mBuffer );
		this->mBuffer = 0;
		this->mSize = 0;
		this->mTop = 0;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ClearErrors () {
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			while ( zglGetError () != ZGL_ERROR_NONE );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ClearSurface ( u32 clearFlags ) {

	if ( clearFlags ) {
		if (( clearFlags & ZGL_CLEAR_DEPTH_BUFFER_BIT ) && !this->mDepthMask ) {
			zglDepthMask ( true );
			zglClear ( clearFlags );
			zglDepthMask ( false );
		}
		else {
			zglClear ( clearFlags );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DetectContext () {

	this->mHasContext = true;
	
	zglBegin ();
	
	zglInitialize ();
	
	this->mIsProgrammable = zglGetCap ( ZGL_CAPS_IS_PROGRAMMABLE ) == 1;
	this->mIsFramebufferSupported = zglGetCap ( ZGL_CAPS_IS_FRAMEBUFFER_SUPPORTED ) == 1;
	
	u32 maxTextureUnits = zglGetCap ( ZGL_CAPS_MAX_TEXTURE_UNITS );
	this->mTextureUnits.Init ( maxTextureUnits );
	this->mTextureUnits.Fill ( 0 );
	
	this->mMaxTextureSize = zglGetCap ( ZGL_CAPS_MAX_TEXTURE_SIZE );

	MOAIGfxResourceMgr::Get ().RenewResources ();
	
	this->mDefaultBuffer->DetectGLFrameBufferID ();
	
	zglEnd ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DetectFramebuffer () {
	
	zglBegin ();
	
	this->mDefaultBuffer->DetectGLFrameBufferID ();
	
	zglEnd ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DisableTextureUnits ( u32 activeTextures ) {

	if ( activeTextures < this->mActiveTextures ) {
		
		this->Flush ();
	
		for ( u32 i = activeTextures; i < this->mActiveTextures; ++i ) {
			
			#if USE_OPENGLES1
				if ( !this->IsProgrammable ()) {
					zglActiveTexture ( i );
					zglDisable ( ZGL_PIPELINE_TEXTURE_2D );
				}
			#endif
			
			this->mTextureUnits [ i ] = 0;
		}
	}
	
	this->mActiveTextures = activeTextures;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DrawPrims () {

	if ( this->mVertexFormat ) {

		u32 vertexSize = this->mVertexFormat->GetVertexSize ();

		if ( vertexSize ) {
			u32 count = this->mPrimSize ? this->mPrimCount * this->mPrimSize : ( u32 )( this->mTop / vertexSize );
			if ( count > 0 ) {
				this->UpdateShaderGlobals ();
				zglDrawArrays ( this->mPrimType, 0, count );
				this->mDrawCount++;
			}
		}
	}
}

//----------------------------------------------------------------//
//void MOAIGfxDevice::DrawPrims ( const MOAIVertexFormat& format, GLenum primType, void* buffer, void* indices, u32 size ) {
//
//	if ( !( buffer && size )) return;
//
//	this->SetVertexFormat ();
//	
//	// draw the prims
//	u32 nVerts = ( u32 )( size / format.GetVertexSize ());
//	if ( nVerts ) {
//		
//		format.Bind ( buffer );
//
//		if ( indices ) {
//			glDrawElements ( primType, nVerts, GL_UNSIGNED_SHORT, indices );
//		}
//		else {
//			glDrawArrays ( primType, 0, nVerts );
//		}
//		format.Unbind ();
//	}
//}

//----------------------------------------------------------------//
void MOAIGfxDevice::EndPrim () {

	if ( this->mPrimSize ) {
		this->mTop = this->mPrimTop;
	}
	++this->mPrimCount;
	
	if (( this->mPrimSize == 0 ) || ( this->mPrimCount >= this->mMaxPrims )) {
		this->Flush ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::Flush () {

	this->DrawPrims ();

	this->mTop = 0;
	this->mPrimTop = 0;
	this->mPrimCount = 0;
}

//----------------------------------------------------------------//
float MOAIGfxDevice::GetDeviceScale () {

	return this->mFrameBuffer->mBufferScale;
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetHeight () const {

	return this->mFrameBuffer->mBufferHeight;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDevice::GetNormToWndMtx () const {

	ZLRect rect = this->mViewRect;

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Wnd
	ZLMatrix4x4 normToWnd;
	normToWnd.Scale ( hWidth, -hHeight, 1.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Translate ( hWidth + rect.mXMin, hHeight + rect.mYMin, 0.0f );
	normToWnd.Append ( mtx );
	
	return normToWnd;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::GetUVMtxMode ( u32& input, u32& output ) const {

	input = this->mUVMtxInput;
	output = this->mUVMtxOutput;
}

//----------------------------------------------------------------//
const ZLMatrix4x4& MOAIGfxDevice::GetUVTransform () const {

	return this->mUVTransform;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::GetVertexMtxMode ( u32& input, u32& output ) const {

	input = this->mVertexMtxInput;
	output = this->mVertexMtxOutput;
}

//----------------------------------------------------------------//
const ZLMatrix4x4& MOAIGfxDevice::GetVertexTransform ( u32 id ) const {

	return this->mVertexTransforms [ id ];
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDevice::GetViewProjMtx () const {

	ZLMatrix4x4 mtx = this->mVertexTransforms [ VTX_VIEW_TRANSFORM ];
	mtx.Append ( this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]);
	return mtx;
}

//----------------------------------------------------------------//
//ZLQuad MOAIGfxDevice::GetViewQuad () const {
//
//	ZLQuad quad;
//
//	ZLMatrix4x4 invMtx;
//	invMtx.Inverse ( this->GetViewProjMtx ());
//	
//	quad.mV [ 0 ].Init ( -1.0f, 1.0f );
//	quad.mV [ 1 ].Init ( 1.0f, 1.0f );
//	quad.mV [ 2 ].Init ( 1.0f, -1.0f );
//	quad.mV [ 3 ].Init ( -1.0f, -1.0f );
//	
//	invMtx.TransformQuad ( quad.mV );
//	return quad;
//}

//----------------------------------------------------------------//
//ZLRect MOAIGfxDevice::GetViewRect () const {
//
//	return this->mViewRect;
//}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetWidth () const {

	return this->mFrameBuffer->mBufferWidth;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDevice::GetWorldToWndMtx () const {

	ZLMatrix4x4 worldToWnd = this->GetViewProjMtx ();
	worldToWnd.Append ( MOAIGfxDevice::GetNormToWndMtx ());
	
	return worldToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDevice::GetWndToNormMtx () const {

	ZLRect rect = this->mViewRect;

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Inv Wnd
	ZLMatrix4x4 wndToNorm;
	wndToNorm.Translate ( -hWidth - rect.mXMin, -hHeight - rect.mYMin, 0.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ), 1.0f );
	wndToNorm.Append ( mtx );
	
	return wndToNorm;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDevice::GetWndToWorldMtx () const {

	ZLMatrix4x4 wndToWorld = MOAIGfxDevice::GetWndToNormMtx ();
	
	// inv viewproj
	ZLMatrix4x4 mtx = this->GetViewProjMtx ();
	mtx.Inverse ();
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::GpuLoadMatrix ( const ZLMatrix4x4& mtx ) const {
	UNUSED ( mtx );
	#if USE_OPENGLES1
		zglLoadMatrix ( mtx.m );
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::GpuMultMatrix ( const ZLMatrix4x4& mtx ) const {
	UNUSED ( mtx );
	
	#if USE_OPENGLES1
		zglMultMatrix ( mtx.m );
	#endif
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::IsOpaque () const {
	
	assert ( this->mDefaultBuffer );
	return this->mDefaultBuffer->IsOpaque ();
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::LogErrors () {

	u32 count = 0;
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			for ( u32 error = zglGetError (); error != ZGL_ERROR_NONE; error = zglGetError (), ++count ) {
				MOAILog ( 0, MOAILogMessages::MOAIGfxDevice_OpenGLError_S, zglGetErrorString ( error ));
			}
		}
	#endif
	return count;
}

//----------------------------------------------------------------//
MOAIGfxDevice::MOAIGfxDevice () :
	mCullFunc ( 0 ),
	mDepthFunc ( 0 ),
	mDepthMask ( true ),
	mBlendEnabled ( 0 ),
	mBuffer ( 0 ),
	mCpuVertexTransform ( false ),
	mCpuUVTransform ( false ),
	mHasContext ( false ),
	mIsFramebufferSupported ( 0 ),
	#if defined ( MOAI_OS_NACL ) || defined ( MOAI_OS_IPHONE ) || defined ( MOAI_OS_ANDROID ) || defined ( EMSCRIPTEN )
		mIsOpenGLES ( true ),
	#else
		mIsOpenGLES ( false ),
	#endif
	mIsProgrammable ( false ),
	mMajorVersion ( 0 ),
	mMaxPrims ( 0 ),
	mMinorVersion ( 0 ),
	mFinalColor32 ( 0xffffffff ),
	mPenWidth ( 1.0f ),
	mPointSize ( 1.0f ),
	mPrimCount ( 0 ),
	mPrimSize ( 0 ),
	mPrimTop ( 0 ),
	mPrimType ( 0xffffffff ),
	mShaderProgram ( 0 ),
	mShaderDirty ( false ),
	mSize ( 0 ),
	mActiveTextures ( 0 ),
	mTextureMemoryUsage ( 0 ),
	mMaxTextureSize ( 0 ),
	mTop ( 0 ),
	mUVMtxInput ( UV_STAGE_MODEL ),
	mUVMtxOutput ( UV_STAGE_MODEL ),
	mVertexFormat ( 0 ),
	mVertexFormatBuffer ( 0 ),
	mVertexMtxInput ( VTX_STAGE_MODEL ),
	mVertexMtxOutput ( VTX_STAGE_MODEL ) {
	
	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	this->Reserve ( DEFAULT_BUFFER_SIZE );
	
	for ( u32 i = 0; i < TOTAL_VTX_TRANSFORMS; ++i ) {
		this->mVertexTransforms [ i ].Ident ();
		this->mCpuVertexTransformCache [ i ] = false;
	}
	this->mUVTransform.Ident ();
	this->mCpuVertexTransformMtx.Ident ();
	
	this->mAmbientColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mFinalColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mViewRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mScissorRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mDefaultBuffer.Set ( *this, new MOAIFrameBuffer ());
	this->mFrameBuffer = this->mDefaultBuffer;
}

//----------------------------------------------------------------//
MOAIGfxDevice::~MOAIGfxDevice () {

	this->mDefaultBuffer.Set ( *this, 0 );
	
	// this->ProcessDeleters (); // TODO: same issue as OnGlobalsFinalize
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::OnGlobalsFinalize () {

	// TODO: do we care about releasing resources on shutdown?
	// commented out for now.
	//this->ReleaseResources ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "LOADING_POLICY_CPU_GPU_ASAP",			( u32 )MOAIGfxResource::LOADING_POLICY_CPU_GPU_ASAP );
	state.SetField ( -1, "LOADING_POLICY_CPU_ASAP_GPU_NEXT",	( u32 )MOAIGfxResource::LOADING_POLICY_CPU_ASAP_GPU_NEXT );
	state.SetField ( -1, "LOADING_POLICY_CPU_ASAP_GPU_BIND",	( u32 )MOAIGfxResource::LOADING_POLICY_CPU_ASAP_GPU_BIND );
	state.SetField ( -1, "LOADING_POLICY_CPU_GPU_BIND",			( u32 )MOAIGfxResource::LOADING_POLICY_CPU_GPU_BIND );

	state.SetField ( -1, "EVENT_RESIZE", ( u32 )EVENT_RESIZE );

	luaL_Reg regTable [] = {
		{ "getFrameBuffer",				_getFrameBuffer },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIGfxDevice > },
		{ "getMaxTextureUnits",			_getMaxTextureUnits },
		{ "getViewSize",				_getViewSize },
		{ "isProgrammable",				_isProgrammable },
		{ "setDefaultTexture",			_setDefaultTexture },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIGfxDevice > },
		{ "setPenColor",				_setPenColor },
		{ "setPenWidth",				_setPenWidth },
		{ "setPointSize",				_setPointSize },
		{ "release",					_release },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ReportTextureAlloc ( cc8* name, size_t size ) {

	this->mTextureMemoryUsage += size;
	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
	MOAILog ( 0, MOAILogMessages::MOAITexture_MemoryUse_SDFS, "+", size, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ReportTextureFree ( cc8* name, size_t size ) {

	this->mTextureMemoryUsage -= size;
	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
	MOAILog ( 0, MOAILogMessages::MOAITexture_MemoryUse_SDFS, "-", size, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::Reserve ( u32 size ) {

	this->mSize = size;
	this->mTop = 0;
	this->mBuffer = malloc ( size );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ResetDrawCount () {
	this->mDrawCount = 0;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ResetState () {

	for ( u32 i = 0; i < TOTAL_VTX_TRANSFORMS; ++i ) {
		this->mVertexTransforms [ i ].Ident ();
	}
	this->mUVTransform.Ident ();
	this->mCpuVertexTransformMtx.Ident ();
	
	this->mVertexMtxInput = VTX_STAGE_MODEL;
	this->mVertexMtxOutput = VTX_STAGE_MODEL;

	this->mTop = 0;
	this->mPrimCount = 0;

	// turn off texture
	#if USE_OPENGLES1
		if ( !this->IsProgrammable ()) {	
			zglDisable ( ZGL_PIPELINE_TEXTURE_2D );	
		}
	#endif
	this->mTextureUnits [ 0 ] = 0;
	
	// turn off blending
	zglDisable ( ZGL_PIPELINE_BLEND );
	this->mBlendEnabled = false;
	
	// disable backface culling
	zglDisable ( ZGL_PIPELINE_CULL );
	this->mCullFunc = 0;
	
	// disable depth test
	zglDisable ( ZGL_PIPELINE_DEPTH );
	this->mDepthFunc = 0;
	
	// disable depth write
	zglDepthMask ( false );
	this->mDepthMask = false;
	
	// clear the vertex format
	this->SetVertexFormat ();

	// clear the shader
	this->mShaderProgram = 0;
	
	// reset the pen width
	this->mPenWidth = 1.0f;
	zglLineWidth ( this->mPenWidth );
	
	// reset the point size
	this->mPointSize = 1.0f;
	
	// reset the scissor rect
	ZLRect scissorRect = this->mFrameBuffer->GetBufferRect ();
	zglScissor (( s32 )scissorRect.mXMin, ( s32 )scissorRect.mYMin, ( u32 )scissorRect.Width (), ( u32 )scissorRect.Height ());
	
	this->mScissorRect = scissorRect;
	
	// fixed function reset
	#if USE_OPENGLES1
		if ( !this->IsProgrammable ()) {
			
			// load identity matrix
			zglMatrixMode ( ZGL_MATRIX_MODELVIEW );
			zglLoadIdentity ();
			
			zglMatrixMode ( ZGL_MATRIX_PROJECTION );
			zglLoadIdentity ();
			
			zglMatrixMode ( ZGL_MATRIX_TEXTURE );
			zglLoadIdentity ();
			
			// reset the current vertex color
			zglColor ( 1.0f, 1.0f, 1.0f, 1.0f );
			
			// reset the point size
			zglPointSize ( this->mPointSize );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetAmbientColor ( u32 color ) {

	this->mAmbientColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetAmbientColor ( const ZLColorVec& colorVec ) {

	this->mAmbientColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetAmbientColor ( float r, float g, float b, float a ) {

	this->mAmbientColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode () {

	if ( this->mBlendEnabled ) {
		this->Flush ();
		zglDisable ( ZGL_PIPELINE_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	if ( !this->mBlendEnabled ) {
		this->Flush ();
		zglEnable ( ZGL_PIPELINE_BLEND );
		this->mBlendMode = blendMode;
		zglBlendMode(this->mBlendMode.mEquation);
		zglBlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
		this->mBlendEnabled = true;
	}
	else if ( !this->mBlendMode.IsSame ( blendMode )) {
		this->Flush ();
		this->mBlendMode = blendMode;
		zglBlendMode(this->mBlendMode.mEquation);
		zglBlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode ( int srcFactor, int dstFactor, int equation ) {

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( srcFactor, dstFactor );
	blendMode.SetBlendEquation( equation );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBufferScale ( float scale ) {

	this->mDefaultBuffer->SetBufferScale ( scale );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBufferSize ( u32 width, u32 height ) {

	this->mDefaultBuffer->SetBufferSize ( width, height );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetCullFunc () {

	this->SetCullFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetCullFunc ( int cullFunc ) {

	if ( this->mCullFunc != cullFunc ) {
	
		this->Flush ();
		this->mCullFunc = cullFunc;
	
		if ( cullFunc ) {
			zglEnable ( ZGL_PIPELINE_CULL );
			zglCullFace ( this->mCullFunc );
		}
		else {
			zglDisable ( ZGL_PIPELINE_CULL );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetDepthFunc () {

	this->SetDepthFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetDepthFunc ( int depthFunc ) {

	if ( this->mDepthFunc != depthFunc ) {
	
		this->Flush ();
		this->mDepthFunc = depthFunc;
	
		if ( depthFunc ) {
			zglEnable ( ZGL_PIPELINE_DEPTH );
			zglDepthFunc ( this->mDepthFunc );
		}
		else {
			zglDisable ( ZGL_PIPELINE_DEPTH );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetDepthMask ( bool depthMask ) {

	if ( this->mDepthMask != depthMask ) {
		this->Flush ();
		this->mDepthMask = depthMask;
		zglDepthMask ( this->mDepthMask );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	this->Flush ();

	if ( this->mIsFramebufferSupported ) {
		if ( frameBuffer ) {
			zglBindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, frameBuffer->mGLFrameBufferID );
			this->mFrameBuffer = frameBuffer;
		}
		else {
			zglBindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, this->mDefaultBuffer->mGLFrameBufferID );
			this->mFrameBuffer = this->mDefaultBuffer;
		}	
	}
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetGfxState ( MOAIGfxState* gfxState ) {

	if ( gfxState ) {
		return gfxState->LoadGfxState ();
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPenColor ( u32 color ) {

	this->mPenColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPenColor ( const ZLColorVec& colorVec ) {

	this->mPenColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPenColor ( float r, float g, float b, float a ) {

	this->mPenColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPenWidth ( float penWidth ) {

	if ( this->mPenWidth != penWidth ) {
		this->Flush ();
		this->mPenWidth = penWidth;
		zglLineWidth ( penWidth );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPointSize ( float pointSize ) {
	UNUSED ( pointSize );

	#if USE_OPENGLES1
		if ( this->mPointSize != pointSize ) {
			this->Flush ();
			this->mPointSize = pointSize;
			zglPointSize ( pointSize );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPrimType ( u32 primType ) {

	if ( this->mPrimType != primType ) {

		this->Flush ();
		this->mPrimType = primType;

		switch ( primType ) {
			
			case ZGL_PRIM_LINES:
				this->mPrimSize = 2;
				break;
			
			case ZGL_PRIM_TRIANGLES:
				this->mPrimSize = 3;
				break;
			
			case ZGL_PRIM_POINTS:
			case ZGL_PRIM_LINE_LOOP:
			case ZGL_PRIM_LINE_STRIP:
			case ZGL_PRIM_TRIANGLE_FAN:
			case ZGL_PRIM_TRIANGLE_STRIP:
			default:
				this->mPrimSize = 0;
				break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetScissorRect () {

	this->SetScissorRect ( this->mFrameBuffer->GetBufferRect ());
	zglDisable ( ZGL_PIPELINE_SCISSOR );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetScissorRect ( ZLRect rect ) {
	
	rect.Bless ();
	this->mViewRect.Clip ( rect );

	ZLRect& current = this->mScissorRect;
	
	if (	( current.mXMin != rect.mXMin ) ||
			( current.mYMin != rect.mYMin ) ||
			( current.mXMax != rect.mXMax ) ||
			( current.mYMax != rect.mYMax )) {
		
		this->Flush ();

		ZLRect deviceRect = this->mFrameBuffer->WndRectToDevice ( rect );

		s32 x = ( s32 )deviceRect.mXMin;
		s32 y = ( s32 )deviceRect.mYMin;
		
		u32 w = ( u32 )( deviceRect.Width () + 0.5f );
		u32 h = ( u32 )( deviceRect.Height () + 0.5f );

		w = h == 0 ? 0 : w;
		h = w == 0 ? 0 : h;
		
		zglScissor ( x, y, w, h );
		this->mScissorRect = rect;
	
		zglEnable ( ZGL_PIPELINE_SCISSOR );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetScreenSpace ( MOAIViewport& viewport ) {
	UNUSED ( viewport );

	// TODO

	//glMatrixMode ( GL_MODELVIEW );
	//glLoadIdentity ();
	//
	//ZLMatrix4x4 wndToNorm;
	//viewport.GetWndToNormMtx ( wndToNorm );
	//
	//glMatrixMode ( GL_PROJECTION );
	//MOAIGfxDevice::LoadMatrix ( wndToNorm );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetShader ( MOAIShader* shader ) {

	if ( shader ) {
		this->SetShaderProgram ( shader->GetProgram ());
		shader->BindUniforms ();
	}
	else {
		this->SetShaderProgram ( 0 );
	}	
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetShaderPreset ( u32 preset ) {

	MOAIShaderMgr::Get ().BindShader ( preset );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetShaderProgram ( MOAIShaderProgram* program ) {

	if (( this->mShaderProgram != program ) && this->mIsProgrammable ) {
	
		this->Flush ();
		this->mShaderProgram = program;
		
		if ( program ) {
			program->Bind ();
		}
	}
	this->mShaderDirty = true;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture () {
	
	this->DisableTextureUnits ( 0 );
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture ( MOAITextureBase* texture ) {
	
	if ( !texture ) {
		return this->SetTexture ();
	}
	
	this->DisableTextureUnits ( 1 );
	return this->SetTexture ( 0, texture );
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture ( MOAIMultiTexture* multi ) {

	if ( !multi ) {
		return this->SetTexture ();
	}
	
	u32 total = 0;
	u32 multiSize = multi->mTextures.Size ();
	for ( ; total < multiSize; ++total ) {
		if ( !multi->mTextures [ total ]) break;
	}
	
	if ( total > this->mTextureUnits.Size ()) {
		total = this->mTextureUnits.Size ();
	}
	
	// disable any unused textures
	this->DisableTextureUnits ( total );
	
	for ( u32 i = 0; i < total; ++i ) {
		this->SetTexture ( i, multi->mTextures [ i ]);
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture ( u32 textureUnit, MOAITextureBase* texture ) {
	
	if ( !texture ) {
		// TODO: is this right?
		this->mTextureUnits [ textureUnit ] = 0;
		return false;
	}
	
	if ( this->mTextureUnits [ textureUnit ] == texture ) return true;
	
	this->Flush ();
	
	zglActiveTexture ( textureUnit );
	
	#if USE_OPENGLES1
		if (( !this->mTextureUnits [ textureUnit ]) && ( !this->IsProgrammable ())) {
			zglEnable ( ZGL_PIPELINE_TEXTURE_2D );
		}
	#endif
	
	if ( !texture->Bind ()) {
		return this->SetTexture ( textureUnit, this->mDefaultTexture );
	}
	
	this->mTextureUnits [ textureUnit ] = texture;
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetUVMtxMode ( u32 input, u32 output ) {

	if (( this->mUVMtxInput != input ) || ( this->mUVMtxOutput != output )) {
		
		this->mUVMtxInput = input;
		this->mUVMtxOutput = output;
		
		this->UpdateUVMtx ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetUVTransform () {

	ZLMatrix4x4 mtx;
	mtx.Ident ();
	this->SetUVTransform ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetUVTransform ( const ZLAffine3D& transform ) {

	ZLMatrix4x4 mtx;
	mtx.Init ( transform );
	this->SetUVTransform ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetUVTransform ( const ZLMatrix4x4& transform ) {

	if ( !this->mUVTransform.IsSame ( transform )) {
		this->mUVTransform = transform;
		this->UpdateUVMtx ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexFormat () {

	this->Flush ();
	
	if ( this->mVertexFormat ) {
		this->mVertexFormat->Unbind ();
	}
	this->mVertexFormat = 0;
	this->mVertexFormatBuffer = 0;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexFormat ( const MOAIVertexFormat& format ) {

	this->SetVertexFormat ( format, this->mBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexFormat ( const MOAIVertexFormat& format, void* buffer ) {

	if (( this->mVertexFormat != &format ) || ( this->mVertexFormatBuffer != buffer )) {

		this->SetVertexFormat ();
		this->mVertexFormat = &format;
		this->mVertexFormat->Bind ( buffer );
		this->mVertexFormatBuffer = buffer;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexMtxMode ( u32 input, u32 output ) {
	
	if (( this->mVertexMtxInput != input ) || ( this->mVertexMtxOutput != output )) {

		this->mVertexMtxInput = input;
		this->mVertexMtxOutput = output;
		
		// Invalidate the lower level matrices (i.e. modelview, etc) matrix in this case to force recalc
		for ( u32 i = input; i < output; ++i ) {
			this->mCpuVertexTransformCache [ i ] = false;
		}
		
		this->UpdateCpuVertexMtx ();
		this->UpdateGpuVertexMtx ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexPreset ( u32 preset ) {

	MOAIVertexFormat* format = MOAIVertexFormatMgr::Get ().GetFormat ( preset );
	if ( format ) {
		this->SetVertexFormat ( *format );
	}
	else {
		this->SetVertexFormat ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexTransform ( u32 id ) {

	ZLMatrix4x4 mtx;
	mtx.Ident ();
	this->SetVertexTransform ( id, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexTransform ( u32 id, const ZLAffine3D& transform ) {

	ZLMatrix4x4 mtx;
	mtx.Init ( transform );
	this->SetVertexTransform ( id, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexTransform ( u32 id, const ZLMatrix4x4& transform ) {

	assert ( id < TOTAL_VTX_TRANSFORMS );

	if ( !this->mVertexTransforms [ id ].IsSame ( transform )) {

		this->mVertexTransforms [ id ] = transform;
		
		// check to see if this is a CPU or GPU matrix and update accordingly
		if ( id < this->mVertexMtxOutput ) {
		
			// Invalidate the lower level matrices (i.e. modelview, etc) matrix in this case to force recalc
			for ( u32 i = this->mVertexMtxInput; i <= id; ++i ) {
				this->mCpuVertexTransformCache [ i ] = false;
			}
			this->UpdateCpuVertexMtx ();
		}
		else {
			this->UpdateGpuVertexMtx ();
		}
	}
	
	this->mShaderDirty = true;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetViewRect () {

	float width = ( float )this->mFrameBuffer->mBufferWidth;
	float height = ( float )this->mFrameBuffer->mBufferHeight;

	MOAIViewport rect;
	rect.Init ( 0.0f, 0.0f, width, height );
	
	this->SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetViewRect ( ZLRect rect ) {

	ZLRect deviceRect;
	
	deviceRect = this->mFrameBuffer->WndRectToDevice ( rect );
	
	s32 x = ( s32 )deviceRect.mXMin;
	s32 y = ( s32 )deviceRect.mYMin;
	
	u32 w = ( u32 )( deviceRect.Width () + 0.5f );
	u32 h = ( u32 )( deviceRect.Height () + 0.5f );
	
	zglViewport ( x, y, w, h );
	
	this->mViewRect = rect;
	this->mShaderDirty = true;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::TransformAndWriteQuad ( ZLVec4D* vtx, ZLVec2D* uv ) {

	if ( this->mCpuVertexTransform ) {
		this->mCpuVertexTransformMtx.TransformQuad ( vtx );
	}
	
	if ( this->mCpuUVTransform ) {
		this->mUVTransform.TransformQuad ( uv );
	}
	
	this->BeginPrim ();
	
		// left top
		this->Write ( vtx [ 0 ]);
		this->Write ( uv [ 0 ]);
		this->WriteFinalColor4b ();
		
		// left bottom
		this->Write ( vtx [ 3 ]);
		this->Write ( uv [ 3 ]);
		this->WriteFinalColor4b ();	
	
		// right bottom
		this->Write ( vtx[ 2 ]);
		this->Write ( uv [ 2 ]);
		this->WriteFinalColor4b ();
		
	this->EndPrim ();
	
	this->BeginPrim ();
	
		// left top
		this->Write ( vtx [ 0 ]);
		this->Write ( uv [ 0 ]);
		this->WriteFinalColor4b ();	
	
		// right bottom
		this->Write ( vtx [ 2 ]);
		this->Write ( uv [ 2 ]);
		this->WriteFinalColor4b ();	
	
		// right top
		this->Write ( vtx [ 1 ]);
		this->Write ( uv [ 1 ]);
		this->WriteFinalColor4b ();
		
	this->EndPrim ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateFinalColor () {

	this->mFinalColor.mR = this->mAmbientColor.mR * this->mPenColor.mR;
	this->mFinalColor.mG = this->mAmbientColor.mG * this->mPenColor.mG;
	this->mFinalColor.mB = this->mAmbientColor.mB * this->mPenColor.mB;
	this->mFinalColor.mA = this->mAmbientColor.mA * this->mPenColor.mA;

	this->mFinalColor32 = this->mFinalColor.PackRGBA ();
	
	this->mShaderDirty = true;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateCpuVertexMtx () {
	
	// Used signed, so we can roll "under" to -1 without an extra range check
	int start = this->mVertexMtxInput;
	int finish = this->mVertexMtxOutput;

	// The matrices are being multiplied A*B*C, but the common case is that
	// B and C are static throughout all/most of a frame. Thus, we can
	// capitalize on the associativity of matrix multiplication by caching
	// (B*C) and save a matrix mult in the common case (assuming they haven't
	// changed since the last update request).

	int i = finish - 1;
	
	if ( this->mCpuVertexTransformCache [ i ]) {
		while ( i >= start && this->mCpuVertexTransformCache [ i ]) {
			--i;
		}
		this->mCpuVertexTransformMtx = this->mCpuVertexTransformCacheMtx [ i + 1 ];
	}
	else {
		this->mCpuVertexTransformMtx.Ident();
	}
	
	for ( ; i >= start; --i ) {
		this->mCpuVertexTransformMtx.Prepend ( this->mVertexTransforms [ i ]);
		this->mCpuVertexTransformCacheMtx [ i ] = this->mCpuVertexTransformMtx;
		this->mCpuVertexTransformCache [ i ] = true;
	}

	this->mCpuVertexTransform = !this->mCpuVertexTransformMtx.IsIdent ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateGpuVertexMtx () {

	if ( this->IsProgrammable ()) return;

	#if USE_OPENGLES1
		this->Flush ();

		// update the gpu matrices
		switch ( this->mVertexMtxOutput ) {
			
			case VTX_STAGE_MODEL:
			
				zglMatrixMode ( ZGL_MATRIX_MODELVIEW );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_WORLD_TRANSFORM ]);
				this->GpuMultMatrix ( this->mVertexTransforms [ VTX_VIEW_TRANSFORM ]);
			
				zglMatrixMode ( ZGL_MATRIX_PROJECTION );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]);
				
				break;
				
			case VTX_STAGE_WORLD:
				
				zglMatrixMode ( ZGL_MATRIX_MODELVIEW );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_VIEW_TRANSFORM ]);
				
				zglMatrixMode ( ZGL_MATRIX_PROJECTION );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]);
			
				break;
				
			case VTX_STAGE_VIEW:
				
				zglMatrixMode ( ZGL_MATRIX_MODELVIEW );
				zglLoadIdentity ();
				
				zglMatrixMode ( ZGL_MATRIX_PROJECTION );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]);
				
				break;
			
			case VTX_STAGE_PROJ:
			
				zglMatrixMode ( ZGL_MATRIX_MODELVIEW );
				zglLoadIdentity ();
				
				zglMatrixMode ( ZGL_MATRIX_PROJECTION );
				zglLoadIdentity ();
			
				break;
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateShaderGlobals () {

	if ( this->mShaderProgram && this->mShaderDirty ) {
		this->mShaderProgram->UpdateGlobals ();
	}
	this->mShaderDirty = false;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateUVMtx () {

	if ( this->mUVMtxOutput == UV_STAGE_TEXTURE ) {
		
		this->mCpuUVTransform = !this->mUVTransform.IsIdent ();

		#if USE_OPENGLES1
			// flush and load gl UV transform
			if ( !this->mIsProgrammable ) {
				this->Flush ();
				zglMatrixMode ( ZGL_MATRIX_TEXTURE );
				zglLoadIdentity ();
			}
		#endif
	}
	else {
		
		this->mCpuUVTransform = false;

		#if USE_OPENGLES1
			// flush and load gl UV transform
			if ( !this->mIsProgrammable ) {
				this->Flush ();
				zglMatrixMode ( ZGL_MATRIX_TEXTURE );
				this->GpuLoadMatrix ( this->mUVTransform );
			}
		#endif
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateViewVolume () {

	ZLMatrix4x4 invViewProj;
	invViewProj.Inverse ( this->GetViewProjMtx ());
	this->mViewVolume.Init ( invViewProj );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv ) {

	ZLVec4D vtxBuffer [ 4 ];
	
	vtxBuffer [ 0 ].mX = vtx [ 0 ].mX;
	vtxBuffer [ 0 ].mY = vtx [ 0 ].mY;
	vtxBuffer [ 0 ].mZ = 0.0f;
	vtxBuffer [ 0 ].mW = 1.0f;
	
	vtxBuffer [ 1 ].mX = vtx [ 1 ].mX;
	vtxBuffer [ 1 ].mY = vtx [ 1 ].mY;
	vtxBuffer [ 1 ].mZ = 0.0f;
	vtxBuffer [ 1 ].mW = 1.0f;
	
	vtxBuffer [ 2 ].mX = vtx [ 2 ].mX;
	vtxBuffer [ 2 ].mY = vtx [ 2 ].mY;
	vtxBuffer [ 2 ].mZ = 0.0f;
	vtxBuffer [ 2 ].mW = 1.0f;
	
	vtxBuffer [ 3 ].mX = vtx [ 3 ].mX;
	vtxBuffer [ 3 ].mY = vtx [ 3 ].mY;
	vtxBuffer [ 3 ].mZ = 0.0f;
	vtxBuffer [ 3 ].mW = 1.0f;

	ZLVec2D uvBuffer [ 4 ];
	memcpy ( uvBuffer, uv, sizeof ( ZLVec2D ) * 4 );
	
	this->TransformAndWriteQuad ( vtxBuffer, uvBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff ) {

	ZLVec4D vtxBuffer [ 4 ];
	
	vtxBuffer [ 0 ].mX = vtx [ 0 ].mX + xOff;
	vtxBuffer [ 0 ].mY = vtx [ 0 ].mY + yOff;
	vtxBuffer [ 0 ].mZ = zOff;
	vtxBuffer [ 0 ].mW = 1.0f;
	
	vtxBuffer [ 1 ].mX = vtx [ 1 ].mX + xOff;
	vtxBuffer [ 1 ].mY = vtx [ 1 ].mY + yOff;
	vtxBuffer [ 1 ].mZ = zOff;
	vtxBuffer [ 1 ].mW = 1.0f;
	
	vtxBuffer [ 2 ].mX = vtx [ 2 ].mX + xOff;
	vtxBuffer [ 2 ].mY = vtx [ 2 ].mY + yOff;
	vtxBuffer [ 2 ].mZ = zOff;
	vtxBuffer [ 2 ].mW = 1.0f;
	
	vtxBuffer [ 3 ].mX = vtx [ 3 ].mX + xOff;
	vtxBuffer [ 3 ].mY = vtx [ 3 ].mY + yOff;
	vtxBuffer [ 3 ].mZ = zOff;
	vtxBuffer [ 3 ].mW = 1.0f;
	
	ZLVec2D uvBuffer [ 4 ];
	memcpy ( uvBuffer, uv, sizeof ( ZLVec2D ) * 4 );
	
	this->TransformAndWriteQuad ( vtxBuffer, uvBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale ) {

	ZLVec4D vtxBuffer [ 4 ];
	
	vtxBuffer [ 0 ].mX = ( vtx [ 0 ].mX * xScale ) + xOff;
	vtxBuffer [ 0 ].mY = ( vtx [ 0 ].mY * yScale ) + yOff;
	vtxBuffer [ 0 ].mZ = zOff;
	vtxBuffer [ 0 ].mW = 1.0f;
	
	vtxBuffer [ 1 ].mX = ( vtx [ 1 ].mX * xScale ) + xOff;
	vtxBuffer [ 1 ].mY = ( vtx [ 1 ].mY * yScale ) + yOff;
	vtxBuffer [ 1 ].mZ = zOff;
	vtxBuffer [ 1 ].mW = 1.0f;

	vtxBuffer [ 2 ].mX = ( vtx [ 2 ].mX * xScale ) + xOff;
	vtxBuffer [ 2 ].mY = ( vtx [ 2 ].mY * yScale ) + yOff;
	vtxBuffer [ 2 ].mZ = zOff;
	vtxBuffer [ 2 ].mW = 1.0f;
	
	vtxBuffer [ 3 ].mX = ( vtx [ 3 ].mX * xScale ) + xOff;
	vtxBuffer [ 3 ].mY = ( vtx [ 3 ].mY * yScale ) + yOff;
	vtxBuffer [ 3 ].mZ = zOff;
	vtxBuffer [ 3 ].mW = 1.0f;
	
	ZLVec2D uvBuffer [ 4 ];
	memcpy ( uvBuffer, uv, sizeof ( ZLVec2D ) * 4 );
	
	this->TransformAndWriteQuad ( vtxBuffer, uvBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale ) {

	ZLVec4D vtxBuffer [ 4 ];
	
	vtxBuffer [ 0 ].mX = ( vtx [ 0 ].mX * xScale ) + xOff;
	vtxBuffer [ 0 ].mY = ( vtx [ 0 ].mY * yScale ) + yOff;
	vtxBuffer [ 0 ].mZ = zOff;
	vtxBuffer [ 0 ].mW = 1.0f;
	
	vtxBuffer [ 1 ].mX = ( vtx [ 1 ].mX * xScale ) + xOff;
	vtxBuffer [ 1 ].mY = ( vtx [ 1 ].mY * yScale ) + yOff;
	vtxBuffer [ 1 ].mZ = zOff;
	vtxBuffer [ 1 ].mW = 1.0f;

	vtxBuffer [ 2 ].mX = ( vtx [ 2 ].mX * xScale ) + xOff;
	vtxBuffer [ 2 ].mY = ( vtx [ 2 ].mY * yScale ) + yOff;
	vtxBuffer [ 2 ].mZ = zOff;
	vtxBuffer [ 2 ].mW = 1.0f;
	
	vtxBuffer [ 3 ].mX = ( vtx [ 3 ].mX * xScale ) + xOff;
	vtxBuffer [ 3 ].mY = ( vtx [ 3 ].mY * yScale ) + yOff;
	vtxBuffer [ 3 ].mZ = zOff;
	vtxBuffer [ 3 ].mW = 1.0f;
	
	ZLVec2D uvBuffer [ 4 ];
	
	uvBuffer [ 0 ].mX = ( uv [ 0 ].mX * uScale ) + uOff;
	uvBuffer [ 0 ].mY = ( uv [ 0 ].mY * vScale ) + vOff;
	
	uvBuffer [ 1 ].mX = ( uv [ 1 ].mX * uScale ) + uOff;
	uvBuffer [ 1 ].mY = ( uv [ 1 ].mY * vScale ) + vOff;

	uvBuffer [ 2 ].mX = ( uv [ 2 ].mX * uScale ) + uOff;
	uvBuffer [ 2 ].mY = ( uv [ 2 ].mY * vScale ) + vOff;
	
	uvBuffer [ 3 ].mX = ( uv [ 3 ].mX * uScale ) + uOff;
	uvBuffer [ 3 ].mY = ( uv [ 3 ].mY * vScale ) + vOff;
	
	this->TransformAndWriteQuad ( vtxBuffer, uvBuffer );
}

