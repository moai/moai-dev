// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLIMAGE_H
#define	ZLIMAGE_H

#include <zl-core/ZLAccessors.h>
#include <zl-core/ZLColor.h>
#include <zl-core/ZLCopyOnWrite.h>
#include <zl-core/ZLFrameSize.h>

//================================================================//
// ZLImageTransform
//================================================================//
namespace ZLImageTransform {
	enum Flags {
		POW_TWO				= 0x01,
		QUANTIZE			= 0x02,
		TRUECOLOR			= 0x04,
		PREMULTIPLY_ALPHA	= 0x08,
	};
}

//================================================================//
// ZLImage
//================================================================//
class ZLImage :
	public virtual ZLFrameSize {
public:

	enum PixelFormat {
		TRUECOLOR,
		INDEX_4,
		INDEX_8,
		PXL_FMT_UNKNOWN,
	};

protected:

	friend class ZLImageFormat;

	PixelFormat				mPixelFormat;
	ZLColor::ColorFormat	mColorFormat;
	
	ZLCopyOnWrite	mBitmap;
	ZLCopyOnWrite	mPalette;

	SET ( PixelFormat, PixelFormat, mPixelFormat )
	SET ( ZLColor::ColorFormat, ColorFormat, mColorFormat )

	SET ( u32, Width, mWidth )
	SET ( u32, Height, mHeight )

	//----------------------------------------------------------------//
	void			ComparePixel			( ZLIntVec2D** grid, ZLIntVec2D& p, int x, int y, int offsetX, int offsetY, int width, int height );
	void			CalculateSDF			( ZLIntVec2D** grid, int width, int height );
	const void*		GetRowAddr				( u32 y ) const;
	void*			GetRowAddrMutable		( u32 y );
	void			NotifyStatusChanged		();
	
	//----------------------------------------------------------------//
	virtual void	ZLImage_OnImageStatusChanged		( bool isOK );

public:

	GET_CONST ( PixelFormat, PixelFormat, mPixelFormat )
	GET_CONST ( ZLColor::ColorFormat, ColorFormat, mColorFormat )
	
	GET_CONST ( void*, Bitmap, mBitmap.GetConstBuffer ())
	GET_CONST ( void*, Palette, mPalette.GetConstBuffer ())
	
	GET ( ZLCopyOnWrite, BitmapCow, mBitmap )
	GET ( ZLCopyOnWrite, PaletteCow, mPalette )
	
	GET ( ZLSharedConstBuffer*, BitmapBuffer, mBitmap.GetSharedConstBuffer ())
	GET ( ZLSharedConstBuffer*, PaletteBuffer, mBitmap.GetSharedConstBuffer ())
	
	enum {
		FILTER_LINEAR,
		FILTER_NEAREST,
	};
	
	//----------------------------------------------------------------//
	void					Alloc							();
	ZLColorVec				Average							() const;
	void					BleedRect						( ZLIntRect rect );
	void					Blit							( const ZLImage& image, int srcX, int srcY, int destX, int destY, int width, int height );
	void					Blur							();
	static void				CalculateGaussianKernel			( float radius, float* kernel, size_t kernalWidth );
	static void				CalculateGaussianKernel			( float radius, float sigma, float* kernel, size_t kernalWidth );
	static size_t			CalculateGaussianKernelWidth	( float radius );
	void					Clear							();
	void					ClearBitmap						();
	void					ClearRect						( ZLIntRect rect );
	bool					Compare							( const ZLImage& image );
	bool					Convert							( const ZLImage& image, ZLColor::ColorFormat colorFmt = ZLColor::RGBA_8888, PixelFormat pixelFmt = TRUECOLOR );
	void					Convolve						( const ZLImage& image, const float* kernel, size_t kernelWidth );
	void					Convolve						( const ZLImage& image, const float* kernel, size_t kernelWidth, size_t kernelHeight );
	void					Convolve1D						( const ZLImage& image, const float* kernel, size_t kernelSize, bool horizontal );
	void					Copy							( const ZLImage& image );
	void					CopyRect						( const ZLImage& image, ZLIntRect srcRect, ZLIntRect destRect, u32 filter = FILTER_LINEAR );
	void					CopyRect						( const ZLImage& image, ZLIntRect srcRect, ZLIntRect destRect, u32 filter, const ZLColorBlendFunc& blendMode );
	void					Desaturate						( const ZLImage& image, float rY, float gY, float bY, float K );
	void					DrawLine						( int p1x, int p1y, int p2x, int p2y, u32 color );
	void					FillCircle						( float x, float y, float xRad, u32 color );
	void					FillEllipse						( int x, int y, int xRad, int yRad, u32 color );
	void					FillRect						( ZLIntRect rect, u32 color );
	void					GammaCorrection					( const ZLImage& image, float gamma );
	void					GenerateOutlineFromSDF			( ZLIntRect rect, float distMin, float distMax, float r, float g, float b, float a );
	void					GenerateSDF						( ZLIntRect rect );
	void					GenerateSDFAA					( ZLIntRect rect, float sizeInPixels );
	void					GenerateSDFDeadReckoning		( ZLIntRect rect, int threshold );
	size_t					GetBitmapSize					() const;
	u32						GetColor						( u32 x, u32 y ) const;
	ZLIntRect				GetContentRect					();
	size_t					GetDataSize						() const;
	ZLIntRect				GetFrame						();
	static u32				GetMinPowerOfTwo				( u32 size ); // gets the smallest power of two greater than size
	u32						GetPaletteColor					( u32 idx ) const;
	u32						GetPaletteCount					() const;
	size_t					GetPaletteSize					() const;
	u32						GetPixel						( u32 x, u32 y ) const;
	u32						GetPixelDepthInBits				() const;
	u32						GetPixelMask					() const;
	ZLIntRect				GetRect							();
	size_t					GetRowSize						() const;
	void					GetSubImage						( const ZLImage& image, ZLIntRect rect );
	void					Init							( const ZLImage& image );
	void					Init							( u32 width, u32 height, ZLColor::ColorFormat colorFmt, PixelFormat pixelFmt );
	void					Init							( const void* bitmap, u32 width, u32 height, ZLColor::ColorFormat colorFmt );
	bool					IsPow2							();
	static bool				IsPow2							( u32 n );
	bool					IsOK							() const;
	bool					Load							( cc8* filename, u32 transform = 0 );
	bool					Load							( ZLStream& stream, u32 transform );
	bool					MipReduce						();
	void					Mix								( const ZLImage& image, const ZLMatrix4x4& mtx, float K );
	void					PadToPow2						( const ZLImage& image );
	void					PremultiplyAlpha				( const ZLImage& image );
	void					Print							();
	void					ResizeCanvas					( const ZLImage& image, ZLIntRect rect );
	u32						SampleColor						( float x, float y, u32 filter, bool wrapX = false, bool wrapY = false ) const;
	void					SetColor						( u32 x, u32 y, u32 color );
	void					SetColor						( u32 x, u32 y, u32 color, const ZLColorBlendFunc& blendMode );
	void					SetPaletteColor					( u32 idx, u32 rgba );
	void					SetPixel						( u32 x, u32 y, u32 pixel );
	void					SimpleThreshold					( const ZLImage& image, float rT, float gT, float bT, float aT );
	void					Take							( ZLImage& image );
	void					Transform						( u32 transform );
	bool					Write							( ZLStream& stream, cc8* formatName ) const;
							ZLImage							();
	virtual					~ZLImage						();
	
	//----------------------------------------------------------------//
	// TODO: move this somewhere more appropriate
	static inline double Gaussian ( double x, double c ) {
	
		return Gaussian ( x, GaussianUnitIntegral ( c ), 0.0, c );
	}
	
	//----------------------------------------------------------------//
	// TODO: move this somewhere more appropriate
	static inline double Gaussian ( double x, double b, double c ) {
	
		return Gaussian ( x, GaussianUnitIntegral ( c ), b, c );
	}
	
	//----------------------------------------------------------------//
	// TODO: move this somewhere more appropriate
	static inline double Gaussian ( double x, double a, double b, double c ) {
	
		return a * exp ( -((( x - b ) * ( x - b )) / ( 2.0 * ( c * c ))));
	}
	
	//----------------------------------------------------------------//
	// TODO: move this somewhere more appropriate
	static inline double GaussianUnitIntegral ( double c ) {
	
		return 1.0 / ( c * sqrt ( TWOPI ));
	}
};

#endif
