// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMAT_H
#define	MOAIVERTEXFORMAT_H

//================================================================//
// MOAIVertexFormatElem
//================================================================//
class MOAIVertexFormatElem {
private:

	friend class MOAIVertexFormat;
	
	GLint		mSize;
	GLenum		mType;		// type of the element
	u32			mOffset;
 	bool		mIsEnabled;
 	
	//----------------------------------------------------------------//
	void	Bind					( void* buffer, u32 stride, GLenum use ) const;
	void	Set						();
	void	Set						( GLint size, GLenum type, u32 offset );
	void	Unbind					( GLenum use ) const;
			MOAIVertexFormatElem	();
};

//================================================================//
// MOAIVertexFormatAttr
//================================================================//
class MOAIVertexFormatAttr {
private:

	friend class MOAIVertexFormat;
	
	GLint		mIndex;
	GLint		mSize;
	GLenum		mType;			// type of the element
	GLboolean	mNormalized;
	u32			mOffset;
 	
	//----------------------------------------------------------------//
	void	Bind					( void* buffer, u32 stride ) const;
	void	Set						( GLint index, GLint size, GLenum type, GLboolean normalized, u32 offset );
	void	Unbind					() const;

public:

	//----------------------------------------------------------------//
			MOAIVertexFormatAttr		();
};

//================================================================//
// MOAIVertexFormat
//================================================================//
/**	@name	MOAIVertexFormat
	@text	Vertex format class.

	@const	GL_BYTE
	@const	GL_FIXED
	@const	GL_FLOAT
	@const	GL_SHORT
	@const	GL_UNSIGNED_BYTE
	@const	GL_UNSIGNED_SHORT
*/
class MOAIVertexFormat :
	public virtual USLuaObject {
private:
	
	static const u32 COLOR_SIZE				= 4;
	static const u32 NORMAL_SIZE			= 3;
	static const u32 DEFAULT_BUFFER_SIZE	= 0x8000;
	
	enum {
		ARRAY_COLOR,
		ARRAY_NORMAL,
		ARRAY_TEX_COORD,
		ARRAY_VERTEX,
		TOTAL_ARRAY_TYPES,
	};
	
	MOAIVertexFormatElem mElements [ TOTAL_ARRAY_TYPES ];
	u32 mVertexSize;

	USLeanArray < MOAIVertexFormatAttr >	mAttributes;
	u32										mTotalAttributes;
	
	//----------------------------------------------------------------//
	static int		_clear					( lua_State* L );
	static int		_declareColor			( lua_State* L );
	static int		_declareCoord			( lua_State* L );
	static int		_declareNormal			( lua_State* L );
	static int		_declareUV				( lua_State* L );

	//----------------------------------------------------------------//
	u32				GetSize					( GLint size, GLenum type );
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexFormat )
	
	GET_CONST ( u32, VertexSize, mVertexSize )
	
	//----------------------------------------------------------------//
	void			Bind					( void* buffer ) const;
	void			Clear					();
	bool			ComputeBounds			( void* buffer, u32 size, USRect& bounds );
	void			DeclareAttribute		( GLint index, GLint size, GLenum type, GLboolean normalized );
	void			DeclareColorArray		( GLenum type );
	void			DeclareNormalArray		( GLenum type );
	void			DeclareTexCoordArray	( GLenum type, GLint size );
	void			DeclareVertexArray		( GLenum type, GLint size );
	GLenum			GetColorType			() const;
					MOAIVertexFormat		();
					~MOAIVertexFormat		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			ReserveAttributes		( u32 total );
	STLString		ToString				();
	void			Unbind					() const;
};

#endif
