// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>
#include <moai-gfx-gl/MOAIFrameBufferMgrGL.h>
#include <moai-gfx-gl/MOAIShaderProgramGL.h>

#include <moai-gfx-gl/shaders/MOAIDeck2DShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DSnappingShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DSnappingShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIFontShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIFontShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIFontSnappingShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIFontSnappingShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAILineShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAILineShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAILineShader3DGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAILineShader3DGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIMeshShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIMeshShaderGL-vsh.h>

//================================================================//
// MOAIFrameBufferAttachmentFactory
//================================================================//
	
//----------------------------------------------------------------//
MOAIFrameBufferAttachableGL* MOAIFrameBufferAttachmentFactory::MakeAttachment ( u32 factoryID, u32 width, u32 height ) {
	return this->MOAIFrameBufferAttachmentFactory_MakeAttachment ( factoryID, width, height );
}

//----------------------------------------------------------------//
MOAIFrameBufferAttachmentFactory::MOAIFrameBufferAttachmentFactory () {
}

MOAIFrameBufferAttachmentFactory::~MOAIFrameBufferAttachmentFactory () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIFrameBufferAttachableGL* MOAIFrameBufferAttachmentFactory::MOAIFrameBufferAttachmentFactory_MakeAttachment ( u32 factoryID, u32 width, u32 height ) {
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFrameBufferMgrGL::_getAttachment ( lua_State* L ) {
	MOAILuaState state ( L );

//	u32 shaderID = state.GetValue < u32 >( 1, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER );
//
//	if ( shaderID < MOAIShaderPresetEnum::TOTAL_SHADERS ) {
//
//		MOAIShaderProgramGL* program = MOAIFrameBufferMgrGL::Get ().GetProgram (( MOAIShaderPresetEnum )shaderID );
//		state.Push ( program );
//		return 1;
//	}
	return 0;
}

//================================================================//
// MOAIFrameBufferMgrGL
//================================================================//

//----------------------------------------------------------------//
MOAIFrameBufferMgrGL::MOAIFrameBufferMgrGL () {
	
	RTTI_BEGIN ( MOAIFrameBufferMgrGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIFrameBufferMgrGL >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferMgrGL::~MOAIFrameBufferMgrGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferMgrGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

//	state.SetField ( -1, "DECK2D_SHADER",			( u32 )MOAIShaderPresetEnum::DECK2D_SHADER );
//	state.SetField ( -1, "DECK2D_SNAPPING_SHADER",	( u32 )MOAIShaderPresetEnum::DECK2D_SNAPPING_SHADER );
//	state.SetField ( -1, "FONT_SHADER",				( u32 )MOAIShaderPresetEnum::FONT_SHADER );
//	state.SetField ( -1, "FONT_SNAPPING_SHADER",	( u32 )MOAIShaderPresetEnum::FONT_SNAPPING_SHADER );
//	state.SetField ( -1, "FONT_EFFECTS_SHADER",	    ( u32 )MOAIShaderPresetEnum::FONT_EFFECTS_SHADER );
//	state.SetField ( -1, "LINE_SHADER",				( u32 )MOAIShaderPresetEnum::LINE_SHADER );
//	state.SetField ( -1, "LINE_SHADER_3D",			( u32 )MOAIShaderPresetEnum::LINE_SHADER_3D );
//	state.SetField ( -1, "MESH_SHADER",				( u32 )MOAIShaderPresetEnum::MESH_SHADER );

	luaL_Reg regTable [] = {
		{ "getAttachment",			_getAttachment },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFrameBufferMgrGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}
