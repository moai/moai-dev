// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxStateGPUCache.h>

//================================================================//
// MOAIAbstractGfxStateGPUCache
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::ClearSurface () {
	this->ZLAbstractGPU_ClearSurface ();
}

//----------------------------------------------------------------//
size_t MOAIAbstractGfxStateGPUCache::CountTextureUnits () {
	return this->ZLAbstractGPU_CountTextureUnits ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::DrawPrims ( u32 primType, u32 base, u32 count ) {
	return this->ZLAbstractGPU_DrawPrims ( primType, base, count );
}

//----------------------------------------------------------------//
MOAIBlendMode MOAIAbstractGfxStateGPUCache::GetBlendMode () const {
	return this->ZLAbstractGPU_GetBlendMode ();
}

//----------------------------------------------------------------//
u32 MOAIAbstractGfxStateGPUCache::GetBufferHeight () const {
	return this->ZLAbstractGPU_GetBufferHeight ();
}

//----------------------------------------------------------------//
u32 MOAIAbstractGfxStateGPUCache::GetBufferWidth () const {
	return this->ZLAbstractGPU_GetBufferWidth ();
}

//----------------------------------------------------------------//
MOAIAbstractFrameBuffer* MOAIAbstractGfxStateGPUCache::GetDefaultFrameBuffer () {
	return this->ZLAbstractGPU_GetDefaultFrameBuffer ();
}

//----------------------------------------------------------------//
MOAIAbstractTexture* MOAIAbstractGfxStateGPUCache::GetDefaultTexture () {
	return this->ZLAbstractGPU_GetDefaultTexture ();
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxStateGPUCache::GetDepthMask () const {
	return this->ZLAbstractGPU_GetDepthMask ();
}

//----------------------------------------------------------------//
MOAIAbstractFrameBuffer* MOAIAbstractGfxStateGPUCache::GetCurrentFrameBuffer () {
	return this->ZLAbstractGPU_GetCurrentFrameBuffer ();
}

//----------------------------------------------------------------//
MOAIAbstractShader* MOAIAbstractGfxStateGPUCache::GetCurrentShader () {
	return this->ZLAbstractGPU_GetCurrentShader ();
}

//----------------------------------------------------------------//
MOAIAbstractVertexFormat* MOAIAbstractGfxStateGPUCache::GetCurrentVtxFormat () {
	return this->ZLAbstractGPU_GetCurrentVtxFormat ();
}

//----------------------------------------------------------------//
ZLRect MOAIAbstractGfxStateGPUCache::GetViewRect () const {
	return this->ZLAbstractGPU_GetViewRect ();
}

//----------------------------------------------------------------//
float MOAIAbstractGfxStateGPUCache::GetViewHeight () const {
	return this->ZLAbstractGPU_GetViewHeight ();
}

//----------------------------------------------------------------//
float MOAIAbstractGfxStateGPUCache::GetViewWidth () const {
	return this->ZLAbstractGPU_GetViewWidth ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxStateGPUCache::MOAIAbstractGfxStateGPUCache () {
}

//----------------------------------------------------------------//
MOAIAbstractGfxStateGPUCache::~MOAIAbstractGfxStateGPUCache () {
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::ResetGPUState () {
	return this->ZLAbstractGPU_ResetGPUState ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetBlendMode () {
	return this->ZLAbstractGPU_SetBlendMode ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetBlendMode ( const MOAIBlendMode& blendMode ) {
	return this->ZLAbstractGPU_SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetBlendMode ( int srcFactor, int dstFactor, int equation ) {
	return this->ZLAbstractGPU_SetBlendMode ( srcFactor, dstFactor, equation );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetCullFunc () {
	return this->ZLAbstractGPU_SetCullFunc ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetCullFunc ( int cullFunc ) {
	return this->ZLAbstractGPU_SetCullFunc ( cullFunc );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetDefaultFrameBuffer ( MOAIAbstractFrameBuffer* frameBuffer ) {
	return this->ZLAbstractGPU_SetDefaultFrameBuffer ( frameBuffer );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetDefaultTexture ( MOAIAbstractTexture* texture ) {
	return this->ZLAbstractGPU_SetDefaultTexture ( texture );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetDepthFunc () {
	return this->ZLAbstractGPU_SetDepthFunc ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetDepthFunc ( int depthFunc ) {
	return this->ZLAbstractGPU_SetDepthFunc ( depthFunc );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetDepthMask ( bool depthMask ) {
	return this->ZLAbstractGPU_SetDepthMask ( depthMask );
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxStateGPUCache::SetFrameBuffer ( MOAIAbstractFrameBuffer* frameBuffer ) {
	return this->ZLAbstractGPU_SetFrameBuffer ( frameBuffer );
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxStateGPUCache::SetIndexBuffer ( MOAIAbstractIndexBuffer* buffer ) {
	return this->ZLAbstractGPU_SetIndexBuffer ( buffer );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetPenWidth ( float penWidth ) {
	return this->ZLAbstractGPU_SetPenWidth ( penWidth );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetScissorRect () {
	return this->ZLAbstractGPU_SetScissorRect ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetScissorRect ( ZLRect rect ) {
	return this->ZLAbstractGPU_SetScissorRect ( rect );
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxStateGPUCache::SetShader ( MOAIAbstractShader* shader ) {
	return this->ZLAbstractGPU_SetShader ( shader );
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxStateGPUCache::SetTexture ( MOAIAbstractTexture* texture, ZLIndex textureUnit ) {
	return this->ZLAbstractGPU_SetTexture ( texture, textureUnit );
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxStateGPUCache::SetVertexArray ( MOAIAbstractVertexArray* vtxArray ) {
	return this->ZLAbstractGPU_SetVertexArray ( vtxArray );
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxStateGPUCache::SetVertexBuffer ( MOAIAbstractVertexBuffer* buffer ) {
	return this->ZLAbstractGPU_SetVertexBuffer ( buffer );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetVertexFormat ( MOAIAbstractVertexFormat* format ) {
	return this->ZLAbstractGPU_SetVertexFormat ( format );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetViewRect () {
	return this->ZLAbstractGPU_SetViewRect ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::SetViewRect ( ZLRect rect ) {
	return this->ZLAbstractGPU_SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxStateGPUCache::UnbindAll () {
	return this->ZLAbstractGPU_UnbindAll ();
}
