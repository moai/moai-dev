// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATEGPUCACHEFRAME_H
#define	MOAIGFXSTATEGPUCACHEFRAME_H

#include <moai-gfx/MOAIBlendMode.h>
#include <moai-gfx/MOAIGfxMgrComponents.h>

class MOAIMesh;
class MOAIFrameBuffer;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAIGfxStateGPUCacheFrame
//================================================================//
class MOAIGfxStateGPUCacheFrame :
	public virtual RTTIBase,
	public virtual ZLRefCountedObject {
public:

	// TODO: multithread will impact caching of buffer behavior as gfx.Copy () may produce a different result each time it is called

	MOAICullFuncEnum::_						mCullFunc;
	
	MOAIDepthFuncEnum::_					mDepthFunc;
	bool									mDepthMask;

	MOAIBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	bool									mScissorEnabled;
	ZLRect									mScissorRect;
	
	ZLStrongPtr < MOAIShader >				mShader;

	ZLStrongPtr < MOAIFrameBuffer >			mFrameBuffer;
	ZLStrongPtr < MOAIMesh >		mMesh;

	ZLRect									mViewRect;
	
	ZLLeanArray < ZLStrongPtr < MOAITexture > >		mTextureUnits;

	//----------------------------------------------------------------//
	void			Clear							();
					MOAIGfxStateGPUCacheFrame		();
					~MOAIGfxStateGPUCacheFrame		();
};

#endif
