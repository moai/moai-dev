// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATVK_H
#define	MOAIVERTEXFORMATVK_H

//================================================================//
// MOAIVertexFormatVK
//================================================================//
/**	@lua	MOAIVertexFormatVK
	@text	Vertex format class.
*/
class MOAIVertexFormatVK :
	public virtual MOAIVertexFormat {
protected:

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIVertexFormatVK )
		
	//----------------------------------------------------------------//
//	void				Bind							( ZLGfx& gfx, ZLSharedConstBuffer* buffer ) const;
						MOAIVertexFormatVK				();
						~MOAIVertexFormatVK				();
//	void				Unbind							( ZLGfx& gfx ) const;
};

#endif
