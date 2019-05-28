// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAIVertexBufferGL.h>
#include <moai-gfx/MOAIVertexFormatGL.h>
#include <moai-gfx/MOAIVertexFormatMgrGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	computeBounds
	@text	Calculates the buffer's axis-aligned bounding box. Returns
			nil if buffer is empty.
 
	@in		MOAIVertexBufferGL self
	@in		MOAIVertexFormatGL format
	@out	xMin
	@out	yMin
	@out	zMin
	@out	xMax
	@out	yMax
	@out	zMax
*/
int MOAIVertexBufferGL::_computeBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBufferGL, "U" )
	
	bool hasBounds = false;
	ZLBox aabb = ZLBox::EMPTY;
	
	// vertex format
	if ( state.CheckParams ( 2, "U", false )) {
	
		MOAIVertexFormatGL* format = state.GetLuaObject < MOAIVertexFormatGL >( 2, 0 );
		if ( format ) {
			u32 length = state.GetValue < u32 >( 3, ( u32 )self->GetCursor ());
			self->Seek ( 0, SEEK_SET );
			hasBounds = format->ComputeAABB ( aabb, *self, length );
		}
	}
	
	if ( hasBounds ) {
		state.Push ( aabb );
		return 6;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	countElements
	@text	Get the number of vertices given either a vertex size or
			a vertex format.
 
	@overload
 
		@in		MOAIVertexBufferGL self
		@in		number vertexSize
		@out	number vertexCount
	
	@overload
 
		@in		MOAIVertexBufferGL self
		@in		MOAIVertexFormatGL format
		@out	number vertexCount
*/
int MOAIVertexBufferGL::_countElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBufferGL, "U" )

	u32 totalElements = 0;
	
	// size
	if ( state.CheckParams ( 2, "N", false )) {
		
		u32  elementSize = state.GetValue < u32 >( 2, 4 );
		totalElements = ( u32 )( self->GetSize () / elementSize ); // TODO: cast
	}
	
	// vertex format
	else if ( state.CheckParams ( 2, "U", false )) {
	
		MOAIVertexFormatGL* format = state.GetLuaObject < MOAIVertexFormatGL >( 2, 0 );
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
 
	@in		MOAIVertexBufferGL self
	@in		MOAIVertexFormatGL format
	@out	nil
*/
int MOAIVertexBufferGL::_printVertices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBufferGL, "U" )
	
	MOAIVertexFormatGL* format = state.GetLuaObject < MOAIVertexFormatGL >( 2, true );
	if ( format ) {
		self->PrintVertices ( *format );
	}
	return 0;
}

//================================================================//
// MOAIVertexBufferGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBufferGL::MOAIVertexBufferGL () {
		
	RTTI_BEGIN
		RTTI_EXTEND ( ZLVertexBufferGL )
		RTTI_EXTEND ( MOAIAbstractGfxResourceGL )
		RTTI_EXTEND ( MOAIAbstractGfxBufferGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexBufferGL::~MOAIVertexBufferGL () {
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::PrintVertices ( MOAIVertexFormatGL& vertexFormat ) {

	size_t cursor = this->GetCursor ();
	if ( cursor ) {
		this->SetCursor ( 0 );
		vertexFormat.PrintVertices ( *this, cursor );
		this->SetCursor ( cursor );
	}
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGfxResourceGL < ZLVertexBufferGL >::RegisterLuaClass ( state );
	MOAIAbstractGfxBufferGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIGfxResourceGL < ZLVertexBufferGL >::RegisterLuaFuncs ( state );
	MOAIAbstractGfxBufferGL::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "computeBounds",			_computeBounds },
		{ "countElements",			_countElements },
		{ "printVertices",			_printVertices },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIAbstractGfxBufferGL::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIAbstractGfxBufferGL::SerializeOut ( state, serializer );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLGfxBufferGL& MOAIVertexBufferGL::MOAIGfxBuffer_ZLGfxBuffer () {

	return *this;
}
