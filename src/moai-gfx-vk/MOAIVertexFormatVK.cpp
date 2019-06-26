// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIVertexFormatVK
//================================================================//

////----------------------------------------------------------------//
//void MOAIVertexFormatVK::Bind ( ZLGfx& gfx, ZLSharedConstBuffer* buffer ) const {
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mTotalAttributes; ++i ) {
//
//		const MOAIVertexAttribute& attr = this->mAttributes [ i ];
//
//		gfx.EnableVertexAttribArray ( attr.mIndex );
//		gfx.VertexAttribPointer ( attr.mIndex, attr.mSize, attr.mType, attr.mNormalized, this->mVertexSize, buffer, attr.mOffset );
//	}
//}

//----------------------------------------------------------------//
MOAIVertexFormatVK::MOAIVertexFormatVK () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIVertexFormat )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexFormatVK::~MOAIVertexFormatVK () {
}

////----------------------------------------------------------------//
//void MOAIVertexFormatVK::Unbind ( ZLGfx& gfx ) const {
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mTotalAttributes; ++i ) {
//		
//		MOAIVertexAttribute& attr = this->mAttributes [ i ];
//		gfx.DisableVertexAttribArray ( attr.mIndex );
//	}
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormatVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIVertexFormatVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIVertexFormatVK::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIVertexFormatVK::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );
}
