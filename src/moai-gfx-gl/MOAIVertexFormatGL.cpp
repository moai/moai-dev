// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxConstsGL.h>
#include <moai-gfx-gl/MOAIVertexFormatGL.h>

//================================================================//
// MOAIVertexFormatGL
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormatGL::Bind ( ZLGfx& gfx, ZLSharedConstBuffer* buffer ) const {

	for ( ZLIndex i = 0; i < this->mTotalAttributes; ++i ) {
		
		const MOAIVertexAttribute& attr = this->mAttributes [ i ];
		
		gfx.EnableVertexAttribArray ( attr.mIndex );
		gfx.VertexAttribPointer (
			attr.mIndex,
			attr.mSize,
			MOAIGfxConstsGL::Remap ( attr.mType ),
			attr.mNormalized,
			this->mVertexSize,
			buffer,
			attr.mOffset
		);
	}
}

//----------------------------------------------------------------//
MOAIVertexFormatGL::MOAIVertexFormatGL () {
	
	RTTI_BEGIN ( MOAIVertexFormatGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIVertexFormatGL >)
		RTTI_EXTEND ( MOAIVertexFormat )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexFormatGL::~MOAIVertexFormatGL () {
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::Unbind ( ZLGfx& gfx ) const {

	for ( ZLIndex i = 0; i < this->mTotalAttributes; ++i ) {
		
		const MOAIVertexAttribute& attr = this->mAttributes [ i ];
		gfx.DisableVertexAttribArray ( attr.mIndex );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
