// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGR_H
#define	MOAIGFXMGR_H

#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIImage.h>

class MOAIFrameBuffer;
class MOAIShader;
class MOAIShaderProgram;
class MOAITexture;
class MOAITextureBase;
class MOAIVertexFormat;

//================================================================//
// MOAIGfxMgr
//================================================================//
/**	@lua	MOAIGfxMgr
	@text	Interface to the graphics singleton.
	
	@const	EVENT_RESIZE
*/
class MOAIGfxMgr :
	public ZLContextClass < MOAIGfxMgr, MOAIGlobalEventSource >,
	public virtual ZLGfxMgr {
public:
	
	enum {
		EVENT_RESIZE,
		TOTAL_EVENTS,
	};
	
private:

	//----------------------------------------------------------------//
	static int			_enablePipelineLogging		( lua_State* L );
	static int			_getFrameBuffer				( lua_State* L );
	static int			_getMaxTextureSize			( lua_State* L );
	static int			_getMaxTextureUnits			( lua_State* L );
	static int			_getViewSize				( lua_State* L );
	static int			_purgeResources				( lua_State* L );
	static int			_renewResources				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				OnGlobalsFinalize			();
	void				OnGlobalsInitialize			();
	
public:
	
	friend class MOAIShaderProgram;
	friend class ZLAbstractGfxResource;
	friend class ZLTexture;
	
	DECL_LUA_SINGLETON ( MOAIGfxMgr )
	
	//----------------------------------------------------------------//
					MOAIGfxMgr				();
					~MOAIGfxMgr				();
	void			RegisterLuaClass		( MOAILuaState& state );
};

#endif
