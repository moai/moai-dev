// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIAL_H
#define	MOAIMATERIAL_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// MOAIMaterial
//================================================================//
class MOAIMaterial {
private:
	
	friend class MOAIMaterialBatch;
	
	enum {
		BLEND_MODE_FLAG		= 0x01 << 0,
		CULL_MODE_FLAG		= 0x01 << 1,
		DEPTH_MASK_FLAG		= 0x01 << 2,
		DEPTH_TEST_FLAG		= 0x01 << 3,
		SHADER_FLAG			= 0x01 << 4,
		TEXTURE_FLAG		= 0x01 << 5,
		
		MAX_FLAG			= 0x01 << 6,
	};
	
	static const u32	DRAW_FLAGS	= BLEND_MODE_FLAG | CULL_MODE_FLAG | DEPTH_MASK_FLAG | DEPTH_TEST_FLAG;
	
	ZLStrongPtr < MOAIShader >			mShader;
	ZLStrongPtr < MOAITextureBase >		mTexture;
	ZLStrongPtr < MOAIImage >			mHitMask; // this shoud be its own thing

	MOAIBlendMode		mBlendMode;
	int					mCullMode;
	int					mDepthTest;
	bool				mDepthMask;

	u32					mFlags;

public:

	GET_CONST ( MOAIBlendMode&, BlendMode, mBlendMode );
	GET ( int, CullMode, mCullMode );
	GET ( int, DepthTest, mDepthTest );
	GET ( bool, DepthMask, mDepthMask );
	GET ( MOAIShader*, Shader, mShader );
	GET ( MOAITextureBase*, Texture, mTexture );

	//----------------------------------------------------------------//
	void			ClearAll				();
	void			ClearBlendMode			();
	void			ClearCullMode			();
	void			ClearDepthMask			();
	void			ClearDepthTest			();
	void			ClearShader				();
	void			ClearTexture			();
	void			Compose					( const MOAIMaterial& material );
	void			LoadGfxState			( u32 defaultShader );
	void			LoadGfxState			( MOAIShader* defaultShader = 0 );
					MOAIMaterial			();
					~MOAIMaterial			();
	void			SetBlendMode			( const MOAIBlendMode& blendMode );
	void			SetCullMode				( int cullMode );
	void			SetDepthMask			( bool depthMask );
	void			SetDepthTest			( int depthTest );
	void			SetShader				( MOAIShader* shader );
	void			SetTexture				( MOAITextureBase* texture );
	bool			StateWillChange			( const MOAIMaterial& material );
};

#endif
