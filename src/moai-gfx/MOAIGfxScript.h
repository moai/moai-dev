// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPT_H
#define	MOAIGFXSCRIPT_H

#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractHasGfxScript.h>

//================================================================//
// MOAIGfxScriptCommand
//================================================================//
struct MOAIGfxScriptCommand {

	MOAIDrawingAPIEnum::_ 		mType;
	ZLSize 						mParamSize;
};

//================================================================//
// MOAIGfxScript
//================================================================//
// TODO: doxygen
class MOAIGfxScript :
	public virtual MOAIAbstractHasGfxScript,
	public virtual MOAIAbstractGfxScript {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIHasGfxScript;

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
	void						_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	void						MOAIAbstractGfxScript_RetainObject			( ZLRefCountedObject* object ) ;
	void						MOAIAbstractGfxScript_RunScript				( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand );
	void						MOAIAbstractGfxScript_SubmitCommand			( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size );
	MOAIAbstractGfxScript&		MOAIAbstractHasGfxScript_AffirmGfxScript	();
	virtual void				MOAIGfxScript_Execute						( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const;

public:

	enum {
		CALL						= ( MOAIDrawingAPIEnum::_ )-1,
		CALL_FROM_SHADER			= ( MOAIDrawingAPIEnum::_ )-2,
	};
	
	DECL_LUA_FACTORY ( MOAIGfxScript )

	//----------------------------------------------------------------//
	void				AffirmBytecode				();
	void				Call						();
	void				CallFromShader				();
	bool				HasContent					();
						MOAIGfxScript				();
						~MOAIGfxScript				();
	void				Reset						();
};

#endif
