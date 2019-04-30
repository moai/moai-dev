// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLAbstractGPU.h>

//================================================================//
// ZLAbstractGPU
//================================================================//

//----------------------------------------------------------------//
void ZLAbstractGPU::ClearSurface () {
	this->ZLAbstractGPU_ClearSurface ();
}

//----------------------------------------------------------------//
size_t ZLAbstractGPU::CountTextureUnits () {
	return this->ZLAbstractGPU_CountTextureUnits ();
}

//----------------------------------------------------------------//
void ZLAbstractGPU::DrawPrims ( u32 primType, u32 base, u32 count ) {
	return this->ZLAbstractGPU_DrawPrims ( primType, base, count );
}

//----------------------------------------------------------------//
ZLBlendMode ZLAbstractGPU::GetBlendMode () const {
	return this->ZLAbstractGPU_GetBlendMode ();
}

//----------------------------------------------------------------//
u32 ZLAbstractGPU::GetBufferHeight () const {
	return this->ZLAbstractGPU_GetBufferHeight ();
}

//----------------------------------------------------------------//
u32 ZLAbstractGPU::GetBufferWidth () const {
	return this->ZLAbstractGPU_GetBufferWidth ();
}

//----------------------------------------------------------------//
ZLFrameBuffer* ZLAbstractGPU::GetDefaultFrameBuffer () {
	return this->ZLAbstractGPU_GetDefaultFrameBuffer ();
}

//----------------------------------------------------------------//
ZLTextureBase* ZLAbstractGPU::GetDefaultTexture () {
	return this->ZLAbstractGPU_GetDefaultTexture ();
}

//----------------------------------------------------------------//
bool ZLAbstractGPU::GetDepthMask () const {
	return this->ZLAbstractGPU_GetDepthMask ();
}

//----------------------------------------------------------------//
ZLFrameBuffer* ZLAbstractGPU::GetCurrentFrameBuffer () {
	return this->ZLAbstractGPU_GetCurrentFrameBuffer ();
}

//----------------------------------------------------------------//
ZLShader* ZLAbstractGPU::GetCurrentShader () {
	return this->ZLAbstractGPU_GetCurrentShader ();
}

//----------------------------------------------------------------//
ZLVertexFormat* ZLAbstractGPU::GetCurrentVtxFormat () {
	return this->ZLAbstractGPU_GetCurrentVtxFormat ();
}

//----------------------------------------------------------------//
ZLRect ZLAbstractGPU::GetViewRect () const {
	return this->ZLAbstractGPU_GetViewRect ();
}

//----------------------------------------------------------------//
float ZLAbstractGPU::GetViewHeight () const {
	return this->ZLAbstractGPU_GetViewHeight ();
}

//----------------------------------------------------------------//
float ZLAbstractGPU::GetViewWidth () const {
	return this->ZLAbstractGPU_GetViewWidth ();
}

//----------------------------------------------------------------//
void ZLAbstractGPU::ResetGPUState () {
	return this->ZLAbstractGPU_ResetGPUState ();
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetBlendMode () {
	return this->ZLAbstractGPU_SetBlendMode ();
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetBlendMode ( const ZLBlendMode& blendMode ) {
	return this->ZLAbstractGPU_SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetBlendMode ( int srcFactor, int dstFactor, int equation ) {
	return this->ZLAbstractGPU_SetBlendMode ( srcFactor, dstFactor, equation );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetCullFunc () {
	return this->ZLAbstractGPU_SetCullFunc ();
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetCullFunc ( int cullFunc ) {
	return this->ZLAbstractGPU_SetCullFunc ( cullFunc );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetDefaultFrameBuffer ( ZLFrameBuffer* frameBuffer ) {
	return this->ZLAbstractGPU_SetDefaultFrameBuffer ( frameBuffer );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetDefaultTexture ( ZLTextureBase* texture ) {
	return this->ZLAbstractGPU_SetDefaultTexture ( texture );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetDepthFunc () {
	return this->ZLAbstractGPU_SetDepthFunc ();
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetDepthFunc ( int depthFunc ) {
	return this->ZLAbstractGPU_SetDepthFunc ( depthFunc );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetDepthMask ( bool depthMask ) {
	return this->ZLAbstractGPU_SetDepthMask ( depthMask );
}

//----------------------------------------------------------------//
bool ZLAbstractGPU::SetFrameBuffer ( ZLFrameBuffer* frameBuffer ) {
	return this->ZLAbstractGPU_SetFrameBuffer ( frameBuffer );
}

//----------------------------------------------------------------//
bool ZLAbstractGPU::SetIndexBuffer ( ZLIndexBuffer* buffer ) {
	return this->ZLAbstractGPU_SetIndexBuffer ( buffer );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetPenWidth ( float penWidth ) {
	return this->ZLAbstractGPU_SetPenWidth ( penWidth );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetScissorRect () {
	return this->ZLAbstractGPU_SetScissorRect ();
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetScissorRect ( ZLRect rect ) {
	return this->ZLAbstractGPU_SetScissorRect ( rect );
}

//----------------------------------------------------------------//
bool ZLAbstractGPU::SetShader ( ZLShader* shader ) {
	return this->ZLAbstractGPU_SetShader ( shader );
}

//----------------------------------------------------------------//
bool ZLAbstractGPU::SetTexture ( ZLTextureBase* texture, ZLIndex textureUnit ) {
	return this->ZLAbstractGPU_SetTexture ( texture, textureUnit );
}

//----------------------------------------------------------------//
bool ZLAbstractGPU::SetVertexArray ( ZLVertexArray* vtxArray ) {
	return this->ZLAbstractGPU_SetVertexArray ( vtxArray );
}

//----------------------------------------------------------------//
bool ZLAbstractGPU::SetVertexBuffer ( ZLVertexBuffer* buffer ) {
	return this->ZLAbstractGPU_SetVertexBuffer ( buffer );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetVertexFormat ( ZLVertexFormat* format ) {
	return this->ZLAbstractGPU_SetVertexFormat ( format );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetViewRect () {
	return this->ZLAbstractGPU_SetViewRect ();
}

//----------------------------------------------------------------//
void ZLAbstractGPU::SetViewRect ( ZLRect rect ) {
	return this->ZLAbstractGPU_SetViewRect ( rect );
}

//----------------------------------------------------------------//
void ZLAbstractGPU::UnbindAll () {
	return this->ZLAbstractGPU_UnbindAll ();
}

//----------------------------------------------------------------//
ZLAbstractGPU::ZLAbstractGPU () {
}

//----------------------------------------------------------------//
ZLAbstractGPU::~ZLAbstractGPU () {
}
