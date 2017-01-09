// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// MOAIShaderProgramGlobal
//================================================================//
	
//----------------------------------------------------------------//
MOAIShaderProgramGlobal::MOAIShaderProgramGlobal () :
	mGlobalID ( INVALID_INDEX ),
	mUniformID ( INVALID_INDEX ),
	mIndex ( 0 ) {
}

//================================================================//
// MOAIShaderGlobals
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderGlobals::CopyGlobals ( const MOAIShaderGlobals& globals ) {

	this->mGlobals.CloneFrom ( globals.mGlobals );
}

//----------------------------------------------------------------//
void MOAIShaderGlobals::ReserveGlobals ( u32 nGlobals ) {

	this->mGlobals.Init ( nGlobals );
}

//----------------------------------------------------------------//
int MOAIShaderGlobals::ReserveGlobals ( lua_State* L, int idx ) {

	MOAILuaState state ( L );

	u32 nGlobals = state.GetValue < u32 >( idx, 0 );
	this->ReserveGlobals ( nGlobals );

	return 0;
}

//----------------------------------------------------------------//
void MOAIShaderGlobals::SetGlobal ( u32 idx, u32 globalID, u32 uniformID, u32 index ) {
	
	MOAIShaderProgramGlobal& global = this->mGlobals [ idx ];
	
	global.mUniformID	= uniformID;
	global.mIndex		= index;
	global.mGlobalID	= globalID;
}

//----------------------------------------------------------------//
int MOAIShaderGlobals::SetGlobal ( lua_State* L, int idx ) {

	MOAILuaState state ( L );

	u32 globalIdx	= state.GetValue < u32 >( idx, 1 ) - 1;
	u32 globalID	= state.GetValue < u32 >( idx + 1, INVALID_INDEX );
	u32 uniformID	= state.GetValue < u32 >( idx + 2, 1 ) - 1;
	u32 index		= state.GetValue < u32 >( idx + 3, 1 ) - 1;
	
	this->SetGlobal ( globalIdx, globalID, uniformID, index );

	return 0;
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	declareUniform
	@text	Declares a uniform mapping.

	@in		MOAIShaderProgram self
	@in		number idx
	@in		string name
	@opt	number type		One of MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_TYPE_INDEX, MOAIShaderProgram.UNIFORM_TYPE_INT,
							MOAIShaderProgram.UNIFORM_TYPE_MATRIX_3X3, MOAIShaderProgram.UNIFORM_TYPE_MATRIX_4X4
	@opt	number width	Used for vector uniforms. Default value is 1. Should be no greather than 4.
	@opt	number count	Declare an array of uniforms. Default value is 1.
	@out	nil
*/
int MOAIShaderProgram::_declareUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNSN" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString name		= state.GetValue < cc8* >( 3, "" );
	u32 type			= state.GetValue < u32 >( 4, MOAIShaderUniform::UNIFORM_TYPE_FLOAT );
	u32 width			= state.GetValue < u32 >( 5, 1 );
	u32 count			= state.GetValue < u32 >( 6, 1 );

	self->DeclareUniform ( idx, name, type, width, count );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	load
	@text	Load a shader program.

	@in		MOAIShaderProgram self
	@in		string vertexShaderSource
	@in		string fragmentShaderSource
	@out	nil
*/
int MOAIShaderProgram::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "USS" )

	cc8* vtxSource	= state.GetValue < cc8* >( 2, 0 );
	cc8* frgSource	= state.GetValue < cc8* >( 3, 0 );

	self->Load ( vtxSource, frgSource );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgram::_reserveGlobals ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	return self->ReserveGlobals ( L, 2 );
}

//----------------------------------------------------------------//
/**	@lua	reserveUniforms
	@text	Reserve shader uniforms.

	@in		MOAIShaderProgram self
	@opt	number nUniforms	Default value is 0.
	@out	nil
*/
int MOAIShaderProgram::_reserveUniforms ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	u32 nUniforms = state.GetValue < u32 >( 2, 0 );
	self->ReserveUniforms ( nUniforms );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgram::_setGlobal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNNN" )
	
	return self->SetGlobal ( L, 2);
}

//----------------------------------------------------------------//
/**	@lua	setVertexAttribute
	@text	Names a shader vertex attribute.

	@in		MOAIShaderProgram self
	@in		number index	Default value is 1.
	@in		string name		Name of attribute.
	@out	nil
*/
int MOAIShaderProgram::_setVertexAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNS" )

	u32 idx					= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString attribute		= state.GetValue < cc8* >( 3, "" );

	self->SetVertexAttribute ( idx, attribute );

	return 0;
}

//================================================================//
// MOAIShaderProgram
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderProgram::Clear () {

	this->mVertexShaderSource.clear ();
	this->mFragmentShaderSource.clear ();

	this->mAttributeMap.clear ();
	this->mUniforms.Clear ();
	this->mGlobals.Clear ();
	
	this->Destroy ();
}

//----------------------------------------------------------------//
ZLGfxHandle* MOAIShaderProgram::CompileShader ( u32 type, cc8* source ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	ZLGfx& gfx = gfxMgr.GetDrawingAPI ();

	ZLGfxHandle* shader = gfx.CreateShader ( type );

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
		gfx.Delete ( shader );
	}

	gfx.PopSection ();

	return shader;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::DeclareUniform ( u32 idx, cc8* name, u32 type, u32 width, u32 count ) {

	if ( idx < this->mUniforms.Size ()) {

		MOAIShaderUniform& uniform = this->mUniforms [ idx ];
		uniform.mName = name;
		uniform.Init ( type, width, count );
	}
}

//----------------------------------------------------------------//
u32 MOAIShaderProgram::GetGlobalsMask () {

	if ( this->mGlobals.Size () && !this->mGlobalsMask ) {
		for ( u32 i = 0; i < this->mGlobals.Size (); ++i ) {
			const MOAIShaderProgramGlobal& global = this->mGlobals [ i ];
			this->mGlobalsMask |= MOAIGfxGlobalsCache::GetAttrFlagForID ( global.mGlobalID );
		}
	}
	return this->mGlobalsMask;
}

//----------------------------------------------------------------//
MOAIShaderUniform* MOAIShaderProgram::GetUniform ( u32 uniformID ) {

	return uniformID < this->mUniforms.Size () ? &this->mUniforms [ uniformID ] : 0;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::Load ( cc8* vshSource, cc8* fshSource ) {

	if ( vshSource && fshSource ) {

		this->mVertexShaderSource = vshSource;
		this->mFragmentShaderSource = fshSource;
		
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
MOAIShaderProgram::MOAIShaderProgram () :
	mProgram ( 0 ),
	mVertexShader ( 0 ),
	mFragmentShader ( 0 ),
	mGlobalsMask ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderProgram::~MOAIShaderProgram () {

	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAIShaderProgram::OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnGPUBind () {

	// use shader program.
	MOAIGfxMgr::GetDrawingAPI ().UseProgram ( this->mProgram );
}

//----------------------------------------------------------------//
bool MOAIShaderProgram::OnGPUCreate () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	this->mVertexShader = this->CompileShader ( ZGL_SHADER_TYPE_VERTEX, this->mVertexShaderSource );
	this->mFragmentShader = this->CompileShader ( ZGL_SHADER_TYPE_FRAGMENT, this->mFragmentShaderSource );
	this->mProgram = gfx.CreateProgram ();

	if ( !( this->mVertexShader && this->mFragmentShader && this->mProgram )) {
		this->Clear ();
		return false;
	}

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
	for ( u32 i = 0; i < this->mUniforms.Size (); ++i ) {
		MOAIShaderUniform& uniform = this->mUniforms [ i ];
		gfx.GetUniformLocation ( this->mProgram, uniform.mName, this, ( void* )(( size_t )i )); // TODO: cast?
		
	}

	gfx.Delete ( this->mVertexShader );
	this->mVertexShader = 0;

	gfx.Delete ( this->mFragmentShader );
	this->mFragmentShader = 0;

	//AJV TODO - does the attribute map ever need to be cleared?
	//this->mAttributeMap.clear ();
	
	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	MOAIGfxResourceClerk::DeleteOrDiscardHandle ( this->mVertexShader, shouldDelete );
	MOAIGfxResourceClerk::DeleteOrDiscardHandle ( this->mFragmentShader, shouldDelete );
	MOAIGfxResourceClerk::DeleteOrDiscardHandle ( this->mProgram, shouldDelete );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnGPUUnbind () {

	MOAIGfxMgr::GetDrawingAPI ().UseProgram ( 0 );
}

//----------------------------------------------------------------//
bool MOAIShaderProgram::OnGPUUpdate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnUniformLocation ( u32 addr, void* userdata ) {

	size_t i = ( size_t )userdata;
	
	if ( i < this->mUniforms.Size ()) {
		this->mUniforms [ i ].mGPUBase = addr;
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgram::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaClass ( state );

	state.SetField ( -1, "UNIFORM_TYPE_FLOAT",					( u32 )MOAIShaderUniform::UNIFORM_TYPE_FLOAT );
	state.SetField ( -1, "UNIFORM_TYPE_INT",					( u32 )MOAIShaderUniform::UNIFORM_TYPE_INT );
	
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_2",					( u32 )MOAIShaderUniform::UNIFORM_WIDTH_VEC_2 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_3",					( u32 )MOAIShaderUniform::UNIFORM_WIDTH_VEC_3 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_4",					( u32 )MOAIShaderUniform::UNIFORM_WIDTH_VEC_4 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_3X3",			( u32 )MOAIShaderUniform::UNIFORM_WIDTH_MATRIX_3X3 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_4X4",			( u32 )MOAIShaderUniform::UNIFORM_WIDTH_MATRIX_4X4 );
	
	state.SetField ( -1, "GLOBAL_WORLD_INVERSE",				( u32 )MOAIGfxGlobalsCache::INVERSE_WORLD_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_VIEW_INVERSE",			( u32 )MOAIGfxGlobalsCache::INVERSE_WORLD_VIEW_MTX );
	
	state.SetField ( -1, "GLOBAL_PROJ",							( u32 )MOAIGfxGlobalsCache::PROJ_MTX );
	state.SetField ( -1, "GLOBAL_VIEW",							( u32 )MOAIGfxGlobalsCache::VIEW_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_PROJ",					( u32 )MOAIGfxGlobalsCache::VIEW_PROJ_MTX );
	state.SetField ( -1, "GLOBAL_WORLD",						( u32 )MOAIGfxGlobalsCache::WORLD_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_NORMAL",					( u32 )MOAIGfxGlobalsCache::WORLD_NORMAL_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_VIEW",					( u32 )MOAIGfxGlobalsCache::WORLD_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_VIEW_NORMAL",			( u32 )MOAIGfxGlobalsCache::WORLD_VIEW_NORMAL_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_VIEW_PROJ",				( u32 )MOAIGfxGlobalsCache::WORLD_VIEW_PROJ_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_VIEW_PROJ_NORMAL",		( u32 )MOAIGfxGlobalsCache::WORLD_VIEW_PROJ_NORMAL_MTX );
	
	state.SetField ( -1, "GLOBAL_PEN_COLOR",					( u32 )MOAIGfxGlobalsCache::PEN_COLOR );
	
	state.SetField ( -1, "GLOBAL_VIEW_HEIGHT",					( u32 )MOAIGfxGlobalsCache::VIEW_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_WIDTH",					( u32 )MOAIGfxGlobalsCache::VIEW_WIDTH );
	
	state.SetField ( -1, "GLOBAL_VIEW_HALF_HEIGHT",				( u32 )MOAIGfxGlobalsCache::VIEW_HALF_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_HALF_WIDTH",				( u32 )MOAIGfxGlobalsCache::VIEW_HALF_WIDTH );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "declareUniform",				_declareUniform },
		{ "load",						_load },
		{ "reserveGlobals",				_reserveGlobals },
		{ "reserveUniforms",			_reserveUniforms },
		{ "setGlobal",					_setGlobal },
		{ "setVertexAttribute",			_setVertexAttribute },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::ReserveUniforms ( u32 nUniforms ) {

	this->mUniforms.Init ( nUniforms );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::SetVertexAttribute ( u32 idx, cc8* attribute ) {

	if ( attribute ) {
		this->mAttributeMap [ idx ] = attribute;
	}
}
