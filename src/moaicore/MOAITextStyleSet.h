// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLESET_H
#define	MOAITEXTSTYLESET_H

#include <moaicore/MOAILua.h>

class MOAITextStyle;

//================================================================//
// MOAITextStyleSet
//================================================================//
class MOAITextStyleSet :
	public MOAILuaObject {
private:

	// for now
	STLMap < STLString, MOAITextStyle* > mStyleMap;
	MOAITextStyle* mDefault;

	//----------------------------------------------------------------//
	static int			_setStyle				( lua_State* L );
	
public:

	friend class MOAITextStyler;

	DECL_LUA_FACTORY ( MOAITextStyleSet )
	
	//----------------------------------------------------------------//
	MOAITextStyle*		FindStyle				( cc8* styleName );
	MOAITextStyle*		GetDefault				();
						MOAITextStyleSet		();
						~MOAITextStyleSet		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void				SetDefault				( MOAITextStyle* style );
	void				SetStyle					( cc8* styleName, MOAITextStyle* style );
};

#endif
