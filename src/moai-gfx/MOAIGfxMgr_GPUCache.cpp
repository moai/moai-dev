// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractMesh.h>
#include <moai-gfx/MOAIFrameBuffer.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxMgr_GPUCache.h>
#include <moai-gfx/MOAIIndexBuffer.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAIViewport.h>

//================================================================//
// MOAIGfxMgr_GPUCache
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::ApplyStateChanges () {
	this->MOAIGfxMgr_GPUCache_ApplyStateChanges ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::ClearSurface () {
	this->MOAIGfxMgr_GPUCache_ClearSurface ();
}

//----------------------------------------------------------------//
size_t MOAIGfxMgr_GPUCache::CountTextureUnits () {
	return this->MOAIGfxMgr_GPUCache_CountTextureUnits ();
}

////----------------------------------------------------------------//
//void MOAIGfxMgr_GPUCache::DrawPrims ( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count ) {
//	return this->MOAIGfxMgr_GPUCache_DrawPrims ( primType, base, count );
//}

//----------------------------------------------------------------//
MOAIBlendMode MOAIGfxMgr_GPUCache::GetBlendMode () const {

	assert ( this->mPendingState );
	this->mPendingState->mBlendMode;
}

//----------------------------------------------------------------//
u32 MOAIGfxMgr_GPUCache::GetBufferHeight () const {

	assert ( this->mPendingState );
	const MOAIFrameBuffer* frameBuffer = this->mPendingState->mFrameBuffer ? this->mPendingState->mFrameBuffer : this->mDefaultFrameBuffer;
	return frameBuffer ? frameBuffer->GetHeight () : 0;
}

//----------------------------------------------------------------//
u32 MOAIGfxMgr_GPUCache::GetBufferWidth () const {

	assert ( this->mPendingState );
	const MOAIFrameBuffer* frameBuffer = this->mPendingState->mFrameBuffer ? this->mPendingState->mFrameBuffer : this->mDefaultFrameBuffer;
	return frameBuffer ? frameBuffer->GetWidth () : 0;
}

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAIGfxMgr_GPUCache::GetDefaultFrameBuffer () {
	return this->mDefaultFrameBuffer;
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgr_GPUCache::GetDefaultTexture () {
	return this->mDefaultTexture;
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::GetDepthMask () const {

	assert ( this->mPendingState );
	return this->mPendingState->mDepthMask;
}

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAIGfxMgr_GPUCache::GetFrameBuffer () {

	assert ( this->mPendingState );
	return this->mPendingState->mFrameBuffer ? this->mPendingState->mFrameBuffer : this->mDefaultFrameBuffer;
}

//----------------------------------------------------------------//
MOAIAbstractMesh* MOAIGfxMgr_GPUCache::GetMesh () {

	assert ( this->mPendingState );
	return this->mPendingState->mMesh;
}

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgr_GPUCache::GetShader () {

	assert ( this->mPendingState );
	return this->mPendingState->mShader;
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgr_GPUCache::GetTexture ( ZLIndex textureUnit ) {

	assert ( this->mPendingState );
	return this->mPendingState->mTextureUnits [ textureUnit ];
}

//----------------------------------------------------------------//
float MOAIGfxMgr_GPUCache::GetViewHeight () const {

	assert ( this->mPendingState );
	return this->mPendingState->mViewRect.Height ();
}

//----------------------------------------------------------------//
ZLRect MOAIGfxMgr_GPUCache::GetViewRect () const {

	assert ( this->mPendingState );
	return this->mPendingState->mViewRect;
}

//----------------------------------------------------------------//
float MOAIGfxMgr_GPUCache::GetViewWidth () const {

	assert ( this->mPendingState );
	return this->mPendingState->mViewRect.Width ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::GfxStateWillChange () {
	
	this->GetVertexCache ().FlushToGPU ();
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_GPUCache::HasPendingPrims () {

	return ( this->GetVertexCache ().CountPrims () > 0 );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::InitTextureUnits ( size_t nTextureUnits ) {

	if ( MAX_TEXTURE_UNITS < nTextureUnits ) {
		ZLLog_Warning ( "Hardware textures units (%d) exceed Moai maximum supported texture units (%d)\n", nTextureUnits, MAX_TEXTURE_UNITS );
		nTextureUnits = MAX_TEXTURE_UNITS;
	}

	this->mPendingState->mTextureUnits.Grow ( nTextureUnits, 0 );
	this->mActiveState->mTextureUnits.Grow ( nTextureUnits, 0 );
	
	this->mMaxTextureUnits = nTextureUnits;
}

//----------------------------------------------------------------//
MOAIGfxMgr_GPUCache::MOAIGfxMgr_GPUCache () :
	mDirtyFlags ( 0 ),
	mTextureDirtyFlags ( 0 ),
	mMaxTextureUnits ( 0 ),
	mApplyingStateChanges ( 0 ),
	mCurrentState ( NULL ) {
}

//----------------------------------------------------------------//
MOAIGfxMgr_GPUCache::~MOAIGfxMgr_GPUCache () {
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::ResetGPUState () {
	return this->MOAIGfxMgr_GPUCache_ResetGPUState ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::ResumeChanges () {

	assert ( this->mApplyingStateChanges > 0 );
	this->mApplyingStateChanges--;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetBlendMode () {

	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState->mBlendEnabled = false;
	this->mDirtyFlags = ( !this->mActiveState->mBlendEnabled ) ? ( this->mDirtyFlags & ~BLEND_MODE ) : ( this->mDirtyFlags | BLEND_MODE );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState->mBlendEnabled = true;
	this->mPendingState->mBlendMode = blendMode;
	
	this->SetFlag ( this->mActiveState->mBlendEnabled && ( this->mActiveState->mBlendMode.IsSame ( blendMode )), BLEND_MODE );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetBlendMode ( MOAIBlendFactorEnum::_ srcFactor, MOAIBlendFactorEnum::_ dstFactor, MOAIBlendFuncEnum::_ equation ) {

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( equation, srcFactor, dstFactor );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetCullFunc () {

	this->SetCullFunc ( MOAICullFuncEnum::NONE );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetCullFunc ( MOAICullFuncEnum::_ cullFunc ) {

	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState->mCullFunc = cullFunc;
	this->SetFlag ( this->mActiveState->mCullFunc == cullFunc, CULL_FUNC );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDefaultFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {
	
	this->mDefaultFrameBuffer = frameBuffer;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDefaultTexture ( MOAITexture* texture ) {

	this->mDefaultTexture = texture;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDepthFunc () {

	this->SetDepthFunc ( MOAIDepthFuncEnum::NONE );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDepthFunc ( MOAIDepthFuncEnum::_ depthFunc ) {

	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState->mDepthFunc = depthFunc;
	this->SetFlag (( this->mActiveState->mDepthFunc == depthFunc ) && ( this->mActiveState->mDepthMask == this->mPendingState->mDepthMask ), DEPTH_MODE );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetDepthMask ( bool depthMask ) {

	assert ( !this->mApplyingStateChanges );

	this->mPendingState->mDepthMask = depthMask;
	this->SetFlag (( this->mActiveState->mDepthMask == depthMask ) && ( this->mActiveState->mDepthFunc == this->mPendingState->mDepthFunc ), DEPTH_MODE );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetFlag ( bool isClean, u32 flag ) {

	this->mDirtyFlags = isClean ? ( this->mDirtyFlags & ~flag ) : ( this->mDirtyFlags | flag );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	assert ( !this->mApplyingStateChanges );

	frameBuffer = frameBuffer ? frameBuffer : ( MOAIFrameBuffer* )this->mDefaultFrameBuffer;
	this->mPendingState->mFrameBuffer = frameBuffer;
	this->SetFlag ( this->mActiveState->mFrameBuffer == frameBuffer, FRAME_BUFFER );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetMesh ( MOAIAbstractMesh* mesh ) {

	assert ( !this->mApplyingStateChanges );

	this->mPendingState->mMesh = mesh;
	this->SetFlag ( this->mActiveState->mMesh == mesh, MESH );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetPenWidth ( float penWidth ) {

	assert ( !this->mApplyingStateChanges );

	this->mPendingState->mPenWidth = penWidth;
	this->SetFlag ( this->mActiveState->mPenWidth == penWidth, PEN_WIDTH );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetScissorRect () {

	assert ( !this->mApplyingStateChanges );

	this->mPendingState->mScissorEnabled = false;
	this->SetFlag ( !this->mActiveState->mScissorEnabled, SCISSOR_RECT );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetScissorRect ( ZLRect rect ) {
	
	rect.Bless ();
	
	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState->mScissorEnabled = true;
	this->mPendingState->mScissorRect = rect;
	this->SetFlag ( this->mActiveState->mScissorEnabled && ( this->mActiveState->mScissorRect.IsEqual ( rect )), SCISSOR_RECT );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetScissorRect ( MOAIViewport* viewport ) {

	if ( viewport ) {
		this->SetScissorRect ( *viewport );
	}
	else {
		this->SetScissorRect ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetShader ( MOAIShader* shader ) {

	assert ( !this->mApplyingStateChanges );
	
	shader = ( shader && shader->HasProgram ()) ? shader : NULL;
	
	this->mPendingState->mShader = shader;
	
	// shader dirty flag only cleared if both are null.
	// why? because incoming shader might have changes to uniforms.
	// so we're not using broad-phase rejection for shaders.
	this->SetFlag ( !( shader || this->mActiveState->mShader ), SHADER );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetShader ( MOAIShaderPresetEnum preset ) {

	this->SetShader ( this->GetGfxMgr ().GetShaderPreset ( preset ));
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetTexture ( MOAITexture* texture, ZLIndex textureUnit ) {

	assert ( !this->mApplyingStateChanges );

	u32 mask = 1 << textureUnit;
	this->mPendingState->mTextureUnits [ textureUnit ] = texture;
	if ( this->mActiveState->mTextureUnits [ textureUnit ] == texture ) {
		this->mTextureDirtyFlags = this->mTextureDirtyFlags & ~mask;
	}
	else {
		this->mTextureDirtyFlags = this->mTextureDirtyFlags | mask;
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetViewRect () {

	float width = ( float )this->GetBufferWidth ();
	float height = ( float )this->GetBufferHeight ();

	ZLRect rect;
	rect.Init ( 0.0f, 0.0f, width, height );
	
	this->SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetViewRect ( ZLRect rect ) {
	
	rect.Bless ();
	
	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState->mViewRect = rect;
	this->SetFlag ( this->mActiveState->mViewRect.IsEqual ( rect ), VIEW_RECT );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SetViewRect ( MOAIViewport* viewport ) {

	if ( viewport ) {
		this->SetViewRect ( *viewport );
	}
	else {
		this->SetViewRect ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::SuspendChanges () {

	this->mApplyingStateChanges++;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_GPUCache::UnbindAll () {
	return this->MOAIGfxMgr_GPUCache_UnbindAll ();
}
