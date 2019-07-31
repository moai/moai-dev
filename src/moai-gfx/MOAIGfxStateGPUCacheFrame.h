// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATEGPUCACHEFRAME_H
#define	MOAIGFXSTATEGPUCACHEFRAME_H

#include <moai-gfx/MOAIGfxMgrComponents.h>
#include <moai-gfx/MOAIBlendMode.h>

class MOAIShader;
class MOAITexture;
class MOAIFrameBuffer;
class MOAIIndexBuffer;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIGfxStateGPUCacheFrame
//================================================================//
class MOAIGfxStateGPUCacheFrame :
	public virtual RTTIBase,
	public virtual ZLRefCountedObject {
public:

	// TODO: multithread will impact caching of buffer behavior as gfx.Copy () may produce a different result each time it is called

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	MOAIBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	bool									mScissorEnabled;
	ZLRect									mScissorRect;
	
	ZLStrongPtr < MOAIShader >				mShader;

	ZLStrongPtr < MOAIFrameBuffer >			mFrameBuffer;
	ZLStrongPtr < MOAIIndexBuffer >			mIdxBuffer;
	ZLStrongPtr < MOAIVertexArray >			mVtxArray;
	ZLStrongPtr < MOAIVertexBuffer >		mVtxBuffer;
	ZLStrongPtr < MOAIVertexFormat >		mVtxFormat;

	ZLRect									mViewRect;
	
	ZLLeanArray < ZLStrongPtr < MOAITexture > >		mTextureUnits;

	//----------------------------------------------------------------//
	void			Clear							();
					MOAIGfxStateGPUCacheFrame		();
					~MOAIGfxStateGPUCacheFrame		();
};

#endif
