// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTEXTUREGL_H
#define	ZLTEXTUREGL_H

#include <zl-gfx/ZLTextureBaseGL.h>

class ZLTextureFormat;

//================================================================//
// ZLTextureGL
//================================================================//
/**	@lua	ZLTextureGL
	@text	Texture class.
*/
class ZLTextureGL :
	public virtual ZLTextureBaseGL {
protected:

	// for loading from file
	STLString			mFilename;
	u32					mTransform;
	
	// for loading from image
	ZLImage* 			mImage;
	
	// for loading compressed data
	void*				mTextureData;
	size_t				mTextureDataSize;
	ZLTextureFormat*	mTextureDataFormat;

	//----------------------------------------------------------------//
	void				ClearImage					();
	void				CopyImage					( const ZLImage& image );
	bool				LoadFromStream				( ZLStream& stream, u32 transform );
	
	//----------------------------------------------------------------//
	bool				ZLAbstractGfxResource_OnCPUCreate				();
	void				ZLAbstractGfxResource_OnCPUDestroy				();
	bool				ZLAbstractGfxResource_OnGPUCreate				();

public:
		
	static const u32 DEFAULT_TRANSFORM = ZLImageTransform::TRUECOLOR | ZLImageTransform::PREMULTIPLY_ALPHA;
	
	//----------------------------------------------------------------//
	void					Clear					();
	void					Init					( const ZLImage& image, cc8* debugname );
	void					Init					( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname );
	void					Init					( cc8* filename, u32 transform, cc8* debugname = NULL );
	void					Init					( ZLStream& stream, u32 transform, cc8* debugname );
	void					Init					( const void* data, size_t size, u32 transform, cc8* debugname );
							ZLTextureGL				();
							~ZLTextureGL			();
};

#endif
