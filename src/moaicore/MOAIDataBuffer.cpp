// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataIOTask.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISim.h>

//================================================================//
// MOAIDataBuffer
//================================================================//

//----------------------------------------------------------------//
/**	@name	base64Decode
	@text	If a string is provided, decodes it as a base64 encoded string.  Otherwise, decodes the current data stored in this object as a base64 encoded sequence of characters.

	@opt	MOAIDataBuffer self
	@opt	string data				The string data to decode.  You must either provide either a MOAIDataBuffer (via a :base64Decode type call) or string data (via a .base64Decode type call), but not both.
	@out	string output			If passed a string, returns either a string or nil depending on whether it could be decoded.  Otherwise the decoding occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_base64Decode ( lua_State* L ) {
	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Base64Decode ( 1 ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1, true );
	if ( self ) {
		if ( state.IsType ( 2, LUA_TSTRING )) {
			size_t len;
			cc8* str = lua_tolstring ( state, 2, &len );
			self->Load (( void* )str, len );
		}
		self->Base64Decode ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	base64Encode
	@text	If a string is provided, encodes it in base64.  Otherwise, encodes the current data stored in this object as a base64 encoded sequence of characters.

	@opt	MOAIDataBuffer self
	@opt	string data				The string data to encode.  You must either provide either a MOAIDataBuffer (via a :base64Encode type call) or string data (via a .base64Encode type call), but not both.
	@out	string output			If passed a string, returns either a string or nil depending on whether it could be encoded.  Otherwise the encoding occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_base64Encode ( lua_State* L ) {
	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Base64Encode ( 1 ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1, true );
	if ( self ) {
		if ( state.IsType ( 2, LUA_TSTRING )) {
			size_t len;
			cc8* str = lua_tolstring ( state, 2, &len );
			self->Load (( void* )str, len );
		}
		self->Base64Encode ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDataBuffer::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBuffer, "U" );
	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	deflate
	@text	Compresses the string or the current data stored in this object using the DEFLATE algorithm.

	@opt	MOAIDataBuffer self
	@opt	string data				The string data to deflate.  You must either provide either a MOAIDataBuffer (via a :deflate type call) or string data (via a .deflate type call), but not both.
	@in		number level			The level used in the DEFLATE algorithm.  Pass nil to use the default value.
	@in		number windowBits		The window bits used in the DEFLATE algorithm.  Pass nil to use the default value.
	@out	string output			If passed a string, returns either a string or nil depending on whether it could be compressed.  Otherwise the compression occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_deflate ( lua_State* L ) {
	MOAILuaState state ( L );

	int level = state.GetValue < int >( 2, USDeflateWriter::DEFAULT_LEVEL );
	int windowBits = state.GetValue < int >( 3, USDeflateWriter::DEFAULT_WBITS );

	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Deflate ( 1, level, windowBits ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1, true );
	if ( self ) {
		self->Deflate ( level, windowBits );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getSize
	@text	Returns the number of bytes in this data buffer object.

	@in		MOAIDataBuffer self
	@out	number size				The number of bytes in this data buffer object.
*/
int MOAIDataBuffer::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBuffer, "U" );

	void* bytes;
	size_t size;
	self->Lock ( &bytes, &size );
	
	lua_pushnumber ( state, size );

	self->Unlock ();

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getString
	@text	Returns the contents of the data buffer object as a string value.

	@in		MOAIDataBuffer self
	@out	string data				The data buffer object as a string.
*/
int MOAIDataBuffer::_getString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBuffer, "U" );

	size_t size;
	void* buffer;
	
	self->Lock ( &buffer, &size );
	lua_pushlstring ( state, ( cc8* )buffer, size );
	self->Unlock ();

	return 1;
}

//----------------------------------------------------------------//
/**	@name	hexDecode
	@text	If a string is provided, decodes it as a hex encoded string.  Otherwise, decodes the current data stored in this object as a hex encoded sequence of bytes.

	@opt	MOAIDataBuffer self
	@opt	string data				The string data to decode.  You must either provide either a MOAIDataBuffer (via a :hexDecode type call) or string data (via a .hexDecode type call), but not both.
	@out	string output			If passed a string, returns either a string or nil depending on whether it could be decoded.  Otherwise the decoding occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_hexDecode ( lua_State* L ) {
	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.HexDecode ( 1 ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1, true );
	if ( self ) {
		if ( state.IsType ( 2, LUA_TSTRING )) {
			size_t len;
			cc8* str = lua_tolstring ( state, 2, &len );
			self->Load (( void* )str, len );
		}
		self->HexDecode ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	hexEncode
	@text	If a string is provided, encodes it in hex.  Otherwise, encodes the current data stored in this object as a hex encoded sequence of characters.

	@opt	MOAIDataBuffer self
	@opt	string data				The string data to encode.  You must either provide either a MOAIDataBuffer (via a :hexEncode type call) or string data (via a .hexEncode type call), but not both.
	@out	string output			If passed a string, returns either a string or nil depending on whether it could be encoded.  Otherwise the encoding occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_hexEncode ( lua_State* L ) {
	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.HexEncode ( 1 ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1, true );
	if ( self ) {
		if ( state.IsType ( 2, LUA_TSTRING )) {
			size_t len;
			cc8* str = lua_tolstring ( state, 2, &len );
			self->Load (( void* )str, len );
		}
		self->HexEncode ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	inflate
	@text	Decompresses the string or the current data stored in this object using the DEFLATE algorithm.

	@opt	MOAIDataBuffer self
	@opt	string data				The string data to inflate.  You must either provide either a MOAIDataBuffer (via a :inflate type call) or string data (via a .inflate type call), but not both.
	@in		number windowBits		The window bits used in the DEFLATE algorithm.  Pass nil to use the default value.
	@out	string output			If passed a string, returns either a string or nil depending on whether it could be decompressed.  Otherwise the decompression occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_inflate ( lua_State* L ) {
	MOAILuaState state ( L );

	int windowBits = state.GetValue < int >( 2, USDeflateReader::DEFAULT_WBITS );

	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Inflate ( 1, windowBits ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1, true );
	if ( self ) {
		self->Inflate ( windowBits );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Copies the data from the given file into this object.  This method is a synchronous operation and will block until the file is loaded.

	@in		MOAIDataBuffer self
	@in		string filename			The path to the file that the data should be loaded from.
	@opt	number detectZip		One of MOAIDataBuffer.NO_UNZIP, MOAIDataBuffer.NO_UNZIP, MOAIDataBuffer.NO_UNZIP
	@opt	number windowBits		The window bits used in the DEFLATE algorithm.  Pass nil to use the default value.
	@out	boolean success			Whether the file could be loaded into the object.
*/
int MOAIDataBuffer::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBuffer, "US" );

	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	u32 detectZip	= state.GetValue < u32 >( 3, NO_INFLATE );
	int windowBits	= state.GetValue < int >( 4, USDeflateReader::DEFAULT_WBITS );

	bool success = self->Load ( filename );
	
	if ( success && ( detectZip != NO_INFLATE )) {
		if (( detectZip == FORCE_INFLATE ) || ( MOAIDataBuffer::IsZipFilename ( filename ))) {
			success = self->Inflate ( windowBits );
		}
	}
	
	lua_pushboolean ( state, success );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	loadAsync
	@text	Asynchronously copies the data from the given file into this object.  This method is an asynchronous
			operation and will return immediately.

	@in		MOAIDataBuffer self
	@in		string filename			The path to the file that the data should be loaded from.
	@in		MOAITaskQueue queue		The queue to perform the loading operation.
	@opt	function callback		The function to be called when the asynchronous operation is complete. The MOAIDataBuffer is passed as the first parameter.
	@opt	number detectZip		One of MOAIDataBuffer.NO_INFLATE, MOAIDataBuffer.FORCE_INFLATE, MOAIDataBuffer.INFLATE_ON_EXT
	@opt	bool inflateAsync		'true' to inflate on task thread. 'false' to inflate on subscriber thread. Default value is 'true.'
	@opt	number windowBits		The window bits used in the DEFLATE algorithm.  Pass nil to use the default value.
	@out	MOAIDataIOTask task	A new MOAIDataIOTask which indicates the status of the task.
*/
int MOAIDataBuffer::_loadAsync ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBuffer, "USU" );

	cc8* filename			= state.GetValue < cc8* >( 2, "" );
	MOAITaskQueue* queue	= state.GetLuaObject < MOAITaskQueue >( 3, true );
	u32 detectZip			= state.GetValue < u32 >( 5, NO_INFLATE );
	bool inflateAsync		= state.GetValue < bool >( 6, false );
	int windowBits			= state.GetValue < int >( 7, USDeflateReader::DEFAULT_WBITS );

	if ( !queue ) return 0;

	MOAIDataIOTask* task = new MOAIDataIOTask ();
	task->PushLuaUserdata ( state );
	task->Init ( filename, *self, MOAIDataIOTask::LOAD_ACTION );
	task->SetCallback ( L, 4 );
	
	if (( detectZip != NO_INFLATE ) && (( detectZip == FORCE_INFLATE ) || ( MOAIDataBuffer::IsZipFilename ( filename )))) {
		task->SetInflateOnLoad ( true, inflateAsync, windowBits );
	}
	
	task->Start ( *queue, MOAISim::Get ().GetTaskSubscriber ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name	save
	@text	Saves the data in this object to the given file.  This method is a synchronous operation and will block until the data is saved.

	@in		MOAIDataBuffer self
	@in		string filename			The path to the file that the data should be saved to.
	@out	boolean success			Whether the data could be saved to the file.
*/
int MOAIDataBuffer::_save ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBuffer, "US" );

	cc8* filename = lua_tostring ( state, 2 );

	bool success = self->Save ( filename );
	lua_pushboolean ( state, success );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	saveAsync
	@text	Asynchronously saves the data in this object to the given file.  This method is an asynchronous
			operation and will return immediately.

	@in		MOAIDataBuffer self
	@in		string filename			The path to the file that the data should be saved to.
	@in		MOAITaskQueue queue		The queue to perform the saving operation.
	@opt	function callback		The function to be called when the asynchronous operation is complete. The MOAIDataBuffer is passed as the first parameter.
	@out	MOAIDataIOTask task		A new MOAIDataIOTask which indicates the status of the task.
*/
int MOAIDataBuffer::_saveAsync ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBuffer, "USU" );

	cc8* filename			= state.GetValue < cc8* >( 2, "" );
	MOAITaskQueue* queue	= state.GetLuaObject < MOAITaskQueue >( 3, true );

	if ( !queue ) return 0;

	MOAIDataIOTask* task = new MOAIDataIOTask ();
	task->Init ( filename, *self, MOAIDataIOTask::SAVE_ACTION );
	task->PushLuaUserdata( state );
	task->SetCallback ( L, 4 );
	task->Start ( *queue, MOAISim::Get ().GetTaskSubscriber ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setString
	@text	Replaces the contents of this object with the string specified.

	@in		MOAIDataBuffer self
	@in		string data				The string data to replace the contents of this object with.
	@out	nil
*/
int MOAIDataBuffer::_setString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBuffer, "US" );

	size_t len;
	cc8* str = lua_tolstring ( state, 2, &len );
	
	self->Load (( void* )str, len );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	toCppHeader
	@text	Convert data to CPP header file.

	@overload

		@in		string data				The string data to encode
		@in		string name
		@opt	number columns			Default value is 12
		@out	string output
	
	@overload

		@in		MOAIDataBuffer data		The data buffer to encode
		@in		string name
		@opt	number columns			Default value is 12
		@out	string output
*/
int MOAIDataBuffer::_toCppHeader ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* name		= state.GetValue < cc8* >( 2, "" );
	u32 columns		= state.GetValue < u32 >( 3, 12 );
	
	if ( !strlen ( name )) return 0;
	
	USMemStream memStream;
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
		
		size_t size;
		const void* bytes = lua_tolstring ( state, 1, &size );
		USHexDump::DumpAsCPPHeader ( memStream, name, bytes, size, columns );
	}
	
	MOAIDataBuffer* dataBuffer = state.GetLuaObject < MOAIDataBuffer >( 1, true );
	if ( dataBuffer ) {
		
		size_t size;
		void* bytes;
		dataBuffer->Lock ( &bytes, &size );
		USHexDump::DumpAsCPPHeader ( memStream, name, bytes, size, columns );
	}
	
	if ( memStream.GetLength ()) {
		
		memStream.Seek ( 0, SEEK_SET );
		STLString result = memStream.ReadString ( memStream.GetLength ());
		
		lua_pushstring ( state, result );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIDataBuffer
//================================================================//

//----------------------------------------------------------------//
bool MOAIDataBuffer::Base64Decode () {

	USBase64Reader base64;
	return this->Decode ( base64 );
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::Base64Encode () {

	USBase64Writer base64;
	return this->Encode ( base64 );
}

//----------------------------------------------------------------//
void MOAIDataBuffer::Clear () {

	this->mMutex.Lock ();
	this->mBytes.Clear ();
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::Decode ( USStreamReader& reader ) {
	
	this->mMutex.Lock ();
	
	USByteStream cryptStream;
	cryptStream.SetBuffer ( this->mBytes, this->mBytes.Size ());
	cryptStream.SetLength ( this->mBytes.Size ());
	
	USMemStream plainStream;
	
	reader.Open ( &cryptStream );
	plainStream.WriteStream ( reader );
	reader.Close ();
	
	size_t len = plainStream.GetLength ();
	this->mBytes.Init ( len );
	
	plainStream.Seek ( 0, SEEK_SET );
	plainStream.ReadBytes ( this->mBytes, len );
	
	this->mMutex.Unlock ();
	return true;
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::Deflate ( int level, int windowBits ) {

	USDeflateWriter deflater;
	deflater.SetCompressionLevel ( level );
	deflater.SetWindowBits ( windowBits );
	
	return this->Encode ( deflater );
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::Encode ( USStreamWriter& writer ) {
	
	this->mMutex.Lock ();
	
	USMemStream stream;
	
	writer.Open ( &stream );
	writer.WriteBytes ( this->mBytes, this->mBytes.Size ());
	writer.Close ();
	
	size_t len = stream.GetLength ();
	this->mBytes.Init ( len );
	
	stream.Seek ( 0, SEEK_SET );
	stream.ReadBytes ( this->mBytes, len );

	this->mMutex.Unlock ();
	return true;
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::HexDecode () {

	USHexReader hex;
	return this->Decode ( hex );
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::HexEncode () {

	USHexWriter hex;
	return this->Encode ( hex );
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::Inflate ( int windowBits ) {

	USDeflateReader inflater;
	inflater.SetWindowBits ( windowBits );
	
	return this->Decode ( inflater );
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::IsZipFilename ( cc8* filename ) {

	size_t filenameLength = strlen ( filename );
	if ( filenameLength > 3 ) {
		
		char ext [ 5 ];
		for ( size_t i = filenameLength - 4, j = 0; i < filenameLength; i++, j++ ) {
			ext [ j ] = ( char )tolower ( filename [ i ]);
		}
		ext [ 4 ] = 0;
		
		if ( strcmp ( ext, ".zip" ) == 0 ) {
			return true;
		}
		
		if ( strcmp ( &ext [ 1 ], ".gz" ) == 0 ) {
			return true;
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::Load ( cc8* filename ) {

	USFileStream in;
	if ( !in.OpenRead ( filename )) return false;

	this->mMutex.Lock ();

	u32 size = in.GetLength ();
	this->mBytes.Init ( size );
	in.ReadBytes ( this->mBytes , size );

	this->mMutex.Unlock ();

	return true;
}

//----------------------------------------------------------------//
void MOAIDataBuffer::Load ( void* bytes, size_t size ) {

	this->mMutex.Lock ();
	
	this->mBytes.Init ( size );
	memcpy ( this->mBytes.Data (), bytes, size );
	
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
void MOAIDataBuffer::Lock ( void** bytes, size_t* size ) {

	this->mMutex.Lock ();
	( *bytes ) = this->mBytes;
	( *size ) = this->mBytes.Size ();
}

//----------------------------------------------------------------//
MOAIDataBuffer::MOAIDataBuffer () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDataBuffer::~MOAIDataBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIDataBuffer::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "NO_INFLATE",		( u32 )NO_INFLATE );
	state.SetField ( -1, "FORCE_INFLATE",	( u32 )FORCE_INFLATE );
	state.SetField ( -1, "INFLATE_ON_EXT",	( u32 )INFLATE_ON_EXT );

	luaL_Reg regTable [] = {
		{ "base64Decode",	_base64Decode },
		{ "base64Encode",	_base64Encode },
		{ "deflate",		_deflate },
		{ "hexDecode",		_hexDecode },
		{ "hexEncode",		_hexEncode },
		{ "inflate",		_inflate },
		{ "toCppHeader",	_toCppHeader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDataBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "base64Decode",	_base64Decode },
		{ "base64Encode",	_base64Encode },
		{ "clear",			_clear },
		{ "deflate",		_deflate },
		{ "getSize",		_getSize },
		{ "getString",		_getString },
		{ "hexDecode",		_hexDecode },
		{ "hexEncode",		_hexEncode },
		{ "inflate",		_inflate },
		{ "load",			_load },
		{ "loadAsync",		_loadAsync },
		{ "save",			_save },
		{ "saveAsync",		_saveAsync },
		{ "setString",		_setString },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIDataBuffer::Save ( cc8* filename ) {

	USFileStream out;

	if ( !out.OpenWrite ( filename )) return false;

	this->mMutex.Lock ();
	out.WriteBytes ( this->mBytes , this->mBytes.Size ());
	this->mMutex.Unlock ();

	return true;
}

//----------------------------------------------------------------//
void MOAIDataBuffer::Unlock () {

	this->mMutex.Unlock ();
}
