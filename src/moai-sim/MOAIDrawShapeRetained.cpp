// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIDrawShapeRetained.h>

//================================================================//
// MOAIDrawShapeRetained
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawShapeRetained::DrawShapeStream () {

	size_t top = this->mStream.GetCursor ();
	this->mStream.Seek ( 0 );
	
	MOAIDraw& draw = MOAIDraw::Get ();
	draw.Bind ();

	while ( this->mStream.GetCursor () < top ) {
		
		u32 cmd = this->mStream.Read < u32 >( CMD_DONE );
		
		switch ( cmd ) {
			
			case CMD_LINE: {
			
				ZLVec3D v0 = this->mStream.Read < ZLVec3D >();
				ZLVec3D v1 = this->mStream.Read < ZLVec3D >();
				draw.DrawLine ( v0, v1 );
				break;
			}
			case CMD_PEN_COLOR: {
			
				u32 color = this->mStream.Read < u32 >();
				draw.SetPenColor ( color );
				break;
			}
			case CMD_PEN_WIDTH: {
			
				float width = this->mStream.Read < float >();
				draw.SetPenWidth ( width );
				break;
			}
			case CMD_POINT: {
				
				ZLVec3D v0 = this->mStream.Read < ZLVec3D >();
				draw.DrawPoint ( v0.mX, v0.mY, v0.mZ );
				break;
			}
			case CMD_TRIANGLE: {
			
				ZLVec3D v0 = this->mStream.Read < ZLVec3D >();
				ZLVec3D v1 = this->mStream.Read < ZLVec3D >();
				ZLVec3D v2 = this->mStream.Read < ZLVec3D >();
				draw.DrawTriangleFill ( v0, v1, v2 );
				break;
			}
			case CMD_DONE:
			default:
				return;
		}
	}
}

//----------------------------------------------------------------//
size_t MOAIDrawShapeRetained::GetShapeStreamSize () {

	return this->mStream.GetCursor ();
}

//----------------------------------------------------------------//
MOAIDrawShapeRetained::MOAIDrawShapeRetained () {
}

//----------------------------------------------------------------//
MOAIDrawShapeRetained::~MOAIDrawShapeRetained () {
}

//----------------------------------------------------------------//
void MOAIDrawShapeRetained::ResetShapeStream () {

	this->mStream.Seek ( 0 );
}

//================================================================//
// :implementation:
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawShapeRetained::MOAIAbstractDrawShape_DrawLine ( float x0, float y0, float z0, float x1, float y1, float z1 ) {
	
	this->mStream.Write < u32 >( CMD_LINE );
	this->mStream.Write < ZLVec3D >( ZLVec3D ( x0, y0, z0 ));
	this->mStream.Write < ZLVec3D >( ZLVec3D ( x1, y1, z1 ));
}

//----------------------------------------------------------------//
void MOAIDrawShapeRetained::MOAIAbstractDrawShape_DrawPoint ( float x, float y, float z ) {

	this->mStream.Write < u32 >( CMD_POINT );
	this->mStream.Write < ZLVec3D >( ZLVec3D ( x, y, z ));
}

//----------------------------------------------------------------//
void MOAIDrawShapeRetained::MOAIAbstractDrawShape_DrawTriangleFill ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) {

	this->mStream.Write < u32 >( CMD_TRIANGLE );
	this->mStream.Write < ZLVec3D >( v0 );
	this->mStream.Write < ZLVec3D >( v1 );
	this->mStream.Write < ZLVec3D >( v2 );
}

//----------------------------------------------------------------//
void MOAIDrawShapeRetained::MOAIAbstractDrawShape_SetPenColor ( u32 color ) {

	this->mStream.Write < u32 >( CMD_PEN_COLOR );
	this->mStream.Write < u32 >( color );
}

//----------------------------------------------------------------//
void MOAIDrawShapeRetained::MOAIAbstractDrawShape_SetPenWidth ( float width ) {

	this->mStream.Write < u32 >( CMD_PEN_WIDTH );
	this->mStream.Write < float >( width );
}
