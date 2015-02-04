// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reserveVerts
	@text	Sets capacity of buffer in vertices. This function should
			only be used after attaching a valid MOAIVertexFormat
			to the buffer.
	
	@in		MOAIVertexBuffer self
	@in		number size
	@out	nil
*/
int	MOAIVertexBuffer::_reserveVerts ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	
	if ( self->mFormat ) {
		self->Reserve ( total * self->mFormat->GetVertexSize ());
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFormat
	@text	Sets the vertex format for the buffer.
	
	@in		MOAIVertexBuffer self
	@in		MOAIVertexFormat format
	@out	nil
*/
int MOAIVertexBuffer::_setFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->mFormat.Set ( *self, state.GetLuaObject < MOAIVertexFormat >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	writeColor32
	@text	Write a packed 32-bit color to the vertex buffer.
	
	@in		MOAIVertexBuffer self
	@opt	number r				Default value is 1.
	@opt	number g				Default value is 1.
	@opt	number b				Default value is 1.
	@opt	number a				Default value is 1.
	@out	nil
*/
int MOAIVertexBuffer::_writeColor32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	float r = state.GetValue < float >( 2, 1.0f );
	float g = state.GetValue < float >( 3, 1.0f );
	float b = state.GetValue < float >( 4, 1.0f );
	float a = state.GetValue < float >( 5, 1.0f );
	
	u32 color = ZLColor::PackRGBA ( r, g, b, a );
	self->Write < u32 >( color );
	
	return 0;
}

//================================================================//
// MOAIVertexBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexBuffer::Bless () {
	
	const MOAIVertexFormat* format = this->GetVertexFormat ();
	
	if ( format ) {
		this->MOAIGfxBuffer::Bless ( *format );
	}
}

//----------------------------------------------------------------//
size_t MOAIVertexBuffer::GetSize () {

	return this->GetLength ();
}

//----------------------------------------------------------------//
const MOAIVertexFormat* MOAIVertexBuffer::GetVertexFormat () const {

	if ( this->mFormat ) return this->mFormat;
	if ( this->mDefaultFormat != NULL_FORMAT ) return &MOAIVertexFormatMgr::Get ().GetFormat ( this->mDefaultFormat );
	return 0;
}

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer () :
	mDefaultFormat ( NULL_FORMAT ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxBuffer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexBuffer::~MOAIVertexBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
//bool MOAIVertexBuffer::OnCPUCreate () {
//	return true;
//}

//----------------------------------------------------------------//
//void MOAIVertexBuffer::OnCPUDestroy () {
//}

//----------------------------------------------------------------//
//void MOAIVertexBuffer::OnGPUBind () {
//
//	if ( this->mUseVBOs ) {
//		
//		MOAIGfxDevice::Get ().SetVertexFormat ();
//		
//		if ( this->mIsDirty ) {
//			this->mCurrentVBO = ( this->mCurrentVBO + 1 ) % this->mVBOs.Size ();
//		}
//		
//		MOAIVbo& vbo = this->mVBOs [ this->mCurrentVBO ];
//		
//		if ( this->mIsDirty ) {
//			
//			zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, vbo.mVBO );
//			void* buffer = zglMapBuffer ( ZGL_BUFFER_TARGET_ARRAY );
//			memcpy ( buffer, this->mBuffer, this->GetLength ());
//			zglUnmapBuffer ( ZGL_BUFFER_TARGET_ARRAY );
//			zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, 0 );
//			
//			this->mIsDirty = false;
//		}
//		
//		zglBindVertexArray ( vbo.mVAO );
//	}
//	else {
//	
//		const MOAIVertexFormat* format = this->GetVertexFormat ();
//		if ( format && this->mBuffer ) {
//			MOAIGfxDevice::Get ().SetVertexFormat ( *format, this->mBuffer );
//		}
//	}
//}

//----------------------------------------------------------------//
//bool MOAIVertexBuffer::OnGPUCreate () {
//
//	bool isValid = false;
//
//	if ( this->mUseVBOs ) {
//
//		const MOAIVertexFormat* format = this->GetVertexFormat ();
//		u32 count = 0;
//
//		for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
//			MOAIVbo& vbo = this->mVBOs [ i ];
//			
//			vbo.mVAO = zglCreateVertexArray ();
//			if ( vbo.mVAO ) {
//				zglBindVertexArray ( vbo.mVAO );
//				
//				vbo.mVBO = zglCreateBuffer ();
//				if ( vbo.mVBO ) {
//				
//					zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, vbo.mVBO );
//					zglBufferData ( ZGL_BUFFER_TARGET_ARRAY, this->GetLength (), 0, this->mHint );
//					format->Bind ( 0 );
//					count++;
//				}
//				
//				zglBindVertexArray ( 0 );
//				zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, 0 );
//				format->Unbind ();
//			}
//		}
//		isValid = count == this->mVBOs.Size ();
//	}
//	else {
//		const MOAIVertexFormat* format = this->GetVertexFormat ();
//		isValid = ( format && this->GetLength ());
//	}
//	
//	return isValid;
//}

//----------------------------------------------------------------//
//void MOAIVertexBuffer::OnGPUDestroy () {
//
//	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
//		MOAIVbo& vbo = this->mVBOs [ i ];
//		MOAIGfxResourceMgr::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_BUFFER, vbo.mVBO );
//		vbo.mVBO = 0;
//	}
//}

//----------------------------------------------------------------//
//void MOAIVertexBuffer::OnGPULost () {
//
//	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
//		MOAIVbo& vbo = this->mVBOs [ i ];
//		vbo.mVBO = 0;
//	}
//}

//----------------------------------------------------------------//
//void MOAIVertexBuffer::OnGPUUnbind () {
//
//	if ( this->mUseVBOs ) {
//		zglBindVertexArray ( 0 );
//	}
//	else if ( this->GetVertexFormat ()) {
//		MOAIGfxDevice::Get ().SetVertexFormat ();
//	}
//}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGfxBuffer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxBuffer::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "bless",					_bless },
		{ "reserveVerts",			_reserveVerts },
		{ "setFormat",				_setFormat },
		{ "writeColor32",			_writeColor32 },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

//	state.GetField ( -1, "mBuffer" );
//	if ( state.IsType ( -1, LUA_TSTRING )) {
//		
//		MOAIDataBuffer dataBuffer;
//		dataBuffer.Load ( state, -1 );
//		dataBuffer.Inflate ();
//		
//		size_t bufferSize = dataBuffer.Size ();
//		this->Reserve ( bufferSize );
//		dataBuffer.Read ( this->mBuffer.Data (), bufferSize );
//		
//		this->SetLength ( bufferSize );
//		this->Seek ( bufferSize, SEEK_SET );
//	}
//	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

//	size_t size = this->GetLength ();
//	
//	MOAIDataBuffer dataBuffer;
//	dataBuffer.Load ( this->mBuffer.Data (), size );
//	dataBuffer.Deflate ();
//	dataBuffer.PushString ( state );
//	lua_setfield ( state, -2, "mBuffer" );
}
