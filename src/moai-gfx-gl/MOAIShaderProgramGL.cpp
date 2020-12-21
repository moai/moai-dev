// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIShaderProgramGL.h>
#include <moai-gfx-gl/MOAITextureGL.h>

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

	ZLIndex idx			= state.GetValue < MOAILuaIndex >( 2, 0 );
	STLString name		= state.GetValue < cc8* >( 3, "" );
	u32 type			= state.GetValue < u32 >( 4, MOAIUniformDescriptor::UNIFORM_TYPE_FLOAT );
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
/**	@lua	setVertexAttribute
	@text	Names a shader vertex attribute.

	@in		MOAIShaderProgramGL self
	@in		number index	Default value is 1.
	@in		string name		Name of attribute.
	@out	nil
*/
int MOAIShaderProgramGL::_setVertexAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgramGL, "UNS" )

	ZLIndex idx				= state.GetValue < MOAILuaIndex >( 2, 0 );
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
	
	this->mUniformBufferSize = this->UpdateUniformOffsets ();
	
	this->mUniformBuffer.Clear ();
	this->mUniformBuffer.Init ( this->mUniformBufferSize );
	this->mUniformBuffer.Fill ( 0xff );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ApplyUniforms ( ZLLeanArray < u8 >& buffer ) {

	this->mUniformBuffer.CopyFrom ( buffer );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::BindUniforms () {
	
	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
	
	size_t nUniforms = this->mUniformBindings.Size ();
	
	for ( ZLIndex i = 0; i < nUniforms; ++i ) {
	
		MOAIUniformHandle handle = this->GetUniformHandle ( this->mUniformBuffer.GetBuffer (), i );
		
		if ( handle.IsValid ()) {
			this->mUniformBindings [ i ].Bind ( gfx, this->mUniformDescriptors [ i ], handle.mBuffer );
		}
	}
}

//----------------------------------------------------------------//
ZLGfxHandle MOAIShaderProgramGL::CompileShader ( ZLGfxEnum::_ type, cc8* source ) {

	MOAIGfxMgrGL& gfxMgr = *this->mGfxMgr;
	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	ZLGfxHandle shader = gfx.CreateShader ( type );

	STLString buffer;

	buffer.append ( gfxMgr.IsOpenGLES () ? OPENGL_ES_PREPROC : OPENGL_PREPROC );
	if (( type == ZLGfxEnum::SHADER_TYPE_FRAGMENT ) && gfxMgr.IsOpenGLES ()) {
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

	if ( idx < this->mUniformDescriptors.Size ()) {
		this->mUniformDescriptors [ idx ].Init ( type, width, count );
		this->mUniformBindings [ idx ].mName = name;
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::InitUniformBuffer ( ZLLeanArray < u8 >& buffer ) {

	this->AffirmUniforms ();

	buffer.Clear ();
	
	size_t nUniforms = this->mUniformDescriptors.Size ();
	buffer.Init ( this->mUniformBufferSize );
	
	for ( ZLIndex i = 0; i < nUniforms; ++i ) {
		MOAIUniformHandle handle = this->GetUniformHandle ( buffer.GetBuffer (), i );
		handle.Default ( this->mUniformDescriptors [ i ].mCount );
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::Load ( cc8* vshSource, cc8* fshSource ) {

	if ( vshSource && fshSource ) {

		this->mVertexShaderSource = vshSource;
		this->mFragmentShaderSource = fshSource;
		
		this->ScheduleForGPUUpdate ();
	}
}

//----------------------------------------------------------------//
MOAIShaderProgramGL::MOAIShaderProgramGL () :
	mUniformBufferSize ( 0 ) {
	
	RTTI_BEGIN ( MOAIShaderProgramGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIShaderProgramGL >)
		RTTI_EXTEND ( MOAIGfxResourceGL )
		RTTI_EXTEND ( MOAIHasGfxScript )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderProgramGL::~MOAIShaderProgramGL () {

	this->ScheduleForGPUDestroy ();
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ReserveUniforms ( ZLSize nUniforms ) {

	this->mUniformDescriptors.Init ( nUniforms );
	this->mUniformBindings.Init ( nUniforms );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::SetVertexAttribute ( u32 idx, cc8* attribute ) {

	if ( attribute ) {
		this->mAttributeMap [ idx ] = attribute;
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderProgramGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "declareUniform",				_declareUniform },
		{ "load",						_load },
		{ "reserveUniforms",			_reserveUniforms },
		{ "setVertexAttribute",			_setVertexAttribute },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::MOAIGfxResourceGL_OnGPUBind () {

	// use shader program.
	this->mGfxMgr->GetDrawingAPI ().UseProgram ( this->mProgram );
}

//----------------------------------------------------------------//
bool MOAIShaderProgramGL::MOAIGfxResourceGL_OnGPUCreate () {

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	this->mVertexShader = this->CompileShader ( ZLGfxEnum::SHADER_TYPE_VERTEX, this->mVertexShaderSource );
	this->mFragmentShader = this->CompileShader ( ZLGfxEnum::SHADER_TYPE_FRAGMENT, this->mFragmentShaderSource );
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
	for ( ZLIndex i = 0; i < this->mUniformBindings.Size (); ++i ) {
		MOAIShaderUniformBindingGL& uniform = this->mUniformBindings [ i ];
		gfx.GetUniformLocation ( this->mProgram, uniform.mName, this, ( void* )(( size_t )i )); // TODO: cast?
	}

	gfx.DeleteResource ( this->mVertexShader );
	gfx.DeleteResource ( this->mFragmentShader );

	//AJV TODO - does the attribute map ever need to be cleared?
	//this->mAttributeMap.clear ();
	
	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	this->mGfxMgr->DeleteOrDiscard ( this->mVertexShader, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mFragmentShader, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mProgram, shouldDelete );
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::MOAIGfxResourceGL_OnGPUUnbind () {

	this->mGfxMgr->GetDrawingAPI ().UseProgram ( ZLGfxResource::UNBIND );
}

//----------------------------------------------------------------//
bool MOAIShaderProgramGL::MOAIGfxResourceGL_OnGPUUpdate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgramGL::ZLGfxListener_OnUniformLocation ( u32 addr, void* userdata ) {

	ZLSize i = ( size_t )userdata;
	
	if ( i < this->mUniformBindings.Size ()) {
		this->mUniformBindings [ i ].mGPUBase = addr;
	}
}
