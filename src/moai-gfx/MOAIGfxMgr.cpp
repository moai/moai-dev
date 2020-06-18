// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxScript.h>
#include <moai-gfx/MOAIImageTexture.h>
#include <moai-gfx/MOAITexture2D.h>
#include <moai-gfx/MOAIVertexFormatMgr.h>

#if AKU_WITH_SIM
	#include <moai-gfx/host.h>
	#include <moai-sim/host.h>
#endif

//================================================================//
// MOAIGfxMgr
//================================================================//

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgr::AffirmShader ( MOAILuaState& state, int idx ) const {

	return this->MOAIGfxMgr_AffirmShader ( state, idx );
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgr::AffirmTexture ( MOAILuaState& state, int idx ) const {

	return this->MOAIGfxMgr_AffirmTexture ( state, idx );
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgr::AffirmVertexFormat ( MOAILuaState& state, int idx ) const {

	switch ( lua_type ( state, idx )) {
	
		case LUA_TUSERDATA:
			return state.GetLuaObject < MOAIVertexFormat >( idx, true );
		
		case LUA_TNUMBER:
			return this->GetVertexFormatPreset (( MOAIVertexFormatPresetEnum )state.GetValue < u32 >( idx, ( u32 )MOAIVertexFormatPresetEnum::XYZWC ));
	}
	return NULL;
}

//----------------------------------------------------------------//
void MOAIGfxMgr::BeginFrame () {

	return this->MOAIGfxMgr_BeginFrame ();
}

//----------------------------------------------------------------//
MOAIGfxScript* MOAIGfxMgr::CreateGfxScriptRetained () {

	return this->MOAIGfxMgr_CreateGfxScriptRetained ();
}

//----------------------------------------------------------------//
MOAIImageTexture* MOAIGfxMgr::CreateImageTexture () {

	return this->MOAIGfxMgr_CreateImageTexture ();
}

//----------------------------------------------------------------//
MOAIIndexBuffer* MOAIGfxMgr::CreateIndexBuffer () {

	return this->MOAIGfxMgr_CreateIndexBuffer ();
}

//----------------------------------------------------------------//
MOAIRenderBatch* MOAIGfxMgr::CreateRenderBatch () {

	return this->MOAIGfxMgr_CreateRenderBatch ();
}

//----------------------------------------------------------------//
MOAITexture2D* MOAIGfxMgr::CreateTexture2D () {

	return this->MOAIGfxMgr_CreateTexture2D ();
}

//----------------------------------------------------------------//
MOAIVertexArray* MOAIGfxMgr::CreateVertexArray () {

	return this->MOAIGfxMgr_CreateVertexArray ();
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIGfxMgr::CreateVertexBuffer () {

	return this->MOAIGfxMgr_CreateVertexBuffer ();
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgr::CreateVertexFormat () {

	return this->MOAIGfxMgr_CreateVertexFormat ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr::EndFrame () {

	return this->MOAIGfxMgr_EndFrame ();
}

//----------------------------------------------------------------//
MOAIAbstractDrawingObject* MOAIGfxMgr::GetDrawingObject () {

	return this->MOAIGfxMgr_GetDrawingObject ();
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgr::GetNormToWndMtx () {

	return this->GetNormToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgr::GetNormToWndMtx ( const ZLRect& wndRect ) {

	float hWidth = wndRect.Width () * 0.5f;
	float hHeight = wndRect.Height () * 0.5f;

	// Wnd
	ZLMatrix4x4 normToWnd;
	normToWnd.Scale ( hWidth, -hHeight, 1.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Translate ( hWidth + wndRect.mXMin, hHeight + wndRect.mYMin, 0.0f );
	normToWnd.Append ( mtx );
	
	return normToWnd;
}

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgr::GetShaderPreset ( MOAIShaderPresetEnum preset ) const {

	return this->MOAIGfxMgr_GetShaderPreset ( preset );
}

//----------------------------------------------------------------//
size_t MOAIGfxMgr::GetTextureMemoryUsage () const {

	return this->MOAIGfxMgr_GetTextureMemoryUsage ();
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgr::GetVertexFormatPreset ( MOAIVertexFormatPresetEnum preset ) const {

	return MOAIVertexFormatMgr::Get ().GetFormat ( preset );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgr::GetWorldToWndMtx () {

	return this->GetWorldToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgr::GetWorldToWndMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 worldToWnd = this->GetMtx ( WORLD_TO_CLIP_MTX );
	worldToWnd.Append ( this->GetNormToWndMtx ( wndRect ));
	
	return worldToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgr::GetWndToNormMtx () {

	return this->GetWndToNormMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgr::GetWndToNormMtx ( const ZLRect& wndRect ) {

	float hWidth = wndRect.Width () * 0.5f;
	float hHeight = wndRect.Height () * 0.5f;

	// Inv Wnd
	ZLMatrix4x4 wndToNorm;
	wndToNorm.Translate ( -hWidth - wndRect.mXMin, -hHeight - wndRect.mYMin, 0.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ), 1.0f );
	wndToNorm.Append ( mtx );
	
	return wndToNorm;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgr::GetWndToWorldMtx () {

	return this->GetWndToWorldMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgr::GetWndToWorldMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 wndToWorld = this->GetWndToNormMtx ( wndRect );
	
	// inv viewproj
	ZLMatrix4x4 mtx = this->GetMtx ( WORLD_TO_CLIP_MTX );
	mtx.Inverse ();
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
MOAIGfxMgr::MOAIGfxMgr () {
		
	RTTI_BEGIN ( MOAIGfxMgr )
		RTTI_EXTEND ( MOAIGlobalEventSource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxMgr::~MOAIGfxMgr () {
}

//----------------------------------------------------------------//
void MOAIGfxMgr::PopState () {
	this->MOAIGfxMgr_PopState ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr::PushState () {
	this->MOAIGfxMgr_PushState ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgr_CPUCache& MOAIGfxMgr::MOAIGfxMgrComponents_GetCPUCache () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgr& MOAIGfxMgr::MOAIGfxMgrComponents_GetGfxMgr () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgr_GPUCache& MOAIGfxMgr::MOAIGfxMgrComponents_GetGPUCache () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgr_RenderTree& MOAIGfxMgr::MOAIGfxMgrComponents_GetRenderTree () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgr_VertexCache& MOAIGfxMgr::MOAIGfxMgrComponents_GetVertexCache () {
	return *this;
}

//----------------------------------------------------------------//
void MOAIGfxMgr::ZLContextClass_Finalize () {
}

//----------------------------------------------------------------//
void MOAIGfxMgr::ZLContextClass_Initialize () {

	this->AffirmBuffers ();
}
