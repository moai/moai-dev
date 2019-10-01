// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxBufferVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
///**	@lua	reserveVBOs
//	@text	Reserves one or more VBO objects. Multi-buffering is
//			supported via multiple VBOs.
//
//	@in		MOAIGfxBufferVK self
//	@in		number count
//	@out	nil
//*/
//int	MOAIGfxBufferVK::_reserveVBOs ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIGfxBufferVK, "UN" )
//
//	u32 vbos = state.GetValue < u32 >( 2, 0 );
//	self->ReserveVBOs ( vbos );
//	return 0;
//}

//================================================================//
// MOAIGfxBufferVK
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxBufferVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize size, VkBufferUsageFlags usage ) {

	logicalDevice.AddClient ( logicalDevice, *this );
	this->Alloc ( size );
	this->mUsage = usage;
}

//----------------------------------------------------------------//
MOAIGfxBufferVK::MOAIGfxBufferVK () :
	mUsage ( 0 ) {
		
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResourceVK )
		RTTI_EXTEND ( MOAIGfxBuffer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxBufferVK::~MOAIGfxBufferVK () {

	this->Finalize ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIAbstractLifecycleClientVK_Finalize () {

	this->GetLogicalDevice ().RemoveClient ( *this );
}

//----------------------------------------------------------------//
MOAIGfxBufferSnapshotVK* MOAIGfxBufferVK::MOAIAbstractSnapshotSubjectVK_MakeSnapshot () {

	MOAIGfxBufferSnapshotVK* snapshot = new MOAIGfxBufferSnapshotVK ();
	snapshot->Initialize ( *this );
	return snapshot;
}

//----------------------------------------------------------------//
bool MOAIGfxBufferVK::MOAIGfxResource_OnCPUCreate () {
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResource_OnCPUDestroy () {
	MOAIGfxBuffer::MOAIGfxResource_OnCPUDestroy ();
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResource_OnCPUPurgeRecoverable () {
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUBind () {
}

//----------------------------------------------------------------//
bool MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUCreate () {
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUDeleteOrDiscard ( bool shouldDelete ) {
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUUnbind () {
}

//----------------------------------------------------------------//
bool MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUUpdate () {
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
//	state.SetField ( -1, "INDEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ELEMENT_ARRAY );
//	state.SetField ( -1, "VERTEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ARRAY );
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

//	luaL_Reg regTable [] = {
//		{ "reserveVBOs",			_reserveVBOs },
//		{ NULL, NULL }
//	};
//
//	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {

//	u32 totalVBOs = state.GetFieldValue < cc8*, u32 >( -1, "mTotalVBOs", 0 );
//	this->ReserveVBOs ( totalVBOs );

	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	
//	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVBOs", this->CountVBOs ());
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}
