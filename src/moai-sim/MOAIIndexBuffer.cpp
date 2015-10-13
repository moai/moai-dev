// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIGfxBufferBase
//================================================================//

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	copyFromStream
	@text	Copy the buffer contents from a stream. Optionally convert
			index size in bytes between streams.
			
			Warning: going from 4 bytes to 2 bytes is supported, but use
			at your own risk; truncations will not be reported.

	@overload

		@in		MOAIIndexBuffer self
		@in		MOAIStream stream
		@opt	number sourceSizeInBytes		Default value is 4.
		@out	nil

	@overload

		@in		MOAIIndexBuffer self
		@in		MOAIIndexBuffer indexBuffer		Source size in bytes taken from source buffer.
		@out	nil
*/
int MOAIIndexBuffer::_copyFromStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "U" )
	
	// TODO: report trunctations!
	
	MOAIIndexBuffer* idxBuffer = state.GetLuaObject < MOAIIndexBuffer >( 2, true );
	if ( idxBuffer ) {
	
		self->CopyFromStream ( *idxBuffer, idxBuffer->mIndexSize );
	}
	else {
	
		MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
		if ( stream ) {
			u32 srcInputSizeInBytes = state.GetValue ( 3, 4 );
			self->CopyFromStream ( *stream, srcInputSizeInBytes );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	countElements
	@text	Calculates the number of elements given a prim type.
 
	@in		MOAIIndexBuffer self
	@opt	number primType				Default value is GL_TRIANGLES.
	@out	number totalElements
*/
int MOAIIndexBuffer::_countElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "U" )

	u32 totalElements = 0;
	
	// prim type, index size in bytes
	if ( state.CheckParams ( 2, "N", false )) {
	
		u32  primType = state.GetValue < u32 >( 2, ZGL_PRIM_TRIANGLES );
		
		totalElements = self->GetSize () / self->mIndexSize;
		
		if ( primType == ZGL_PRIM_LINES ) {
			totalElements /= 2;
		}
		else if ( primType == ZGL_PRIM_TRIANGLES ) {
			totalElements /= 3;
		}
	}
	
	state.Push ( totalElements );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	printIndices
	@text	Print the indices (for debugging purposes).
 
	@in		MOAIIndexBuffer self
	@out	nil
*/
int MOAIIndexBuffer::_printIndices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "U" )
	
	self->PrintIndices ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndexSize
	@text	Sets the index size in bytes.
	
			NOTE: The current implementation does not convert between sizes;
			going from 4 to 2 will result in twice as many indices.
 
	@in		MOAIIndexBuffer self
	@out	nil
*/
int MOAIIndexBuffer::_setIndexSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "U" )
	
	u32 indexSize = state.GetValue < u32 >( 2, 4 );
	self->SetIndexSize ( indexSize ); // TODO: convert index size
	return 0;
}

//================================================================//
// MOAIIndexBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIIndexBuffer::CopyFromStream ( ZLStream& stream, u32 srcInputSizeInBytes ) {

	u32 idxSizeInBytes = this->mIndexSize;

	u32 size = ( u32 )( stream.GetLength () - stream.GetCursor ());
	
	u32 totalIndices = ( u32 )( size / srcInputSizeInBytes );
	
	this->Reserve ( totalIndices * idxSizeInBytes );
	
	for ( u32 i = 0; i < totalIndices; ++i ) {
	
		u32 idx = 0;
		
		if ( srcInputSizeInBytes == 4 ) {
			idx = stream.Read < u32 >( 0 );
		}
		else {
			idx = stream.Read < u16 >( 0 );
		}
	
		if ( idxSizeInBytes == 4 ) {
			this->Write < u32 >( idx );
		}
		else {
			this->Write < u16 >(( u16 )idx );
		}
	}
	this->mNeedsFlush = true;
}

//----------------------------------------------------------------//
MOAIIndexBuffer::MOAIIndexBuffer () {
	
	this->mTarget = ZGL_BUFFER_TARGET_ELEMENT_ARRAY;
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxBuffer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIIndexBuffer::~MOAIIndexBuffer () {
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::PrintIndices () {

	size_t cursor = this->GetCursor ();
	u32 length = cursor / this->mIndexSize;
	
	if ( length ) {
		this->Seek ( 0, SEEK_SET );
		for ( u32 i = 0; i < length; ++i ) {
		
			u32 v = this->mIndexSize == 4 ? this->Read < u32 >( 0 ) : this->Read < u16 >( 0 );
			printf ( "%d: %d\n", i, v );
		}
		this->Seek ( cursor, SEEK_SET );
	}
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGfxBuffer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIGfxBuffer::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "copyFromStream",			_copyFromStream },
		{ "countElements",			_countElements },
		{ "printIndices",			_printIndices },
		{ "setIndexSize",			_setIndexSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIGfxBuffer::SerializeIn ( state, serializer );
	
	this->mIndexSize = state.GetField < u32 >( -1, "mIndexSize", 0 );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIGfxBuffer::SerializeOut ( state, serializer );
	
	state.SetField < u32 >( -1, "mIndexSize", this->mIndexSize );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::SetIndexSize ( u32 idxSize ) {

	assert (( idxSize == 2 ) || ( idxSize == 4 ));
	this->mIndexSize = idxSize;
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::WriteIndex ( u32 index ) {

	if ( this->mIndexSize == 4 ) {
		this->Write < u32 >( index );
	}
	else {
		this->Write < u16 >(( u16 )index );
	}
}
