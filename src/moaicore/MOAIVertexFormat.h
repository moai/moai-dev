// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMAT_H
#define	MOAIVERTEXFORMAT_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIVertexAttributeUse
//================================================================//
class MOAIVertexAttributeUse {
private:

	friend class MOAIVertexFormat;
	
	GLenum		mUse;
	u32			mAttrID;
};

//================================================================//
// MOAIVertexAttribute
//================================================================//
class MOAIVertexAttribute {
private:

	friend class MOAIVertexFormat;
	
	GLint		mIndex;
	GLint		mSize;
	GLenum		mType;			// type of the element
	GLboolean	mNormalized;
	u32			mOffset;
};

//================================================================//
// MOAIVertexFormat
//================================================================//
/**	@name	MOAIVertexFormat
	@text	Vertex format class.
*/
class MOAIVertexFormat :
	public virtual MOAILuaObject {
public:

	enum {
		ARRAY_COLOR,
		ARRAY_NORMAL,
		ARRAY_TEX_COORD,
		ARRAY_VERTEX,
		TOTAL_ARRAY_TYPES,
	};

private:

	static const u32 COLOR_SIZE				= 4;
	static const u32 NORMAL_SIZE			= 3;
	static const u32 NULL_INDEX				= 0xffffffff;
	
	USLeanArray < MOAIVertexAttribute >		mAttributes;
	u32										mTotalAttributes;
	u32										mVertexSize;

	MOAIVertexAttributeUse					mAttributeUseTable [ TOTAL_ARRAY_TYPES ]; // use for fixed function pipeline
	
	//----------------------------------------------------------------//
	static int		_declareAttribute				( lua_State* L );
	static int		_declareColor					( lua_State* L );
	static int		_declareCoord					( lua_State* L );
	static int		_declareNormal					( lua_State* L );
	static int		_declareUV						( lua_State* L );
	
	//----------------------------------------------------------------//
	bool			Bind							( void* buffer ) const;
	void			BindFixed						( void* buffer ) const;
	void			BindProgrammable				( void* buffer ) const;
	static u32		GetComponentSize				( GLint size, GLenum type );
	static u32		GetIndexForUse					( GLenum use );
	static GLenum	GetUseForIndex					( u32 idx );
	void			Unbind							() const;
	void			UnbindFixed						() const;
	void			UnbindProgrammable				() const;
	
public:
	
	friend class MOAIGfxDevice;
	
	DECL_LUA_FACTORY ( MOAIVertexFormat )
	
	GET_CONST ( u32, VertexSize, mVertexSize )
	
	//----------------------------------------------------------------//
	bool			ComputeBounds					( void* buffer, u32 size, USBox& bounds );
	void			DeclareAttribute				( GLint index, GLenum type, GLint size, GLenum use, GLboolean normalized );
					MOAIVertexFormat				();
					~MOAIVertexFormat				();
	void			RegisterLuaClass				( MOAILuaState& state );
	void			RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
