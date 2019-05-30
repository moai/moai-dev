// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgr_GPUCache.h>

//================================================================//
// MOAIGfxMgr_GPUCache
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::ClearSurface () {
	this->MOAIGfxMgr_GPUCache_ClearSurface ();
}

//----------------------------------------------------------------//
size_t MOAIGfxMgr_GPUCache::CountTextureUnits () {
	return this->MOAIGfxMgr_GPUCache_CountTextureUnits ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::DrawPrims ( u32 primType, u32 base, u32 count ) {
	return this->MOAIGfxMgr_GPUCache_DrawPrims ( primType, base, count );
}

//----------------------------------------------------------------//
MOAIBlendMode MOAIGfxMgr_GPUCache::GetBlendMode () const {
	return this->MOAIGfxMgr_GPUCache_GetBlendMode ();
}

//----------------------------------------------------------------//
u32 MOAIGfxMgr_GPUCache::GetBufferHeight () const {
	return this->MOAIGfxMgr_GPUCache_GetBufferHeight ();
}

//----------------------------------------------------------------//
u32 MOAIGfxMgr_GPUCache::GetBufferWidth () const {
	return this->MOAIGfxMgr_GPUCache_GetBufferWidth ();
}

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAIGfxMgr_GPUCache::GetDefaultFrameBuffer () {
	return this->MOAIGfxMgr_GPUCache_GetDefaultFrameBuffer ();
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgr_GPUCache::GetDefaultTexture () {
	return this->MOAIGfxMgr_GPUCache_GetDefaultTexture ();
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::GetDepthMask () const {
	return this->MOAIGfxMgr_GPUCache_GetDepthMask ();
}

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAIGfxMgr_GPUCache::GetCurrentFrameBuffer () {
	return this->MOAIGfxMgr_GPUCache_GetCurrentFrameBuffer ();
}

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgr_GPUCache::GetCurrentShader () {
	return this->MOAIGfxMgr_GPUCache_GetCurrentShader ();
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgr_GPUCache::GetCurrentVtxFormat () {
	return this->MOAIGfxMgr_GPUCache_GetCurrentVtxFormat ();
}

//----------------------------------------------------------------//
ZLRect MOAIGfxMgr_GPUCache::GetViewRect () const {
	return this->MOAIGfxMgr_GPUCache_GetViewRect ();
}

//----------------------------------------------------------------//
float MOAIGfxMgr_GPUCache::GetViewHeight () const {
	return this->MOAIGfxMgr_GPUCache_GetViewHeight ();
}

//----------------------------------------------------------------//
float MOAIGfxMgr_GPUCache::GetViewWidth () const {
	return this->MOAIGfxMgr_GPUCache_GetViewWidth ();
}

//----------------------------------------------------------------//
MOAIGfxMgr_GPUCache::MOAIGfxMgr_GPUCache () {
}

//----------------------------------------------------------------//
MOAIGfxMgr_GPUCache::~MOAIGfxMgr_GPUCache () {
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::ResetGPUState () {
	return this->MOAIGfxMgr_GPUCache_ResetGPUState ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetBlendMode () {
	return this->MOAIGfxMgr_GPUCache_SetBlendMode ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetBlendMode ( const MOAIBlendMode& blendMode ) {
	return this->MOAIGfxMgr_GPUCache_SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetBlendMode ( int srcFactor, int dstFactor, int equation ) {
	return this->MOAIGfxMgr_GPUCache_SetBlendMode ( srcFactor, dstFactor, equation );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetCullFunc () {
	return this->MOAIGfxMgr_GPUCache_SetCullFunc ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetCullFunc ( int cullFunc ) {
	return this->MOAIGfxMgr_GPUCache_SetCullFunc ( cullFunc );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDefaultFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {
	return this->MOAIGfxMgr_GPUCache_SetDefaultFrameBuffer ( frameBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDefaultTexture ( MOAITexture* texture ) {
	return this->MOAIGfxMgr_GPUCache_SetDefaultTexture ( texture );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDepthFunc () {
	return this->MOAIGfxMgr_GPUCache_SetDepthFunc ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDepthFunc ( int depthFunc ) {
	return this->MOAIGfxMgr_GPUCache_SetDepthFunc ( depthFunc );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDepthMask ( bool depthMask ) {
	return this->MOAIGfxMgr_GPUCache_SetDepthMask ( depthMask );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {
	return this->MOAIGfxMgr_GPUCache_SetFrameBuffer ( frameBuffer );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetIndexBuffer ( MOAIIndexBuffer* buffer ) {
	return this->MOAIGfxMgr_GPUCache_SetIndexBuffer ( buffer );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetPenWidth ( float penWidth ) {
	return this->MOAIGfxMgr_GPUCache_SetPenWidth ( penWidth );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetScissorRect () {
	return this->MOAIGfxMgr_GPUCache_SetScissorRect ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetScissorRect ( ZLRect rect ) {
	return this->MOAIGfxMgr_GPUCache_SetScissorRect ( rect );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetShader ( MOAIShader* shader ) {
	return this->MOAIGfxMgr_GPUCache_SetShader ( shader );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetTexture ( MOAITexture* texture, ZLIndex textureUnit ) {
	return this->MOAIGfxMgr_GPUCache_SetTexture ( texture, textureUnit );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetVertexArray ( MOAIVertexArray* vtxArray ) {
	return this->MOAIGfxMgr_GPUCache_SetVertexArray ( vtxArray );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetVertexBuffer ( MOAIVertexBuffer* buffer ) {
	return this->MOAIGfxMgr_GPUCache_SetVertexBuffer ( buffer );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetVertexFormat ( MOAIVertexFormat* format ) {
	return this->MOAIGfxMgr_GPUCache_SetVertexFormat ( format );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetViewRect () {
	return this->MOAIGfxMgr_GPUCache_SetViewRect ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetViewRect ( ZLRect rect ) {
	return this->MOAIGfxMgr_GPUCache_SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::UnbindAll () {
	return this->MOAIGfxMgr_GPUCache_UnbindAll ();
}
