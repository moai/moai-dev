// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATGL_H
#define	MOAIVERTEXFORMATGL_H

//================================================================//
// MOAIVertexFormatGL
//================================================================//
/**	@lua	MOAIVertexFormatGL
	@text	Vertex format class.
*/
class MOAIVertexFormatGL :
	public virtual MOAILuaObject,
	public virtual ZLVertexFormatGL {
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
	
	DECL_LUA_FACTORY ( MOAIVertexFormatGL )
		
	//----------------------------------------------------------------//
						MOAIVertexFormatGL				();
						~MOAIVertexFormatGL				();
	void				RegisterLuaClass				( MOAILuaState& state );
	void				RegisterLuaFuncs				( MOAILuaState& state );
	void				SerializeIn						( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut					( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
