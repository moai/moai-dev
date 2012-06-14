// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USCOLOR_H
#define USCOLOR_H

class USColorVec;

//================================================================//
// USColor
//================================================================//
namespace USColor {

	enum Format {
		A_8,
		RGB_888,
		RGB_565,
		RGBA_5551,
		RGBA_4444,
		RGBA_8888,
		CLR_FMT_UNKNOWN,
	};
	
	//----------------------------------------------------------------//
	u32				Average				( u32 c0, u32 c1 );
	u32				Average				( u32 c0, u32 c1, u32 c2, u32 c3 );
	u32				BilerpFixed			( u32 c0, u32 c1, u32 c2, u32 c3, u8 xt, u8 yt );
	void			Convert				( void* dest, Format destFmt, const void* src, Format srcFmt, u32 nColors );
	u32				ConvertFromRGBA		( u32 color, Format format );
	u32				ConvertToRGBA		( u32 color, Format format );
	u32				GetDepth			( Format format );
	u32				GetMask				( Format format );
	u32				GetSize				( Format format );
	u32				LerpFixed			( u32 c0, u32 c1, u8 t );
	u32				NearestNeighbor		( u32 c0, u32 c1, u32 c2, u32 c3, u8 xt, u8 yt );
	u32				PackRGBA			( int r, int g, int b, int a );
	u32				PackRGBA			( float r, float g, float b, float a );
	void			PremultiplyAlpha	( void* colors, Format format, u32 nColors );
	u32				ReadRGBA			( const void* stream, Format format );
	USColorVec		Set					( u32 c0 );
	void			WriteRGBA			( void* stream, u32 color, Format format );
};

//================================================================//
// USPixel
//================================================================//
namespace USPixel {

	enum Format {
		TRUECOLOR,
		INDEX_4,
		INDEX_8,
		PXL_FMT_UNKNOWN,
	};

	//----------------------------------------------------------------//
	u32				GetDepth			( Format format, USColor::Format colorFormat );
	u32				GetMask				( Format format, USColor::Format colorFormat );
	u32				GetPaletteCount		( Format format );
	u32				GetPaletteSize		( Format format, USColor::Format colorFormat );
	float			GetSize				( Format format, USColor::Format colorFormat );
	u32				ReadPixel			( const void* stream, u32 nBytes );
	void			ToTrueColor			( void* destColors, const void* srcColors, const void* palette, u32 nColors, USColor::Format colorFormat, Format pixelFormat );
	void			WritePixel			( void* stream, u32 pixel, u32 nBytes );
};

//================================================================//
// USColorVec
//================================================================//
class USColorVec {
public:

	float	mR;
	float	mG;
	float	mB;
	float	mA;
	
	//----------------------------------------------------------------//
	void			Add					( const USColorVec& c );
	void			FromYUV				( float y, float u, float v );
	float			GetLuma				();
	void			Lerp				( const USColorVec& v0, const USColorVec& v1, float t );
	void			LoadGfxState		() const;
	void			Modulate			( const USColorVec& v0 );
	u32				PackRGBA			();
	void			SetRGBA				( u32 color );
	void			Set					( float r, float g, float b, float a );
	void			SetBlack			();
	void			SetWhite			();
	void			ToYUV				( float& y, float& u, float& v );
					USColorVec			();
					USColorVec			( float r, float g, float b, float a );
};

#endif
