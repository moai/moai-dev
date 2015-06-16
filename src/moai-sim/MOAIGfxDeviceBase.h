// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICEBASE_H
#define	MOAIGFXDEVICEBASE_H

class MOAIIndexBuffer;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIGfxDeviceBase
//================================================================//
class MOAIGfxDeviceBase {
protected:

	bool			mShaderDirty;
	ZLRect			mViewRect;
	ZLFrustum		mViewVolume;
	
	u32				mDrawCount;

	MOAIIndexBuffer*		mCurrentIdxBuffer;
	MOAIVertexArray*		mCurrentVtxArray;
	MOAIVertexBuffer*		mCurrentVtxBuffer;
	MOAIVertexFormat*		mCurrentVtxFormat;

	//----------------------------------------------------------------//
	virtual void	UpdateShaderGlobals			() = 0;

public:
	
	GET ( const ZLFrustum&, ViewVolume, mViewVolume )
	GET ( const ZLRect&, ViewRect, mViewRect )
	
	GET ( u32, DrawCount, mDrawCount );
	
	//----------------------------------------------------------------//
	void			BindIndexBuffer				( MOAIIndexBuffer* buffer = 0 );
	void			BindVertexArray				( MOAIVertexArray* vtxArray = 0 );
	void			BindVertexBuffer			( MOAIVertexBuffer* buffer = 0 );
	void			BindVertexFormat			( MOAIVertexFormat* format = 0 );

	virtual void	FlushBufferedPrims			() = 0;
					MOAIGfxDeviceBase			();
	virtual			~MOAIGfxDeviceBase			();
};

#endif
