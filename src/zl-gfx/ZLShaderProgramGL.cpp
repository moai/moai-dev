// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfx.h>
#include <zl-gfx/ZLGfxMgrGL.h>
#include <zl-gfx/ZLShaderProgramGL.h>
#include <zl-gfx/ZLTextureBaseGL.h>

//================================================================//
// ZLShaderProgramGlobalGL
//================================================================//
	
//----------------------------------------------------------------//
ZLShaderProgramGlobalGL::ZLShaderProgramGlobalGL () :
	mGlobalID ( ZLIndexOp::INVALID ),
	mUniformID ( ZLIndexOp::INVALID ),
	mIndex ( ZLIndexOp::ZERO ) {
}

//================================================================//
// ZLShaderProgramTextureGL
//================================================================//

//----------------------------------------------------------------//
ZLShaderProgramTextureGL::ZLShaderProgramTextureGL () :
	mName ( ZLShaderProgramGL::MOAI_UNKNOWN_MATERIAL_GLOBAL ),
	mUnit ( ZLIndexOp::ZERO ) {
}

//================================================================//
// ZLShaderProgramGL
//================================================================//

//----------------------------------------------------------------//
void ZLShaderProgramGL::AffirmUniforms () {

	if ( this->mUniformBufferSize ) return;
	
	this->mUniformBufferSize = 0;
	size_t nUniforms = this->mUniforms.Size ();
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
		
		ZLShaderUniformGL& uniform = this->mUniforms [ i ];
		uniform.mCPUOffset = this->mUniformBufferSize;
		this->mUniformBufferSize += uniform.GetSize ();
		
		this->mMaxCount = this->mMaxCount < uniform.mCount ? uniform.mCount : this->mMaxCount;
	}
	
	this->mUniformBuffer.Clear ();
	this->mUniformBuffer.Init ( this->mUniformBufferSize );
	this->mUniformBuffer.Fill ( 0xff );
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ApplyUniforms ( ZLLeanArray < u8 >& buffer ) {

	this->mUniformBuffer.CopyFrom ( buffer);
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::BindUniforms () {
	
	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
	
	size_t nUniforms = this->mUniforms.Size ();
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
	
		ZLShaderUniformHandleGL uniform = this->GetUniformHandle ( this->mUniformBuffer.GetBuffer (), i );
		
		if ( uniform.IsValid ()) {
			this->mUniforms [ i ].Bind ( gfx, uniform.mBuffer );
		}
	}
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::Clear () {

	this->mVertexShaderSource.clear ();
	this->mFragmentShaderSource.clear ();

	this->mAttributeMap.clear ();
	this->mUniforms.Clear ();
	this->mGlobals.Clear ();
	
	this->Destroy ();
}

//----------------------------------------------------------------//
ZLGfxHandleGL ZLShaderProgramGL::CompileShader ( u32 type, cc8* source ) {

	ZLGfxMgrGL& gfxMgr = *this->mGfxMgr;
	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	ZLGfxHandleGL shader = gfx.CreateShader ( type );

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
void ZLShaderProgramGL::DeclareUniform ( ZLIndex idx, cc8* name, u32 type, u32 width, u32 count ) {

	if ( idx < this->mUniforms.Size ()) {

		ZLShaderUniformGL& uniform = this->mUniforms [ idx ];
		uniform.mName = name;
		uniform.Init ( type, width, count );
	}
}

//----------------------------------------------------------------//
ZLShaderUniformGL* ZLShaderProgramGL::GetUniform ( ZLIndex uniformID ) {

	return uniformID < this->mUniforms.Size () ? &this->mUniforms [ uniformID ] : 0;
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::InitUniformBuffer ( ZLLeanArray < u8 >& buffer ) {

	this->AffirmUniforms ();

	buffer.Clear ();
	
	size_t nUniforms = this->mUniforms.Size ();
	buffer.Init ( this->mUniformBufferSize );
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
		ZLShaderUniformHandleGL uniform = this->GetUniformHandle ( buffer.GetBuffer (), i );
		uniform.Default ( this->mUniforms [ i ].mCount );
	}
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::Load ( cc8* vshSource, cc8* fshSource ) {

	if ( vshSource && fshSource ) {

		this->mVertexShaderSource = vshSource;
		this->mFragmentShaderSource = fshSource;
		
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ReserveGlobals ( ZLSize nGlobals ) {

	this->mGlobals.Init ( nGlobals );
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ReserveTextures ( ZLSize nTextures ) {

	this->mTextures.Init ( nTextures );
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ReserveUniforms ( ZLSize nUniforms ) {

	this->mUniforms.Init ( nUniforms );
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ScheduleTextures () {

	// TODO: ZLGfx

//	ZLGfxMgrGL& gfxMgr = this->mGfxMgr;
//	MOAIMaterialMgr& materialStack = MOAIMaterialMgr::Get ();
//
//	size_t nTextures = this->mTextures.Size ();
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < nTextures; ++i ) {
//
//		ZLShaderProgramTextureGL& shaderTexture = this->mTextures [ i ];
//
//		ZLTextureBaseGL* texture = shaderTexture.mTexture;
//
//		// load texture by name
//		if ( !texture ) {
//			texture = materialStack.GetTexture ( shaderTexture.mName );
//		}
//
//		gfx.mGfxState.SetTexture ( texture, shaderTexture.mUnit );
//	}
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::SetGlobal ( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index ) {
	
	ZLShaderProgramGlobalGL& global = this->mGlobals [ idx ];
	
	global.mUniformID	= uniformID;
	global.mIndex		= index;
	global.mGlobalID	= globalID;
}

//----------------------------------------------------------------//
//int ZLShaderProgramGL::SetGlobal ( lua_State* L, int idx ) {
//
//	MOAILuaState state ( L );
//
//	ZLIndex globalIdx	= state.GetValue < MOAILuaIndex >( idx, ZLIndexOp::ZERO );
//	u32 globalID		= state.GetValue < u32 >( idx + 1, ZLGfxMgrGL::NULL_GLOBAL );
//	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( idx + 2, ZLIndexOp::ZERO );
//	ZLIndex index		= state.GetValue < MOAILuaIndex >( idx + 3, ZLIndexOp::ZERO );
//
//	this->SetGlobal ( globalIdx, globalID, uniformID, index );
//
//	return 0;
//}

//----------------------------------------------------------------//
void ZLShaderProgramGL::SetTexture ( ZLIndex idx, u32 name, ZLIndex unit, ZLTextureBaseGL* fallback ) {

	if ( idx < this->mTextures.Size ()) {
	
		ZLShaderProgramTextureGL& shaderTexture = this->mTextures [ idx ];
		shaderTexture.mName = name;
		shaderTexture.mUnit = unit;
		shaderTexture.mTexture = fallback;
	}
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::SetTexture ( ZLIndex idx, ZLTextureBaseGL* texture, ZLIndex unit ) {

	if ( idx < this->mTextures.Size ()) {
	
		ZLShaderProgramTextureGL& shaderTexture = this->mTextures [ idx ];
		shaderTexture.mName = MOAI_UNKNOWN_MATERIAL_GLOBAL;
		shaderTexture.mUnit = unit;
		shaderTexture.mTexture = texture;
	}
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::SetVertexAttribute ( u32 idx, cc8* attribute ) {

	if ( attribute ) {
		this->mAttributeMap [ idx ] = attribute;
	}
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::UpdateUniforms ( ZLLeanArray < u8 >& buffer ) {

	ZLGfxMgrGL& gfxMgr = *this->mGfxMgr;
	
	ZLRect viewRect = gfxMgr.GetViewRect ();

	// NOTE: matrices are submitted *column major*; it is up to the shader to transform vertices correctly
	// vert * matrix implicitely transposes the matrix; martix * vert uses the matrix as submitted

	u32 nGlobals = this->mGlobals.Size ();

	for ( ZLIndex i = ZLIndexOp::ZERO; i < nGlobals; ++i ) {
	
		const ZLShaderProgramGlobalGL& global = this->mGlobals [ i ];
		
		if ( global.mUniformID == ZLIndexOp::INVALID ) continue;
		
		ZLShaderUniformHandleGL uniform = this->GetUniformHandle ( buffer.GetBuffer (), global.mUniformID, global.mIndex );
		if ( !uniform.IsValid ()) continue;
		
		if ( global.mGlobalID < ZLGfxMgrGL::TOTAL_MATRICES ) {
		
			uniform.SetValue ( gfxMgr.GetMtx ( global.mGlobalID ));
		}
		else {
		
			switch (( ZLSize )global.mGlobalID ) {
				
				case ZLGfxMgrGL::PEN_COLOR:
				
					uniform.SetValue ( gfxMgr.GetFinalColor ());
					break;
				
				case ZLGfxMgrGL::VIEW_HALF_HEIGHT:
				
					uniform.SetValue ( viewRect.Height () * 0.5f );
					break;
					
				case ZLGfxMgrGL::VIEW_HALF_WIDTH: {
				
					uniform.SetValue ( viewRect.Width () * 0.5f );
					break;
				}
				case ZLGfxMgrGL::VIEW_HEIGHT:
				
					uniform.SetValue ( viewRect.Height ());
					break;
					
				case ZLGfxMgrGL::VIEW_WIDTH:
				
					uniform.SetValue ( viewRect.Width ());
					break;
			}
		}
	}
}

//----------------------------------------------------------------//
ZLShaderProgramGL::ZLShaderProgramGL () :
	mMaxCount ( 0 ),
	mUniformBufferSize ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( ZLAbstractGfxResourceGL )
		RTTI_EXTEND ( ZLAbstractShaderUniformSchemaGL )
	RTTI_END
}

//----------------------------------------------------------------//
ZLShaderProgramGL::~ZLShaderProgramGL () {

	this->Clear ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool ZLShaderProgramGL::ZLAbstractGfxResource_OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ZLAbstractGfxResource_OnCPUDestroy () {
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ZLAbstractGfxResource_OnGPUBind () {

	// use shader program.
	this->mGfxMgr->GetDrawingAPI ().UseProgram ( this->mProgram );
}

//----------------------------------------------------------------//
bool ZLShaderProgramGL::ZLAbstractGfxResource_OnGPUCreate () {

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

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
		ZLShaderUniformGL& uniform = this->mUniforms [ i ];
		gfx.GetUniformLocation ( this->mProgram, uniform.mName, this, ( void* )(( size_t )i )); // TODO: cast?
	}

	gfx.DeleteResource ( this->mVertexShader );
	gfx.DeleteResource ( this->mFragmentShader );

	//AJV TODO - does the attribute map ever need to be cleared?
	//this->mAttributeMap.clear ();
	
	return true;
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	this->mGfxMgr->DeleteOrDiscard ( this->mVertexShader, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mFragmentShader, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mProgram, shouldDelete );
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ZLAbstractGfxResource_OnGPUUnbind () {

	this->mGfxMgr->GetDrawingAPI ().UseProgram ( ZLGfxResourceGL::UNBIND );
}

//----------------------------------------------------------------//
bool ZLShaderProgramGL::ZLAbstractGfxResource_OnGPUUpdate () {

	return true;
}

//----------------------------------------------------------------//
ZLShaderUniformHandleGL ZLShaderProgramGL::ZLAbstractShaderUniformSchema_GetUniformHandle ( void* buffer, ZLIndex uniformID ) const {

	ZLShaderUniformHandleGL uniform;
	uniform.mBuffer = 0;

	if ( uniformID < this->mUniforms.Size ()) {
		const ZLShaderUniformGL& programUniform = this->mUniforms [ uniformID ];
		uniform.mType		= programUniform.mType;
		uniform.mWidth		= programUniform.mWidth;
		uniform.mBuffer		= ( void* )(( size_t )buffer + this->mUniforms [ uniformID ].mCPUOffset );
	}
	return uniform;
}

//----------------------------------------------------------------//
void ZLShaderProgramGL::ZLGfxListener_OnUniformLocation ( u32 addr, void* userdata ) {

	ZLSize i = ( size_t )userdata;
	
	if ( i < this->mUniforms.Size ()) {
		this->mUniforms [ ZLIndexCast ( i )].mGPUBase = addr;
	}
}
