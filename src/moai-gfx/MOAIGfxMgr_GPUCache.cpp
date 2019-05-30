// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgr_GPUCache.h>

//================================================================//
// MOAIGfxMgr_GPUCache
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::ClearSurface () {
	this->ZLAbstractGPU_ClearSurface ();
}

//----------------------------------------------------------------//
size_t MOAIGfxMgr_GPUCache::CountTextureUnits () {
	return this->ZLAbstractGPU_CountTextureUnits ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::DrawPrims ( u32 primType, u32 base, u32 count ) {
	return this->ZLAbstractGPU_DrawPrims ( primType, base, count );
}

//----------------------------------------------------------------//
MOAIBlendMode MOAIGfxMgr_GPUCache::GetBlendMode () const {
	return this->ZLAbstractGPU_GetBlendMode ();
}

//----------------------------------------------------------------//
u32 MOAIGfxMgr_GPUCache::GetBufferHeight () const {
	return this->ZLAbstractGPU_GetBufferHeight ();
}

//----------------------------------------------------------------//
u32 MOAIGfxMgr_GPUCache::GetBufferWidth () const {
	return this->ZLAbstractGPU_GetBufferWidth ();
}

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAIGfxMgr_GPUCache::GetDefaultFrameBuffer () {
	return this->ZLAbstractGPU_GetDefaultFrameBuffer ();
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgr_GPUCache::GetDefaultTexture () {
	return this->ZLAbstractGPU_GetDefaultTexture ();
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::GetDepthMask () const {
	return this->ZLAbstractGPU_GetDepthMask ();
}

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAIGfxMgr_GPUCache::GetCurrentFrameBuffer () {
	return this->ZLAbstractGPU_GetCurrentFrameBuffer ();
}

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgr_GPUCache::GetCurrentShader () {
	return this->ZLAbstractGPU_GetCurrentShader ();
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgr_GPUCache::GetCurrentVtxFormat () {
	return this->ZLAbstractGPU_GetCurrentVtxFormat ();
}

//----------------------------------------------------------------//
ZLRect MOAIGfxMgr_GPUCache::GetViewRect () const {
	return this->ZLAbstractGPU_GetViewRect ();
}

//----------------------------------------------------------------//
float MOAIGfxMgr_GPUCache::GetViewHeight () const {
	return this->ZLAbstractGPU_GetViewHeight ();
}

//----------------------------------------------------------------//
float MOAIGfxMgr_GPUCache::GetViewWidth () const {
	return this->ZLAbstractGPU_GetViewWidth ();
}

//----------------------------------------------------------------//
MOAIGfxMgr_GPUCache::MOAIGfxMgr_GPUCache () {
}

//----------------------------------------------------------------//
MOAIGfxMgr_GPUCache::~MOAIGfxMgr_GPUCache () {
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::ResetGPUState () {
	return this->ZLAbstractGPU_ResetGPUState ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetBlendMode () {
	return this->ZLAbstractGPU_SetBlendMode ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetBlendMode ( const MOAIBlendMode& blendMode ) {
	return this->ZLAbstractGPU_SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetBlendMode ( int srcFactor, int dstFactor, int equation ) {
	return this->ZLAbstractGPU_SetBlendMode ( srcFactor, dstFactor, equation );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetCullFunc () {
	return this->ZLAbstractGPU_SetCullFunc ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetCullFunc ( int cullFunc ) {
	return this->ZLAbstractGPU_SetCullFunc ( cullFunc );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDefaultFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {
	return this->ZLAbstractGPU_SetDefaultFrameBuffer ( frameBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDefaultTexture ( MOAITexture* texture ) {
	return this->ZLAbstractGPU_SetDefaultTexture ( texture );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDepthFunc () {
	return this->ZLAbstractGPU_SetDepthFunc ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDepthFunc ( int depthFunc ) {
	return this->ZLAbstractGPU_SetDepthFunc ( depthFunc );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDepthMask ( bool depthMask ) {
	return this->ZLAbstractGPU_SetDepthMask ( depthMask );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {
	return this->ZLAbstractGPU_SetFrameBuffer ( frameBuffer );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetIndexBuffer ( MOAIIndexBuffer* buffer ) {
	return this->ZLAbstractGPU_SetIndexBuffer ( buffer );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetPenWidth ( float penWidth ) {
	return this->ZLAbstractGPU_SetPenWidth ( penWidth );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetScissorRect () {
	return this->ZLAbstractGPU_SetScissorRect ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetScissorRect ( ZLRect rect ) {
	return this->ZLAbstractGPU_SetScissorRect ( rect );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetShader ( MOAIShader* shader ) {
	return this->ZLAbstractGPU_SetShader ( shader );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetTexture ( MOAITexture* texture, ZLIndex textureUnit ) {
	return this->ZLAbstractGPU_SetTexture ( texture, textureUnit );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetVertexArray ( MOAIVertexArray* vtxArray ) {
	return this->ZLAbstractGPU_SetVertexArray ( vtxArray );
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::SetVertexBuffer ( MOAIVertexBuffer* buffer ) {
	return this->ZLAbstractGPU_SetVertexBuffer ( buffer );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetVertexFormat ( MOAIVertexFormat* format ) {
	return this->ZLAbstractGPU_SetVertexFormat ( format );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetViewRect () {
	return this->ZLAbstractGPU_SetViewRect ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetViewRect ( ZLRect rect ) {
	return this->ZLAbstractGPU_SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::UnbindAll () {
	return this->ZLAbstractGPU_UnbindAll ();
}
