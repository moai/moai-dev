// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIShaderProgramGL.h>
#include <moai-gfx-gl/MOAITextureGL.h>

//================================================================//
// MOAIShaderProgramGlobalGL
//================================================================//
	
//----------------------------------------------------------------//
MOAIShaderProgramGlobalGL::MOAIShaderProgramGlobalGL () :
	mGlobalID ( ZLIndexOp::INVALID ),
	mUniformID ( ZLIndexOp::INVALID ),
	mIndex ( ZLIndexOp::ZERO ) {
}

//================================================================//
// MOAIShaderProgramTextureGL
//================================================================//

//----------------------------------------------------------------//
MOAIShaderProgramTextureGL::MOAIShaderProgramTextureGL () :
	mName ( MOAI_UNKNOWN_MATERIAL_GLOBAL ),
	mUnit ( ZLIndexOp::ZERO ) {
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	declareUniform
	@text	Declares a uniform mapping.

	@in		MOAIShaderProgramGL self
	@in		number idx
	@in		string name
	@opt	number type		One of MOAIShaderProgramGL.UNIFORM_TYPE_FLOAT, MOAIShaderProgramGL.UNIFORM_TYPE_INDEX, MOAIShaderProgramGL.UNIFORM_TYPE_INT,
							MOAIShaderProgramGL.UNIFORM_TYPE_MATRIX_3X3, MOAIShaderProgramGL.UNIFORM_TYPE_MATRIX_4X4
	@opt	number width	Used for vector uniforms. Default value is 1. Should be no greather than 4.
	@opt	number count	Declare an array of uniforms. Default value is 1.
	@out	nil
*/
int MOAIShaderProgramGL::_declareUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgramGL, "UNSN" )

	ZLIndex idx			= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	STLString name		= state.GetValue < cc8* >( 3, "" );
	u32 type			= state.GetValue < u32 >( 4, MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT );
	u32 width			= state.GetValue < u32 >( 5, 1 );
	u32 count			= state.GetValue < u32 >( 6, 1 );

	self->DeclareUniform ( idx, name, type, width, count );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	load
	@text	Load a shader program.

	@in		MOAIShaderProgramGL self
	@in		string vertexShaderSource
	@in		string fragmentShaderSource
	@out	nil
*/
int MOAIShaderProgramGL::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgramGL, "USS" )

	cc8* vtxSource	= state.GetValue < cc8* >( 2, 0 );
	cc8* frgSource	= state.GetValue < cc8* >( 3, 0 );

	self->Load ( vtxSource, frgSource );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgramGL::_reserveGlobals ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgramGL, "U" )

//	return self->ReserveGlobals ( L, 2 );
	
	u32 nGlobals = state.GetValue < u32 >( 2, 0 );
	self->ReserveGlobals ( nGlobals );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgramGL::_reserveTextures ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgramGL, "U" )

	self->ReserveTextures ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveUniforms
	@text	Reserve shader uniforms.

	@in		MOAIShaderProgramGL self
	@opt	number nUniforms	Default value is 0.
	@out	nil
*/
int MOAIShaderProgramGL::_reserveUniforms ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgramGL, "U" )

	u32 nUniforms = state.GetValue < u32 >( 2, 0 );
	self->ReserveUniforms ( nUniforms );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgramGL::_setGlobal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgramGL, "UNNN" )

	ZLIndex globalIdx	= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 globalID		= state.GetValue < u32 >( 3, MOAIGfxMgrGL::NULL_GLOBAL );
	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 4, ZLIndexOp::ZERO );
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 5, ZLIndexOp::ZERO );
	
	self->SetGlobal ( globalIdx, globalID, uniformID, index );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgramGL::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgramGL, "U" )
	
	ZLIndex idx		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	ZLIndex unit	= state.GetValue < MOAILuaIndex >( 3, ZLIndexOp::ZERO );
	
	if ( state.IsType ( 3, LUA_TUSERDATA )) {
	
		self->SetTexture ( idx, state.GetLuaObject < MOAITextureGL >( 3, true ), unit );
	}
	else {
	
		self->SetTexture (
			idx,
			state.GetValue < u32 >( 3, MOAI_UNKNOWN_MATERIAL_GLOBAL + 1 ) - 1,
			unit,
			state.GetLuaObject < MOAITextureGL >( 5, false )
		);
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setVertexAttribute
	@text	Names a shader vertex attribute.

	@in		MOAIShaderProgramGL self
	@in		number index	Default value is 1.
	@in		string name		Name of attribute.
	@out	nil
*/
int MOAIShaderProgramGL::_setVertexAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgramGL, "UNS" )

	ZLIndex idx				= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	STLString attribute		= state.GetValue < cc8* >( 3, "" );

	self->SetVertexAttribute ( idx, attribute );

	return 0;
}

//================================================================//
// MOAIShaderProgramGL
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderProgramGL::AffirmUniforms () {

	if ( this->mUniformBufferSize ) return;
	
	this->mUniformBufferSize = 0;
	size_t nUniforms = this->mUniforms.Size ();
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
		
		MOAIShaderUniformGL& uniform = this->mUniforms [ i ];
		uniform.mCPUOffset = this->mUniformBufferSize;
		this->mUniformBufferSize += uniform.GetSize ();
		
		this->mMaxCount = this->mMaxCount < uniform.mCount ? uniform.mCount : this->mMaxCount;
	}
	
	this->mUniformBuffer.Clear ();
	this->mUniformBuffer.Init ( this->mUniformBufferSize );
	this->mUniformBuffer.Fill ( 0xff );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ApplyUniforms ( ZLLeanArray < u8 >& buffer ) {

	this->mUniformBuffer.CopyFrom ( buffer);
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::BindUniforms () {
	
	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
	
	size_t nUniforms = this->mUniforms.Size ();
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
	
		MOAIShaderUniformHandle uniform = this->GetUniformHandle ( this->mUniformBuffer.GetBuffer (), i );
		
		if ( uniform.IsValid ()) {
			this->mUniforms [ i ].Bind ( gfx, uniform.mBuffer );
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::Clear () {

	this->mVertexShaderSource.clear ();
	this->mFragmentShaderSource.clear ();

	this->mAttributeMap.clear ();
	this->mUniforms.Clear ();
	this->mGlobals.Clear ();
	
	this->Destroy ();
}

//----------------------------------------------------------------//
ZLGfxHandle MOAIShaderProgramGL::CompileShader ( u32 type, cc8* source ) {

	MOAIGfxMgrGL& gfxMgr = *this->mGfxMgr;
	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

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
void MOAIShaderProgramGL::DeclareUniform ( ZLIndex idx, cc8* name, u32 type, u32 width, u32 count ) {

	if ( idx < this->mUniforms.Size ()) {

		MOAIShaderUniformGL& uniform = this->mUniforms [ idx ];
		uniform.mName = name;
		uniform.Init ( type, width, count );
	}
}

//----------------------------------------------------------------//
MOAIShaderUniformGL* MOAIShaderProgramGL::GetUniform ( ZLIndex uniformID ) {

	return uniformID < this->mUniforms.Size () ? &this->mUniforms [ uniformID ] : 0;
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::InitUniformBuffer ( ZLLeanArray < u8 >& buffer ) {

	this->AffirmUniforms ();

	buffer.Clear ();
	
	size_t nUniforms = this->mUniforms.Size ();
	buffer.Init ( this->mUniformBufferSize );
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
		MOAIShaderUniformHandle uniform = this->GetUniformHandle ( buffer.GetBuffer (), i );
		uniform.Default ( this->mUniforms [ i ].mCount );
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::Load ( cc8* vshSource, cc8* fshSource ) {

	if ( vshSource && fshSource ) {

		this->mVertexShaderSource = vshSource;
		this->mFragmentShaderSource = fshSource;
		
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
MOAIShaderProgramGL::MOAIShaderProgramGL () :
	mMaxCount ( 0 ),
	mUniformBufferSize ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAbstractGfxResourceGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderProgramGL::~MOAIShaderProgramGL () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAbstractGfxResourceGL::RegisterLuaClass ( state );

	state.SetField ( -1, "UNIFORM_TYPE_FLOAT",						( u32 )MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT );
	state.SetField ( -1, "UNIFORM_TYPE_INT",						( u32 )MOAIShaderUniformGL::UNIFORM_TYPE_INT );
	
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_2",						( u32 )MOAIShaderUniformGL::UNIFORM_WIDTH_VEC_2 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_3",						( u32 )MOAIShaderUniformGL::UNIFORM_WIDTH_VEC_3 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_4",						( u32 )MOAIShaderUniformGL::UNIFORM_WIDTH_VEC_4 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_3X3",				( u32 )MOAIShaderUniformGL::UNIFORM_WIDTH_MATRIX_3X3 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_4X4",				( u32 )MOAIShaderUniformGL::UNIFORM_WIDTH_MATRIX_4X4 );
	
	state.SetField ( -1, "GLOBAL_CLIP_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgrGL::CLIP_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_MODEL_MTX",				( u32 )MOAIGfxMgrGL::CLIP_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_VIEW_MTX",					( u32 )MOAIGfxMgrGL::CLIP_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_WINDOW_MTX",				( u32 )MOAIGfxMgrGL::CLIP_TO_WINDOW_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_WORLD_MTX",				( u32 )MOAIGfxMgrGL::CLIP_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_DISPLAY_TO_CLIP_MTX",				( u32 )MOAIGfxMgrGL::DISPLAY_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_MODEL_MTX",				( u32 )MOAIGfxMgrGL::DISPLAY_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_VIEW_MTX",				( u32 )MOAIGfxMgrGL::DISPLAY_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_WORLD_MTX",				( u32 )MOAIGfxMgrGL::DISPLAY_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_MODEL_TO_CLIP_MTX",				( u32 )MOAIGfxMgrGL::MODEL_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgrGL::MODEL_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_UV_MTX",					( u32 )MOAIGfxMgrGL::MODEL_TO_UV_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_VIEW_MTX",				( u32 )MOAIGfxMgrGL::MODEL_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_WORLD_MTX",				( u32 )MOAIGfxMgrGL::MODEL_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_CLIP_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_MODEL_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_CLIP_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_VIEW_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_CLIP_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WINDOW_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_CLIP_TO_WINDOW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WORLD_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_CLIP_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_CLIP_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_DISPLAY_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_MODEL_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_DISPLAY_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_VIEW_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_DISPLAY_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_WORLD_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_DISPLAY_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_CLIP_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_MODEL_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_MODEL_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_UV_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_MODEL_TO_UV_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_VIEW_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_MODEL_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_WORLD_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_MODEL_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_WORLD_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_VIEW_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_WORLD_TO_VIEW_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_UV_TO_MODEL_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_UV_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_CLIP_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_VIEW_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_MODEL_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_VIEW_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_WORLD_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_VIEW_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_WINDOW_TO_CLIP_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_WINDOW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_CLIP_MTX",			( u32 )MOAIGfxMgrGL::NORMAL_WORLD_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_MODEL_MTX",		( u32 )MOAIGfxMgrGL::NORMAL_WORLD_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_PEN_COLOR",						( u32 )MOAIGfxMgrGL::PEN_COLOR );

	state.SetField ( -1, "GLOBAL_UV_TO_MODEL_MTX",					( u32 )MOAIGfxMgrGL::UV_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_VIEW_TO_CLIP_MTX",					( u32 )MOAIGfxMgrGL::VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgrGL::VIEW_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_MODEL_MTX",				( u32 )MOAIGfxMgrGL::VIEW_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_WORLD_MTX",				( u32 )MOAIGfxMgrGL::VIEW_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_WINDOW_TO_CLIP_MTX",				( u32 )MOAIGfxMgrGL::WINDOW_TO_CLIP_MTX );

	state.SetField ( -1, "GLOBAL_WORLD_TO_CLIP_MTX",				( u32 )MOAIGfxMgrGL::WORLD_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgrGL::WORLD_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_MODEL_MTX",				( u32 )MOAIGfxMgrGL::WORLD_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_VIEW_MTX",				( u32 )MOAIGfxMgrGL::WORLD_TO_VIEW_MTX );

	state.SetField ( -1, "GLOBAL_VIEW_HEIGHT",						( u32 )MOAIGfxMgrGL::VIEW_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_WIDTH",						( u32 )MOAIGfxMgrGL::VIEW_WIDTH );
	
	state.SetField ( -1, "GLOBAL_VIEW_HALF_HEIGHT",					( u32 )MOAIGfxMgrGL::VIEW_HALF_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_HALF_WIDTH",					( u32 )MOAIGfxMgrGL::VIEW_HALF_WIDTH );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAbstractGfxResourceGL::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "declareUniform",				_declareUniform },
		{ "load",						_load },
		{ "reserveGlobals",				_reserveGlobals },
		{ "reserveTextures",			_reserveTextures },
		{ "reserveUniforms",			_reserveUniforms },
		{ "setGlobal",					_setGlobal },
		{ "setTexture",					_setTexture },
		{ "setVertexAttribute",			_setVertexAttribute },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ReserveGlobals ( ZLSize nGlobals ) {

	this->mGlobals.Init ( nGlobals );
}

//----------------------------------------------------------------//
int MOAIShaderProgramGL::ReserveGlobals ( lua_State* L, int idx ) {

	MOAILuaState state ( L );

	u32 nGlobals = state.GetValue < u32 >( idx, 0 );
	this->ReserveGlobals ( nGlobals );

	return 0;
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ReserveTextures ( ZLSize nTextures ) {

	this->mTextures.Init ( nTextures );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ReserveUniforms ( ZLSize nUniforms ) {

	this->mUniforms.Init ( nUniforms );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ScheduleTextures () {

	// TODO: ZLGfx

//	MOAIGfxMgrGL& gfxMgr = this->mGfxMgr;
//	MOAIMaterialMgr& materialStack = MOAIMaterialMgr::Get ();
//
//	size_t nTextures = this->mTextures.Size ();
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < nTextures; ++i ) {
//
//		MOAIShaderProgramTextureGL& shaderTexture = this->mTextures [ i ];
//
//		MOAITextureGL* texture = shaderTexture.mTexture;
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
void MOAIShaderProgramGL::SetGlobal ( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index ) {
	
	MOAIShaderProgramGlobalGL& global = this->mGlobals [ idx ];
	
	global.mUniformID	= uniformID;
	global.mIndex		= index;
	global.mGlobalID	= globalID;
}

//----------------------------------------------------------------//
//int MOAIShaderProgramGL::SetGlobal ( lua_State* L, int idx ) {
//
//	MOAILuaState state ( L );
//
//	ZLIndex globalIdx	= state.GetValue < MOAILuaIndex >( idx, ZLIndexOp::ZERO );
//	u32 globalID		= state.GetValue < u32 >( idx + 1, MOAIGfxMgrGL::NULL_GLOBAL );
//	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( idx + 2, ZLIndexOp::ZERO );
//	ZLIndex index		= state.GetValue < MOAILuaIndex >( idx + 3, ZLIndexOp::ZERO );
//
//	this->SetGlobal ( globalIdx, globalID, uniformID, index );
//
//	return 0;
//}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::SetTexture ( ZLIndex idx, u32 name, ZLIndex unit, MOAITextureGL* fallback ) {

	if ( idx < this->mTextures.Size ()) {
	
		MOAIShaderProgramTextureGL& shaderTexture = this->mTextures [ idx ];
		shaderTexture.mName = name;
		shaderTexture.mUnit = unit;
		shaderTexture.mTexture = fallback;
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::SetTexture ( ZLIndex idx, MOAITextureGL* texture, ZLIndex unit ) {

	if ( idx < this->mTextures.Size ()) {
	
		MOAIShaderProgramTextureGL& shaderTexture = this->mTextures [ idx ];
		shaderTexture.mName = MOAI_UNKNOWN_MATERIAL_GLOBAL;
		shaderTexture.mUnit = unit;
		shaderTexture.mTexture = texture;
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::SetVertexAttribute ( u32 idx, cc8* attribute ) {

	if ( attribute ) {
		this->mAttributeMap [ idx ] = attribute;
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::UpdateUniforms ( ZLLeanArray < u8 >& buffer ) {

	MOAIGfxMgrGL& gfxMgr = *this->mGfxMgr;
	
	ZLRect viewRect = gfxMgr.GetViewRect ();

	// NOTE: matrices are submitted *column major*; it is up to the shader to transform vertices correctly
	// vert * matrix implicitely transposes the matrix; martix * vert uses the matrix as submitted

	u32 nGlobals = this->mGlobals.Size ();

	for ( ZLIndex i = ZLIndexOp::ZERO; i < nGlobals; ++i ) {
	
		const MOAIShaderProgramGlobalGL& global = this->mGlobals [ i ];
		
		if ( global.mUniformID == ZLIndexOp::INVALID ) continue;
		
		MOAIShaderUniformHandle uniform = this->GetUniformHandle ( buffer.GetBuffer (), global.mUniformID, global.mIndex );
		if ( !uniform.IsValid ()) continue;
		
		if ( global.mGlobalID < MOAIGfxMgrGL::TOTAL_MATRICES ) {
		
			uniform.SetValue ( gfxMgr.GetMtx ( global.mGlobalID ));
		}
		else {
		
			switch (( ZLSize )global.mGlobalID ) {
				
				case MOAIGfxMgrGL::PEN_COLOR:
				
					uniform.SetValue ( gfxMgr.GetFinalColor ());
					break;
				
				case MOAIGfxMgrGL::VIEW_HALF_HEIGHT:
				
					uniform.SetValue ( viewRect.Height () * 0.5f );
					break;
					
				case MOAIGfxMgrGL::VIEW_HALF_WIDTH: {
				
					uniform.SetValue ( viewRect.Width () * 0.5f );
					break;
				}
				case MOAIGfxMgrGL::VIEW_HEIGHT:
				
					uniform.SetValue ( viewRect.Height ());
					break;
					
				case MOAIGfxMgrGL::VIEW_WIDTH:
				
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
bool MOAIShaderProgramGL::ZLAbstractGfxResource_OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ZLAbstractGfxResource_OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ZLAbstractGfxResource_OnGPUBind () {

	// use shader program.
	this->mGfxMgr->GetDrawingAPI ().UseProgram ( this->mProgram );
}

//----------------------------------------------------------------//
bool MOAIShaderProgramGL::ZLAbstractGfxResource_OnGPUCreate () {

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
		MOAIShaderUniformGL& uniform = this->mUniforms [ i ];
		gfx.GetUniformLocation ( this->mProgram, uniform.mName, this, ( void* )(( size_t )i )); // TODO: cast?
	}

	gfx.DeleteResource ( this->mVertexShader );
	gfx.DeleteResource ( this->mFragmentShader );

	//AJV TODO - does the attribute map ever need to be cleared?
	//this->mAttributeMap.clear ();
	
	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	this->mGfxMgr->DeleteOrDiscard ( this->mVertexShader, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mFragmentShader, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mProgram, shouldDelete );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ZLAbstractGfxResource_OnGPUUnbind () {

	this->mGfxMgr->GetDrawingAPI ().UseProgram ( ZLGfxResource::UNBIND );
}

//----------------------------------------------------------------//
bool MOAIShaderProgramGL::ZLAbstractGfxResource_OnGPUUpdate () {

	return true;
}

//----------------------------------------------------------------//
MOAIShaderUniformHandle MOAIShaderProgramGL::ZLAbstractShaderUniformSchema_GetUniformHandle ( void* buffer, ZLIndex uniformID ) const {

	MOAIShaderUniformHandle uniform;
	uniform.mBuffer = 0;

	if ( uniformID < this->mUniforms.Size ()) {
		const MOAIShaderUniformGL& programUniform = this->mUniforms [ uniformID ];
		uniform.mType		= programUniform.mType;
		uniform.mWidth		= programUniform.mWidth;
		uniform.mBuffer		= ( void* )(( size_t )buffer + this->mUniforms [ uniformID ].mCPUOffset );
	}
	return uniform;
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ZLGfxListener_OnUniformLocation ( u32 addr, void* userdata ) {

	ZLSize i = ( size_t )userdata;
	
	if ( i < this->mUniforms.Size ()) {
		this->mUniforms [ ZLIndexCast ( i )].mGPUBase = addr;
	}
}
