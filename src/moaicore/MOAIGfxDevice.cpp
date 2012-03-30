// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxResource.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMultiTexture.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAISim.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAIVertexFormat.h>
#include <moaicore/MOAIVertexFormatMgr.h>
#include <moaicore/MOAIViewport.h>

#define REMAP_EXTENSION_PTR(target, ext) target = target ? target : ext;

//================================================================//
// MOAIGfxDeleter
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDeleter::Delete () {

	switch ( this->mType ) {
		
		case DELETE_BUFFER:
			glDeleteBuffers ( 1, &this->mResourceID );
			break;
		
		case DELETE_FRAMEBUFFER:
			glDeleteFramebuffers ( 1, &this->mResourceID );
			break;
		
		case DELETE_PROGRAM:
			glDeleteProgram ( this->mResourceID );
			break;
		
		case DELETE_SHADER:
			glDeleteShader ( this->mResourceID );
			break;
		
		case DELETE_TEXTURE:
			glDeleteTextures ( 1, &this->mResourceID );
			break;
		
		case DELETE_RENDERBUFFER:
			glDeleteRenderbuffers ( 1, &this->mResourceID );
			break;
	}
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getMaxTextureUnits
	@text	Returns the total number of texture units available on the device.

	@in		MOAIGfxDevice self
	@out	number maxTextureUnits
*/
int MOAIGfxDevice::_getMaxTextureUnits ( lua_State* L ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	lua_pushnumber ( L, gfxDevice.mTextureUnits.Size ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getViewSize
	@text	Returns the width and height of the view
	
	@out	int width
	@out	int height
*/
int MOAIGfxDevice::_getViewSize ( lua_State* L  ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	lua_pushnumber ( L, gfxDevice.GetWidth ());
	lua_pushnumber ( L, gfxDevice.GetHeight ());
	
	return 2;
}


//----------------------------------------------------------------//
/**	@name	isProgrammable
	@text	Returns a boolean indicating whether or not Moai is running
			under the programmable pipeline.

	@in		MOAIGfxDevice self
	@out	boolean isProgrammable
*/
int MOAIGfxDevice::_isProgrammable ( lua_State* L ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	lua_pushboolean ( L, gfxDevice.IsProgrammable ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setClearColor
	@text	At the start of each frame the device will by default automatically render a background color.  Using this function you can set the background color that is drawn each frame.  If you specify no arguments to this function, then automatic redraw of the background color will be turned off (i.e. the previous render will be used as the background).

	@overload

		@opt	number red			The red value of the color.
		@opt	number green		The green value of the color.
		@opt	number blue			The blue value of the color.
		@opt	number alpha		The alpha value of the color.
		@out	nil
	
	@overload
	
		@in		MOAIColor color
		@out	nil
*/
int MOAIGfxDevice::_setClearColor ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAIGfxDevice& device = MOAIGfxDevice::Get ();
	
	MOAIColor* color = state.GetLuaObject < MOAIColor >( 1 );
	if ( color ) {
		device.SetClearColor ( color );
		device.mClearFlags |= GL_COLOR_BUFFER_BIT;
		return 0;
	}
	
	// don't clear the color
	device.mClearFlags &= ~GL_COLOR_BUFFER_BIT;
	device.SetClearColor ( 0 );

	if ( state.GetTop () > 0 ) {
	
		float r = state.GetValue < float >( 1, 0.0f );
		float g = state.GetValue < float >( 2, 0.0f );
		float b = state.GetValue < float >( 3, 0.0f );
		float a = state.GetValue < float >( 4, 1.0f );
		
		device.mClearColor = USColor::PackRGBA ( r, g, b, a );
		device.mClearFlags |= GL_COLOR_BUFFER_BIT;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setClearDepth
	@text	At the start of each frame the device will by default automatically
			clear the depth buffer.  This function sets whether or not the depth
			buffer should be cleared at the start of each frame.

	@in		boolean clearDepth	Whether to clear the depth buffer each frame.
	@out	nil
*/
int MOAIGfxDevice::_setClearDepth ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAIGfxDevice& device = MOAIGfxDevice::Get ();
	
	bool clearDepth = state.GetValue < bool >( 1, false );
	
	if ( clearDepth ) {
		device.mClearFlags |= GL_DEPTH_BUFFER_BIT;
	}
	else {
		device.mClearFlags &= ~GL_DEPTH_BUFFER_BIT;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPenColor

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
/**	@name	setPenWidth

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
/**	@name	setPointSize

	@in		number size
	@out	nil
*/
int MOAIGfxDevice::_setPointSize ( lua_State* L ) {

	MOAILuaState state ( L );

	float size = state.GetValue < float >( 1, 1.0f );
	MOAIGfxDevice::Get ().SetPointSize ( size );
	return 0;
}

//================================================================//
// MOAIGfxDevice
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDevice::BeginDrawing () {

	mDrawCount = 0;

	if ( this->mClearFlags & GL_COLOR_BUFFER_BIT ) {
	
		USColorVec clearColor;
		
		if ( this->mClearColorNode ) {
			clearColor = this->mClearColorNode->GetColorTrait ();
		}
		else {
			clearColor.SetRGBA ( this->mClearColor );
		}
		
		glClearColor (
			clearColor.mR,
			clearColor.mG,
			clearColor.mB,
			clearColor.mA 
		);
	}

	if ( this->mClearFlags ) {
		glClear ( this->mClearFlags );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::BeginLayer () {

	float width = ( float )this->mWidth;
	float height = ( float )this->mHeight;
	
	MOAIViewport viewport;
	viewport.Init ( 0.0f, 0.0f, width, height );
	viewport.SetScale ( width, -height );
	viewport.SetOffset ( -1.0f, 1.0f );
	
	this->SetViewport ( viewport );

	for ( u32 i = 0; i < TOTAL_VTX_TRANSFORMS; ++i ) {
		this->mVertexTransforms [ i ].Ident ();
	}
	this->mUVTransform.Ident ();
	this->mCpuVertexTransformMtx.Ident ();
	
	this->mVertexMtxInput = VTX_STAGE_MODEL;
	this->mVertexMtxOutput = VTX_STAGE_MODEL;
	
	USMatrix4x4 projMtx;
	projMtx.Init ( viewport.GetProjMtx ());
	this->mVertexTransforms [ VTX_PROJ_TRANSFORM ] = projMtx;
	
	// fixed function reset
#if USE_OPENGLES1
	if ( !this->IsProgrammable ()) {
		
		// load identity matrix
		glMatrixMode ( GL_MODELVIEW );
		glLoadIdentity ();
		
		glMatrixMode ( GL_PROJECTION );
		this->GpuLoadMatrix ( projMtx );
		
		glMatrixMode ( GL_TEXTURE );
		glLoadIdentity ();
		
		// reset the current vertex color
		glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
		
		// reset the point size
		glPointSize (( GLfloat )this->mPointSize );
	}
#endif
}

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

	this->ProcessDeleters ();

	if ( this->mBuffer ) {
		free ( this->mBuffer );
		this->mBuffer = 0;
		this->mSize = 0;
		this->mTop = 0;
	}
	
	this->SetClearColor ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ClearColorBuffer ( u32 color ) {

	USColorVec colorVec;
	colorVec.SetRGBA ( color );
	
	glClearColor ( colorVec.mR, colorVec.mG, colorVec.mB, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ClearErrors () {
#ifndef MOAI_OS_NACL
	if ( this->mHasContext ) {
		while ( glGetError () != GL_NO_ERROR );
	}
#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DetectContext () {

	this->mHasContext = true;

	#ifdef __GLEW_H__
		static bool initGlew = true;
		if ( initGlew ) {
			glewInit ();
			initGlew = false;
		}
	#endif

	const GLubyte* driverVersion = glGetString ( GL_VERSION );
	
	STLString version = ( cc8* )driverVersion;
	version.to_lower ();
	
	STLString gles = "opengl es";
	
	if ( version.find ( gles ) != version.npos ) {
		this->mIsOpenGLES = true;
		version = version.substr ( gles.length ());
		
		size_t space = version.find ( ' ' );
		if ( space != version.npos ) {
			version = version.substr ( space + 1 );
		}
	}
	else {
		this->mIsOpenGLES = false;
	}
	
	version = version.substr ( 0, 3 );
	
	this->mMajorVersion = version.at ( 0 ) - '0';
	this->mMinorVersion = version.at ( 2 ) - '0';
	
	this->mIsProgrammable = ( this->mMajorVersion >= 2 );
	this->mIsFramebufferSupported = true;
	
	#if defined ( __GLEW_H__ )
	
		// if framebuffer object is not in code, check to see if it's available as
		// an extension and remap to core function pointers if so
		if (( this->mIsOpenGLES == false ) && ( this->mMajorVersion < 3 )) {
			
			if ( glewIsSupported ( "GL_EXT_framebuffer_object" )) {
		  
				REMAP_EXTENSION_PTR ( glBindFramebuffer,						glBindFramebufferEXT )
				REMAP_EXTENSION_PTR ( glCheckFramebufferStatus,					glCheckFramebufferStatusEXT )
				REMAP_EXTENSION_PTR ( glDeleteFramebuffers,						glDeleteFramebuffersEXT )
				REMAP_EXTENSION_PTR ( glDeleteRenderbuffers,					glDeleteRenderbuffersEXT )
				REMAP_EXTENSION_PTR ( glFramebufferRenderbuffer,				glFramebufferRenderbufferEXT )
				REMAP_EXTENSION_PTR ( glFramebufferTexture1D,					glFramebufferTexture1DEXT )
				REMAP_EXTENSION_PTR ( glFramebufferTexture2D,					glFramebufferTexture2DEXT )
				REMAP_EXTENSION_PTR ( glFramebufferTexture3D,					glFramebufferTexture3DEXT )
				REMAP_EXTENSION_PTR ( glGenFramebuffers,						glGenFramebuffersEXT )
				REMAP_EXTENSION_PTR ( glGenRenderbuffers,						glGenRenderbuffersEXT )
				REMAP_EXTENSION_PTR ( glGenerateMipmap,							glGenerateMipmapEXT )
				REMAP_EXTENSION_PTR ( glGetFramebufferAttachmentParameteriv,	glGetFramebufferAttachmentParameterivEXT )
				REMAP_EXTENSION_PTR ( glGetRenderbufferParameteriv,				glGetRenderbufferParameterivEXT )
				REMAP_EXTENSION_PTR ( glIsFramebuffer,							glIsFramebufferEXT )
				REMAP_EXTENSION_PTR ( glIsRenderbuffer,							glIsRenderbufferEXT )
				REMAP_EXTENSION_PTR ( glRenderbufferStorage,					glRenderbufferStorageEXT )	
			}
			else {
				// looks like frame buffer isn't supported
				this->mIsFramebufferSupported = false;
			}
		}
	#endif
	
	int maxTextureUnits;
	glGetIntegerv ( GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits );
	this->mTextureUnits.Init ( maxTextureUnits );
	this->mTextureUnits.Fill ( 0 );
	
	int maxTextureSize;
	glGetIntegerv ( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
	this->mMaxTextureSize = maxTextureSize;

	this->mDeleterStack.Reset ();
	this->ResetResources ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DrawPrims () {

	if ( this->mVertexFormat ) {

		u32 vertexSize = this->mVertexFormat->GetVertexSize ();

		if ( vertexSize ) {
			u32 count = this->mPrimSize ? this->mPrimCount * this->mPrimSize : ( u32 )( this->mTop / vertexSize );
			if ( count > 0 ) {
				glDrawArrays ( this->mPrimType, 0, count );
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

	return this->mDeviceScale;
}

//----------------------------------------------------------------//
cc8* MOAIGfxDevice::GetErrorString ( int error ) const {

	switch ( error ) {
		case GL_INVALID_ENUM:		return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:		return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:	return "GL_INVALID_OPERATION";
		
		#if USE_OPENGLES1
			case GL_STACK_OVERFLOW:		return "GL_STACK_OVERFLOW";
			case GL_STACK_UNDERFLOW:	return "GL_STACK_UNDERFLOW";
		#endif
		
		case GL_OUT_OF_MEMORY:		return "GL_OUT_OF_MEMORY";
	}
	return "";
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetHeight () const {

	return this->mHeight;
}

//----------------------------------------------------------------//
//USMatrix4x4 MOAIGfxDevice::GetModelToWorldMtx () const {
//
//	return this->mVertexTransforms [ VTX_WORLD_TRANSFORM ];
//}

//----------------------------------------------------------------//
//USMatrix4x4 MOAIGfxDevice::GetModelToWndMtx () const {
//
//	USMatrix4x4 modelToWnd = this->GetModelToWorldMtx ();
//	modelToWnd.Append ( this->GetWorldToWndMtx ());
//	return modelToWnd;
//}

//----------------------------------------------------------------//
USRect MOAIGfxDevice::GetRect () const {

	USRect rect;
	rect.mXMin = 0;
	rect.mYMin = 0;
	rect.mXMax = ( float )this->mWidth;
	rect.mYMax = ( float )this->mHeight;
	
	return rect;
}

//----------------------------------------------------------------//
USMatrix4x4 MOAIGfxDevice::GetUVTransform () const {

	return this->mUVTransform;
}

//----------------------------------------------------------------//
USMatrix4x4 MOAIGfxDevice::GetVertexTransform ( u32 id ) const {

	return this->mVertexTransforms [ id ];
}

//----------------------------------------------------------------//
USMatrix4x4 MOAIGfxDevice::GetViewProjMtx () const {

	USMatrix4x4 mtx = this->mVertexTransforms [ VTX_VIEW_TRANSFORM ];
	mtx.Append ( this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]);
	return mtx;
}

//----------------------------------------------------------------//
//USQuad MOAIGfxDevice::GetViewQuad () const {
//
//	USQuad quad;
//
//	USMatrix4x4 invMtx;
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
//USRect MOAIGfxDevice::GetViewRect () const {
//
//	return this->mViewRect;
//}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetWidth () const {

	return this->mWidth;
}

//----------------------------------------------------------------//
//USMatrix4x4 MOAIGfxDevice::GetWndToModelMtx () const {
//
//	USMatrix4x4 wndToModel;
//	wndToModel.Inverse ( this->GetModelToWndMtx ());
//	return wndToModel;
//}

//----------------------------------------------------------------//
//USMatrix4x4 MOAIGfxDevice::GetWndToWorldMtx () const {
//
//	USMatrix4x4 wndToWorld;
//	USMatrix4x4 mtx;
//
//	USRect rect = this->GetViewRect ();
//	
//	float hWidth = rect.Width () * 0.5f;
//	float hHeight = rect.Height () * 0.5f;
//
//	// Inv Wnd
//	wndToWorld.Translate ( -hWidth - rect.mXMin, -hHeight - rect.mYMin, 0.0f );
//		
//	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ), 1.0f );
//	wndToWorld.Append ( mtx );
//	
//	// inv viewproj
//	mtx = this->GetViewProjMtx ();
//	mtx.Inverse ();
//	wndToWorld.Append ( mtx );
//	
//	return wndToWorld;
//}

//----------------------------------------------------------------//
//USMatrix4x4 MOAIGfxDevice::GetWorldToModelMtx () const {
//	
//	USMatrix4x4 worldToModel;
//	worldToModel.Inverse ( this->mVertexTransforms [ VTX_WORLD_TRANSFORM ]);
//	return worldToModel;
//}

//----------------------------------------------------------------//
//USMatrix4x4 MOAIGfxDevice::GetWorldToWndMtx ( float xScale, float yScale ) const {
//
//	USMatrix4x4 worldToWnd;
//	USMatrix4x4 mtx;
//
//	USRect rect = this->GetViewRect ();
//	
//	float hWidth = rect.Width () * 0.5f;
//	float hHeight = rect.Height () * 0.5f;
//
//	// viewproj
//	worldToWnd = this->GetViewProjMtx ();
//	
//	// wnd
//	mtx.Scale ( hWidth * xScale, hHeight * yScale, 1.0f );
//	worldToWnd.Append ( mtx );
//		
//	mtx.Translate ( hWidth + rect.mXMin, hHeight + rect.mYMin, 0.0f );
//	worldToWnd.Append ( mtx );
//	
//	return worldToWnd;
//}

//----------------------------------------------------------------//
void MOAIGfxDevice::GpuLoadMatrix ( const USMatrix4x4& mtx ) const {
	#if USE_OPENGLES1
		glLoadMatrixf ( mtx.m );
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::GpuMultMatrix ( const USMatrix4x4& mtx ) const {
	#if USE_OPENGLES1
		glMultMatrixf ( mtx.m );
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::InsertGfxResource ( MOAIGfxResource& resource ) {

	this->mResources.PushBack ( resource.mLink );
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::LogErrors () {

	u32 count = 0;
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			for ( int error = glGetError (); error != GL_NO_ERROR; error = glGetError (), ++count ) {
				MOAILog ( 0, MOAILogMessages::MOAIGfxDevice_OpenGLError_S, this->GetErrorString ( error ));
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
	mClearFlags ( GL_COLOR_BUFFER_BIT ),
	mClearColor ( 0xff000000 ),
	mClearColorNode ( 0 ),
	mCpuVertexTransform ( false ),
	mCpuUVTransform ( false ),
	mDefaultFrameBuffer ( 0 ),
	mDeviceScale ( 1.0f ),
	mHasContext ( false ),
	mHeight ( 0 ),
	mIsFramebufferSupported ( 0 ),
	mIsOpenGLES ( false ),
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
	mPrimType ( GL_POINTS ),
	mShader ( 0 ),
	mSize ( 0 ),
	mActiveTextures ( 0 ),
	mTextureMemoryUsage ( 0 ),
	mMaxTextureSize ( 0 ),
	mTop ( 0 ),
	mUVMtxInput ( UV_STAGE_MODEL ),
	mUVMtxOutput ( UV_STAGE_MODEL ),
	mVertexFormat ( 0 ),
	mVertexMtxInput ( VTX_STAGE_MODEL ),
	mVertexMtxOutput ( VTX_STAGE_MODEL ),
	mWidth ( 0 ) {
	
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
}

//----------------------------------------------------------------//
MOAIGfxDevice::~MOAIGfxDevice () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ProcessDeleters () {

	u32 top = this->mDeleterStack.GetTop ();
	for ( u32 i = 0; i < top; ++i ) {
		MOAIGfxDeleter& deleter = this->mDeleterStack [ i ];
		deleter.Delete ();
	}
	this->mDeleterStack.Reset ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::PushDeleter ( u32 type, GLuint id ) {

	MOAIGfxDeleter deleter;
	deleter.mType = type;
	deleter.mResourceID = id;
	
	this->mDeleterStack.Push ( deleter );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "EVENT_RESIZE", ( u32 )EVENT_RESIZE );

	luaL_Reg regTable [] = {
		{ "getMaxTextureUnits",			_getMaxTextureUnits },
		{ "getViewSize",				_getViewSize },
		{ "isProgrammable",				_isProgrammable },
		{ "setClearColor",				_setClearColor },
		{ "setClearDepth",				_setClearDepth },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIGfxDevice > },
		{ "setPenColor",				_setPenColor },
		{ "setPenWidth",				_setPenWidth },
		{ "setPointSize",				_setPointSize },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ReleaseResources () {

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Destroy ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::RemoveGfxResource ( MOAIGfxResource& resource ) {

	this->mResources.Remove ( resource.mLink );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::RenewResources () {

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Load ();
	}
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
void MOAIGfxDevice::ResetResources () {

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Invalidate ();
		resourceIt->Data ()->Load ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ResetState () {

	this->mTop = 0;
	this->mPrimCount = 0;

	// turn off texture
	#if USE_OPENGLES1
		if ( !this->IsProgrammable ()) {	
			glDisable ( GL_TEXTURE_2D );	
		}
	#endif
	this->mTextureUnits [ 0 ] = 0;
	
	// turn off blending
	glDisable ( GL_BLEND );
	this->mBlendEnabled = false;
	
	// disable backface culling
	glDisable ( GL_CULL_FACE );
	this->mCullFunc = 0;
	
	// disable depth test
	glDisable ( GL_DEPTH_TEST );
	this->mDepthFunc = 0;
	
	// enable depth write
	glDepthMask ( true );
	this->mDepthMask = true;
	
	// clear the vertex format
	this->SetVertexFormat ();

	// clear the shader
	this->mShader = 0;
	
	// reset the pen width
	this->mPenWidth = 1.0f;
	glLineWidth (( GLfloat )this->mPenWidth );
	
	// reset the point size
	this->mPointSize = 1.0f;
	
	// reset the scissor rect
	MOAIGfxDevice& device = MOAIGfxDevice::Get ();
	USRect scissorRect = device.GetRect ();
	glScissor (( int )scissorRect.mXMin, ( int )scissorRect.mYMin, ( int )scissorRect.Width (), ( int )scissorRect.Height ());
	
	this->mScissorRect = scissorRect;
	
	#if USE_OPENGLES1
		// fixed function reset
		if ( !this->IsProgrammable ()) {
			
			// reset the current vertex color
			glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
			
			// reset the point size
			glPointSize (( GLfloat )this->mPointSize );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetAmbientColor ( u32 color ) {

	this->mAmbientColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetAmbientColor ( const USColorVec& colorVec ) {

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
		glDisable ( GL_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	if ( !this->mBlendEnabled ) {
		this->Flush ();
		glEnable ( GL_BLEND );
		this->mBlendMode = blendMode;
		glBlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
		this->mBlendEnabled = true;
	}
	else if ( !this->mBlendMode.IsSame ( blendMode )) {
		this->Flush ();
		this->mBlendMode = blendMode;
		glBlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode ( int srcFactor, int dstFactor ) {

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( srcFactor, dstFactor );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetClearColor ( MOAIColor* color ) {

	if ( this->mClearColorNode != color ) {
		this->LuaRelease ( this->mClearColorNode );
		this->LuaRetain ( color );
		this->mClearColorNode = color;
	}
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
			glEnable ( GL_CULL_FACE );
			glCullFace ( this->mCullFunc );
		}
		else {
			glDisable ( GL_CULL_FACE );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetDeviceScale ( float scale ) {

	this->mDeviceScale = scale;

}
//----------------------------------------------------------------//
void MOAIGfxDevice::SetDefaultFrameBuffer ( GLuint frameBuffer ) {

	this->mDefaultFrameBuffer = frameBuffer;
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
			glEnable ( GL_DEPTH_TEST );
			glDepthFunc ( this->mDepthFunc );
		}
		else {
			glDisable ( GL_DEPTH_TEST );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetDepthMask ( bool depthMask ) {

	if ( this->mDepthMask != depthMask ) {
		this->Flush ();
		this->mDepthMask = depthMask;
		glDepthMask ( this->mDepthMask );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	this->Flush ();

	if ( frameBuffer ) {
		frameBuffer->BindAsFrameBuffer ();
	}
	else {
		if ( this->mIsFramebufferSupported ) {
			glBindFramebuffer ( GL_FRAMEBUFFER, this->mDefaultFrameBuffer ); // TODO: crash?
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
void MOAIGfxDevice::SetPenColor ( const USColorVec& colorVec ) {

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
		glLineWidth (( GLfloat )penWidth );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPointSize ( float pointSize ) {

	#if USE_OPENGLES1
		if ( this->mPointSize != pointSize ) {
			this->Flush ();
			this->mPointSize = pointSize;

			glPointSize (( GLfloat )pointSize );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPrimType ( u32 primType ) {

	if ( this->mPrimType != primType ) {

		this->Flush ();
		this->mPrimType = primType;

		switch ( primType ) {
		
			case GL_POINTS:
				this->mPrimSize = 1;
				break;
			
			case GL_LINES:
				this->mPrimSize = 2;
				break;
			
			case GL_TRIANGLES:
				this->mPrimSize = 3;
				break;
			
			case GL_LINE_LOOP:
			case GL_LINE_STRIP:
			case GL_TRIANGLE_FAN:
			case GL_TRIANGLE_STRIP:
			default:
				this->mPrimSize = 0;
				break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetScissorRect () {

	this->SetScissorRect ( this->GetRect ());
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetScissorRect ( const USRect& rect ) {
	
	USRect& current = this->mScissorRect;
	
	if (	( current.mXMin != rect.mXMin ) ||
			( current.mYMin != rect.mYMin ) ||
			( current.mXMax != rect.mXMax ) ||
			( current.mYMax != rect.mYMax )) {
	
		this->Flush ();
		glScissor (( int )rect.mXMin, ( int )rect.mYMin, ( int )rect.Width (), ( int )rect.Height ());
		this->mScissorRect = rect;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetScreenSpace ( MOAIViewport& viewport ) {
	UNUSED ( viewport );

	// TODO

	//glMatrixMode ( GL_MODELVIEW );
	//glLoadIdentity ();
	//
	//USMatrix4x4 wndToNorm;
	//viewport.GetWndToNormMtx ( wndToNorm );
	//
	//glMatrixMode ( GL_PROJECTION );
	//MOAIGfxDevice::LoadMatrix ( wndToNorm );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetShader ( MOAIShader* shader ) {

	if (( this->mShader != shader ) && this->mIsProgrammable ) {
	
		this->Flush ();
		this->mShader = shader;
		
		if ( shader ) {
			shader->Bind ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetShaderPreset ( u32 preset ) {

	MOAIShaderMgr::Get ().BindShader ( preset );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetSize ( u32 width, u32 height ) {

	this->mWidth = width;
	this->mHeight = height;
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( EVENT_RESIZE, state )) {
		lua_pushnumber ( state, width );
		lua_pushnumber ( state, height );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture () {
	
	if ( this->mActiveTextures ) {
	
		this->Flush ();
	
		for ( u32 i = 0; i < this->mActiveTextures; ++i ) {
			glActiveTexture ( GL_TEXTURE0 + i );
			
			#if USE_OPENGLES1
				if ( !this->IsProgrammable ()) {
					glDisable ( GL_TEXTURE_2D );
				}
			#endif
			
			this->mTextureUnits [ i ] = 0;
		}
		this->mActiveTextures = 0;
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture ( MOAITextureBase* texture ) {
	
	if ( !texture ) {
		return this->SetTexture ();
	}
	
	// disable any active textures beyond the first unit
	if ( this->mActiveTextures > 1 ) {
		this->Flush ();
	
		for ( u32 i = 1; i < this->mActiveTextures; ++i ) {
			glActiveTexture ( GL_TEXTURE0 + i );
			
			#if USE_OPENGLES1
				if ( !this->IsProgrammable ()) {
					glDisable ( GL_TEXTURE_2D );
				}
			#endif
			
			this->mTextureUnits [ i ] = 0;
		}
	}
	
	this->mActiveTextures = 1;
	
	if ( this->mTextureUnits [ 0 ] == texture ) {
		return true;
	}
	
	this->Flush ();
	
	glActiveTexture ( GL_TEXTURE0 );
	
	if ( !this->mTextureUnits [ 0 ]) {
	
		#if USE_OPENGLES1
			if ( !this->IsProgrammable ()) {
				glEnable ( GL_TEXTURE_2D );
			}
		#endif
	}
	
	this->mTextureUnits [ 0 ] = texture;
	return texture->Bind ();
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
	if ( total < this->mActiveTextures ) {
		this->Flush ();
	
		for ( u32 i = total; i < this->mActiveTextures; ++i ) {
			glActiveTexture ( GL_TEXTURE0 + i );
			
			#if USE_OPENGLES1
				if ( !this->IsProgrammable ()) {
					glDisable ( GL_TEXTURE_2D );
				}
			#endif
			
			this->mTextureUnits [ i ] = 0;
		}
	}
	
	for ( u32 i = 0; i < total; ++i ) {
	
		if ( this->mTextureUnits [ i ] != multi->mTextures [ i ]) {
			
			this->Flush ();
			glActiveTexture ( GL_TEXTURE0 + i );
			
			if ( !this->mTextureUnits [ i ]) {
			
				#if USE_OPENGLES1
					if ( !this->IsProgrammable ()) {
						glEnable ( GL_TEXTURE_2D );
					}
				#endif
			}
			this->mTextureUnits [ i ] = multi->mTextures [ i ];
			this->mTextureUnits [ i ]->Bind ();
		}
	}
	this->mActiveTextures = total;
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

	USMatrix4x4 mtx;
	mtx.Ident ();
	this->SetUVTransform ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetUVTransform ( const USAffine3D& transform ) {

	USMatrix4x4 mtx;
	mtx.Init ( transform );
	this->SetUVTransform ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetUVTransform ( const USMatrix4x4& transform ) {

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
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexFormat ( const MOAIVertexFormat& format ) {

	if ( this->mVertexFormat != &format ) {

		this->SetVertexFormat ();
		this->mVertexFormat = &format;
		this->mVertexFormat->Bind ( this->mBuffer );
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

	this->SetVertexFormat ( MOAIVertexFormatMgr::Get ().GetPreset ( preset ));
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexTransform ( u32 id ) {

	USMatrix4x4 mtx;
	mtx.Ident ();
	this->SetVertexTransform ( id, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexTransform ( u32 id, const USAffine3D& transform ) {

	USMatrix4x4 mtx;
	mtx.Init ( transform );
	this->SetVertexTransform ( id, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexTransform ( u32 id, const USMatrix4x4& transform ) {

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
	
	// update any transforms in the shader that rely on the pipeline
	// the shader caches the state of each uniform; only reloads when changed
	if ( this->mShader ) {
		this->mShader->UpdatePipelineTransforms (
			this->mVertexTransforms [ VTX_WORLD_TRANSFORM ],
			this->mVertexTransforms [ VTX_VIEW_TRANSFORM ],
			this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]
		);
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetViewport () {

	float width = ( float )this->mWidth;
	float height = ( float )this->mHeight;

	MOAIViewport viewport;
	viewport.Init ( 0.0f, 0.0f, width, height );
	viewport.SetScale ( width, -height );
	viewport.SetOffset ( -1.0f, 1.0f );
	
	this->SetViewport ( viewport );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetViewport ( const USRect& viewport ) {

	// set us up the viewport
	
	GLint x = ( GLint )viewport.mXMin;
	GLint y = ( GLint )viewport.mYMin;
	
	GLsizei w = ( GLsizei )( viewport.Width () + 0.5f );
	GLsizei h = ( GLsizei )( viewport.Height () + 0.5f );
	
	glViewport (
		( GLint )( x * this->mDeviceScale ),
		( GLint )( y * this->mDeviceScale ),
		( GLsizei )( w * this->mDeviceScale ),
		( GLsizei )( h * this->mDeviceScale )
	);

	this->mViewRect = viewport;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SoftReleaseResources ( u32 age ) {

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->SoftRelease ( age );
	}
	
	// Horrible to call this, but generally soft release is only used
	// in response to a low memory warning and we want to free as soon as possible.
	glFlush ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateFinalColor () {

	this->mFinalColor.mR = this->mAmbientColor.mR * this->mPenColor.mR;
	this->mFinalColor.mG = this->mAmbientColor.mG * this->mPenColor.mG;
	this->mFinalColor.mB = this->mAmbientColor.mB * this->mPenColor.mB;
	this->mFinalColor.mA = this->mAmbientColor.mA * this->mPenColor.mA;

	this->mFinalColor32 = this->mFinalColor.PackRGBA ();
	
	if ( this->mShader ) {
		this->mShader->UpdatePenColor ( this->mFinalColor.mR, this->mFinalColor.mG, this->mFinalColor.mB, this->mFinalColor.mA );
	}
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
			
				glMatrixMode ( GL_MODELVIEW );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_WORLD_TRANSFORM ]);
				this->GpuMultMatrix ( this->mVertexTransforms [ VTX_VIEW_TRANSFORM ]);
			
				glMatrixMode ( GL_PROJECTION );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]);
				
				break;
				
			case VTX_STAGE_WORLD:
				
				glMatrixMode ( GL_MODELVIEW );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_VIEW_TRANSFORM ]);
				
				glMatrixMode ( GL_PROJECTION );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]);
			
				break;
				
			case VTX_STAGE_VIEW:
				
				glMatrixMode ( GL_MODELVIEW );
				glLoadIdentity ();
				
				glMatrixMode ( GL_PROJECTION );
				this->GpuLoadMatrix ( this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]);
				
				break;
			
			case VTX_STAGE_PROJ:
			
				glMatrixMode ( GL_MODELVIEW );
				glLoadIdentity ();
				
				glMatrixMode ( GL_PROJECTION );
				glLoadIdentity ();
			
				break;
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateUVMtx () {

	if ( this->mUVMtxOutput == UV_STAGE_TEXTURE ) {
		
		this->mCpuUVTransform = !this->mUVTransform.IsIdent ();

		#if USE_OPENGLES1
			// flush and load gl UV transform
			if ( !this->mIsProgrammable ) {
				this->Flush ();
				glMatrixMode ( GL_TEXTURE );
				glLoadIdentity ();
			}
		#endif
	}
	else {
		
		this->mCpuUVTransform = false;

		#if USE_OPENGLES1
			// flush and load gl UV transform
			if ( !this->mIsProgrammable ) {
				this->Flush ();
				glMatrixMode ( GL_TEXTURE );
				this->GpuLoadMatrix ( this->mUVTransform );
			}
		#endif
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateViewVolume () {

	USMatrix4x4 invViewProj;
	invViewProj.Inverse ( this->GetViewProjMtx ());
	this->mViewVolume.Init ( invViewProj );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::WriteQuad ( USVec2D* vtx, USVec2D* uv ) {

	USVec4D vtx4D [ 4 ];
	
	vtx4D [ 0 ].mX = vtx [ 0 ].mX;
	vtx4D [ 0 ].mY = vtx [ 0 ].mY;
	vtx4D [ 0 ].mZ = 0.0f;
	vtx4D [ 0 ].mW = 1.0f;

	vtx4D [ 1 ].mX = vtx [ 1 ].mX;
	vtx4D [ 1 ].mY = vtx [ 1 ].mY;
	vtx4D [ 1 ].mZ = 0.0f;
	vtx4D [ 1 ].mW = 1.0f;
	
	vtx4D [ 2 ].mX = vtx [ 2 ].mX;
	vtx4D [ 2 ].mY = vtx [ 2 ].mY;
	vtx4D [ 2 ].mZ = 0.0f;
	vtx4D [ 2 ].mW = 1.0f;
	
	vtx4D [ 3 ].mX = vtx [ 3 ].mX;
	vtx4D [ 3 ].mY = vtx [ 3 ].mY;
	vtx4D [ 3 ].mZ = 0.0f;
	vtx4D [ 3 ].mW = 1.0f;

	this->WriteQuad ( vtx4D, uv );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::WriteQuad ( USVec4D* vtx, USVec2D* uv ) {

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
