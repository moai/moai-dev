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
	public virtual MOAIAbstractVertexFormat {
public:
	
	DECL_LUA_FACTORY ( MOAIVertexFormatGL )
		
	//----------------------------------------------------------------//
	void				Bind							( ZLGfx& gfx, ZLSharedConstBuffer* buffer ) const;
						MOAIVertexFormatGL				();
						~MOAIVertexFormatGL				();
	void				RegisterLuaClass				( MOAILuaState& state );
	void				RegisterLuaFuncs				( MOAILuaState& state );
	void				SerializeIn						( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut					( MOAILuaState& state, MOAISerializer& serializer );
	void				Unbind							( ZLGfx& gfx ) const;
};

#endif
