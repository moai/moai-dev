// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTRETAINED_H
#define	MOAIGFXSCRIPTRETAINED_H

#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractDrawingLuaAPI.h>

class MOAIAbstractUniformBuffer;
class MOAIUniformSchema;

//================================================================//
// MOAIGfxScriptCommand
//================================================================//
struct MOAIGfxScriptCommand {

	MOAIDrawingAPIEnum::_ 		mType;
	ZLSize 						mParamSize;
};

//================================================================//
// MOAIGfxScriptRetained
//================================================================//
// TODO: doxygen
class MOAIGfxScriptRetained :
	public virtual MOAIAbstractGfxScript,
	public virtual MOAIAbstractDrawingLuaAPI {
protected:

	friend class MOAIAbstractGfxScriptInterface;
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLMemStream											mCommandStream;
	ZLLeanArray < u8 > 									mBytecode;
	ZLLeanStack < ZLStrongPtr < ZLRefCountedObject > >	mRetainedObjects;
	
	//----------------------------------------------------------------//
	static int			_affirmBytecode				( lua_State* L );
	static int			_call						( lua_State* L );
	static int			_callFromShader				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				Execute						( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const;
	void 				ExecuteBytecode				( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand );
		
	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
//	void				MOAIAbstractDrawingAPI_Call				();
	void				MOAIAbstractDrawingAPI_RetainObject		( ZLRefCountedObject* object );
	void				MOAIAbstractDrawingAPI_SubmitCommand	( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size );
	void				MOAIAbstractGfxScript_RunScript			( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand );
	virtual void		MOAIGfxScriptRetained_Execute			( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const;

public:

	enum {
		CALL						= ( MOAIDrawingAPIEnum::_ )-1,
		CALL_FROM_SHADER			= ( MOAIDrawingAPIEnum::_ )-2,
	};

	//----------------------------------------------------------------//
	void				AffirmBytecode				();
	void				Call						();
	void				CallFromShader				();
	bool				HasContent					();
						MOAIGfxScriptRetained		();
						~MOAIGfxScriptRetained		();
	void				Reset						();
};

#endif
