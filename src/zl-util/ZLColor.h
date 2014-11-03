// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCOLOR_H
#define ZLCOLOR_H

class ZLColorVec;
class ZLColorBlendFunc;

//================================================================//
// ZLColor
//================================================================//
namespace ZLColor {

	// do not re-order
	enum {
		R_BYTE,
		G_BYTE,
		B_BYTE,
		A_BYTE,
	};

	enum Format {
		A_8,
		RGB_888,
		RGB_565,
		RGBA_5551,
		RGBA_4444,
		RGBA_8888,
		CLR_FMT_UNKNOWN,
	};
	
	enum BlendEquation {
		BLEND_EQ_ADD,
		BLEND_EQ_NONE,
		BLEND_EQ_SUBTRACT,
	};
	
	enum BlendFactor {
		BLEND_FACTOR_ONE,
		BLEND_FACTOR_ZERO,
		BLEND_FACTOR_DST_ALPHA,
		BLEND_FACTOR_DST_COLOR,
		BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
		BLEND_FACTOR_ONE_MINUS_DST_COLOR,
		BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
		BLEND_FACTOR_SRC_ALPHA,
		BLEND_FACTOR_SRC_COLOR,
	};
	
	//----------------------------------------------------------------//
	u32				Add							( u32 c0, u32 c1 );
	u32				AddAndClamp					( u32 c0, u32 c1 );
	u32				Average						( u32 c0, u32 c1 );
	u32				Average						( u32 c0, u32 c1, u32 c2, u32 c3 );
	u32				BilerpFixed					( u32 c0, u32 c1, u32 c2, u32 c3, u8 xt, u8 yt );
	u32				Blend						( u32 src32, u32 dst32, const ZLColorBlendFunc& blendFunc );
	u32				Blend						( u32 src32, u32 dst32, BlendFactor srcFactor, BlendFactor dstFactor, BlendEquation blendEq );
	u32				BlendFactorAlpha			( u32 color32 );
	u32				BlendFactorOneMinusAlpha	( u32 color32 );
	u32				BlendFactorOneMinusColor	( u32 color32 );
	void			Convert						( void* dest, Format destFmt, const void* src, Format srcFmt, u32 nColors );
	u32				ConvertFromRGBA				( u32 color, Format format );
	u32				ConvertToRGBA				( u32 color, Format format );
	void			Desaturate					( void* colors, Format format, u32 nColors, float K );
	void			GammaCorrection				( void* colors, Format format, u32 nColors, float gamma );
	u32				GetBlendFactor				( u32 src32, u32 dst32, BlendFactor factor );
	u32				GetDepth					( Format format );
	u32				GetMask						( Format format );
	u32				GetSize						( Format format );
	u32				LerpFixed					( u32 c0, u32 c1, u8 t );
	u32				Mul							( u32 c0, u32 c1 );
	u32				NearestNeighbor				( u32 c0, u32 c1, u32 c2, u32 c3, u8 xt, u8 yt );
	u32				PackRGBA					( int r, int g, int b, int a );
	u32				PackRGBA					( float r, float g, float b, float a );
	void			PremultiplyAlpha			( void* colors, Format format, u32 nColors );
	u32				ReadRGBA					( const void* stream, Format format );
	u32				Scale						( u32 c0, u8 s ); // scale all components by s and normalize back to 255
	ZLColorVec		Set							( u32 c0 );
	u32				Set							( u32 c0, u8 b, u8 v ); // c0 [ b ] = v
	u32				Sub							( u32 c0, u32 c1 );
	u32				Swizzle						( u32 c0, u32 sw );
	void			WriteRGBA					( void* stream, u32 color, Format format );
};

//================================================================//
// ZLColorBlendFunc
//================================================================//
class ZLColorBlendFunc {
public:

	ZLColor::BlendFactor		mSrcFactor;
	ZLColor::BlendFactor		mDstFactor;
	ZLColor::BlendEquation		mEquation;
};

//================================================================//
// ZLPixel
//================================================================//
namespace ZLPixel {

	enum Format {
		TRUECOLOR,
		INDEX_4,
		INDEX_8,
		PXL_FMT_UNKNOWN,
	};

	//----------------------------------------------------------------//
	u32				GetDepth			( Format format, ZLColor::Format colorFormat );
	u32				GetMask				( Format format, ZLColor::Format colorFormat );
	u32				GetPaletteCount		( Format format );
	u32				GetPaletteSize		( Format format, ZLColor::Format colorFormat );
	float			GetSize				( Format format, ZLColor::Format colorFormat );
	u32				ReadPixel			( const void* stream, u32 nBytes );
	void			ToTrueColor			( void* destColors, const void* srcColors, const void* palette, u32 nColors, ZLColor::Format colorFormat, Format pixelFormat );
	void			WritePixel			( void* stream, u32 pixel, u32 nBytes );
};

//================================================================//
// ZLColorVec
//================================================================//
class ZLColorVec {
public:

	float	mR;
	float	mG;
	float	mB;
	float	mA;
	
	//----------------------------------------------------------------//
	void			Add					( const ZLColorVec& c );
	void			FromHSV				( float h, float s, float v );
	void			FromYUV				( float y, float u, float v);
	float			GetLuma				() const;
	bool			IsClear				() const;
	bool			IsOpaque			() const;
	void			Lerp				( u32 mode, const ZLColorVec& v0, const ZLColorVec& v1, float t );
	void			Modulate			( const ZLColorVec& v0 );
	u32				PackRGBA			() const;
	void			SetRGBA				( u32 color );
	void			Set					( float r, float g, float b, float a );
	void			SetBlack			();
	void			SetWhite			();
	void			ToHSV				( float& h, float& s, float& v );
	void			ToYUV				( float& y, float& u, float& v );
					ZLColorVec			();
					ZLColorVec			( float r, float g, float b, float a );
	
	//----------------------------------------------------------------//
	bool operator!=(const ZLColorVec &other) const {
		return ((this->mR != other.mR) ||
		        (this->mG != other.mG) ||
		        (this->mB != other.mB) ||
		        (this->mA != other.mA));
	}
	
	//----------------------------------------------------------------//
	ZLColorVec operator*(const ZLColorVec &other) const {
		return ZLColorVec(this->mR * other.mR,
		        	  this->mG * other.mG,
		        	  this->mB * other.mB,
		        	  this->mA * other.mA);
	}
	
	//----------------------------------------------------------------//
	ZLColorVec ScaleColor(const float other) const {
		return ZLColorVec(this->mR * other,
		        	  this->mG * other,
		        	  this->mB * other,
		        	  this->mA);
	}
	
	//----------------------------------------------------------------//
	ZLColorVec ScaleAlpha(const float other) const {
		return ZLColorVec(this->mR,
		        	  this->mG,
		        	  this->mB,
		        	  this->mA * other);
	}
	
	//----------------------------------------------------------------//
	ZLColorVec operator*(const float other) const {
		return ZLColorVec(this->mR * other,
		        	  this->mG * other,
		        	  this->mB * other,
		        	  this->mA);
	}
};

#endif
