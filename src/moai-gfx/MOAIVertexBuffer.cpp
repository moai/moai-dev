// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIVertexBuffer.h>
#include <moai-gfx/MOAIVertexFormat.h>

//================================================================//
// lua
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
	ZLBox aabb = ZLBox::EMPTY;
	
	// vertex format
	if ( state.CheckParams ( 2, "U", false )) {
	
		MOAIVertexFormat* format = state.GetLuaObject < MOAIVertexFormat >( 2, 0 );
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
		totalElements = ( u32 )( self->GetSize () / elementSize ); // TODO: cast
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
// ZLShaderGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIGfxResource ( context ),
	MOAIStream ( context ),
	MOAIGfxBuffer ( context ) {
	
	RTTI_BEGIN ( MOAIVertexBuffer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIVertexBuffer >)
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
		this->SetCursor ( 0 );
		vertexFormat.PrintVertices ( *this, cursor );
		this->SetCursor ( cursor );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexBuffer::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "computeBounds",			_computeBounds },
		{ "countElements",			_countElements },
		{ "printVertices",			_printVertices },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
