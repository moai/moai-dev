// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAIIndexBufferGL.h>
#include <moai-gfx/MOAIVertexFormatGL.h>
#include <moai-gfx/MOAIVertexFormatMgrGL.h>

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

		@in		MOAIIndexBufferGL self
		@in		MOAIStream stream
		@opt	number sourceSizeInBytes		Default value is 4.
		@out	nil

	@overload

		@in		MOAIIndexBufferGL self
		@in		MOAIIndexBufferGL indexBuffer		Source size in bytes taken from source buffer.
		@out	nil
*/
int MOAIIndexBufferGL::_copyFromStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBufferGL, "U" )
	
	// TODO: report trunctations!
	
	MOAIIndexBufferGL* idxBuffer = state.GetLuaObject < MOAIIndexBufferGL >( 2, false );
	if ( idxBuffer ) {
	
		size_t size = state.GetValue < u32 >( 3, ( u32 )( idxBuffer->GetLength () - idxBuffer->GetCursor () ));
		self->CopyFromStream ( *idxBuffer, size, idxBuffer->mIndexSize );
	}
	else {
	
		MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
		if ( stream ) {
			size_t size = state.GetValue < u32 >( 3, ( u32 )( stream->GetLength () - stream->GetCursor () ));
			u32 srcInputSizeInBytes = state.GetValue ( 4, 4 );
			self->CopyFromStream ( *stream, size, srcInputSizeInBytes );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	countElements
	@text	Calculates the number of elements given a prim type.
 
	@in		MOAIIndexBufferGL self
	@opt	number primType				Default value is GL_TRIANGLES.
	@out	number totalElements
*/
int MOAIIndexBufferGL::_countElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBufferGL, "U" )

	u32 totalElements = 0;
	
	// prim type, index size in bytes
	u32  primType = state.GetValue < u32 >( 2, ZGL_PRIM_TRIANGLES );
	
	totalElements = ( u32 )( self->GetSize () / self->mIndexSize );
	
	if ( primType == ZGL_PRIM_LINES ) {
		totalElements /= 2;
	}
	else if ( primType == ZGL_PRIM_TRIANGLES ) {
		totalElements /= 3;
	}
	
	state.Push ( totalElements );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	printIndices
	@text	Print the indices (for debugging purposes).
 
	@in		MOAIIndexBufferGL self
	@out	nil
*/
int MOAIIndexBufferGL::_printIndices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBufferGL, "U" )
	
	self->PrintIndices ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndexSize
	@text	Sets the index size in bytes.
	
			NOTE: The current implementation does not convert between sizes;
			going from 4 to 2 will result in twice as many indices.
 
	@in		MOAIIndexBufferGL self
	@out	nil
*/
int MOAIIndexBufferGL::_setIndexSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBufferGL, "U" )
	
	u32 indexSize = state.GetValue < u32 >( 2, 4 );
	self->SetIndexSize ( indexSize ); // TODO: convert index size
	return 0;
}

//================================================================//
// MOAIIndexBufferGL
//================================================================//

//----------------------------------------------------------------//
u32 MOAIIndexBufferGL::CountIndices () {

	return this->GetLength () / this->mIndexSize;
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::CopyFromStream ( ZLStream& stream, size_t size, u32 srcInputSizeInBytes ) {

	u32 idxSizeInBytes = this->mIndexSize;	
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
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
u32 MOAIIndexBufferGL::GetIndex ( u32 element ) {

	const void* data = this->ZLCopyOnWrite::GetBuffer ();
	return this->mIndexSize == 4 ? (( const u32* )data )[ element ] : (( const u16* )data )[ element ];
}

//----------------------------------------------------------------//
MOAIIndexBufferGL::MOAIIndexBufferGL () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( ZLIndexBufferGL )
		RTTI_EXTEND ( MOAIAbstractGfxResourceGL )
		RTTI_EXTEND ( MOAIAbstractGfxBufferGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIIndexBufferGL::~MOAIIndexBufferGL () {
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::PrintIndices () {

	size_t cursor = this->GetCursor ();
	u32 length = ( u32 )( cursor / this->mIndexSize );
	
	if ( length ) {
		this->Seek ( 0, SEEK_SET );
		for ( u32 i = 0; i < length; ++i ) {
		
			u32 v = this->mIndexSize == 4 ? this->Read < u32 >( 0 ) : ( u32 )this->Read < u16 >( 0 );
			printf ( "%d: %d\n", i, v );
		}
		this->SetCursor ( cursor );
	}
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGfxResourceGL < ZLIndexBufferGL >::RegisterLuaClass ( state );
	MOAIAbstractGfxBufferGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIGfxResourceGL < ZLIndexBufferGL >::RegisterLuaFuncs ( state );
	MOAIAbstractGfxBufferGL::RegisterLuaFuncs ( state );
	
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
void MOAIIndexBufferGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIAbstractGfxBufferGL::SerializeIn ( state, serializer );
	
	this->mIndexSize = state.GetFieldValue < cc8*, u32 >( -1, "mIndexSize", 0 );
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIAbstractGfxBufferGL::SerializeOut ( state, serializer );
	
	state.SetField < cc8*, u32 >( -1, "mIndexSize", this->mIndexSize );
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::SetIndexSize ( u32 idxSize ) {

	assert (( idxSize == 2 ) || ( idxSize == 4 ));
	this->mIndexSize = idxSize;
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::WriteIndex ( u32 index ) {

	if ( this->mIndexSize == 4 ) {
		this->Write < u32 >( index );
	}
	else {
		this->Write < u16 >(( u16 )index );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLGfxBufferGL& MOAIIndexBufferGL::MOAIGfxBuffer_ZLGfxBuffer () {

	return *this;
}
