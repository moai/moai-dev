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

	//----------------------------------------------------------------//
	virtual void	UpdateShaderGlobals			() = 0;

public:
	
	GET ( const ZLFrustum&, ViewVolume, mViewVolume )
	GET ( const ZLRect&, ViewRect, mViewRect )
	
	GET ( u32, DrawCount, mDrawCount );
	
	//----------------------------------------------------------------//
					MOAIGfxDeviceBase			();
	virtual			~MOAIGfxDeviceBase			();
};

#endif
