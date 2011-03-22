// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMAT_H
#define	MOAIVERTEXFORMAT_H

//================================================================//
// MOAIVertexFormatElement
//================================================================//
class MOAIVertexFormatElement {
private:

	u32		mUsage;
	u32		mType;
	u32		mDim;
	u32		mSize;
	u32		mOffset;

	//----------------------------------------------------------------//
	void	Init			( u32 usage, u32 type, u32 dim );

public:

	friend class MOAIVertexFormat;
	
	//----------------------------------------------------------------//
			MOAIVertexFormatElement		();
};

//================================================================//
// MOAIVertexFormat
//================================================================//
/**	@brief Vertex format class.
*/
class MOAIVertexFormat :
	public USLuaData {
private:

	USLeanArray < MOAIVertexFormatElement > mFormat;
	u32		mVertexSize;
	bool	mDirty;
	
	//----------------------------------------------------------------//
	static int	_reserve				( lua_State* L );
	static int	_setElement				( lua_State* L );

	//----------------------------------------------------------------//
	void		Bless					();
	
public:
	
	enum {
		NONE,
		VERTEX_BONE_IDX,
		VERTEX_BONE_WEIGHT,
		VERTEX_COLOR,
		VERTEX_COORD,
		VERTEX_NORMAL,
		VERTEX_TEXCOORD_0,
		VERTEX_TEXCOORD_1,
	};
	
	DECL_LUA_DATA ( MOAIVertexFormat )
	
	//----------------------------------------------------------------//
	void		Bind					();
	u32			GetVertexSize			();
				MOAIVertexFormat		();
				~MOAIVertexFormat		();
	void		RegisterLuaClass		( USLuaState& state );
	void		RegisterLuaFuncs		( USLuaState& state );
	void		Reserve					( u32 formatSize );
	void		SetElement				( u32 idx, u32 usage, u32 type, u32 dim );
	void		SetVertexElement		( void* buffer, size_t bufferSize, u32 vtx, u32 elemIdx, float* params );
	STLString	ToString				();
};

#endif
