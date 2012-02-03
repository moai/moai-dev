// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEPLUGIN_H
#define	MOAIPARTICLEPLUGIN_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAIParticleScript.h>
#include <aku/AKU-particles.h>
#include <tinyxml.h>
#include <uslscore/STLString.h>

//================================================================//
// MOAIParticlePlugin
//================================================================//
/**	@name	MOAIParticlePlugin
	@text	Allows custom particle processing via C language callbacks.
*/
class MOAIParticlePlugin :
	public virtual MOAILuaObject {
private:
	enum EmitterType{
		EMITTER_GRAVITY,
		EMITTER_RADIAL,
	};
	
	int							mSize;
	AKUParticleInitFunc			mInitFunc;
	AKUParticleRenderFunc		mRenderFunc;

	//----------------------------------------------------------------//
	static int		_getSize			( lua_State* L );
	static int		_loadExternal		( lua_State* L );

	static void		Parse						( MOAILuaState& state, TiXmlNode* node );
	static void		SetRegister					( MOAIParticleScript::Instruction &instruction, u32 idx, u64 reg);
	static void		SetValue					( MOAIParticleScript::Instruction &instruction, u32 idx, u64 value);
	static void		SetEase						( MOAIParticleScript::Instruction &instruction, u32 idx, u32 easeType);
	static void		_add						( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1);
	static void		_cycle						( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1, u64 val2);
	static void		_div						( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1);
	static void		_ease						( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1, u32 ease);
	static void		_easeDelta					( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1, u32 ease);
	static void		_mul						( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1 );
	static void		_norm						( MOAIParticleScript* script, u64 reg0, u64 reg1, u64 val0, u64 val1 );
	static void		_rand						( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1 );
	static void		_randVec					( MOAIParticleScript* script, u64 reg0, u64 reg1, u64 val0, u64 val1);
	static void		_set						( MOAIParticleScript* script, u64 reg, u64 val0);
	static void		_sprite						( MOAIParticleScript* script);
	static void		_sub						( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1 );
	static void		_time						( MOAIParticleScript* script, u64 reg);
	static void		_timeDelta					( MOAIParticleScript* script, u64 reg);
	static void		_vecAngle					( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1);
	static void		_angleVec					( MOAIParticleScript* script, u64 reg0, u64 reg1, u64 val);
	static void		_wrap						( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1, u64 val2);
	static u64		_packConst					( float val );

public:
	
	friend class MOAIParticleState;
	
	DECL_LUA_FACTORY ( MOAIParticlePlugin )
	
	//----------------------------------------------------------------//
	void			Init						( AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size );
					MOAIParticlePlugin			();
					~MOAIParticlePlugin			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
