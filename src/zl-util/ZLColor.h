// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCOLOR_H
#define ZLCOLOR_H

#include <zl-util/ZLMatrix4x4.h>

class ZLColorVec;
class ZLColorBlendFunc;

#define LUMA_601_R 0.299
#define LUMA_601_G 0.587
#define LUMA_601_B 0.114

#define LUMA_709_R 0.2126
#define LUMA_709_G 0.7152
#define LUMA_709_B 0.0722

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

	enum ColorFormat {
		A_1,
		A_4,
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
		BLEND_EQ_SUB,
		BLEND_EQ_SUB_INV,
	};
	
	enum BlendFactor {
		BLEND_FACTOR_0001,
		BLEND_FACTOR_1110,
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
	void			Convert						( void* dest, ColorFormat destFmt, const void* src, ColorFormat srcFmt, u32 nColors );
	u32				ConvertFromRGBA				( u32 color, ColorFormat format );
	u32				ConvertToRGBA				( u32 color, ColorFormat format );
	void			Desaturate					( void* colors, ColorFormat format, u32 nColors, float rY, float gY, float bY, float K );
	void			GammaCorrection				( void* colors, ColorFormat format, u32 nColors, float gamma );
	u32				GetBlendFactor				( u32 src32, u32 dst32, BlendFactor factor );
	u32				GetDepthInBits				( ColorFormat format );
	u32				GetMask						( ColorFormat format );
	u32				LerpFixed					( u32 c0, u32 c1, u8 t );
	void			Mix							( void* colors, ColorFormat format, u32 nColors, const ZLMatrix4x4& mtx, float K );
	u32				Mul							( u32 c0, u32 c1 );
	u32				NearestNeighbor				( u32 c0, u32 c1, u32 c2, u32 c3, u8 xt, u8 yt );
	u32				PackRGBA					( int r, int g, int b, int a );
	u32				PackRGBA					( float r, float g, float b, float a );
	void			PremultiplyAlpha			( void* colors, ColorFormat format, u32 nColors );
	u32				Scale						( u32 c0, u8 s ); // scale all components by s and normalize back to 255
	ZLColorVec		Set							( u32 c0 );
	u32				Set							( u32 c0, u8 b, u8 v ); // c0 [ b ] = v
	void			SimpleThreshold				( void* colors, ColorFormat format, u32 nColors, u32 color );
	u32				Sub							( u32 c0, u32 c1 );
	u32				SubAndClamp					( u32 c0, u32 c1 );
	u32				Swizzle						( u32 c0, u32 sw );
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
	bool			Compare				( const ZLColorVec& c );
	bool			Compare				( const ZLColorVec& c, float res );
	void			FromHSV				( float h, float s, float v );
	void			FromYUV				( float y, float u, float v);
	float			GetLuma				() const;
	bool			IsClear				() const;
	bool			IsOpaque			() const;
	void			Lerp				( u32 mode, const ZLColorVec& v0, const ZLColorVec& v1, float t );
	void			Modulate			( const ZLColorVec& v0 );
	u32				PackRGBA			() const;
	void			Scale				( float scale );
	void			SetRGBA				( u32 color );
	void			Set					( float r, float g, float b, float a );
	void			SetBlack			();
	void			SetWhite			();
	void			ToHSV				( float& h, float& s, float& v );
	void			ToYUV				( float& y, float& u, float& v );
					ZLColorVec			();
					ZLColorVec			( u32 rgba );
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
