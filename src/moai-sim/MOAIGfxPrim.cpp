// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIGfxPrim.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAIGfxPrim::_getBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxPrim, "U" )
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxPrim::_reserveVAOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxPrim, "U" )
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxPrim::_reserveVBOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxPrim, "U" )
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxPrim::_setFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxPrim, "U" )
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxPrim::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxPrim, "U" )
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxPrim::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxPrim, "U" )
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxPrim::_updateBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxPrim, "U" )
	return 0;
}

//================================================================//
// MOAIGfxPrim
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxPrim::Clear () {

//	this->Reserve ( 0 );
//	this->ReserveVBOs ( 0 );
//	this->mFormat.Set ( *this, 0 );
	
	this->Destroy ();
}

//----------------------------------------------------------------//
u32 MOAIGfxPrim::GetLoadingPolicy () {

	return MOAIGfxResource::LOADING_POLICY_CPU_GPU_BIND;
}

//----------------------------------------------------------------//
MOAIGfxPrim::MOAIGfxPrim () :
	mCurrentVAO ( 0 ),
	mVertexCount ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
	
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIGfxPrim::~MOAIGfxPrim () {

	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAIGfxPrim::OnCPUCreate () {
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxPrim::OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIGfxPrim::OnGPUBind () {

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
//		const MOAIVertexFormat* format = this->GetFormat ();
//		if ( format && this->mBuffer ) {
//			MOAIGfxDevice::Get ().SetVertexFormat ( *format, this->mBuffer );
//		}
//	}
}

//----------------------------------------------------------------//
bool MOAIGfxPrim::OnGPUCreate () {

	bool isValid = false;

//	if ( this->mUseVBOs ) {
//
//		const MOAIVertexFormat* format = this->GetFormat ();
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
//		const MOAIVertexFormat* format = this->GetFormat ();
//		isValid = ( format && this->GetLength ());
//	}
	
	return isValid;
}

//----------------------------------------------------------------//
void MOAIGfxPrim::OnGPUDestroy () {

//	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
//		MOAIVbo& vbo = this->mVBOs [ i ];
//		MOAIGfxResourceMgr::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_BUFFER, vbo.mVBO );
//		vbo.mVBO = 0;
//	}
}

//----------------------------------------------------------------//
void MOAIGfxPrim::OnGPULost () {

//	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
//		MOAIVbo& vbo = this->mVBOs [ i ];
//		vbo.mVBO = 0;
//	}
}

//----------------------------------------------------------------//
void MOAIGfxPrim::OnGPUUnbind () {

//	if ( this->mUseVBOs ) {
//		zglBindVertexArray ( 0 );
//	}
//	else if ( this->GetFormat ()) {
//		MOAIGfxDevice::Get ().SetVertexFormat ();
//	}
}

//----------------------------------------------------------------//
void MOAIGfxPrim::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGfxPrim::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getBounds",					_getBounds },
		{ "reserveVAOs",				_reserveVAOs },
		{ "reserveVBOs",				_reserveVBOs },
		{ "setFormat",					_setFormat },
		{ "setIndexBuffer",				_setIndexBuffer },
		{ "setVertexBuffer",			_setVertexBuffer },
		{ "updateBounds",				_updateBounds },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxPrim::ReserveVAOs ( u32 size ) {

//	this->mBuffer.Init ( size );
//	this->SetBuffer ( this->mBuffer, size );
//	this->DoCPUAffirm ();
}

//----------------------------------------------------------------//
void MOAIGfxPrim::ReserveVBOs ( u32 size ) {

//	if ( gpuBuffers ) {
//	
//		MOAIVbo blank;
//	
//		blank.mVAO = 0;
//		blank.mVBO = 0;
//		
//		this->mVBOs.Resize ( gpuBuffers, blank );
//		this->mCurrentVBO = gpuBuffers - 1;
//		this->mUseVBOs = true;
//	}
//	else {
//		this->mVBOs.Clear ();
//		this->mUseVBOs = false;
//	}
}

//----------------------------------------------------------------//
void MOAIGfxPrim::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
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
void MOAIGfxPrim::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

//	size_t size = this->GetLength ();
//	
//	MOAIDataBuffer dataBuffer;
//	dataBuffer.Load ( this->mBuffer.Data (), size );
//	dataBuffer.Deflate ();
//	dataBuffer.PushString ( state );
//	lua_setfield ( state, -2, "mBuffer" );
}

//----------------------------------------------------------------//
void MOAIGfxPrim::SetFormat ( u32 idx, MOAIVertexFormat* format ) {
}

//----------------------------------------------------------------//
void MOAIGfxPrim::SetIndexBuffer ( MOAIGfxBuffer* buffer ) {
}

//----------------------------------------------------------------//
void MOAIGfxPrim::SetVertexFormat ( u32 idx, MOAIGfxBuffer* buffer ) {
}

//----------------------------------------------------------------//
void MOAIGfxPrim::UpdateBounds () {
}
