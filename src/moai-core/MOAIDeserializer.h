// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESERIALIZER_H
#define MOAIDESERIALIZER_H

#include <moai-core/MOAISerializerBase.h>

//================================================================//
// MOAIDeserializer
//================================================================//
class MOAIDeserializer :
	public MOAISerializerBase {
private:
	
	//----------------------------------------------------------------//
	static int		_base64Decode				( lua_State* L );
	static int		_createObject				( lua_State* L );
	static int		_initObject					( lua_State* L );
	static int		_registerObjectID			( lua_State* L );
	
public:
	
	enum {
		SUCCESS,
		INVALID_FILE,
		LOAD_ERROR,
		LUA_ERROR,
	};
	
	DECL_LUA_FACTORY ( MOAIDeserializer )
	
	//----------------------------------------------------------------//
	u32					IsLuaFile				( cc8* filename );
	MOAILuaObject*		MemberIDToObject		( ObjID memberID );
	u32					SerializeFromFile		( cc8* filename );
						MOAIDeserializer		();
	virtual				~MOAIDeserializer		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* MemberIDToObject ( ObjID memberID ) {
		
		MOAILuaObject* object = this->MemberIDToObject ( memberID );
		if ( object ) {
			return object->AsType < TYPE >();
		}
		return 0;
	}
};


#endif
