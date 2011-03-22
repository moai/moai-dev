// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef TEXTURE_H
#define TEXTURE_H

#include <uslsext/USImage.h>
#include <uslsext/USMatrix2D.h>

class USTextureLoader;
class USData;

//================================================================//
// USTexture
//================================================================//
class USTexture {
protected:
	
	// GL texture
	GLuint			mGLTexID;
	
	// size of the original texture
	u32				mWidth;
	u32				mHeight;
	
	// size on the device (padded to nearest powers of two)
	u32				mDevWidth;
	u32				mDevHeight;
	
	// GL_LINEAR
	// GL_NEAREST
	int				mMinFilter;
	int				mMagFilter;
	
	// GL_CLAMP_TO_EDGE
	// GL_REPEAT
	int				mWrap;
	
	int				mGLInternalFormat;
	int				mGLPixelType;
	
	STLString			mFilename;
	USTextureLoader*	mLoader;
	
	//----------------------------------------------------------------//
	void			AffirmTexture		();
	void			CreateTexture		( USImage& image );
	void			ReleaseLoader		();

public:

	//----------------------------------------------------------------//
	bool			Bind				();
	u32				GetHeight			();
	float			GetU				();
	u32				GetWidth			();
	float			GetV				();
	void			Init				( USData& data, u32 transform = ( USImageTransform::TRUECOLOR | USImageTransform::QUANTIZE ));
	void			Init				( cc8* filename, u32 transform = ( USImageTransform::TRUECOLOR | USImageTransform::QUANTIZE ));
	void			Init				( const void* data, u32 size, u32 transform = ( USImageTransform::TRUECOLOR | USImageTransform::QUANTIZE ));
	void			Init				( USImage& image, u32 transform = 0 );
	bool			IsOK				();
	void			Release				();
	void			SetFilter			( int filter );
	void			SetFilter			( int min, int mag );
	void			SetWrap				( int wrap );
					USTexture			();
	virtual			~USTexture			();
};

#endif
