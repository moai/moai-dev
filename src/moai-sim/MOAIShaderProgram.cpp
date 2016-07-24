// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	clearUniform
	@text	Clears a uniform mapping.

	@in		MOAIShaderProgram self
	@in		number idx
	@out	nil
*/
int MOAIShaderProgram::_clearUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;

	self->ClearUniform ( idx );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareUniform
	@text	Declares a uniform mapping.

	@in		MOAIShaderProgram self
	@in		number idx
	@in		string name
	@opt	number type		One of MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_TYPE_INDEX, MOAIShaderProgram.UNIFORM_TYPE_INT,
							MOAIShaderProgram.UNIFORM_TYPE_MATRIX_3X3, MOAIShaderProgram.UNIFORM_TYPE_MATRIX_4X4
	@opt	number width	Used for vector uniforms. Default value is 1. Should be no greather than 4.
	@out	nil
*/
int MOAIShaderProgram::_declareUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNS" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString name		= state.GetValue < cc8* >( 3, "" );
	u32  type			= state.GetValue < u32 >( 4, MOAIShaderUniform::UNIFORM_TYPE_FLOAT );
	u32  width			= state.GetValue < u32 >( 5, 1 );

	self->DeclareUniform ( idx, name, type, width );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareUniformSampler
	@text	Declares an uniform to be used as a texture unit index. This uniform is
			internally an int, but when loaded into the shader the number one is subtracted
			from its value. This allows the user to maintain consistency with Lua's
			convention of indexing from one.

	@in		MOAIShaderProgram self
	@in		number idx
	@in		string name
	@opt	number textureUnit		Default value is 1.
	@out	nil
*/
//int MOAIShaderProgram::_declareUniformSampler ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNSN" )
//
//	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
//	STLString name		= state.GetValue < cc8* >( 3, "" );
//
//	self->DeclareUniform ( idx, name, MOAIShaderUniform::UNIFORM_TYPE_INDEX, 1 );
//	
//	return 0;
//}

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

	self->SetSource ( vtxSource, frgSource );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgram::_reserveGlobals ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	u32 nGlobals = state.GetValue < u32 >( 2, 0 );
	self->ReserveGlobals ( nGlobals );

	return 0;
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
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	u32 idx			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 uniformID	= state.GetValue < u32 >( 3, 1 ) - 1;
	u32 globalID	= state.GetValue < u32 >( 4, INVALID_INDEX );
	
	self->SetGlobal ( idx, uniformID, globalID );

	return 0;
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
void MOAIShaderProgram::ApplyGlobals () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	ZLRect viewRect = gfxMgr.mGfxState.GetViewRect ();

	// NOTE: matrices are submitted *column major*; it is up to the shader to transform vertices correctly
	// vert * matrix implicitely transposes the matrix; martix * vert uses the matrix as submitted

	for ( u32 i = 0; i < this->mGlobals.Size (); ++i ) {
	
		const MOAIShaderProgramGlobal& global = this->mGlobals [ i ];
		
		if ( global.mUniformID == INVALID_INDEX ) continue;
		
		MOAIShaderUniform& uniform = this->mUniforms [ global.mUniformID ];
		
		if ( global.mGlobalID < MOAIGfxGlobalsCache::TOTAL_MATRICES ) {
		
			uniform.mFlags |= uniform.SetValue ( gfxMgr.mGfxState.GetMtx ( global.mGlobalID ));
		}
		else {
		
			switch ( global.mGlobalID ) {
				
				case MOAIGfxGlobalsCache::PEN_COLOR:
				
					uniform.mFlags |= uniform.SetValue ( gfxMgr.mGfxState.GetFinalColor ());
					break;
				
				case MOAIGfxGlobalsCache::VIEW_HALF_HEIGHT:
				
					uniform.mFlags |= uniform.SetValue ( viewRect.Height () * 0.5f );
					break;
					
				case MOAIGfxGlobalsCache::VIEW_HALF_WIDTH: {
				
					uniform.mFlags |= uniform.SetValue ( viewRect.Width () * 0.5f );
					break;
				}
				case MOAIGfxGlobalsCache::VIEW_HEIGHT:
				
					uniform.mFlags |= uniform.SetValue ( viewRect.Height ());
					break;
					
				case MOAIGfxGlobalsCache::VIEW_WIDTH:
				
					uniform.mFlags |= uniform.SetValue ( viewRect.Width ());
					break;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgram::BindUniforms () {
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	bool flushed = false;
	
	for ( u32 i = 0; i < this->mUniforms.Size (); ++i ) {
		MOAIShaderUniform& uniform = this->mUniforms [ i ];
		
		// NOTE: dirty flag will be cleared only after GFX_EVENT_UPDATED_UNIFORMS
		if ( uniform.IsValid () && ( uniform.mFlags & MOAIShaderUniform::UNIFORM_FLAG_DIRTY )) {
			if ( !flushed ) {
				gfxMgr.mGfxState.GfxStateWillChange ();
				flushed = true;
			}
			uniform.Bind ();
		}
	}
	
	// in multi-threaded gfx mode, drawing frames may get dropped. for this reason, we
	// only clear the dirty flag once we're sure a draw frame has been completed.
	if ( flushed ) {
		ZLGfx& gfx = gfxMgr.GetDrawingAPI ();
		gfx.Event ( this, GFX_EVENT_UPDATED_UNIFORMS, 0 );
	}
}

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
void MOAIShaderProgram::ClearUniform ( u32 idx ) {

	if ( idx < this->mUniforms.Size ()) {
		this->mUniforms [ idx ].Clear ();
	}
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
void MOAIShaderProgram::DeclareUniform ( u32 idx, cc8* name, u32 type, u32 width ) {

	if ( idx < this->mUniforms.Size ()) {

		this->ClearUniform ( idx );

		MOAIShaderUniform& uniform = this->mUniforms [ idx ];
		uniform.mName = name;
		uniform.Init ( type, width );
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
void MOAIShaderProgram::OnGfxEvent ( u32 event, void* userdata ) {

	if ( event == GFX_EVENT_UPDATED_UNIFORMS ) {
	
		// getting this event proves that the draw frame was completed. so now
		// it's safe to clear the dirty flag.
	
		for ( u32 i = 0; i < this->mUniforms.Size (); ++i ) {
			this->mUniforms [ i ].mFlags &= ~MOAIShaderUniform::UNIFORM_FLAG_DIRTY;
		}
	}
	
	MOAIGfxResource::OnGfxEvent ( event, userdata );
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

	// get the uniform locations and clear out the names (no longer needed)
	for ( u32 i = 0; i < this->mUniforms.Size (); ++i ) {
		MOAIShaderUniform& uniform = this->mUniforms [ i ];

		gfx.GetUniformLocation ( this->mProgram, uniform.mName, this, ( void* )(( size_t )i )); // TODO: cast?
		//uniform.ClearValue ();
		uniform.mFlags |= MOAIShaderUniform::UNIFORM_FLAG_DIRTY;
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
		this->mUniforms [ i ].mAddr = addr;
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
		{ "clearUniform",				_clearUniform },
		{ "declareUniform",				_declareUniform },
		//{ "declareUniformSampler",		_declareUniformSampler },
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
void MOAIShaderProgram::ReserveGlobals ( u32 nGlobals ) {

	// clear out the old globals (if any)
	for ( size_t i = 0; i < this->mGlobals.Size (); ++i ) {
		this->SetGlobal (( u32 )i, INVALID_INDEX, INVALID_INDEX ); // TODO: cast
	}

	this->mGlobals.Init ( nGlobals );
	
	for ( size_t i = 0; i < nGlobals; ++i ) {
		this->mGlobals [ i ].mUniformID = INVALID_INDEX;
		this->mGlobals [ i ].mGlobalID = INVALID_INDEX;
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgram::ReserveUniforms ( u32 nUniforms ) {

	this->mUniforms.Init ( nUniforms );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::SetGlobal ( u32 idx, u32 uniformID, u32 globalID ) {

	MOAIShaderProgramGlobal& global = this->mGlobals [ idx ];
	
	// clear out the old flag (if any )
	if ( global.mUniformID != INVALID_INDEX ) {
		this->mUniforms [ global.mUniformID ].mFlags &= ~MOAIShaderUniform::UNIFORM_FLAG_GLOBAL;
	}
	
	uniformID = globalID == INVALID_INDEX ? INVALID_INDEX : uniformID;
	
	if ( uniformID != INVALID_INDEX ) {
	
		// if the uniform is already pointed at a global, clear that global
		if ( this->mUniforms [ uniformID ].mFlags & MOAIShaderUniform::UNIFORM_FLAG_GLOBAL ) {
		
			for ( size_t i = 0; i < this->mGlobals.Size (); ++i ) {
				if ( this->mGlobals [ i ].mUniformID == uniformID ) {
					this->mGlobals [ i ].mUniformID = INVALID_INDEX;
					this->mGlobals [ i ].mGlobalID = INVALID_INDEX;
				}
			}
		}
		
		// set the global flag
		this->mUniforms [ uniformID ].mFlags |= MOAIShaderUniform::UNIFORM_FLAG_GLOBAL;
	}
	
	this->mGlobals [ idx ].mUniformID = uniformID;
	this->mGlobals [ idx ].mGlobalID = globalID;
	
	this->mGlobalsMask = 0;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::SetSource ( cc8* vshSource, cc8* fshSource ) {

	if ( vshSource && fshSource ) {
		this->mVertexShaderSource = vshSource;
		this->mFragmentShaderSource = fshSource;
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgram::SetVertexAttribute ( u32 idx, cc8* attribute ) {

	if ( attribute ) {
		this->mAttributeMap [ idx ] = attribute;
	}
}
