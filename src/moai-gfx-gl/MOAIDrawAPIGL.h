// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWAPIGL_H
#define	MOAIDRAWAPIGL_H

#include <moai-gfx/MOAIGfxConsts.h>

class MOAIBlendMode;
class MOAIFrameBuffer;
class MOAIIndexBuffer;
class MOAIShaderGL;
class MOAITexture;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIDrawingParamGL
//================================================================//
namespace MOAIDrawingParamGL {

	//----------------------------------------------------------------//
	struct LoadShaderUniform {
		ZLIndex				mPipelineGlobalID;
		ZLIndex				mTargetUniformID;
		ZLIndex				mTargetUniformIndex;
	};
};

//================================================================//
// MOAIDrawAPIGL
//================================================================//
// TODO: doxygen
class MOAIDrawAPIGL :
	public virtual MOAIAbstractCmdAPI {
public:

	enum {
		LOAD_SHADER_UNIFORM_GL,
		TOTAL_BASE_COMMANDS,
	};

protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	static const u32 DEFAULT_ELLIPSE_STEPS			= 64;

	//----------------------------------------------------------------//
	static int				_loadShaderUniformGL		( lua_State* L );

	//----------------------------------------------------------------//
	void					_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	void					MOAIAbstractCmdAPI_RegisterLuaAPI		( MOAILuaState& state );

public:

	//----------------------------------------------------------------//
	MOAIDrawAPIGL&			LoadShaderUniformGL			( ZLIndex globalID, ZLIndex uniformID, ZLIndex index );
							MOAIDrawAPIGL				();
	virtual					~MOAIDrawAPIGL				();
};

#endif
