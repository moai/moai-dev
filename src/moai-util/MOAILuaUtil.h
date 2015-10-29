// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAI_WITH_LUAJIT //can't convert luajit bytecode
#ifndef	MOAILUAUTIL_H
#define	MOAILUAUTIL_H

//================================================================//
// MOAILuaHeader
//================================================================//
class MOAILuaHeader {
public:

	u32			mSignature;				// "ESCLua" or 0x6175C41B
	u8			mVersion;				// 0x51 for Lua 5.1
	u8			mFormat;				// 0 = official version
	u8			mByteOrder;				// 0 = big endian, 1 = little endian
	u8			mSizeOfInt;				// default 4
	u8			mSizeOfSizeT;			// default 4
	u8			mSizeOfInstruction;		// default 4
	u8			mSizeOfLuaNumber;		// default 8
	u8			mTypeOfLuaNumber;		// 0 = floating point, 1 = integral
	
	//----------------------------------------------------------------//
	void		Init			();
	void		Read			( ZLStream& stream );
	void		Write			( ZLStream& stream ) const;
};

//================================================================//
// MOAILuaUtil
//================================================================//
/**	@lua	MOAILuaUtil
	@text	Methods for reading and converting Lua bytecode.
*/
class MOAILuaUtil :
	public MOAIGlobalClass < MOAILuaUtil, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int			_convert				( lua_State* L );
	static int			_getHeader				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				ConvertBytes					( ZLStream& srcStream, ZLStream& dstStream, size_t srcSize, size_t dstSize );
	void				ConvertConst					( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );
	void				ConvertFunction					( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );
	s64					ConvertInt						( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );
	void				ConvertString					( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );

public:
	
	DECL_LUA_SINGLETON ( MOAILuaUtil )
	
	//----------------------------------------------------------------//
	bool				Convert					( const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );
						MOAILuaUtil				();
						~MOAILuaUtil			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif