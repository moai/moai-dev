// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIGfxBufferBase
//================================================================//

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	computeBounds
	@text	Calculates the buffer's axis-aligned bounding box. Returns
			nil if buffer is empty.
 
	@in		MOAIVertexBuffer self
	@in		MOAIVertexFormat format
	@out	xMin
	@out	yMin
	@out	zMin
	@out	xMax
	@out	yMax
	@out	zMax
*/
int MOAIVertexBuffer::_computeBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	bool hasBounds = false;
	ZLBox bounds;
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	
	// vertex format
	if ( state.CheckParams ( 2, "U", false )) {
	
		MOAIVertexFormat* format = state.GetLuaObject < MOAIVertexFormat >( 2, 0 );
		if ( format ) {
			u32 length = state.GetValue < u32 >( 3, ( u32 )self->GetCursor ());
			self->Seek ( 0, SEEK_SET );
			hasBounds = format->ComputeBounds ( bounds, *self, length );
		}
	}
	
	if ( hasBounds ) {
		state.Push ( bounds );
		return 6;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	countElements
	@text	Get the number of vertices given either a vertex size or
			a vertex format.
 
	@overload
 
		@in		MOAIVertexBuffer self
		@in		number vertexSize
		@out	number vertexCount
	
	@overload
 
		@in		MOAIVertexBuffer self
		@in		MOAIVertexFormat format
		@out	number vertexCount
*/
int MOAIVertexBuffer::_countElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )

	u32 totalElements = 0;
	
	// size
	if ( state.CheckParams ( 2, "N", false )) {
		
		u32  elementSize = state.GetValue < u32 >( 2, 4 );
		totalElements = self->GetSize () / elementSize;
	}
	
	// vertex format
	else if ( state.CheckParams ( 2, "U", false )) {
	
		MOAIVertexFormat* format = state.GetLuaObject < MOAIVertexFormat >( 2, 0 );
		if ( format ) {
			u32 length = state.GetValue < u32 >( 3, ( u32 )self->GetLength ());
			totalElements = ( u32 )( length / format->GetVertexSize ());
		}
	}
	
	state.Push ( totalElements );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	printVertices
	@text	Print the vertices (for debugging purposes).
 
	@in		MOAIVertexBuffer self
	@in		MOAIVertexFormat format
	@out	nil
*/
int MOAIVertexBuffer::_printVertices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	MOAIVertexFormat* format = state.GetLuaObject < MOAIVertexFormat >( 2, true );
	if ( format ) {
		self->PrintVertices ( *format );
	}
	return 0;
}

//================================================================//
// MOAIVertexBuffer
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer () {
	
	this->mTarget = ZGL_BUFFER_TARGET_ARRAY;
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxBuffer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexBuffer::~MOAIVertexBuffer () {
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::PrintVertices ( MOAIVertexFormat& vertexFormat ) {

	size_t cursor = this->GetCursor ();
	if ( cursor ) {
		this->Seek ( 0, SEEK_SET );
		vertexFormat.PrintVertices ( *this, cursor );
		this->Seek ( cursor, SEEK_SET );
	}
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGfxBuffer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIGfxBuffer::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "computeBounds",			_computeBounds },
		{ "countElements",			_countElements },
		{ "printVertices",			_printVertices },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIGfxBuffer::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIGfxBuffer::SerializeOut ( state, serializer );
}
