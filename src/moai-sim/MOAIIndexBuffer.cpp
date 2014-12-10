// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIGfxResourceMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	release
	@text	Release any memory held by this index buffer.
	
	@in		MOAIIndexBuffer self
	@out	nil
*/
int	MOAIIndexBuffer::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "U" )
	
	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserve
	@text	Set capacity of buffer.
	
	@in		MOAIIndexBuffer self
	@in		number nIndices
	@out	nil
*/
int	MOAIIndexBuffer::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "UN" )
	
	u32 capacity = state.GetValue < u32 >( 2, 0 );
	u32 indexSizeInBytes = state.GetValue < u32 >( 3, self->mIndexSizeInBytes );
	
	self->ReserveIndices ( capacity, indexSizeInBytes );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndex
	@text	Initialize an index.
	
	@in		MOAIIndexBuffer self
	@in		number idx
	@in		number value
	@out	nil
*/
int	MOAIIndexBuffer::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "UNN" )
	
	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 value	= state.GetValue < u32 >( 3, 1 ) - 1;
	
	self->SetIndex ( idx, value );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIIndexBuffer::_setIndexSizeInBytes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "UNN" )
	
	u32 indexSizeInBytes = state.GetValue < u32 >( 2, DEFAULT_INDEX_SIZE_IN_BYTES );
	assert ( self->IsValidIndexSize ( indexSizeInBytes ));
	self->mIndexSizeInBytes = indexSizeInBytes;
	
	return 0;
}

//================================================================//
// MOAIGfxQuadListDeck2D
//================================================================//

//----------------------------------------------------------------//
void MOAIIndexBuffer::Clear () {

	if ( this->mIndexBuffer ) {
		free ( this->mIndexBuffer );
		this->mIndexBuffer = 0;
		this->mCapacity = 0;
	}
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::CopyFromStream ( ZLStream& stream, u32 streamIndexSizeInBytes ) {

	this->Clear ();
	u32 count = ( u32 )stream.GetLength () / streamIndexSizeInBytes;
	
	this->ReserveIndices ( count );

	ZLByteStream indexStream;
	indexStream.SetBuffer ( this->mIndexBuffer, this->mCapacity * this->mIndexSizeInBytes );
	
	if ( this->mIndexSizeInBytes == streamIndexSizeInBytes ) {
		indexStream.WriteStream ( stream );
	}
	else {
		assert ( this->IsValidIndexSize ( streamIndexSizeInBytes ));
		
		if ( this->mIndexSizeInBytes == 2 ) {
			for ( u32 i = 0; i < count; ++i ) {
				indexStream.Write < u16 >(( u16 )stream.Read < u32 >( 0 ));
			}
		}
		else {
			for ( u32 i = 0; i < count; ++i ) {
				indexStream.Write < u32 >(( u32 )stream.Read < u16 >( 0 ));
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::Draw ( u32 zglPrimType ) {

	zglDrawElements ( zglPrimType, this->mCapacity, this->mIndexSizeInBytes == 2 ? ZGL_TYPE_UNSIGNED_SHORT : ZGL_TYPE_UNSIGNED_INT, 0 );
}

//----------------------------------------------------------------//
u32 MOAIIndexBuffer::GetLoadingPolicy () {

	return MOAIGfxResource::LOADING_POLICY_CPU_GPU_BIND;
}

//----------------------------------------------------------------//
bool MOAIIndexBuffer::IsValidIndexSize ( u32 indexSizeInBytes ) {

	return (( indexSizeInBytes == 2 ) || ( indexSizeInBytes == 4 ));
}

//----------------------------------------------------------------//
MOAIIndexBuffer::MOAIIndexBuffer () :
	mIndexSizeInBytes ( DEFAULT_INDEX_SIZE_IN_BYTES ),
	mIndexBuffer ( 0 ),
	mCapacity ( 0 ),
	mGLBufferID ( 0 ),
	mHint ( ZGL_BUFFER_USAGE_STATIC_DRAW ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIIndexBuffer::~MOAIIndexBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAIIndexBuffer::OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnGPUBind () {

	if ( this->mGLBufferID ) {
		zglBindBuffer ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY, this->mGLBufferID );
	}
}

//----------------------------------------------------------------//
bool MOAIIndexBuffer::OnGPUCreate () {

	if ( this->mCapacity ) {
		
		this->mGLBufferID = zglCreateBuffer ();
		if ( this->mGLBufferID ) {
		
			zglBindBuffer ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY, this->mGLBufferID );
			zglBufferData ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY, this->mCapacity * this->mIndexSizeInBytes, this->mIndexBuffer, this->mHint );
		
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnGPUDestroy () {

	MOAIGfxResourceMgr::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_BUFFER, this->mGLBufferID );
	this->mGLBufferID = 0;
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnGPULost () {

	this->mGLBufferID = 0;
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnGPUUnbind () {

	zglBindBuffer ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY, 0 );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "release",			_release },
		{ "reserve",			_reserve },
		{ "setIndex",			_setIndex },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::ReserveIndices ( u32 capacity ) {

	this->ReserveIndices ( capacity, this->mIndexSizeInBytes );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::ReserveIndices ( u32 capacity, u32 indexSizeInBytes ) {

	this->Clear ();
	
	assert ( this->IsValidIndexSize ( indexSizeInBytes ));
	
	this->mCapacity = capacity;
	this->mIndexSizeInBytes = indexSizeInBytes;
	
	size_t bufferSize = capacity * indexSizeInBytes;
	this->mIndexBuffer = malloc ( bufferSize );
	
	this->DoCPUAffirm ();
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	u32 indexCount			= state.GetField < u32 >( -1, "mTotalIndices", 0 );
	u32 indexSizeInBytes	= state.GetField < u32 >( -1, "mIndexSizeInBytes", LEGACY_INDEX_SIZE_IN_BYTES ); // default is 4 bytes
	this->mHint				= state.GetField < u32 >( -1, "mHint", 0 );

	this->ReserveIndices ( indexCount, indexSizeInBytes );

	state.GetField ( -1, "mIndices" );

	if ( state.IsType ( -1, LUA_TSTRING )) {
		
		size_t bufferSize = this->mCapacity * this->mIndexSizeInBytes;
		
		STLString zipString = lua_tostring ( state, -1 );
		size_t unzipLen = zipString.zip_inflate ( this->mIndexBuffer, bufferSize );
		assert ( unzipLen == bufferSize ); // TODO: fail gracefully
	}
	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	state.SetField ( -1, "mTotalIndices", this->mCapacity );
	state.SetField ( -1, "mIndexSizeInBytes", this->mIndexSizeInBytes );
	state.SetField ( -1, "mHint", this->mHint );
	
	STLString zipString;
	zipString.zip_deflate ( this->mIndexBuffer, this->mCapacity * this->mIndexSizeInBytes );
	
	lua_pushstring ( state, zipString.str ());
	lua_setfield ( state, -2, "mIndices" );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::SetIndex ( u32 idx, u32 value ) {

	if ( idx < this->mCapacity) {
	
		if ( this->mIndexSizeInBytes == 2 ) {
			(( u16* )this->mIndexBuffer )[ idx ] = ( u16 )value;
		}
		else {
			(( u32* )this->mIndexBuffer )[ idx ] = value;
		}
	}
}
