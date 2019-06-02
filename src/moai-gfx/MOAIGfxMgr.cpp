// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIImageTexture.h>
#include <moai-gfx/MOAITexture2D.h>

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
MOAIImageTexture* MOAIGfxMgr::CreateImageTexture () const {

	return this->MOAIGfxMgr_CreateImageTexture ();
}

//----------------------------------------------------------------//
MOAIIndexBuffer* MOAIGfxMgr::CreateIndexBuffer () const {

	return this->MOAIGfxMgr_CreateIndexBuffer ();
}

//----------------------------------------------------------------//
MOAITexture2D* MOAIGfxMgr::CreateTexture2D () const {

	return this->MOAIGfxMgr_CreateTexture2D ();
}

//----------------------------------------------------------------//
MOAIVertexArray* MOAIGfxMgr::CreateVertexArray () const {

	return this->MOAIGfxMgr_CreateVertexArray ();
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIGfxMgr::CreateVertexBuffer () const {

	return this->MOAIGfxMgr_CreateVertexBuffer ();
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

	return this->MOAIGfxMgr_GetVertexFormatPreset ( preset );
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
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIGlobalEventSource )
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

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgr::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxMgr_RenderTree, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIGfxMgr::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxMgr_RenderTree, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}
