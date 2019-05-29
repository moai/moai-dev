// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIVertexFormatGL.h>

//================================================================//
// MOAIVertexFormatGL
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormatGL::Bind ( ZLGfx& gfx, ZLSharedConstBuffer* buffer ) const {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mTotalAttributes; ++i ) {
		
		const MOAIVertexAttribute& attr = this->mAttributes [ i ];
		
		gfx.EnableVertexAttribArray ( attr.mIndex );
		gfx.VertexAttribPointer ( attr.mIndex, attr.mSize, attr.mType, attr.mNormalized, this->mVertexSize, buffer, attr.mOffset );
	}
}

//----------------------------------------------------------------//
MOAIVertexFormatGL::MOAIVertexFormatGL () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAbstractVertexFormat )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexFormatGL::~MOAIVertexFormatGL () {
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::RegisterLuaClass ( MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::RegisterLuaFuncs ( MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::Unbind ( ZLGfx& gfx ) const {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mTotalAttributes; ++i ) {
		
		MOAIVertexAttribute& attr = this->mAttributes [ i ];
		gfx.DisableVertexAttribArray ( attr.mIndex );
	}
}
