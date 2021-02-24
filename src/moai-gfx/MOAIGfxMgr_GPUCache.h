// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGR_GPUCACHE_H
#define	MOAIGFXMGR_GPUCACHE_H

#include <moai-gfx/MOAIBlendMode.h>
#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIGfxMgrComponents.h>
#include <moai-gfx/MOAIGfxPresetEnum.h>
#include <moai-gfx/MOAIGfxStateGPUCacheFrame.h>

class MOAIAbstractMesh;
class MOAIShader;
class MOAITexture;
class MOAIFrameBuffer;
class MOAIViewport;

//================================================================//
// MOAIGfxMgr_GPUCache
//================================================================//
class MOAIGfxMgr_GPUCache :
	public virtual MOAIGfxMgrComponents {
protected:

	static const u32 MAX_TEXTURE_UNITS = 32; // enough? will need more flags below if not.

	// right now we just test every flag for an update in a for loop.
	// these flags ordered roughly by (guessed) call frequency so we can bail out of the
	// state update loop early. if the update loop is ever a bottleneck (or if this list grows
	// a lot) then we'll have to look at a data structure for scheduling updates.
	// note that some of these also have to be done in a certain order.
	enum {
		SHADER					= 1 << 0,
		MESH					= 1 << 1, // must come *before* vertex buffer
		DEPTH_MODE				= 1 << 2,
		BLEND_MODE				= 1 << 3,
		PEN_WIDTH				= 1 << 4,
		CULL_FUNC				= 1 << 5,
		FRAME_BUFFER			= 1 << 6,
		VIEW_RECT				= 1 << 7,	// must come *after* frame buffer
		SCISSOR_RECT			= 1 << 8,	// must come *after* frame buffer
		END_STATE_FLAGS			= 1 << 9,
	};

	u32									mMaxTextureUnits;
	
	ZLStrongPtr < MOAIFrameBuffer >		mDefaultFrameBuffer;
	ZLStrongPtr < MOAITexture >			mDefaultTexture;
	
	u32									mDirtyFlags;
	u32									mTextureDirtyFlags;
	u32									mApplyingStateChanges;

	MOAIGfxStateGPUCacheFrame*					mCurrentState;
	ZLStrongPtr < MOAIGfxStateGPUCacheFrame >	mActiveState;
	ZLStrongPtr < MOAIGfxStateGPUCacheFrame >	mPendingState;

	//----------------------------------------------------------------//
	bool					HasPendingPrims				();
	void					InitTextureUnits			( size_t nTextureUnits );
	void					ResumeChanges				();
	void					SuspendChanges				();

	//----------------------------------------------------------------//
	virtual void			MOAIGfxMgr_GPUCache_ApplyStateChanges			() = 0;
	virtual void			MOAIGfxMgr_GPUCache_ClearSurface				() = 0; // takes zgl clear flags
	virtual size_t			MOAIGfxMgr_GPUCache_CountTextureUnits			() = 0;
//	virtual void			MOAIGfxMgr_GPUCache_DrawPrims					( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count ) = 0;
	virtual void			MOAIGfxMgr_GPUCache_ResetGPUState				() = 0;
	virtual void			MOAIGfxMgr_GPUCache_UnbindAll					() = 0;

public:
	
	//----------------------------------------------------------------//
	void					ApplyStateChanges			();
	void					ClearSurface				(); // takes zgl clear flags
	size_t					CountTextureUnits			();
//	void					DrawPrims					( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count );
	MOAIBlendMode			GetBlendMode				() const;
	u32						GetBufferHeight				() const;
	u32						GetBufferWidth				() const;
	MOAIFrameBuffer*		GetDefaultFrameBuffer		();
	MOAITexture*			GetDefaultTexture			();
	bool					GetDepthMask				() const;
	MOAIFrameBuffer*		GetFrameBuffer				();
	MOAIAbstractMesh*		GetMesh						();
	MOAIShader*				GetShader					();
	MOAITexture*			GetTexture					( ZLIndex textureUnit = 0 );
	float					GetViewHeight				() const;
	ZLRect					GetViewRect					() const;
	float					GetViewWidth				() const;
	void					GfxStateWillChange			();
							MOAIGfxMgr_GPUCache			();
	virtual					~MOAIGfxMgr_GPUCache		();
	void					ResetGPUState				();
	void					SetBlendMode				();
	void					SetBlendMode				( const MOAIBlendMode& blendMode );
	void					SetBlendMode				( MOAIBlendFactorEnum::_ srcFactor, MOAIBlendFactorEnum::_ dstFactor, MOAIBlendFuncEnum::_ equation = MOAIBlendFuncEnum::ADD );
	void					SetCullFunc					();
	void					SetCullFunc					( MOAICullFuncEnum::_ cullFunc );
	void					SetDefaultFrameBuffer		( MOAIFrameBuffer* frameBuffer );
	void					SetDefaultTexture			( MOAITexture* texture );
	void					SetDepthFunc				();
	void					SetDepthFunc				( MOAIDepthFuncEnum::_ depthFunc );
	void					SetDepthMask				( bool depthMask );
	void					SetFlag						( bool isClean, u32 flag );
	void					SetFrameBuffer				( MOAIFrameBuffer* frameBuffer = NULL );
	void					SetMesh						( MOAIAbstractMesh* mesh = NULL );
	void					SetPenWidth					( float penWidth );
	void					SetScissorRect				();
	void					SetScissorRect				( ZLRect rect );
	void					SetScissorRect				( MOAIViewport* viewport );
	void					SetShader					( MOAIShader* shader = NULL );
	void					SetShader					( MOAIShaderPresetEnum preset );
	void					SetTexture					( MOAITexture* texture = NULL, ZLIndex textureUnit = 0 );
	void					SetViewRect					();
	void					SetViewRect					( ZLRect rect );
	void					SetViewRect					( MOAIViewport* viewport );
	void					UnbindAll					();
};

#endif
