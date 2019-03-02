// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMAT_H
#define	MOAIVERTEXFORMAT_H

#include <moai-sim/ZLVertexFormat.h>

//================================================================//
// MOAIVertexFormat
//================================================================//
/**	@lua	MOAIVertexFormat
	@text	Vertex format class.
*/
class MOAIVertexFormat :
	public virtual MOAILuaObject,
	public virtual ZLVertexFormat {
private:

	//----------------------------------------------------------------//
	static int			_clear							( lua_State* L );
	static int			_declareAttribute				( lua_State* L );
	static int			_declareBoneCount				( lua_State* L );
	static int			_declareBoneIndices				( lua_State* L );
	static int			_declareBoneWeights				( lua_State* L );
	static int			_declareColor					( lua_State* L );
	static int			_declareCoord					( lua_State* L );
	static int			_declareNormal					( lua_State* L );
	static int			_declareUV						( lua_State* L );
	static int			_getVertexSize					( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexFormat )
		
	//----------------------------------------------------------------//
						MOAIVertexFormat				();
						~MOAIVertexFormat				();
	void				RegisterLuaClass				( MOAILuaState& state );
	void				RegisterLuaFuncs				( MOAILuaState& state );
	void				SerializeIn						( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut					( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
