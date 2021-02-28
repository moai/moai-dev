// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#ifndef	MOAILUAUTIL_H
#define	MOAILUAUTIL_H

#ifndef MOAI_WITH_LUAJIT //can't convert luajit bytecode

//================================================================//
// MOAILuaHeader
//================================================================//
class MOAILuaHeader {
public:

	static const u32 SIGNATURE = 0x61754C1B;

	u32			mSignature;				// "ESCLua" or 0x61754C1B
	u8			mVersion;				// 0x51 for Lua 5.1
	u8			mFormat;				// 0 = official version
	u8			mByteOrder;				// 0 = big endian, 1 = little endian
	u8			mSizeOfInt;				// default 4
	u8			mSizeOfSizeT;			// default 4
	u8			mSizeOfInstruction;		// default 4
	u8			mSizeOfLuaNumber;		// default 8
	u8			mTypeOfLuaNumber;		// 0 = floating point, 1 = integral
	
	//----------------------------------------------------------------//
	void				Init				();
	bool				IsBytecode			();
	bool				IsCompatible		( const MOAILuaHeader& check ) const;
						MOAILuaHeader		();
						~MOAILuaHeader		(); // no vtable
	ZLResultCode		Read				( ZLStream& stream );
	ZLResultCode		Read				( void* buffer, size_t size );
	ZLResultCode		Write				( ZLStream& stream ) const;
	ZLResultCode		Write				( void* buffer, size_t size );
};

//================================================================//
// MOAILuaUtil
//================================================================//
/**	@lua	MOAILuaUtil
	@text	Methods for reading and converting Lua bytecode.
*/
class MOAILuaUtil :
	public virtual ZLContextClass,
	public virtual MOAILuaObject {
private:

	//----------------------------------------------------------------//
	static int			_convert				( lua_State* L );
	static int			_getHeader				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				ConvertBytes			( ZLStream& srcStream, ZLStream& dstStream, size_t srcSize, size_t dstSize );
	void				ConvertConst			( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );
	void				ConvertFunction			( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );
	s64					ConvertInt				( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );
	void				ConvertString			( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_SINGLETON ( MOAILuaUtil )
	
	//----------------------------------------------------------------//
	bool				Convert					( const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream );
						MOAILuaUtil				( ZLContext& context );
						~MOAILuaUtil			();
};

#endif
#endif


