// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#include "pch.h"
#include <moai-util/MOAILuaUtil.h>

SUPPRESS_EMPTY_FILE_WARNING

#ifndef MOAI_WITH_LUAJIT

#include <lundump.h>

//================================================================//
// MOAILuaHeader
//================================================================//

//----------------------------------------------------------------//
void MOAILuaHeader::Init () {
 
	int x = 1;
 
	this->mSignature			= *(( u32* )LUA_SIGNATURE );
	this->mVersion				= LUAC_VERSION;
	this->mFormat				= LUAC_FORMAT;
	this->mByteOrder			= *( char* )&x;
	this->mSizeOfInt			= sizeof ( int );
	this->mSizeOfSizeT			= sizeof ( size_t );
	this->mSizeOfInstruction	= sizeof ( Instruction );
	this->mSizeOfLuaNumber		= sizeof ( lua_Number );
	this->mTypeOfLuaNumber		= ((( lua_Number )0.5 ) == 0 );
}

//----------------------------------------------------------------//
bool MOAILuaHeader::IsBytecode () {

	return this->mSignature == SIGNATURE;
}

//----------------------------------------------------------------//
bool MOAILuaHeader::IsCompatible ( const MOAILuaHeader& check ) const {

	return ( memcmp ( this, &check, sizeof ( MOAILuaHeader )) == 0 );
}

//----------------------------------------------------------------//
MOAILuaHeader::MOAILuaHeader () :
	mSignature ( 0 ) {
}

//----------------------------------------------------------------//
MOAILuaHeader::~MOAILuaHeader () {
}

//----------------------------------------------------------------//
ZLResultCode MOAILuaHeader::Read ( ZLStream& stream ) {

	ZLResultCodeAccumulator result;

	result = stream.Read < u32 >( this->mSignature, 0 );
	result = stream.Read < u8 >( this->mVersion, 0 );
	result = stream.Read < u8 >( this->mFormat, 0 );
	result = stream.Read < u8 >( this->mByteOrder,  0 );
	result = stream.Read < u8 >( this->mSizeOfInt, 0 );
	result = stream.Read < u8 >( this->mSizeOfSizeT, 0 );
	result = stream.Read < u8 >( this->mSizeOfInstruction, 0 );
	result = stream.Read < u8 >( this->mSizeOfLuaNumber, 0 );
	result = stream.Read < u8 >( this->mTypeOfLuaNumber, 0 );
	
	return result;
}

//----------------------------------------------------------------//
ZLResultCode MOAILuaHeader::Read ( void* buffer, size_t size ) {

	if ( sizeof ( MOAILuaHeader ) <= size ) {
		memcpy ( this, buffer, sizeof ( MOAILuaHeader ));
		return ZL_OK;
	}
	return ZL_ERROR;
}

//----------------------------------------------------------------//
ZLResultCode MOAILuaHeader::Write ( ZLStream& stream ) const {

	ZLResultCodeAccumulator result;

	result = stream.Write < u32 >( this->mSignature ).mCode;
	result = stream.Write < u8 >( this->mVersion ).mCode;
	result = stream.Write < u8 >( this->mFormat ).mCode;
	result = stream.Write < u8 >( this->mByteOrder ).mCode;
	result = stream.Write < u8 >( this->mSizeOfInt ).mCode;
	result = stream.Write < u8 >( this->mSizeOfSizeT ).mCode;
	result = stream.Write < u8 >( this->mSizeOfInstruction ).mCode;
	result = stream.Write < u8 >( this->mSizeOfLuaNumber ).mCode;
	result = stream.Write < u8 >( this->mTypeOfLuaNumber ).mCode;
	
	return result;
}

//----------------------------------------------------------------//
ZLResultCode MOAILuaHeader::Write ( void* buffer, size_t size ) {

	if ( sizeof ( MOAILuaHeader ) <= size ) {
		memcpy ( buffer, this, sizeof ( MOAILuaHeader ));
		return ZL_OK;
	}
	return ZL_ERROR;
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	convert
	@text	Convert 32-bit Lua bytecode to 64-bit Lua bytecode.
	
	@in		string bytecode		32-bit bytecode
	@out	string bytecode		64-bit bytecode
*/
int MOAILuaUtil::_convert ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAILuaUtil, "S" )
	
	size_t bufflen;
	cc8* buffer = lua_tolstring ( L, 1, &bufflen );
	
	ZLByteStream srcStream;
	srcStream.SetBuffer ( buffer, bufflen, bufflen );
	
	MOAILuaHeader dstFormat;
	dstFormat.Init ();
	
	ZLMemStream dstStream;
	if ( self->Convert ( dstFormat, srcStream, dstStream )) {
	
		size_t resultSize = dstStream.GetLength ();
		void* result = malloc ( resultSize );
		
		dstStream.Seek ( 0, SEEK_SET );
		dstStream.ReadBytes ( result, resultSize );
		
		state.Push ( result, resultSize );
		free ( result);
	}
	else {
		state.CopyToTop ( 1 );
	}
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getHeader
	@text	Read the Lua bytecode header.
	
	@in		string bytecode
	@out	table header		Returns 'nil' is no valid signature is found.
*/
int MOAILuaUtil::_getHeader ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAILuaUtil, "" )
	
	MOAILuaHeader header;
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
	
		size_t bufflen;
		cc8* buffer = lua_tolstring ( L, -1, &bufflen );
	
		ZLByteStream stream;
		stream.SetBuffer ( buffer, bufflen, bufflen );
	
		header.Read ( stream );
	}
	else {
		header.Init ();
	}
	
	if ( header.mSignature == MOAILuaHeader::SIGNATURE ) {
	
		lua_newtable ( state );
		state.SetField ( -1, "signature",			header.mSignature );
		state.SetField ( -1, "version",				header.mVersion );
		state.SetField ( -1, "format",				header.mFormat );
		state.SetField ( -1, "byteOrder",			header.mByteOrder );
		state.SetField ( -1, "sizeOfInt",			header.mSizeOfInt );
		state.SetField ( -1, "sizeOfSizeT",			header.mSizeOfSizeT );
		state.SetField ( -1, "sizeOfInstruction",	header.mSizeOfInstruction );
		state.SetField ( -1, "sizeOfLuaNumber",		header.mSizeOfLuaNumber );
		state.SetField ( -1, "typeOfNumber",		header.mTypeOfLuaNumber );
		
		return 1;
	}
	return 0;
}

//================================================================//
// MOAILuaUtil
//================================================================//

//----------------------------------------------------------------//
bool MOAILuaUtil::Convert ( const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream ) {

	size_t base = srcStream.GetCursor ();

	MOAILuaHeader srcFormat;
	srcFormat.Read ( srcStream );
	
	if ( memcmp ( &srcFormat, &dstFormat, sizeof ( MOAILuaHeader )) == 0 ) {
		srcStream.Seek (( long )base, SEEK_SET );
		return false;
	}
	
	// functionality is limited now, but wouldn't be hard to extend
	assert ( srcFormat.mSignature == dstFormat.mSignature );
	assert ( srcFormat.mVersion == dstFormat.mVersion );
	assert ( srcFormat.mFormat == dstFormat.mFormat );
	assert ( srcFormat.mByteOrder == dstFormat.mByteOrder );
	assert ( srcFormat.mSizeOfLuaNumber == dstFormat.mSizeOfLuaNumber );
	assert ( srcFormat.mTypeOfLuaNumber == dstFormat.mTypeOfLuaNumber );
	
	dstFormat.Write ( dstStream );
	this->ConvertFunction ( srcFormat, dstFormat, srcStream, dstStream );

	return true;
}

//----------------------------------------------------------------//
void MOAILuaUtil::ConvertBytes ( ZLStream& srcStream, ZLStream& dstStream, size_t srcSize, size_t dstSize ) {

	void* buffer = alloca ( MAX ( srcSize, dstSize ));
	memset ( buffer, 0, dstSize );
	srcStream.ReadBytes ( buffer, srcSize );
	dstStream.WriteBytes ( buffer, dstSize );
}

//----------------------------------------------------------------//
void MOAILuaUtil::ConvertConst ( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream ) {

	u8 constType = srcStream.Read < u8 >( 0 );
	dstStream.Write < u8 >( constType );
	
	switch ( constType ) {
		
		case LUA_TBOOLEAN:
			dstStream.WriteStream ( srcStream, 1 );
			break;
		
		case LUA_TNIL:
			break;
		
		case LUA_TNUMBER:
			this->ConvertBytes ( srcStream, dstStream, srcFormat.mSizeOfLuaNumber, dstFormat.mSizeOfLuaNumber );
			break;
		
		case LUA_TSTRING:
			this->ConvertString ( srcFormat, dstFormat, srcStream, dstStream );
	}
}

//----------------------------------------------------------------//
void MOAILuaUtil::ConvertFunction ( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream ) {

	this->ConvertString ( srcFormat, dstFormat, srcStream, dstStream ); // source name
	
	this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream ); // line defined
	this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream ); // last line defined
	
	dstStream.WriteStream ( srcStream, 1 ); // number of upvalues
	dstStream.WriteStream ( srcStream, 1 ); // number of parameters
	dstStream.WriteStream ( srcStream, 1 ); // vararg flag
	dstStream.WriteStream ( srcStream, 1 ); // max stack size
	
	s64 nInstructions = this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream ); // size of instruction list
	
	if ( srcFormat.mSizeOfInstruction == dstFormat.mSizeOfInstruction ) {
		dstStream.WriteStream ( srcStream, ( size_t )( nInstructions * srcFormat.mSizeOfInstruction ));
	}
	else {
		for	( s64 i = 0; i < nInstructions; ++i ) {
			this->ConvertBytes ( srcStream, dstStream, srcFormat.mSizeOfInstruction, dstFormat.mSizeOfInstruction );
		}
	}
	
	s64 nConstants = this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream ); // size of constants list
	
	for ( s64 i = 0; i < nConstants; ++i ) {
		this->ConvertConst ( srcFormat, dstFormat, srcStream, dstStream );
	}
	
	s64 nFunctions = this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream ); // size of functions list
	
	for ( s64 i = 0; i < nFunctions; ++i ) {
		this->ConvertFunction ( srcFormat, dstFormat, srcStream, dstStream );
	}
	
	s64 nSourceLinePositions = this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream ); // size of source line positions list

	for ( s64 i = 0; i < nSourceLinePositions; ++i ) {
		this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream );
	}
	
	s64 nLocals = this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream ); // size of locals list

	for ( s64 i = 0; i < nLocals; ++i ) {
		this->ConvertString ( srcFormat, dstFormat, srcStream, dstStream );
		this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream );
		this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream );
	}
	
	s64 nUpvalues = this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream ); // size of upvalues list

	for ( s64 i = 0; i < nUpvalues; ++i ) {
		this->ConvertInt ( srcFormat, dstFormat, srcStream, dstStream );
		this->ConvertString ( srcFormat, dstFormat, srcStream, dstStream );
	}
}

//----------------------------------------------------------------//
s64 MOAILuaUtil::ConvertInt ( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream ) {

	s64 value = 0;
	srcStream.ReadBytes ( &value, srcFormat.mSizeOfInt ); // TODO: respect byte order
	dstStream.WriteBytes ( &value, dstFormat.mSizeOfInt );
	return value;
}

//----------------------------------------------------------------//
void MOAILuaUtil::ConvertString ( const MOAILuaHeader& srcFormat, const MOAILuaHeader& dstFormat, ZLStream& srcStream, ZLStream& dstStream ) {

	u64 size = 0;
	srcStream.ReadBytes ( &size, srcFormat.mSizeOfSizeT ); // TODO: respect byte order
	dstStream.WriteBytes ( &size, dstFormat.mSizeOfSizeT );
	dstStream.WriteStream ( srcStream, ( size_t ) size );
}

//----------------------------------------------------------------//
MOAILuaUtil::MOAILuaUtil () {
}

//----------------------------------------------------------------//
MOAILuaUtil::~MOAILuaUtil () {
}

//----------------------------------------------------------------//
void MOAILuaUtil::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "convert",					_convert },
		{ "getHeader",					_getHeader },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILuaUtil::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

#endif 