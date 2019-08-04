// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// MOAIShaderProgramGlobalVK
//================================================================//
	
////----------------------------------------------------------------//
//MOAIShaderProgramGlobalVK::MOAIShaderProgramGlobalVK () :
//	mGlobalID ( ZLIndexOp::INVALID ),
//	mUniformID ( ZLIndexOp::INVALID ),
//	mIndex ( ZLIndexOp::ZERO ) {
//}
//
////================================================================//
//// MOAIShaderProgramTextureVK
////================================================================//
//
////----------------------------------------------------------------//
//MOAIShaderProgramTextureVK::MOAIShaderProgramTextureVK () :
//	mName ( MOAI_UNKNOWN_MATERIAL_GLOBAL ),
//	mUnit ( ZLIndexOp::ZERO ) {
//}

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
///**	@lua	declareUniform
//	@text	Declares a uniform mapping.
//
//	@in		MOAIShaderProgramVK self
//	@in		number idx
//	@in		string name
//	@opt	number type		One of MOAIShaderProgramVK.UNIFORM_TYPE_FLOAT, MOAIShaderProgramVK.UNIFORM_TYPE_INDEX, MOAIShaderProgramVK.UNIFORM_TYPE_INT,
//							MOAIShaderProgramVK.UNIFORM_TYPE_MATRIX_3X3, MOAIShaderProgramVK.UNIFORM_TYPE_MATRIX_4X4
//	@opt	number width	Used for vector uniforms. Default value is 1. Should be no greather than 4.
//	@opt	number count	Declare an array of uniforms. Default value is 1.
//	@out	nil
//*/
//int MOAIShaderProgramVK::_declareUniform ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgramVK, "UNSN" )
//
//	ZLIndex idx			= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
//	STLString name		= state.GetValue < cc8* >( 3, "" );
//	u32 type			= state.GetValue < u32 >( 4, MOAIShaderUniformVK::UNIFORM_TYPE_FLOAT );
//	u32 width			= state.GetValue < u32 >( 5, 1 );
//	u32 count			= state.GetValue < u32 >( 6, 1 );
//
//	self->DeclareUniform ( idx, name, type, width, count );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	load
//	@text	Load a shader program.
//
//	@in		MOAIShaderProgramVK self
//	@in		string vertexShaderSource
//	@in		string fragmentShaderSource
//	@out	nil
//*/
//int MOAIShaderProgramVK::_load ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgramVK, "USS" )
//
//	cc8* vtxSource	= state.GetValue < cc8* >( 2, 0 );
//	cc8* frgSource	= state.GetValue < cc8* >( 3, 0 );
//
//	self->Load ( vtxSource, frgSource );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIShaderProgramVK::_reserveGlobals ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgramVK, "U" )
//
////	return self->ReserveGlobals ( L, 2 );
//
//	u32 nGlobals = state.GetValue < u32 >( 2, 0 );
//	self->ReserveGlobals ( nGlobals );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIShaderProgramVK::_reserveTextures ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgramVK, "U" )
//
//	self->ReserveTextures ( state.GetValue < u32 >( 2, 0 ));
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	reserveUniforms
//	@text	Reserve shader uniforms.
//
//	@in		MOAIShaderProgramVK self
//	@opt	number nUniforms	Default value is 0.
//	@out	nil
//*/
//int MOAIShaderProgramVK::_reserveUniforms ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgramVK, "U" )
//
//	u32 nUniforms = state.GetValue < u32 >( 2, 0 );
//	self->ReserveUniforms ( nUniforms );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIShaderProgramVK::_setGlobal ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgramVK, "UNNN" )
//
//	ZLIndex globalIdx	= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
//	u32 globalID		= state.GetValue < u32 >( 3, MOAIGfxMgrVK::NULL_GLOBAL );
//	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 4, ZLIndexOp::ZERO );
//	ZLIndex index		= state.GetValue < MOAILuaIndex >( 5, ZLIndexOp::ZERO );
//
//	self->SetGlobal ( globalIdx, globalID, uniformID, index );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIShaderProgramVK::_setTexture ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgramVK, "U" )
//
//	ZLIndex idx		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
//	ZLIndex unit	= state.GetValue < MOAILuaIndex >( 3, ZLIndexOp::ZERO );
//
//	if ( state.IsType ( 3, LUA_TUSERDATA )) {
//
//		self->SetTexture ( idx, state.GetLuaObject < MOAITextureVK >( 3, true ), unit );
//	}
//	else {
//
//		self->SetTexture (
//			idx,
//			state.GetValue < u32 >( 3, MOAI_UNKNOWN_MATERIAL_GLOBAL + 1 ) - 1,
//			unit,
//			state.GetLuaObject < MOAITextureVK >( 5, false )
//		);
//	}
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	setVertexAttribute
//	@text	Names a shader vertex attribute.
//
//	@in		MOAIShaderProgramVK self
//	@in		number index	Default value is 1.
//	@in		string name		Name of attribute.
//	@out	nil
//*/
//int MOAIShaderProgramVK::_setVertexAttribute ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgramVK, "UNS" )
//
//	ZLIndex idx				= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
//	STLString attribute		= state.GetValue < cc8* >( 3, "" );
//
//	self->SetVertexAttribute ( idx, attribute );
//
//	return 0;
//}

//================================================================//
// MOAIShaderProgramVK
//================================================================//

////----------------------------------------------------------------//
//void MOAIShaderProgramVK::AffirmUniforms () {
//
//	if ( this->mUniformBufferSize ) return;
//
//	this->mUniformBufferSize = 0;
//	size_t nUniforms = this->mUniforms.Size ();
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
//
//		MOAIShaderUniformVK& uniform = this->mUniforms [ i ];
//		uniform.mCPUOffset = this->mUniformBufferSize;
//		this->mUniformBufferSize += uniform.GetSize ();
//
//		this->mMaxCount = this->mMaxCount < uniform.mCount ? uniform.mCount : this->mMaxCount;
//	}
//
//	this->mUniformBuffer.Clear ();
//	this->mUniformBuffer.Init ( this->mUniformBufferSize );
//	this->mUniformBuffer.Fill ( 0xff );
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::ApplyUniforms ( ZLLeanArray < u8 >& buffer ) {
//
//	this->mUniformBuffer.CopyFrom ( buffer);
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::BindUniforms () {
//
//	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//
//	size_t nUniforms = this->mUniforms.Size ();
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
//
//		MOAIUniformHandle uniform = this->GetUniformHandle ( this->mUniformBuffer.GetBuffer (), i );
//
//		if ( uniform.IsValid ()) {
//			this->mUniforms [ i ].Bind ( gfx, uniform.mBuffer );
//		}
//	}
//}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::Clear () {

	for ( size_t i = 0; i < TOTAL_MODULES; ++i ) {
		this->Clear (( ModuleID )i );
	}

//	this->mVertexShaderSource.clear ();
//	this->mFragmentShaderSource.clear ();
//
//	this->mAttributeMap.clear ();
//	this->mUniforms.Clear ();
//	this->mGlobals.Clear ();
//
//	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::Clear ( ModuleID moduleID ) {

	if ( this->mModules [ moduleID ]) {
		vkDestroyShaderModule ( MOAIGfxMgrVK::Get ().GetLogicalDevice (), this->mModules [ moduleID ], NULL );
		this->mModules [ moduleID ] = NULL;
	}
}

//----------------------------------------------------------------//
VkShaderStageFlagBits MOAIShaderProgramVK::GetShaderStageBit ( ModuleID moduleID ) {

	switch ( moduleID ) {
		case FRAGMENT_MODULE:	return VK_SHADER_STAGE_FRAGMENT_BIT;
		case VERTEX_MODULE:		return VK_SHADER_STAGE_VERTEX_BIT;
	}
	return ( VkShaderStageFlagBits )0;
}

////----------------------------------------------------------------//
//ZLGfxHandle MOAIShaderProgramVK::CompileShader ( u32 type, cc8* source ) {
//
//	MOAIGfxMgrVK& gfxMgr = *this->mGfxMgr;
//	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//
//	ZLGfxHandle shader = gfx.CreateShader ( type );
//
//	STLString buffer;
//
//	buffer.append ( gfxMgr.IsOpenGLES () ? OPENVK_ES_PREPROC : OPENVK_PREPROC );
//	if (( type == ZGL_SHADER_TYPE_FRAGMENT ) && gfxMgr.IsOpenGLES ()) {
//		buffer.append ( WEBVK_PREPROC );
//	}
//
//	buffer.append ( source );
//
//	gfx.PushSection ();
//
//	gfx.ShaderSource ( shader, buffer.c_str (), buffer.size ());
//	gfx.CompileShader ( shader, true );
//
//	if ( gfx.PushErrorHandler ()) {
//		gfx.DeleteResource ( shader );
//	}
//
//	gfx.PopSection ();
//
//	return shader;
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::DeclareUniform ( ZLIndex idx, cc8* name, u32 type, u32 width, u32 count ) {
//
//	if ( idx < this->mUniforms.Size ()) {
//
//		MOAIShaderUniformVK& uniform = this->mUniforms [ idx ];
//		uniform.mName = name;
//		uniform.Init ( type, width, count );
//	}
//}
//
////----------------------------------------------------------------//
//MOAIShaderUniformVK* MOAIShaderProgramVK::GetUniform ( ZLIndex uniformID ) {
//
//	return uniformID < this->mUniforms.Size () ? &this->mUniforms [ uniformID ] : 0;
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::InitUniformBuffer ( ZLLeanArray < u8 >& buffer ) {
//
//	this->AffirmUniforms ();
//
//	buffer.Clear ();
//
//	size_t nUniforms = this->mUniforms.Size ();
//	buffer.Init ( this->mUniformBufferSize );
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
//		MOAIUniformHandle uniform = this->GetUniformHandle ( buffer.GetBuffer (), i );
//		uniform.Default ( this->mUniforms [ i ].mCount );
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::Load ( cc8* vshSource, cc8* fshSource ) {
//
//	if ( vshSource && fshSource ) {
//
//		this->mVertexShaderSource = vshSource;
//		this->mFragmentShaderSource = fshSource;
//
//		this->ScheduleForGPUUpdate ();
//	}
//}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::LoadModule ( ModuleID moduleID, const void* shaderCode, size_t shaderSize ) {

	MOAILogicalDeviceVK& logicalDevice = MOAIGfxMgrVK::Get ().GetLogicalDevice ();

	// Create a new shader module that will be used for pipeline creation
	VkShaderModuleCreateInfo moduleCreateInfo = MOAIGfxStructVK::shaderModuleCreateInfo (( uint32_t* )shaderCode, shaderSize );

	VkShaderModule module;
	VK_CHECK_RESULT ( vkCreateShaderModule ( logicalDevice, &moduleCreateInfo, NULL, &module ));
	this->mModules [ moduleID ] = module;
}

////----------------------------------------------------------------//
//MOAIShaderProgramVK::MOAIShaderProgramVK () :
//	mMaxCount ( 0 ),
//	mUniformBufferSize ( 0 ) {
//
//	RTTI_BEGIN
//		RTTI_EXTEND ( MOAIGfxResourceVK )
//	RTTI_END
//}

//----------------------------------------------------------------//
MOAIShaderProgramVK::MOAIShaderProgramVK () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResourceVK )
	RTTI_END
	
	memset ( this->mModules, NULL, sizeof ( this->mModules ));
}


//----------------------------------------------------------------//
MOAIShaderProgramVK::~MOAIShaderProgramVK () {

	this->Clear ();
}

////----------------------------------------------------------------//
//void MOAIShaderProgramVK::ReserveGlobals ( ZLSize nGlobals ) {
//
//	this->mGlobals.Init ( nGlobals );
//}
//
////----------------------------------------------------------------//
//int MOAIShaderProgramVK::ReserveGlobals ( lua_State* L, int idx ) {
//
//	MOAILuaState state ( L );
//
//	u32 nGlobals = state.GetValue < u32 >( idx, 0 );
//	this->ReserveGlobals ( nGlobals );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::ReserveTextures ( ZLSize nTextures ) {
//
//	this->mTextures.Init ( nTextures );
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::ReserveUniforms ( ZLSize nUniforms ) {
//
//	this->mUniforms.Init ( nUniforms );
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::ScheduleTextures () {
//
//	// TODO: ZLGfx
//
////	MOAIGfxMgrVK& gfxMgr = this->mGfxMgr;
////	MOAIMaterialMgr& materialMgr = MOAIMaterialMgr::Get ();
////
////	size_t nTextures = this->mTextures.Size ();
////	for ( ZLIndex i = ZLIndexOp::ZERO; i < nTextures; ++i ) {
////
////		MOAIShaderProgramTextureVK& shaderTexture = this->mTextures [ i ];
////
////		MOAITextureVK* texture = shaderTexture.mTexture;
////
////		// load texture by name
////		if ( !texture ) {
////			texture = materialMgr.GetTexture ( shaderTexture.mName );
////		}
////
////		gfx.mGfxState.SetTexture ( texture, shaderTexture.mUnit );
////	}
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::SetGlobal ( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index ) {
//
//	MOAIShaderProgramGlobalVK& global = this->mGlobals [ idx ];
//
//	global.mUniformID	= uniformID;
//	global.mIndex		= index;
//	global.mGlobalID	= globalID;
//}
//
////----------------------------------------------------------------//
////int MOAIShaderProgramVK::SetGlobal ( lua_State* L, int idx ) {
////
////	MOAILuaState state ( L );
////
////	ZLIndex globalIdx	= state.GetValue < MOAILuaIndex >( idx, ZLIndexOp::ZERO );
////	u32 globalID		= state.GetValue < u32 >( idx + 1, MOAIGfxMgrVK::NULL_GLOBAL );
////	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( idx + 2, ZLIndexOp::ZERO );
////	ZLIndex index		= state.GetValue < MOAILuaIndex >( idx + 3, ZLIndexOp::ZERO );
////
////	this->SetGlobal ( globalIdx, globalID, uniformID, index );
////
////	return 0;
////}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::SetTexture ( ZLIndex idx, u32 name, ZLIndex unit, MOAITextureVK* fallback ) {
//
//	if ( idx < this->mTextures.Size ()) {
//
//		MOAIShaderProgramTextureVK& shaderTexture = this->mTextures [ idx ];
//		shaderTexture.mName = name;
//		shaderTexture.mUnit = unit;
//		shaderTexture.mTexture = fallback;
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::SetTexture ( ZLIndex idx, MOAITextureVK* texture, ZLIndex unit ) {
//
//	if ( idx < this->mTextures.Size ()) {
//
//		MOAIShaderProgramTextureVK& shaderTexture = this->mTextures [ idx ];
//		shaderTexture.mName = MOAI_UNKNOWN_MATERIAL_GLOBAL;
//		shaderTexture.mUnit = unit;
//		shaderTexture.mTexture = texture;
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIShaderProgramVK::SetVertexAttribute ( u32 idx, cc8* attribute ) {
//
//	if ( attribute ) {
//		this->mAttributeMap [ idx ] = attribute;
//	}
//}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::UpdatePipelineCreateInfo ( VkGraphicsPipelineCreateInfo& info ) {

	if ( this->mStageInfos.Size () == 0 ) {
	
		size_t moduleCount = 0;
		for ( size_t i = 0; i < TOTAL_MODULES; ++i ) {
			if ( this->mModules [( ModuleID )i ]) moduleCount++;
		}
		this->mStageInfos.Init ( moduleCount );
		
		size_t cursor = 0;
		for ( size_t i = 0; i < TOTAL_MODULES; ++i ) {
		
			ModuleID moduleID = ( ModuleID )i;
			VkShaderModule module = this->mModules [ i ];
			if ( module ) {
			
				this->mStageInfos [ ZLIndexCast ( cursor++ )] = MOAIGfxStructVK::pipelineShaderStageCreateInfo (
					this->GetShaderStageBit ( moduleID ),
					module,
					"main"
				);
			}
		}
	}
	
	info.pStages = this->mStageInfos.GetBuffer ();
	info.stageCount = ( u32 )this->mStageInfos.Size ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIShaderProgramVK::MOAIGfxResource_OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::MOAIGfxResource_OnCPUPurgeRecoverable () {
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::MOAIGfxResourceVK_OnGPUBind () {

	// use shader program.
//	this->mGfxMgr->GetDrawingAPI ().UseProgram ( this->mProgram );
}

//----------------------------------------------------------------//
bool MOAIShaderProgramVK::MOAIGfxResourceVK_OnGPUCreate () {

//	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//
//	this->mVertexShader = this->CompileShader ( ZGL_SHADER_TYPE_VERTEX, this->mVertexShaderSource );
//	this->mFragmentShader = this->CompileShader ( ZGL_SHADER_TYPE_FRAGMENT, this->mFragmentShaderSource );
//	this->mProgram = gfx.CreateProgram ();
//
//	// TODO: error handling
////	if ( !( this->mVertexShader && this->mFragmentShader && this->mProgram )) {
////		this->Clear ();
////		return false;
////	}
//
//	gfx.AttachShader ( this->mProgram, this->mVertexShader );
//	gfx.AttachShader ( this->mProgram, this->mFragmentShader );
//
//	// bind attribute locations.
//	// this needs to be done prior to linking.
//	AttributeMapIt attrMapIt = this->mAttributeMap.begin ();
//	for ( ; attrMapIt != this->mAttributeMap.end (); ++attrMapIt ) {
//		gfx.BindAttribLocation ( this->mProgram, attrMapIt->first, attrMapIt->second.str ());
//	}
//
//	gfx.LinkProgram ( this->mProgram, true );
//
//	// get the uniform locations
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mUniforms.Size (); ++i ) {
//		MOAIShaderUniformVK& uniform = this->mUniforms [ i ];
//		gfx.GetUniformLocation ( this->mProgram, uniform.mName, this, ( void* )(( size_t )i )); // TODO: cast?
//	}
//
//	gfx.DeleteResource ( this->mVertexShader );
//	gfx.DeleteResource ( this->mFragmentShader );
//
//	//AJV TODO - does the attribute map ever need to be cleared?
//	//this->mAttributeMap.clear ();
	
	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::MOAIGfxResourceVK_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

//	this->mGfxMgr->DeleteOrDiscard ( this->mVertexShader, shouldDelete );
//	this->mGfxMgr->DeleteOrDiscard ( this->mFragmentShader, shouldDelete );
//	this->mGfxMgr->DeleteOrDiscard ( this->mProgram, shouldDelete );
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::MOAIGfxResourceVK_OnGPUUnbind () {

//	this->mGfxMgr->GetDrawingAPI ().UseProgram ( ZLGfxResource::UNBIND );
}

//----------------------------------------------------------------//
bool MOAIShaderProgramVK::MOAIGfxResourceVK_OnGPUUpdate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

//	luaL_Reg regTable [] = {
//		{ "declareUniform",				_declareUniform },
//		{ "load",						_load },
//		{ "reserveGlobals",				_reserveGlobals },
//		{ "reserveTextures",			_reserveTextures },
//		{ "reserveUniforms",			_reserveUniforms },
//		{ "setGlobal",					_setGlobal },
//		{ "setTexture",					_setTexture },
//		{ "setVertexAttribute",			_setVertexAttribute },
//		{ NULL, NULL }
//	};
//	luaL_register ( state, 0, regTable );
}
