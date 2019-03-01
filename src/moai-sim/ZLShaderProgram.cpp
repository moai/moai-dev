// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/ZLShaderProgram.h>

//================================================================//
// ZLShaderProgramGlobal
//================================================================//
	
//----------------------------------------------------------------//
ZLShaderProgramGlobal::ZLShaderProgramGlobal () :
	mGlobalID ( ZLIndexOp::INVALID ),
	mUniformID ( ZLIndexOp::INVALID ),
	mIndex ( ZLIndexOp::ZERO ) {
}

//================================================================//
// ZLShaderProgramTexture
//================================================================//

//----------------------------------------------------------------//
ZLShaderProgramTexture::ZLShaderProgramTexture () :
	mName ( MOAI_UNKNOWN_MATERIAL_GLOBAL ),
	mUnit ( ZLIndexOp::ZERO ) {
}

//================================================================//
// ZLShaderProgram
//================================================================//

//----------------------------------------------------------------//
void ZLShaderProgram::AffirmUniforms () {

	if ( this->mUniformBufferSize ) return;
	
	this->mUniformBufferSize = 0;
	size_t nUniforms = this->mUniforms.Size ();
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
		
		ZLShaderUniform& uniform = this->mUniforms [ i ];
		uniform.mCPUOffset = this->mUniformBufferSize;
		this->mUniformBufferSize += uniform.GetSize ();
		
		this->mMaxCount = this->mMaxCount < uniform.mCount ? uniform.mCount : this->mMaxCount;
	}
	
	this->mUniformBuffer.Clear ();
	this->mUniformBuffer.Init ( this->mUniformBufferSize );
	this->mUniformBuffer.Fill ( 0xff );
}

//----------------------------------------------------------------//
void ZLShaderProgram::ApplyUniforms ( ZLLeanArray < u8 >& buffer ) {

	this->mUniformBuffer.CopyFrom ( buffer);
}

//----------------------------------------------------------------//
void ZLShaderProgram::BindUniforms () {
	
	ZLGfxStateCache& gfxState = MOAIGfxMgr::Get ().mGfxState;
	
	size_t nUniforms = this->mUniforms.Size ();
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
	
		ZLShaderUniformHandle uniform = this->GetUniformHandle ( this->mUniformBuffer.GetBuffer (), i );
		
		if ( uniform.IsValid ()) {
			this->mUniforms [ i ].Bind ( uniform.mBuffer );
		}
	}
}

//----------------------------------------------------------------//
void ZLShaderProgram::Clear () {

	this->mVertexShaderSource.clear ();
	this->mFragmentShaderSource.clear ();

	this->mAttributeMap.clear ();
	this->mUniforms.Clear ();
	this->mGlobals.Clear ();
	
	this->Destroy ();
}

//----------------------------------------------------------------//
ZLGfxHandle ZLShaderProgram::CompileShader ( u32 type, cc8* source ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	ZLGfx& gfx = gfxMgr.GetDrawingAPI ();

	ZLGfxHandle shader = gfx.CreateShader ( type );

	STLString buffer;

	buffer.append ( gfxMgr.IsOpenGLES () ? OPENGL_ES_PREPROC : OPENGL_PREPROC );
	if (( type == ZGL_SHADER_TYPE_FRAGMENT ) && gfxMgr.IsOpenGLES ()) {
		buffer.append ( WEBGL_PREPROC );
	}

	buffer.append ( source );

	gfx.PushSection ();

	gfx.ShaderSource ( shader, buffer.c_str (), buffer.size ());
	gfx.CompileShader ( shader, true );

	if ( gfx.PushErrorHandler ()) {
		gfx.DeleteResource ( shader );
	}

	gfx.PopSection ();

	return shader;
}

//----------------------------------------------------------------//
void ZLShaderProgram::DeclareUniform ( ZLIndex idx, cc8* name, u32 type, u32 width, u32 count ) {

	if ( idx < this->mUniforms.Size ()) {

		ZLShaderUniform& uniform = this->mUniforms [ idx ];
		uniform.mName = name;
		uniform.Init ( type, width, count );
	}
}

//----------------------------------------------------------------//
ZLShaderUniform* ZLShaderProgram::GetUniform ( ZLIndex uniformID ) {

	return uniformID < this->mUniforms.Size () ? &this->mUniforms [ uniformID ] : 0;
}

//----------------------------------------------------------------//
void ZLShaderProgram::InitUniformBuffer ( ZLLeanArray < u8 >& buffer ) {

	this->AffirmUniforms ();

	buffer.Clear ();
	
	size_t nUniforms = this->mUniforms.Size ();
	buffer.Init ( this->mUniformBufferSize );
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
		ZLShaderUniformHandle uniform = this->GetUniformHandle ( buffer.GetBuffer (), i );
		uniform.Default ( this->mUniforms [ i ].mCount );
	}
}

//----------------------------------------------------------------//
void ZLShaderProgram::Load ( cc8* vshSource, cc8* fshSource ) {

	if ( vshSource && fshSource ) {

		this->mVertexShaderSource = vshSource;
		this->mFragmentShaderSource = fshSource;
		
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
ZLShaderProgram::ZLShaderProgram () :
	mMaxCount ( 0 ),
	mUniformBufferSize ( 0 ) {
}

//----------------------------------------------------------------//
ZLShaderProgram::~ZLShaderProgram () {

	this->Clear ();
}

//----------------------------------------------------------------//
void ZLShaderProgram::ReserveGlobals ( ZLSize nGlobals ) {

	this->mGlobals.Init ( nGlobals );
}

//----------------------------------------------------------------//
void ZLShaderProgram::ReserveTextures ( ZLSize nTextures ) {

	this->mTextures.Init ( nTextures );
}

//----------------------------------------------------------------//
void ZLShaderProgram::ReserveUniforms ( ZLSize nUniforms ) {

	this->mUniforms.Init ( nUniforms );
}

//----------------------------------------------------------------//
void ZLShaderProgram::ScheduleTextures () {

	MOAIGfxMgr& gfx = MOAIGfxMgr::Get ();
	MOAIMaterialMgr& materialStack = MOAIMaterialMgr::Get ();

	size_t nTextures = this->mTextures.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nTextures; ++i ) {
	
		ZLShaderProgramTexture& shaderTexture = this->mTextures [ i ];
		
		ZLTexture* texture = shaderTexture.mTexture;
		
		// load texture by name
		if ( !texture ) {
			texture = materialStack.GetTexture ( shaderTexture.mName );
		}
		
		gfx.mGfxState.SetTexture ( texture, shaderTexture.mUnit );
	}
}

//----------------------------------------------------------------//
void ZLShaderProgram::SetGlobal ( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index ) {
	
	ZLShaderProgramGlobal& global = this->mGlobals [ idx ];
	
	global.mUniformID	= uniformID;
	global.mIndex		= index;
	global.mGlobalID	= globalID;
}

//----------------------------------------------------------------//
//int ZLShaderProgram::SetGlobal ( lua_State* L, int idx ) {
//
//	MOAILuaState state ( L );
//
//	ZLIndex globalIdx	= state.GetValue < MOAILuaIndex >( idx, ZLIndexOp::ZERO );
//	u32 globalID		= state.GetValue < u32 >( idx + 1, ZLGfxStateCache::NULL_GLOBAL );
//	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( idx + 2, ZLIndexOp::ZERO );
//	ZLIndex index		= state.GetValue < MOAILuaIndex >( idx + 3, ZLIndexOp::ZERO );
//
//	this->SetGlobal ( globalIdx, globalID, uniformID, index );
//
//	return 0;
//}

//----------------------------------------------------------------//
void ZLShaderProgram::SetTexture ( ZLIndex idx, u32 name, ZLIndex unit, ZLTexture* fallback ) {

	if ( idx < this->mTextures.Size ()) {
	
		ZLShaderProgramTexture& shaderTexture = this->mTextures [ idx ];
		shaderTexture.mName = name;
		shaderTexture.mUnit = unit;
		shaderTexture.mTexture = fallback;
	}
}

//----------------------------------------------------------------//
void ZLShaderProgram::SetTexture ( ZLIndex idx, ZLTexture* texture, ZLIndex unit ) {

	if ( idx < this->mTextures.Size ()) {
	
		ZLShaderProgramTexture& shaderTexture = this->mTextures [ idx ];
		shaderTexture.mName = MOAI_UNKNOWN_MATERIAL_GLOBAL;
		shaderTexture.mUnit = unit;
		shaderTexture.mTexture = texture;
	}
}

//----------------------------------------------------------------//
void ZLShaderProgram::SetVertexAttribute ( u32 idx, cc8* attribute ) {

	if ( attribute ) {
		this->mAttributeMap [ idx ] = attribute;
	}
}

//----------------------------------------------------------------//
void ZLShaderProgram::UpdateUniforms ( ZLLeanArray < u8 >& buffer ) {

	ZLGfxStateCache& gfxState = MOAIGfxMgr::Get ().mGfxState;
	
	ZLRect viewRect = gfxState.GetViewRect ();

	// NOTE: matrices are submitted *column major*; it is up to the shader to transform vertices correctly
	// vert * matrix implicitely transposes the matrix; martix * vert uses the matrix as submitted

	u32 nGlobals = this->mGlobals.Size ();

	for ( ZLIndex i = ZLIndexOp::ZERO; i < nGlobals; ++i ) {
	
		const ZLShaderProgramGlobal& global = this->mGlobals [ i ];
		
		if ( global.mUniformID == ZLIndexOp::INVALID ) continue;
		
		ZLShaderUniformHandle uniform = this->GetUniformHandle ( buffer.GetBuffer (), global.mUniformID, global.mIndex );
		if ( !uniform.IsValid ()) continue;
		
		if ( global.mGlobalID < ZLGfxStateCache::TOTAL_MATRICES ) {
		
			uniform.SetValue ( gfxState.GetMtx ( global.mGlobalID ));
		}
		else {
		
			switch (( ZLSize )global.mGlobalID ) {
				
				case ZLGfxStateCache::PEN_COLOR:
				
					uniform.SetValue ( gfxState.GetFinalColor ());
					break;
				
				case ZLGfxStateCache::VIEW_HALF_HEIGHT:
				
					uniform.SetValue ( viewRect.Height () * 0.5f );
					break;
					
				case ZLGfxStateCache::VIEW_HALF_WIDTH: {
				
					uniform.SetValue ( viewRect.Width () * 0.5f );
					break;
				}
				case ZLGfxStateCache::VIEW_HEIGHT:
				
					uniform.SetValue ( viewRect.Height ());
					break;
					
				case ZLGfxStateCache::VIEW_WIDTH:
				
					uniform.SetValue ( viewRect.Width ());
					break;
			}
		}
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool ZLShaderProgram::ZLAbstractGfxResource_OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void ZLShaderProgram::ZLAbstractGfxResource_OnCPUDestroy () {
}

//----------------------------------------------------------------//
void ZLShaderProgram::ZLAbstractGfxResource_OnGPUBind () {

	// use shader program.
	MOAIGfxMgr::GetDrawingAPI ().UseProgram ( this->mProgram );
}

//----------------------------------------------------------------//
bool ZLShaderProgram::ZLAbstractGfxResource_OnGPUCreate () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	this->mVertexShader = this->CompileShader ( ZGL_SHADER_TYPE_VERTEX, this->mVertexShaderSource );
	this->mFragmentShader = this->CompileShader ( ZGL_SHADER_TYPE_FRAGMENT, this->mFragmentShaderSource );
	this->mProgram = gfx.CreateProgram ();

	// TODO: error handling
//	if ( !( this->mVertexShader && this->mFragmentShader && this->mProgram )) {
//		this->Clear ();
//		return false;
//	}

	gfx.AttachShader ( this->mProgram, this->mVertexShader );
	gfx.AttachShader ( this->mProgram, this->mFragmentShader );

	// bind attribute locations.
	// this needs to be done prior to linking.
	AttributeMapIt attrMapIt = this->mAttributeMap.begin ();
	for ( ; attrMapIt != this->mAttributeMap.end (); ++attrMapIt ) {
		gfx.BindAttribLocation ( this->mProgram, attrMapIt->first, attrMapIt->second.str ());
	}

	gfx.LinkProgram ( this->mProgram, true );

	// get the uniform locations
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mUniforms.Size (); ++i ) {
		ZLShaderUniform& uniform = this->mUniforms [ i ];
		gfx.GetUniformLocation ( this->mProgram, uniform.mName, this, ( void* )(( size_t )i )); // TODO: cast?
	}

	gfx.DeleteResource ( this->mVertexShader );
	gfx.DeleteResource ( this->mFragmentShader );

	//AJV TODO - does the attribute map ever need to be cleared?
	//this->mAttributeMap.clear ();
	
	return true;
}

//----------------------------------------------------------------//
void ZLShaderProgram::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	ZLGfxResourceClerk::DeleteOrDiscard ( this->mVertexShader, shouldDelete );
	ZLGfxResourceClerk::DeleteOrDiscard ( this->mFragmentShader, shouldDelete );
	ZLGfxResourceClerk::DeleteOrDiscard ( this->mProgram, shouldDelete );
}

//----------------------------------------------------------------//
void ZLShaderProgram::ZLAbstractGfxResource_OnGPUUnbind () {

	MOAIGfxMgr::GetDrawingAPI ().UseProgram ( ZLGfxResource::UNBIND );
}

//----------------------------------------------------------------//
bool ZLShaderProgram::ZLAbstractGfxResource_OnGPUUpdate () {

	return true;
}

//----------------------------------------------------------------//
ZLShaderUniformHandle ZLShaderProgram::ZLAbstractShaderUniformSchema_GetUniformHandle ( void* buffer, ZLIndex uniformID ) const {

	ZLShaderUniformHandle uniform;
	uniform.mBuffer = 0;

	if ( uniformID < this->mUniforms.Size ()) {
		const ZLShaderUniform& programUniform = this->mUniforms [ uniformID ];
		uniform.mType		= programUniform.mType;
		uniform.mWidth		= programUniform.mWidth;
		uniform.mBuffer		= ( void* )(( size_t )buffer + this->mUniforms [ uniformID ].mCPUOffset );
	}
	return uniform;
}

//----------------------------------------------------------------//
void ZLShaderProgram::ZLGfxListener_OnUniformLocation ( u32 addr, void* userdata ) {

	ZLSize i = ( size_t )userdata;
	
	if ( i < this->mUniforms.Size ()) {
		this->mUniforms [ ZLIndexCast ( i )].mGPUBase = addr;
	}
}
