// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <contrib/moai_edtaa3func.h>
#include <float.h>
#include <zl-util/ZLBitBuffer.h>
#include <zl-util/ZLColor.h>
#include <zl-util/ZLFileStream.h>
#include <zl-util/ZLImage.h>
#include <zl-util/ZLImageFormatMgr.h>

//================================================================//
// ZLImage
//================================================================//

//----------------------------------------------------------------//
void ZLImage::Alloc () {

	this->mBitmap.Alloc ( this->GetBitmapSize ());
	this->mPalette.Alloc ( this->GetPaletteSize ());
}

//----------------------------------------------------------------//
ZLColorVec ZLImage::Average () const {

	ZLColorVec average;
	average.Set ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	u32 width = this->mWidth;
	u32 height = this->mHeight;
	
	for ( u32 y = 0; y < height; ++y ) {
		for ( u32 x = 0; x < width; ++x ) {
			average.Add ( ZLColorVec ( this->GetColor ( x, y )));
		}
	}
	
	float scale = 1.0f / ( float )( width * height );
	average.Scale ( scale );
	return average;
}

//----------------------------------------------------------------//
void ZLImage::BleedRect ( ZLIntRect rect ) {

	rect.Bless ();

	int xMin = rect.mXMin;
	int yMin = rect.mYMin;
	int xMax = rect.mXMax;
	int yMax = rect.mYMax;

	int width = ( int )this->mWidth;
	int height = ( int )this->mHeight;

	// clamp min to 0
	xMin = ( xMin < 0 ) ? 0 : xMin;
	yMin = ( yMin < 0 ) ? 0 : yMin;
	
	// clamp max to image width, height
	xMax = ( xMax > width ) ? width : xMax;
	yMax = ( yMax > height ) ? height : yMax;

	if ( xMax <= xMin ) return;
	if ( yMax <= yMin ) return;

	if ( xMin >= width ) return;
	if ( yMin >= height ) return;
	
	if ( xMax <= 0 ) return;
	if ( yMax <= 0 ) return;
	
	// copy the wings (left and right)
	if (( xMin > 0 ) || ( xMax < width )) {
		for ( int y = yMin; y < yMax; ++y ) {
		
			// left wing
			if ( xMin > 0 ) {
				this->SetPixel ( xMin - 1, y, this->GetPixel ( xMin, y ));
			}
			
			// right wing
			if ( xMax < width ) {
				this->SetPixel ( xMax, y, this->GetPixel ( xMax - 1, y ));
			}
		}
	}
	
	// expand and clamp left and right
	xMin = ( xMin > 1 ) ? xMin - 1 : 0;
	xMax = ( xMax < width ) ? xMax + 1 : width;
	
	u32 bitDepth = this->GetPixelDepthInBits ();
	
	if ( yMin > 0 ) {
		ZLBitBuffer::Blit ( this->GetRowAddrMutable ( yMin - 1 ), xMin, this->GetRowAddr ( yMin ), xMin, xMax - xMin, bitDepth );
	}
	
	if ( yMax < height ) {
		ZLBitBuffer::Blit ( this->GetRowAddrMutable ( yMax ), xMin, this->GetRowAddr ( yMax - 1 ), xMin, xMax - xMin, bitDepth );
	}
}

//----------------------------------------------------------------//
void ZLImage::Blit ( const ZLImage& image, int srcX, int srcY, int destX, int destY, int width, int height ) {

	int srcXMax = srcX + width;
	int srcYMax = srcY + height;

	if ( srcX < 0 ) {
		destX -= srcX;
		width += srcX;
		srcX = 0;
	}
	
	if ( srcY < 0 ) {
		destY -= srcY;
		height += srcY;
		srcY = 0;
	}
	
	if ( srcXMax > ( int )image.mWidth ) {
		width -= srcXMax - ( int )image.mWidth;
	}
	
	if ( srcYMax > ( int )image.mHeight ) {
		height -= srcYMax - ( int )image.mHeight;
	}
	
	int destXMax = destX + width;
	int destYMax = destY + height;
	
	if ( destX < 0 ) {
		srcX -= destX;
		width += destX;
		destX = 0;
	}
	
	if ( destY < 0 ) {
		srcY -= destY;
		height += destY;
		destY = 0;
	}
	
	if ( destXMax > ( int )this->mWidth ) {
		width -= destXMax - ( int )this->mWidth;
	}
	
	if ( destYMax > ( int )this->mHeight ) {
		height -= destYMax - ( int )this->mHeight;
	}
	
	if ( width <= 0 ) return;
	if ( height <= 0 ) return;
	
	if ( srcX >= ( int )image.mWidth ) return;
	if ( srcY >= ( int )image.mHeight ) return;
	
	if ( destX >= ( int )this->mWidth ) return;
	if ( destY >= ( int )this->mHeight ) return;
	
	u32 pixelDepth = this->GetPixelDepthInBits ();
	
	for ( int y = 0; y < height; ++y ) {
		
		const void* srcRow = image.GetRowAddr ( y + srcY );
		void* destRow = this->GetRowAddrMutable ( y + destY );
		
		ZLBitBuffer::Blit ( destRow, destX, srcRow, srcX, width, pixelDepth );
	}
}

//----------------------------------------------------------------//
//static int reflect ( int M, int x ) {
//	
//	if ( x < 0 ) {
//		return -x - 1;
//	}
//	if ( x >= M ) {
//		return 2 * M - x - 1;
//	}
//	return x;
//}

//----------------------------------------------------------------//
void ZLImage::CalculateGaussianKernel ( float radius, float sigma, float* kernel, size_t kernelWidth ) {

	float sum = 0.0;

	double x = -( double )radius;
	for ( size_t i = 0; i < kernelWidth; ++i, x += 1.0 ) {
	
		float g = ( float )Gaussian ( x, 2.0, 0.0, sigma ); // since we're going to normalize, we can use any value for a
		
		kernel [ i ] = g;
		sum += g;
	}
	
	for ( size_t i = 0; i < kernelWidth; ++i ) {
		kernel [ i ] /= sum;
	}
}

//----------------------------------------------------------------//
size_t ZLImage::CalculateGaussianKernelWidth ( float radius ) {

	return ((( size_t )ceil ( radius )) * 2 ) + 1;
}

//----------------------------------------------------------------//
void ZLImage::CalculateSDF( ZLIntVec2D** grid, int width, int height ) {
	
	// Pass 0
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			ZLIntVec2D p = grid[y][x];
			this->ComparePixel( grid, p, x, y, -1,  0, width, height );
			this->ComparePixel( grid, p, x, y,  0, -1, width, height );
			this->ComparePixel( grid, p, x, y, -1, -1, width, height );
			this->ComparePixel( grid, p, x, y,  1, -1, width, height );
			grid[y][x] = p;
		}
		
		for ( int x = width - 1; x >= 0; x-- ) {
			ZLIntVec2D p = grid[y][x];
			this->ComparePixel( grid, p, x, y, 1, 0, width, height );
			grid[y][x] = p;
		}
	}
	
	// Pass 1
	for ( int y = height - 1; y >= 0; y-- ) {
		for ( int x = width - 1; x >= 0; x-- ) {
			ZLIntVec2D p = grid[y][x];
			this->ComparePixel( grid, p, x, y,  1,  0, width, height );
			this->ComparePixel( grid, p, x, y,  0,  1, width, height );
			this->ComparePixel( grid, p, x, y, -1,  1, width, height );
			this->ComparePixel( grid, p, x, y,  1,  1, width, height );
			grid[y][x] = p;
		}
		
		for ( int x = 0; x < width; x++ ) {
			ZLIntVec2D p = grid[y][x];
			this->ComparePixel( grid, p, x, y, -1, 0, width, height );
			grid[y][x] = p;
		}
	}
}

//----------------------------------------------------------------//
void ZLImage::Clear () {

	this->mBitmap.Free ();
	this->mPalette.Free ();
	
	this->mColorFormat	= ZLColor::CLR_FMT_UNKNOWN;
	this->mPixelFormat	= PXL_FMT_UNKNOWN;

	this->mWidth		= 0;
	this->mHeight		= 0;
}

//----------------------------------------------------------------//
void ZLImage::ClearBitmap () {

	void* bitmap = this->mBitmap.Invalidate ();
	if ( bitmap ) {
		memset ( bitmap, 0, this->GetBitmapSize ());
	}
}

//----------------------------------------------------------------//
void ZLImage::ClearRect ( ZLIntRect rect ) {

	rect.Bless ();
	
	this->GetFrame ().Clip ( rect );
	
	int width = rect.Width ();
	
	if ( !width ) return;
	if ( !rect.Height ()) return;

	u32 pixelDepth = this->GetPixelDepthInBits ();
	
	for ( int y = rect.mYMin; y < rect.mYMax; ++y ) {
		ZLBitBuffer::Clear ( this->GetRowAddrMutable ( y ), pixelDepth, width, pixelDepth );
	}
}

//----------------------------------------------------------------//
bool ZLImage::Compare ( const ZLImage& image ) {

	if (( this->mWidth != image.mWidth ) || ( this->mHeight != image.mHeight )) {
		return false;
	}

	float r [ 2 ][ 4 ] = {{ 0 }};
	float g [ 2 ][ 4 ] = {{ 0 }};
	float b [ 2 ][ 4 ] = {{ 0 }};
	float a [ 2 ][ 4 ] = {{ 0 }};

	for ( u32 i = 0; i < this->mHeight; i++ ) {
		for ( u32 j = 0; j < this->mWidth; j++ ) {
			ZLColorVec color1, color2;
			color1.SetRGBA ( this->GetColor ( j, i ));
			color2.SetRGBA ( image.GetColor ( j, i ));

			r [ 0 ][ u32 ( ZLFloat::Max ( color1.mR * 4 - 1, 0 ))]++;
			g [ 0 ][ u32 ( ZLFloat::Max ( color1.mG * 4 - 1, 0 ))]++;
			b [ 0 ][ u32 ( ZLFloat::Max ( color1.mB * 4 - 1, 0 ))]++;
			a [ 0 ][ u32 ( ZLFloat::Max ( color1.mA * 4 - 1, 0 ))]++;

			r [ 1 ][ u32 ( ZLFloat::Max ( color2.mR * 4 - 1, 0 ))]++;
			g [ 1 ][ u32 ( ZLFloat::Max ( color2.mG * 4 - 1, 0 ))]++;
			b [ 1 ][ u32 ( ZLFloat::Max ( color2.mB * 4 - 1, 0 ))]++;
			a [ 1 ][ u32 ( ZLFloat::Max ( color2.mA * 4 - 1, 0 ))]++;
		}
	}

	for ( u32 i = 0; i < 2; i++ ) {
		for ( u32 j = 0; j < 4; j++ ) {
			r [ i ][ j ] = r [ i ][ j ] / ( this->mWidth * this->mHeight );
			g [ i ][ j ] = g [ i ][ j ] / ( this->mWidth * this->mHeight );
			b [ i ][ j ] = b [ i ][ j ] / ( this->mWidth * this->mHeight );
			a [ i ][ j ] = a [ i ][ j ] / ( this->mWidth * this->mHeight );
		}
	}

	for ( u32 i = 0; i < 4; i++ ) {
		r [ 0 ][ i ] = ZLFloat::Abs ( r [ 0 ][ i ] - r [ 1 ][ i ]);
		g [ 0 ][ i ] = ZLFloat::Abs ( g [ 0 ][ i ] - g [ 1 ][ i ]);
		b [ 0 ][ i ] = ZLFloat::Abs ( b [ 0 ][ i ] - b [ 1 ][ i ]);
		a [ 0 ][ i ] = ZLFloat::Abs ( a [ 0 ][ i ] - a [ 1 ][ i ]);
	}

	float similar = 0;

	for ( u32 i = 0; i < 4; i++ ) {
		similar += ( r [ 0 ][ i ] + g [ 0 ][ i ] + b [ 0 ][ i ] + a [ 0 ][ i ]);
	}

	return similar < .02; // TODO: WTF?
}

//----------------------------------------------------------------//
void ZLImage::ComparePixel( ZLIntVec2D** grid, ZLIntVec2D& p, int x, int y, int offsetX, int offsetY, int width, int height ) {
	
	// Need to check for boundaries unless there is a 1 pixel gutter
	ZLIntVec2D otherPixel; //= grid[y + offsetY][x + offsetX];
	
	int newX = x + offsetX;
	int newY = y + offsetY;
	
	if ( newX >= 0 && newY >= 0 && newX < width && newY < height ) {
		otherPixel = grid[newY][newX];
	}
	else {
		otherPixel.mX = 9999;
		otherPixel.mY = 9999;
	}
		
	otherPixel.mX += offsetX;
	otherPixel.mY += offsetY;
	
	if ( otherPixel.LengthSquared () < p.LengthSquared ()) {
		p = otherPixel;
	}
}

//----------------------------------------------------------------//
bool ZLImage::Convert ( const ZLImage& image, ZLColor::ColorFormat colorFmt, PixelFormat pixelFmt ) {
	
	if (( pixelFmt != TRUECOLOR ) && ( pixelFmt != image.mPixelFormat )) {
		// TODO: we don't yet support conversion to indexed color or between indexed color formats
		// TODO: log an error
		this->Clear ();
		return false;
	}
	
	// if everything's the same, just copy it over and bail
	if (( colorFmt == image.mColorFormat ) && ( pixelFmt == image.mPixelFormat )) {
		if ( this != &image ) {
			this->Copy ( image );
		}
		return true;
	}

	ZLImage newImage;
	newImage.Init ( image.mWidth, image.mHeight, colorFmt, pixelFmt );
	
	if ( pixelFmt == TRUECOLOR ) {
	
		// we want this in truecolor
		if ( image.mPixelFormat != TRUECOLOR ) {
		
			// we're converting from indexed color, so read and set every pixel
			for ( u32 y = 0; y < image.mHeight; ++y ) {
				for ( u32 x = 0; x < image.mWidth; ++x ) {
					newImage.SetColor ( x, y, image.GetColor ( x, y ));
				}
			}
		}
		else {
		
			// it's already truecolor, so we're just converting the color format
			// we'll use the helper method in ZLColor which is a little more efficient
			for ( u32 y = 0; y < image.mHeight; ++y ) {
				const void* srcRow = image.GetRowAddr ( y );
				void* destRow = newImage.GetRowAddrMutable ( y );
				ZLColor::Convert ( destRow, newImage.mColorFormat, srcRow, image.mColorFormat, image.mWidth );
			}
		}
	}
	else {
	
		// pixel format is indexed color, so just convert the palette
		u32 total = image.GetPaletteCount ();
		ZLColor::Convert ( newImage.mPalette.Invalidate (), newImage.mColorFormat, image.mPalette.GetConstBuffer (), image.mColorFormat, total );
	}
	
	this->Take ( newImage );
	return true;
}

//----------------------------------------------------------------//
void ZLImage::Convolve ( const ZLImage& image, const float* kernel, size_t kernelWidth ) {

	ZLImage temp;
	temp.Convolve1D ( image, kernel, kernelWidth, true );
	this->Convolve1D ( temp, kernel, kernelWidth, false );
}

//----------------------------------------------------------------//
void ZLImage::Convolve ( const ZLImage& image, const float* kernel, size_t kernelWidth, size_t kernelHeight ) {

	ZLImage temp;
	temp.Copy ( image );
	
	ZLColorVec sum;
	ZLColorVec color;
	
	size_t hKernelWidth = kernelWidth >> 1;
	size_t hKernelHeight = kernelHeight >> 1;
	
	for ( u32 y = 0; y < image.mHeight; ++y ) {
		for ( u32 x = 0; x < image.mHeight; ++x ) {
		
			sum.SetRGBA ( 0 );
		
			for ( size_t ky  = 0; ky < kernelHeight; ++ky ) {
				for ( size_t kx  = 0; kx < kernelWidth; ++kx ) {
			
					int sx = ( int )(( x + kx ) - hKernelWidth );
					sx = sx < 0 ? 0 : ( sx < ( int )image.mWidth ? sx : image.mWidth - 1 );
					
					int sy = ( int )(( y + ky ) - hKernelHeight );
					sy = sy < 0 ? 0 : ( sy < ( int )image.mHeight ? sy : image.mHeight - 1 );
					
					color.SetRGBA ( image.GetColor ( sx, sy ));
					
					float c = kernel [( ky * kernelWidth ) + kx ];
					
					sum.mR += color.mR * c;
					sum.mG += color.mG * c;
					sum.mB += color.mB * c;
					sum.mA += color.mA * c;
				}
			}
			
			sum.Clamp ();
			temp.SetColor ( x, y, sum.PackRGBA ());
		}
	}
	
	this->Take ( temp );
}

//----------------------------------------------------------------//
void ZLImage::Convolve1D ( const ZLImage& image, const float* kernel, size_t kernelSize, bool horizontal ) {

	ZLImage temp;
	temp.Copy ( image );
	
	ZLColorVec sum;
	ZLColorVec color;
	
	size_t hKernel = kernelSize >> 1;
	
	for ( u32 y = 0; y < image.mHeight; ++y ) {
		for ( u32 x = 0; x < image.mWidth; ++x ) {
		
			sum.SetRGBA ( 0 );
		
			for ( size_t k  = 0; k < kernelSize; ++k ) {

				int sx;
				int sy;
				
				if ( horizontal ) {
				
					sx = ( int )(( x + k ) - hKernel );
					sx = sx < 0 ? 0 : ( sx < ( int )image.mWidth ? sx : image.mWidth - 1 );
				
					sy = y;
				}
				else {
				
					sx = x;
					
					sy = ( int )(( y + k ) - hKernel );
					sy = sy < 0 ? 0 : ( sy < ( int )image.mHeight ? sy : image.mHeight - 1 );
				}
				
				color.SetRGBA ( image.GetColor ( sx, sy ));
				
				float c = kernel [ k ];
				
				sum.mR += color.mR * c;
				sum.mG += color.mG * c;
				sum.mB += color.mB * c;
				sum.mA += color.mA * c;
			}
			
			sum.Clamp ();
			temp.SetColor ( x, y, sum.PackRGBA ());
		}
	}
	
	this->Take ( temp );
}

//----------------------------------------------------------------//
void ZLImage::Copy ( const ZLImage& image ) {

	if ( this == &image ) return;

	this->mColorFormat	= image.mColorFormat;
	this->mPixelFormat	= image.mPixelFormat;

	this->mWidth		= image.mWidth;
	this->mHeight		= image.mHeight;
	
	this->mBitmap		= image.mBitmap;
	this->mPalette		= image.mPalette;
	
	this->ZLImage_OnImageStatusChanged ( true );
}

//----------------------------------------------------------------//
void ZLImage::CopyRect ( const ZLImage& image, ZLIntRect srcRect, ZLIntRect destRect, u32 filter ) {

	this->CopyRect ( image, srcRect, destRect, filter, ZLColorBlendFunc ());
}

//----------------------------------------------------------------//
void ZLImage::CopyRect ( const ZLImage& image, ZLIntRect srcRect, ZLIntRect destRect, u32 filter, const ZLColorBlendFunc& blendMode ) {

	if (( this->mPixelFormat != TRUECOLOR ) && ( image.mPixelFormat != TRUECOLOR )) return; // TODO: warn about this case

	bool xFlip = srcRect.IsXFlipped () != destRect.IsXFlipped ();
	bool yFlip = srcRect.IsYFlipped () != destRect.IsYFlipped ();

	srcRect.Bless ();
	destRect.Bless ();

	// if everything's the same size and format, we can do a simple blit
	if ( !( xFlip || yFlip )) {
		if (( blendMode.mEquation == ZLColor::BLEND_EQ_NONE ) && ( this->mPixelFormat == image.mPixelFormat ) && ( this->mColorFormat == image.mColorFormat )) {
			if (( srcRect.Width () == destRect.Width ()) && ( srcRect.Height () == destRect.Height ())) {
				this->Blit ( image, srcRect.mXMin, srcRect.mYMin, destRect.mXMin, destRect.mYMin, srcRect.Width (), srcRect.Height ());
				return;
			}
		}
	}

	// prepare the rectangles
	ZLIntRect srcBounds ( 0, 0, image.mWidth, image.mHeight );
	
	ZLIntRect destBounds ( 0, 0, this->mWidth, this->mHeight );

	if ( !srcRect.Overlap ( srcBounds )) return;
	if ( !destRect.Overlap ( destBounds )) return;
	
	ZLVec2D srcToDest (( float )( srcRect.Width () / destRect.Width ()), ( float )( srcRect.Height () / destRect.Height ()));
	ZLVec2D destToSrc ( 1.0f / srcToDest.mX, 1.0f / srcToDest.mY );
	
	ZLIntRect srcClipA = srcRect;
	ZLIntRect srcClipB = srcRect;
	
	ZLIntRect destClipA = destRect;
	ZLIntRect destClipB = destRect;

	// get the rects clipped against their respective bounds
	srcBounds.Clip ( srcClipA );
	destBounds.Clip ( destClipA );
	
	// now we need to get each rect's subrect adjusted for the *other* rect's clip

	if ( srcClipA.Width () < srcRect.Width ()) {
	
		int offset = ( int )floorf (( srcClipA.mXMin - srcRect.mXMin ) * srcToDest.mX );
		int width = ( int )floorf ( srcClipA.Width () * srcToDest.mX );
	
		if ( xFlip ) {
			destClipB.mXMax -= offset;
			destClipB.mXMin = destClipB.mXMax - width;
		}
		else {
			destClipB.mXMin += offset;
			destClipB.mXMax = destClipB.mXMin + width;
		}
	}
	
	if ( srcClipA.Height () < srcRect.Height ()) {
	
		int offset = ( int )floorf (( srcClipA.mYMin - srcRect.mYMin ) * srcToDest.mY );
		int height = ( int )floorf ( srcClipA.Height () * srcToDest.mY );
	
		if ( yFlip ) {
			destClipB.mYMax -= offset;
			destClipB.mYMin = destClipB.mYMax - height;
		}
		else {
			destClipB.mYMin += offset;
			destClipB.mYMax = destClipB.mYMin + height;
		}
	}
	
	if ( destClipA.Width () < destRect.Width ()) {
	
		int offset = ( int )floorf (( destClipA.mXMin - destRect.mXMin ) * destToSrc.mX );
		int width = ( int )floorf ( destClipA.Width () * destToSrc.mX );
	
		if ( xFlip ) {
			srcClipB.mXMax -= offset;
			srcClipB.mXMin = srcClipB.mXMax - width;
		}
		else {
			srcClipB.mXMin += offset;
			srcClipB.mXMax = srcClipB.mXMin + width;
		}
	}
	
	if ( destClipA.Height () < destRect.Height ()) {
	
		int offset = ( int )floorf (( destClipA.mYMin - destRect.mYMin ) * destToSrc.mY );
		int height = ( int )floorf ( destClipA.Height () * destToSrc.mY );
	
		if ( yFlip ) {
			srcClipB.mYMax -= offset;
			srcClipB.mYMin = srcClipB.mYMax - height;
		}
		else {
			srcClipB.mYMin += offset;
			srcClipB.mYMax = srcClipB.mYMin + height;
		}
	}
	
	// the final rects are the intersection of clipA and clipB
	srcRect = srcClipA;
	if ( !srcRect.Intersect ( srcClipB, srcRect )) return;
	
	destRect = destClipA;
	if ( !destRect.Intersect ( destClipB, destRect )) return;

	// TODO: if *image == this *and* srcRect, destRect overlap, we need to copy srcRect out of image
	bool useCopyImage = false;
	ZLImage copyImage;
	
	if (( this == &image ) && srcRect.OverlapExcludeEdge ( destRect )) {
		
		ZLIntRect newSrcRect = srcRect;
		newSrcRect.Offset ( -newSrcRect.mXMin, -newSrcRect.mYMin );
		
		copyImage.Init ( newSrcRect.mXMax, newSrcRect.mYMax, image.mColorFormat, image.mPixelFormat );
		copyImage.Blit ( image, srcRect.mXMin, srcRect.mYMin, newSrcRect.mXMin, newSrcRect.mYMin, newSrcRect.mXMax, newSrcRect.mYMax );
		
		srcRect = newSrcRect;
		useCopyImage = true;
	}

	const ZLImage& srcImage = useCopyImage ? copyImage : image;

	// now set up the copy
	int srcWidth = srcRect.Width ();
	int srcHeight = srcRect.Height ();
	
	int destWidth = destRect.Width ();
	int destHeight = destRect.Height ();
	
	float xSrcStep = ( float )srcWidth / ( float )destWidth;
	float ySrcStep = ( float )srcHeight / ( float )destHeight;
	
	int xSrcSampleSteps = ( int )ceilf ( xSrcStep );
	int ySrcSampleSteps = ( int )ceilf ( ySrcStep );
	
	float sampleAverage = 1.0f / ( float )( xSrcSampleSteps * ySrcSampleSteps );
	
	float xSampleStep = xSrcStep / ( float )xSrcSampleSteps;
	float ySampleStep = ySrcStep / ( float )ySrcSampleSteps;
	
	float xSrcOrigin = ( float )srcRect.mXMin;
	float ySrcOrigin = ( float )srcRect.mYMin;
	
	if ( xFlip ) {
		xSrcOrigin = ( float )srcRect.mXMax;
		xSampleStep = -xSampleStep;
		xSrcStep = -xSrcStep;
	}
	
	if ( yFlip ) {
		ySrcOrigin = ( float )srcRect.mYMax;
		ySampleStep = -ySampleStep;
		ySrcStep = -ySrcStep;
	}
	
	if ( this->mPixelFormat != TRUECOLOR ) {
	
		// TODO: warn about this case
	
		assert ( this->mColorFormat == srcImage.mColorFormat );
		
		for ( int y = 0; y < destHeight; ++y ) {
			for ( int x = 0; x < destWidth; ++x ) {
				
				u32 xPixel = ( u32 )floorf ( xSrcOrigin + (( float )x * xSrcStep ) + 0.5f );
				u32 yPixel = ( u32 )floorf ( ySrcOrigin + (( float )y * ySrcStep ) + 0.5f );
				
				u32 pixel = srcImage.GetPixel ( xPixel, yPixel );
				this->SetPixel ( xPixel, yPixel, pixel );
			}
		}
	}
	else if (( filter == FILTER_NEAREST ) || (( xSrcSampleSteps == 1 ) && ( ySrcSampleSteps == 1 ))) {
	
		for ( int y = 0; y < destHeight; ++y ) {
			for ( int x = 0; x < destWidth; ++x ) {
				
				float xSample = xSrcOrigin + (( float )x * xSrcStep );
				float ySample = ySrcOrigin + (( float )y * ySrcStep );
				
				u32 color = srcImage.SampleColor ( xSample, ySample, filter );
				this->SetColor ( destRect.mXMin + x, destRect.mYMin + y, color, blendMode );
			}
		}
	}
	else {
	
		for ( int y = 0; y < destHeight; ++y ) {
			for ( int x = 0; x < destWidth; ++x ) {
				
				float r = 0.0f;
				float g = 0.0f;
				float b = 0.0f;
				float a = 0.0f;
				
				float xSample = xSrcOrigin + (( float )x * xSrcStep );
				float ySample = ySrcOrigin + (( float )y * ySrcStep );
				
				for ( int ys = 0; ys < ySrcSampleSteps; ++ys ) {
					for ( int xs = 0; xs < xSrcSampleSteps; ++xs ) {
					
						u32 color = srcImage.SampleColor ( xSample + (( float )xs * xSampleStep ), ySample + (( float )ys * ySampleStep ), filter );
						
						ZLColorVec vec ( color );
						
						r += vec.mR;
						g += vec.mG;
						b += vec.mB;
						a += vec.mA;
					}
				}
				
				r *= sampleAverage;
				g *= sampleAverage;
				b *= sampleAverage;
				a *= sampleAverage;
				
				this->SetColor ( destRect.mXMin + x, destRect.mYMin + y, ZLColor::PackRGBA ( r, g, b, a ), blendMode );
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLImage::Desaturate ( const ZLImage& image, float rY, float gY, float bY, float K ) {

	if ( this != &image ) {
		this->Copy ( image );
	}

	if ( this->mPixelFormat == TRUECOLOR ) {
		for ( u32 y = 0; y < this->mHeight; y++ ) {
			ZLColor::Desaturate ( this->GetRowAddrMutable ( y ), this->mColorFormat, this->mWidth, rY, gY, bY, K );
		}
	}
	else {
		ZLColor::Desaturate ( this->mPalette.Invalidate (), this->mColorFormat, this->GetPaletteCount (), rY, gY, bY, K );
	}
}

//----------------------------------------------------------------//
void ZLImage::DrawLine(int p1x, int p1y, int p2x, int p2y, u32 color)
{
    int F, x, y;
	
	// Swap points if p1 is on the right of p2
    if ( p1x > p2x ) {
        std::swap ( p1x, p2x );
        std::swap ( p1y, p2y );
    }
	
    // Handle trivial cases separately for algorithm speed up.
    // Trivial case 1: m = +/-INF (Vertical line)
    if ( p1x == p2x ) {
	
		// Swap y-coordinates if p1 is above p2
        if (p1y > p2y) {
            std::swap ( p1y, p2y );
        }
		
        x = p1x;
        y = p1y;
        while ( y <= p2y ) {
            this->SetColor ( x, y, color );
            y++;
        }
        return;
    }
	
    // Trivial case 2: m = 0 (Horizontal line)
    else if ( p1y == p2y ) {
        x = p1x;
        y = p1y;
		
        while ( x <= p2x ) {
            this->SetColor ( x, y, color );
            x++;
        }
        return;
    }
	
    int dy				= p2y - p1y;  // y-increment from p1 to p2
    int dx				= p2x - p1x;  // x-increment from p1 to p2
    int dy2				= ( dy << 1 );  // dy << 1 == 2*dy
    int dx2				= ( dx << 1 );
    int dy2_minus_dx2	= dy2 - dx2;  // precompute constant for speed up
    int dy2_plus_dx2	= dy2 + dx2;
	
	// m >= 0
    if ( dy >= 0 ) {
	
        // Case 1: 0 <= m <= 1 (Original case)
        if ( dy <= dx ) {
            F = dy2 - dx;    // initial F
			
            x = p1x;
            y = p1y;
            while ( x <= p2x ) {
			
                this->SetColor ( x, y, color );
                if ( F <= 0 ) {
                    F += dy2;
                }
                else {
                    y++;
                    F += dy2_minus_dx2;
                }
                x++;
            }
        }
        else { // Case 2: 1 < m < INF (Mirror about y=x line replace all dy by dx and dx by dy)
			
            F = dx2 - dy;    // initial F
			
            y = p1y;
            x = p1x;
            while ( y <= p2y ) {
			
                this->SetColor ( x, y, color );
				
                if ( F <= 0 ) {
                    F += dx2;
                }
                else {
                    x++;
                    F -= dy2_minus_dx2;
                }
                y++;
            }
        }
    }
    else { // m < 0
	
        // Case 3: -1 <= m < 0 (Mirror about x-axis, replace all dy by -dy)
        if ( dx >= -dy ) {
            F = -dy2 - dx;    // initial F
			
            x = p1x;
            y = p1y;
            while ( x <= p2x ) {
                this->SetColor ( x, y, color );
                if ( F <= 0 ) {
                    F -= dy2;
                }
                else {
                    y--;
                    F -= dy2_plus_dx2;
                }
                x++;
            }
        }
        else { // Case 4: -INF < m < -1 (Mirror about x-axis and mirror about y=x line, replace all dx by -dy and dy by dx)
			
            F = dx2 + dy;    // initial F
			
            y = p1y;
            x = p1x;
            while ( y >= p2y ) {
                this->SetColor ( x, y, color );
                if ( F <= 0 ) {
                    F += dx2;
                }
                else {
                    x++;
                    F += dy2_plus_dx2;
                }
                y--;
            }
        }
    }
}

//----------------------------------------------------------------//
// TODO: should fill ellipse (not just circle)
void ZLImage::FillCircle ( float centerX, float centerY, float xRad, u32 color ) {
	
	int radius = ( int )xRad;
	int x0 = ( int )centerX;
	int y0 = ( int )centerY;
	
	int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
	
	this->DrawLine ( x0, y0 + radius, x0, y0 - radius, color );
    this->DrawLine ( x0 + radius, y0, x0 - radius, y0, color );
	
    while ( x < y ) {
    
		//ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if ( f >= 0 ) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		
		this->DrawLine ( x0 - x, y0 + y, x0 + x, y0 + y, color );
		this->DrawLine ( x0 - x, y0 - y, x0 + x, y0 - y, color );
		this->DrawLine ( x0 + y, y0 + x, x0 - y, y0 + x, color );
		this->DrawLine ( x0 + y, y0 - x, x0 - y, y0 - x, color );
    }
}

//----------------------------------------------------------------//
void ZLImage::FillEllipse ( int centerX, int centerY, int xRad, int yRad, u32 color ) {
	
	int x0 = centerX;
	int y0 = centerY;
	s64 err = 0;
	s64 aa2 = 2 * xRad * xRad;
	s64 bb2 = 2 * yRad * yRad;
	s64 x = xRad;
	s64 y = 0;
	s64 stopX = bb2 * xRad;
	s64 stopY = 0;
	s64 dx = yRad * yRad * ( 1 - 2 * xRad );
	s64 dy = xRad * xRad;
	
	while ( stopX >= stopY ) {

		this->DrawLine (( int )( x0 - x ), ( int )( y0 + y ), ( int )( x0 + x ), ( int )( y0 + y ), color );
		this->DrawLine (( int )( x0 - x ), ( int )( y0 - y ), ( int )( x0 + x ), ( int )( y0 - y ), color );
		
		y++;
		stopY += aa2;
		err += dy;
		dy += aa2;
		
		if ( 2 * err + dx > 0 ) {
			x--;
			stopX -= bb2;
			err += dx;
			dx += bb2;
		}
	}
	
	x = 0;
	y = yRad;
	dx = yRad * yRad;
	dy = xRad * xRad * ( 1 - 2 * yRad );
	err = 0;
	stopX = 0;
	stopY = aa2 * yRad;
	while ( stopX <= stopY ) {
		
		this->DrawLine (( int )( x0 - x ), ( int )( y0 + y ), ( int )( x0 + x ), ( int )( y0 + y ), color );
		this->DrawLine (( int )( x0 - x ), ( int )( y0 - y ), ( int )( x0 + x ), ( int )( y0 - y ), color );
		x++;
		stopX += bb2;
		err += dx;
		dx += bb2;
		if ( 2 * err + dy > 0 ) {
			y--;
			stopY -= aa2;
			err += dy;
			dy += aa2;
		}
	}
}

//----------------------------------------------------------------//
void ZLImage::FillRect ( ZLIntRect rect, u32 color ) {

	if ( !color ) {
		this->ClearRect ( rect );
		return;
	}

	rect.Bless ();
	
	this->GetFrame ().Clip ( rect );
	
	if ( !rect.Width ()) return;
	if ( !rect.Height ()) return;
	
	for ( int y = rect.mYMin; y < rect.mYMax; ++y ) {
		this->DrawLine ( rect.mXMin, y, rect.mXMax, y, color );
	}
}

//----------------------------------------------------------------//
void ZLImage::GammaCorrection ( const ZLImage& image, float gamma ) {

	if ( this != &image ) {
		this->Copy ( image );
	}

	if ( this->mPixelFormat == TRUECOLOR ) {
		for ( u32 y = 0; y < this->mHeight; y++ ) {
			ZLColor::GammaCorrection ( this->GetRowAddrMutable ( y ), this->mColorFormat, this->mWidth, gamma );
		}
	}
	else {
		ZLColor::GammaCorrection ( this->mPalette.Invalidate (), this->mColorFormat, this->GetPaletteCount (), gamma );
	}
}

//----------------------------------------------------------------//
void ZLImage::GenerateOutlineFromSDF ( ZLIntRect rect, float distMin, float distMax, float r, float g, float b, float a ) {

	int width = rect.Width () + 1;
	int height = rect.Height () + 1;
	
	for ( int y = 0; y < height; ++y ) {
		for ( int x = 0; x < width; ++x ) {
		
			u32 color = this->GetColor ( x + rect.mXMin, y + rect.mYMin );
			ZLColorVec colorVec;
			colorVec.SetRGBA ( color );
			
			if ( colorVec.mA > distMin && colorVec.mA <= distMax ) {
				colorVec.mR = r;
				colorVec.mG = g;
				colorVec.mB = b;
				colorVec.mA = a;
			}
			else {
				colorVec.mR = 0;
				colorVec.mG = 0;
				colorVec.mB = 0;
				colorVec.mA = 0;
			}
			
			this->SetColor ( x + rect.mXMin, y + rect.mYMin, colorVec.PackRGBA () );
		}
	}
}

//----------------------------------------------------------------//
void ZLImage::GenerateSDF ( ZLIntRect rect ) {
	
	// Plus one because rect goes to exact end
	int width = rect.Width () + 1;
	int height = rect.Height () + 1;
	
	ZLIntVec2D** grid1 = new ZLIntVec2D* [ height ];
	ZLIntVec2D** grid2 = new ZLIntVec2D* [ height ];
	int** gridDistance = new int* [ height ];
	
	for ( int i = 0; i < height; ++i ) {
		grid1 [ i ] = new ZLIntVec2D [ width ];
		grid2 [ i ] = new ZLIntVec2D [ width ];
		gridDistance [ i ] = new int [ width ];
	}
	
	ZLIntVec2D inside ( 0, 0 );
	ZLIntVec2D empty ( 9999, 9999 );
	
	// Set up the initial grid
	for ( int y = 0; y < height; ++y ) {
		for ( int x = 0; x < width; ++x ) {
			
			ZLColorVec colorVec;
			
			u32 color = this->GetColor ( x + rect.mXMin, y + rect.mYMin );
			colorVec.SetRGBA ( color );
			
			// Points inside get marked with a dx/dy of zero.
			// Points outside get marked with an infinitely large distance.
			if ( colorVec.mA == 0.0f ) {
				grid1 [ y ][ x ] = inside;
				grid2 [ y ][ x ] = empty;
			}
			else {
				grid2 [ y ][ x ] = inside;
				grid1 [ y ][ x ] = empty;
			}
		}
	}
	
	CalculateSDF ( grid1, width, height );
	CalculateSDF ( grid2, width, height );
		
	int maxDist = INT_MIN;
	int minDist = INT_MAX;
	
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			// Calculate the actual distance from the dx/dy
			int dist1 = ( int )( grid1 [ y ][ x ].Length ());
			int dist2 = ( int )( grid2 [ y ][ x ].Length ());
			int dist = dist1 - dist2;
			gridDistance [ y ][ x ] = dist;

			if ( minDist > dist ) {
				minDist = dist;
			}
			
			if ( maxDist < dist ) {
				maxDist = dist;
			}
		}
	}
	
	// Hard coded spread factor for testing, need to pass in!
	int range = 60;

	// Have to scale the distance value from minDis - maxDis to 0 - 1
	for( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			
			float scaledDistVal = ( float )gridDistance [ y ][ x ];
			scaledDistVal = ( scaledDistVal + 30 ) / range;
			ZLColorVec colorVec;
			colorVec.Set ( 0, 0, 0, scaledDistVal );
			this->SetColor ( x + rect.mXMin, y + rect.mYMin, colorVec.PackRGBA ());

		}
	}
	
	
	for ( int i = 0; i < height; i++ ) {
  		delete [] grid1 [ i ];
		delete [] grid2 [ i ];
		delete [] gridDistance [ i ];
  	}
	
	delete [] grid1;
	delete [] grid2;
	delete [] gridDistance;
}

//----------------------------------------------------------------//
void ZLImage::GenerateSDFAA ( ZLIntRect rect, float sizeInPixels ) {
	
	u32 width = ( u32 )rect.Width ();
	u32 height = ( u32 )rect.Height ();
	
	short* xdist	= ( short* ) malloc ( width * height * sizeof ( short ));
	short* ydist	= ( short* ) malloc ( width * height * sizeof ( short ));
	double* gx		= ( double* ) calloc ( width * height, sizeof ( double ));
	double* gy		= ( double* ) calloc ( width * height, sizeof ( double ));
	double* data	= ( double* ) calloc ( width * height, sizeof ( double ));
	double* outside	= ( double* ) calloc ( width * height, sizeof ( double ));
	double* inside	= ( double* ) calloc ( width * height, sizeof ( double ));
	
	// Convert img into double (data)
	for ( u32 y = 0; y < height; ++y ) {
		for ( u32 x = 0; x < width; ++x ) {
			
			u32 color = this->GetColor ( x + rect.mXMin, y + rect.mYMin );
			ZLColorVec colorVec;
			colorVec.SetRGBA ( color );
			double v = colorVec.mA;
			data [ y * width + x ] = v;
		}
	}
	
	// Compute outside = edtaa3(bitmap); % Transform background (0's)
	moai_computegradient ( data, width, height, gx, gy );
	moai_edtaa3 ( data, gx, gy, width, height, xdist, ydist, outside );
	for ( u32 i = 0; i < width * height; ++i ) {
		if ( outside [ i ] < 0 ) {
			outside [ i ] = 0.0;
		}
	}
	
	// Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
	memset ( gx, 0, sizeof ( double ) * width * height );
	memset ( gy, 0, sizeof ( double ) * width * height );
	for ( u32 i = 0; i < width * height; ++i ) {
		data [ i ] = 1.0 - data [ i ];
	}
	moai_computegradient ( data, width, height, gx, gy );
	moai_edtaa3 ( data, gx, gy, width, height, xdist, ydist, inside );
	for ( u32 i = 0; i < width * height; ++i ) {
		if ( inside [ i ] < 0 ) {
			inside [ i ] = 0.0;
		}
	}
	
	for ( u32 y = 0; y < height; ++y ) {
		for ( u32 x = 0; x < width; ++x ) {
			
			u32 i = y * width + x;
			
			double dist = outside [ i ] - inside [ i ]; // distance in pixels
			
			dist = ( 1.0 - ( dist / sizeInPixels )) * 0.5; // normalize
			dist = MAX ( 0.0, MIN ( dist, 1.0 )); // clamp
			
			ZLColorVec colorVec;
			colorVec.Set ( 0, 0, 0, ( float )dist );
			this->SetColor ( x + rect.mXMin, y + rect.mYMin, colorVec.PackRGBA ());
		}
	}
	
	free ( xdist );
	free ( ydist );
	free ( gx );
	free ( gy );
	free ( data );
	free ( outside );
	free ( inside );
}

//----------------------------------------------------------------//
void ZLImage::GenerateSDFDeadReckoning ( ZLIntRect rect, int threshold ) {
	
	// Specified in the paper
	// d1 is horizontal pixel distance, d2 is diagonal pixel distance
	const int d1 = 3;
	const int d2 = 4;
	
	// Plus one because rect goes to exact end
	int width = rect.Width () + 1;
	int height = rect.Height () + 1;
	int size = height * width;
	
	// Treating 1d array as 2d
	float* distanceMap = ( float * ) malloc ( sizeof ( float ) * size );

	if ( distanceMap == NULL ) {
		printf("ERROR: Out of memory\n");
    	return;
	} 

	// Treating 1d array as 2d
	int* binaryMap = ( int * ) malloc ( sizeof ( int ) * size );

	if ( binaryMap == NULL ) {
		free ( distanceMap );
		printf("ERROR: Out of memory\n");
    	return;
	}

	// Init the binary map and distance map
	for ( int y = 0; y < height; ++y ) {
		for ( int x = 0; x < width; ++x ) {
			
			u32 color = this->GetColor ( x + rect.mXMin, y + rect.mYMin );
			ZLColorVec colorVec;
			colorVec.SetRGBA ( color );
			//printf("color: %f, %f, %f, %f\n", colorVec.mR, colorVec.mG, colorVec.mB, colorVec.mA);
			if ( colorVec.mA > 0.5f ) {
				binaryMap [ y * width + x ] = 1;
			}
			else {
				binaryMap [ y * width + x ] = 0;
			}
			
			distanceMap [ y * width + x ] = FLT_MAX;
		}
	}
	
	// Looking for the edge
	for ( int y = 1; y < height - 1; ++y ) {
		for ( int x = 1; x < width - 1; ++x ) {
			
			int currentVal = binaryMap [ y * width + x ];
			if ( binaryMap [ y * width + ( x - 1 )] != currentVal ||
				binaryMap [ y * width + ( x + 1 )] != currentVal ||
				binaryMap [( y - 1 ) * width + x ] != currentVal ||
				binaryMap [( y + 1 ) * width + x ] != currentVal ) {
				distanceMap [ y * width + x ] = 0;
			}
		}
	}
	
	// Perform first pass
	for ( int y = 1; y < height - 1; ++y ) {
		for (int x = 1; x < width - 1; ++x ) {
			
			float *pCurDistVal = &distanceMap[y * width + x];
			
			if ( distanceMap [( y - 1) * width + ( x - 1 )] + d2 < *pCurDistVal )
				*pCurDistVal = distanceMap [( y - 1 ) * width + ( x - 1 )] + d2;
			
			if ( distanceMap [( y - 1) * width + x ] + d1 < *pCurDistVal )
				*pCurDistVal = distanceMap [( y - 1 ) * width + x ] + d1;
			
			if ( distanceMap [( y - 1) * width + (x + 1)] + d2 < *pCurDistVal )
				*pCurDistVal = distanceMap[(y - 1) * width + ( x + 1 )] + d2;
			
			if ( distanceMap [ y * width + ( x - 1 )] + d1 < *pCurDistVal )
				*pCurDistVal = distanceMap [ y * width + ( x - 1 )] + d1;
		}
	}
	
	// Perform second pass
	for ( int y = height - 2; y > 0; --y ) {
		for ( int x = width - 2; x > 0; --x ) {
			
			float *pCurDistVal = &distanceMap[y * width + x];
			
			if ( distanceMap [ y * width + ( x + 1 )] + d1 < *pCurDistVal ) {
				*pCurDistVal = distanceMap [ y * width + ( x + 1 )] + d1;
			}
			
			if ( distanceMap [( y + 1 ) * width + ( x - 1 )] + d2 < *pCurDistVal ) {
				*pCurDistVal = distanceMap [( y + 1 ) * width + ( x - 1 )] + d2;
			}
			
			if ( distanceMap [( y + 1) * width + x ] + d1 < *pCurDistVal ) {
				*pCurDistVal = distanceMap [( y + 1 ) * width + x ] + d1;
			}
			
			if ( distanceMap [( y + 1 ) * width + ( x + 1 )] + d2 < *pCurDistVal ) {
				*pCurDistVal = distanceMap[( y + 1 ) * width + ( x + 1 )] + d2;
			}
		}
	}
	
	// Indicate inside and outside
	for ( int y = height - 1; y > 0; --y ) {
		for ( int x = width - 1; x > 0; --x ) {
			
			if ( binaryMap [ y * width + x ] == 0 ) {
				distanceMap [ y * width + x ] *= -1;
			}
		}
	}
	
	// Hard coded spread factor for testing, need to pass in!
	int half = threshold / 2;
	
	// Have to scale the distance value from minDis - maxDis to 0 - 1
	for( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			
			float scaledDistVal = distanceMap [ y * width + x ];
			scaledDistVal = ( scaledDistVal + half ) / threshold;
			
			// If distance is more than the max threshold specified, snap to 0
			if ( scaledDistVal < 0 ) {
				scaledDistVal = 0;
			}
			
			ZLColorVec colorVec;
			colorVec.Set ( 0, 0, 0, scaledDistVal );
			this->SetColor ( x + rect.mXMin, y + rect.mYMin, colorVec.PackRGBA ());
			
		}
	}
	
	free ( binaryMap );
	free ( distanceMap );
}

//----------------------------------------------------------------//
size_t ZLImage::GetBitmapSize () const {

	return this->GetRowSize () * ( size_t )this->mHeight;
}

//----------------------------------------------------------------//
u32 ZLImage::GetColor ( u32 x, u32 y ) const {

	if ( !this->mBitmap.GetSize ()) return 0;

	if ( this->mPixelFormat == TRUECOLOR ) {
		return ZLColor::ConvertToRGBA ( this->GetPixel ( x, y ), this->mColorFormat );
	}
	return this->GetPaletteColor ( this->GetPixel ( x, y ));
}

//----------------------------------------------------------------//
ZLIntRect ZLImage::GetContentRect () {

	// TODO: this is crazy inefficient
	// TODO: one iteration through all pixels should be sufficient
	// TODO: should just check alpha bitmask from GetColor ()

	static const u32 NO_CROP = ( u32 )-1;
	u32 cropLeft = NO_CROP;
	for ( u32 x = 0; x < this->mWidth; x++ ) {
		for ( u32 y = 0; y < this->mHeight; y++ ) {
			ZLColorVec color;
			color.SetRGBA ( this->GetColor ( x, y ));
			if ( color.mA > 0 ) {
				cropLeft = x;
				break;
			}
		}
		if ( cropLeft != NO_CROP )
			break;
	}
	
	if ( cropLeft == NO_CROP ) {
		// completely empty image!
		return ZLIntRect::EMPTY;
	}

	u32 cropRight = NO_CROP;
	for ( u32 x = this->mWidth - 1; x >= 0; x-- ) {
		for ( u32 y = 0; y < this->mHeight; y++ ) {
			ZLColorVec color;
			color.SetRGBA ( this->GetColor ( x, y ));
			if ( color.mA > 0 ) {
				cropRight = x + 1;
				break;
			}
		}
		if ( cropRight != NO_CROP )
			break;
	}

	u32 cropTop = NO_CROP;
	for ( u32 y = 0; y < this->mHeight; y++ ) {
		for ( u32 x = 0; x < this->mWidth; x++ ) {
			ZLColorVec color;
			color.SetRGBA ( this->GetColor ( x, y ));
			if ( color.mA > 0 ) {
				cropTop = y;
				break;
			}
		}
		if ( cropTop != NO_CROP )
			break;
	}

	u32 cropBottom = NO_CROP;
	for ( u32 y = this->mHeight - 1; y >= 0; y-- ) {
		for ( u32 x = 0; x < this->mWidth; x++ ) {
			ZLColorVec color;
			color.SetRGBA ( this->GetColor ( x, y ));
			if ( color.mA > 0 ) {
				cropBottom = y+1;
				break;
			}
		}
		if ( cropBottom != NO_CROP )
			break;
	}

	return ZLIntRect ( cropLeft, cropTop, cropRight, cropBottom );
}

//----------------------------------------------------------------//
size_t ZLImage::GetDataSize () const {

	return this->GetPaletteSize () + this->GetBitmapSize ();
}

//----------------------------------------------------------------//
ZLIntRect ZLImage::GetFrame () {

	return ZLIntRect ( 0, 0, this->mWidth, this->mHeight );
}

//----------------------------------------------------------------//
u32 ZLImage::GetMinPowerOfTwo ( u32 size ) {

	if ( ZLImage::IsPow2 ( size )) return size;

	u32 pow2 = 1;
	while ( pow2 < size ) pow2 = pow2 << 0x01;
	return pow2;
}

//----------------------------------------------------------------//
u32 ZLImage::GetPaletteColor ( u32 i ) const {

	u32 total = this->GetPaletteCount ();
	if ( i < total ) {
		u32 pixel = ZLBitBuffer::GetValue ( this->mPalette.GetConstBuffer (), i, ZLColor::GetDepthInBits ( this->mColorFormat ));
		return ZLColor::ConvertToRGBA ( pixel, this->mColorFormat );
	}
	return 0;
}

//----------------------------------------------------------------//
u32 ZLImage::GetPaletteCount () const {

	switch ( this->mPixelFormat ) {
		case TRUECOLOR:		return 0;
		case INDEX_4:		return 16;
		case INDEX_8:		return 256;
		default:			break;
	}
	return 0;
}

//----------------------------------------------------------------//
size_t ZLImage::GetPaletteSize () const {
	
	return ZLBitBuffer::CalculateSize ( ZLColor::GetDepthInBits ( this->mColorFormat ), this->GetPaletteCount ());
}

//----------------------------------------------------------------//
u32 ZLImage::GetPixel ( u32 x, u32 y ) const {
	
	if (( x >= this->mWidth ) || ( y >= this->mHeight )) return 0;
	
	return ZLBitBuffer::GetValue ( this->GetRowAddr ( y ), x, this->GetPixelDepthInBits ());
}

//----------------------------------------------------------------//
u32 ZLImage::GetPixelDepthInBits () const {

	switch ( this->mPixelFormat ) {
		case TRUECOLOR:		return ZLColor::GetDepthInBits ( this->mColorFormat );
		case INDEX_4:		return 4;
		case INDEX_8:		return 8;
		default:			break;
	}
	return 0;
}

//----------------------------------------------------------------//
ZLIntRect ZLImage::GetRect () {

	return ZLIntRect ( 0, 0, this->mWidth, this->mHeight );
}

//----------------------------------------------------------------//
const void* ZLImage::GetRowAddr ( u32 y ) const {

	return ( const void* )(( size_t )this->mBitmap.GetConstBuffer () + ( this->GetRowSize () * y ));
}

//----------------------------------------------------------------//
void* ZLImage::GetRowAddrMutable ( u32 y ) {

	return ( void* )(( size_t )this->mBitmap.Invalidate () + ( this->GetRowSize () * y ));
}

//----------------------------------------------------------------//
size_t ZLImage::GetRowSize () const {

	return ZLBitBuffer::CalculateSize ( this->GetPixelDepthInBits (), this->mWidth );
}

//----------------------------------------------------------------//
void ZLImage::GetSubImage ( const ZLImage& image, ZLIntRect rect ) {

	int width = rect.Width ();
	int height = rect.Height ();
	
	this->Init ( width, height, image.mColorFormat, image.mPixelFormat );
	this->Blit ( image, rect.mXMin, rect.mYMin, 0, 0, width, height );
}

//----------------------------------------------------------------//
void ZLImage::Init ( const ZLImage& image ) {

	this->Init ( image.mWidth, image.mHeight, image.mColorFormat, image.mPixelFormat );
}

//----------------------------------------------------------------//
void ZLImage::Init ( u32 width, u32 height, ZLColor::ColorFormat colorFmt, PixelFormat pixelFmt ) {

	this->Clear ();

	this->mColorFormat = colorFmt;
	this->mPixelFormat = pixelFmt;

	this->mWidth = width;
	this->mHeight = height;
	
	this->Alloc ();
	this->ClearBitmap ();
	
	this->ZLImage_OnImageStatusChanged ( this->IsOK ());
}

//----------------------------------------------------------------//
void ZLImage::Init ( const void* bitmap, u32 width, u32 height, ZLColor::ColorFormat colorFmt ) {

	this->Clear ();

	if ( !bitmap ) return;

	this->mPixelFormat = TRUECOLOR;
	this->mColorFormat = colorFmt;

	this->mWidth = width;
	this->mHeight = height;
	
	this->Alloc ();
	size_t size = this->GetBitmapSize ();
	memcpy ( this->mBitmap.Invalidate (), bitmap, size );
	
	this->ZLImage_OnImageStatusChanged ( this->IsOK ());
}

//----------------------------------------------------------------//
bool ZLImage::IsPow2 () {

	return ( ZLImage::IsPow2 ( this->mWidth ) && ZLImage::IsPow2 ( this->mHeight ));
}

//----------------------------------------------------------------//
bool ZLImage::IsPow2 ( u32 n ) {

	return (( n == 1 ) || (( n & ( n - 1 )) == 0 ));
}

//----------------------------------------------------------------//
bool ZLImage::IsOK () const {

	return ( this->mBitmap.GetSize () != 0 );
}

//----------------------------------------------------------------//
bool ZLImage::Load ( cc8* filename, u32 transform ) {

	this->Clear ();
	
	ZLFileStream stream;
	if ( stream.OpenRead ( filename )) {
		this->ZLImage::Load ( stream, transform ); // TODO: use file extension as name
		stream.Close ();
		this->NotifyStatusChanged ();
	}
	return this->IsOK ();
}

//----------------------------------------------------------------//
bool ZLImage::Load ( ZLStream& stream, u32 transform ) {
	UNUSED ( stream );
	UNUSED ( transform );

	this->Clear ();
	
	ZLImageFormat* format = ZLImageFormatMgr::Get ().FindFormat ( stream ); // TODO: make use of name
	if ( format ) {
		format->ReadImage ( *this, stream, transform );
		this->NotifyStatusChanged ();
		return this->IsOK ();
	}
	
	return false;
}

//----------------------------------------------------------------//
bool ZLImage::MipReduce () {

	if (( this->mWidth == 0 ) || ( this->mHeight == 0 )) return false;
	if (( this->mWidth <= 1 ) && ( this->mHeight <= 1 )) return false;

	u32 width = this->mWidth & ~1;
	u32 height = this->mHeight & ~1;

	ZLImage nextMip;

	if ( width == 0 ) {
	
		nextMip.Init ( 1, height >> 1, this->mColorFormat, this->mPixelFormat );
		
		for ( u32 y = 0; y < height; y += 2 ) {
			
			u32 c0 = this->GetColor ( 0, y );
			u32 c1 = this->GetColor ( 0, y + 1 );

			u32 result = ZLColor::Average ( c0, c1 );
			nextMip.SetColor ( 0, y >> 1, result );
		}
	}
	else if ( height == 0 ) {
	
		nextMip.Init ( width >> 1, 1, this->mColorFormat, this->mPixelFormat );
	
		for ( u32 x = 0; x < width; x += 2 ) {
			
			u32 c0 = this->GetColor ( x, 0 );
			u32 c1 = this->GetColor ( x + 1, 0 );

			u32 result = ZLColor::Average ( c0, c1 );
			nextMip.SetColor ( x >> 1, 0, result );
		}
	}
	else {
		
		nextMip.Init ( width >> 1, height >> 1, this->mColorFormat, this->mPixelFormat );

		for ( u32 y0 = 0; y0 < height; y0 += 2 ) {
			for ( u32 x0 = 0; x0 < width; x0 += 2 ) {
				
				u32 x1 = x0 + 1;
				u32 y1 = y0 + 1;
				
				u32 c0 = this->GetColor ( x0, y0 );
				u32 c1 = this->GetColor ( x1, y0 );
				u32 c2 = this->GetColor ( x0, y1 );
				u32 c3 = this->GetColor ( x1, y1 );

				u32 result = ZLColor::Average ( c0, c1, c2, c3 );
				nextMip.SetColor ( x0 >> 1, y0 >> 1, result );
			}
		}
	}
	this->Take ( nextMip );
	return true;
}

//----------------------------------------------------------------//
void ZLImage::Mix ( const ZLImage& image, const ZLMatrix4x4& mtx, float K ) {
	
	if ( this != &image ) {
		this->Copy ( image );
	}

	if ( this->mPixelFormat == TRUECOLOR ) {
		for ( u32 y = 0; y < this->mHeight; y++ ) {
			ZLColor::Mix ( this->GetRowAddrMutable ( y ), this->mColorFormat, this->mWidth, mtx, K );
		}
	}
	else {
		ZLColor::Mix ( this->mPalette.Invalidate (), this->mColorFormat, this->GetPaletteCount (), mtx, K );
	}
}

//----------------------------------------------------------------//
void ZLImage::NotifyStatusChanged () {

	this->ZLImage_OnImageStatusChanged ( this->IsOK ());
}

//----------------------------------------------------------------//
void ZLImage::PadToPow2 ( const ZLImage& image ) {

	ZLIntRect canvas;
	canvas.mXMin = 0;
	canvas.mYMin = 0;
	canvas.mXMax = this->GetMinPowerOfTwo ( image.GetWidth ());
	canvas.mYMax = this->GetMinPowerOfTwo ( image.GetHeight ());
	
	this->ResizeCanvas ( image, canvas );
}

//----------------------------------------------------------------//
void ZLImage::PremultiplyAlpha ( const ZLImage& image ) {

	if ( this != &image ) {
		this->Copy ( image );
	}

	if ( this->mPixelFormat == TRUECOLOR ) {
		for ( u32 y = 0; y < this->mHeight; ++y ) {
			ZLColor::PremultiplyAlpha ( this->GetRowAddrMutable ( y ), this->mColorFormat, this->mWidth );
		}
	}
	else {
		ZLColor::PremultiplyAlpha ( this->mPalette.Invalidate (), this->mColorFormat, this->GetPaletteCount ());
	}
}

//----------------------------------------------------------------//
void ZLImage::Print () {

	for ( u32 y = 0; y < this->mHeight; ++y ) {
		for ( u32 x = 0; x < this->mWidth; ++x ) {
			printf ( "0x%08x, ", this->GetColor ( x, y ));
		}
		printf ( "\n" );
	}
}

//----------------------------------------------------------------//
void ZLImage::ResizeCanvas ( const ZLImage& image, ZLIntRect rect ) {

	// TODO: specify sides (a la photoshop)

	rect.Bless ();
	
	int width = rect.Width ();
	int height = rect.Height ();
	
	ZLImage newImage;
	newImage.Init (( u32 )width, ( u32 )height, image.mColorFormat, image.mPixelFormat );
	
	ZLIntRect srcRect;
	srcRect.mXMin = -rect.mXMin;
	srcRect.mYMin = -rect.mYMin;
	srcRect.mXMax = ( int )image.GetWidth () + srcRect.mXMin;
	srcRect.mYMax = ( int )image.GetHeight () + srcRect.mYMin;
	
	rect.Offset ( -rect.mXMin, -rect.mYMin );
	
	if ( srcRect.Overlap ( rect )) {
	
		u32 beginSpan = 0;
		u32 leftSize = 0;
		
		if ( srcRect.mXMin > 0 ) {
			beginSpan = srcRect.mXMin;
			leftSize = beginSpan;
		}
		
		u32 endSpan = width;
		u32 rightSize = 0;
		
		if ( srcRect.mXMax < width ) {
			endSpan = srcRect.mXMax;
			rightSize = width - endSpan;
		}
		
		u32 spanSize = endSpan - beginSpan;
		
		u32 bitDepth = newImage.GetPixelDepthInBits ();
		
		//u32 srcRowSize = image.GetRowSize ();
		u32 srcRowXOff = srcRect.mXMin < 0 ? -srcRect.mXMin : 0;
		
		for ( int y = 0; y < height; ++y ) {
		
			void* destRow = newImage.GetRowAddrMutable ( y );
		
			if (( y < srcRect.mYMin ) || ( y >= srcRect.mYMax )) {
			
				ZLBitBuffer::Clear ( destRow, bitDepth, 0, newImage.mWidth );
			}
			else {
			
				if ( leftSize ) {
					ZLBitBuffer::Clear ( destRow, bitDepth, 0, leftSize );
				}
				
				if ( spanSize ) {
					const void* srcRow = image.GetRowAddr ( y - srcRect.mYMin );
					ZLBitBuffer::Blit ( destRow, leftSize, srcRow, srcRowXOff, spanSize, bitDepth );
				}
				
				if ( rightSize ) {
					ZLBitBuffer::Clear ( destRow, bitDepth, leftSize + spanSize, rightSize );
				}
			}
		}
	}
	else {
		newImage.ClearBitmap ();
	}
	this->Take ( newImage );
}

//----------------------------------------------------------------//
u32 ZLImage::SampleColor ( float x, float y, u32 filter, bool wrapX, bool wrapY ) const {

	u32 x0 = ( u32 )floorf ( x );
	u32 y0 = ( u32 )floorf ( y );
	
	u32 x1 = x0 + 1;
	u32 y1 = y0 + 1;
	
	if ( x1 >= this->mWidth ) {
		x1 = wrapX ? x1 % this->mWidth : this->mWidth - 1;
	}
	
	if ( y1 >= this->mHeight ) {
		y1 = wrapY ? y1 % this->mHeight : this->mHeight - 1;
	}
	
	u32 c0 = this->GetColor ( x0, y0 );
	u32 c1 = this->GetColor ( x1, y0 );
	u32 c2 = this->GetColor ( x0, y1 );
	u32 c3 = this->GetColor ( x1, y1 );

	u8 xt = ( u8 )(( x - ( float )x0 ) * 255.0f );
	u8 yt = ( u8 )(( y - ( float )y0 ) * 255.0f );
	
	if ( filter == FILTER_LINEAR ) {
		return ZLColor::BilerpFixed ( c0, c1, c2, c3, xt, yt );
	}
	return ZLColor::NearestNeighbor ( c0, c1, c2, c3, xt, yt );
}

//----------------------------------------------------------------//
void ZLImage::SetColor ( u32 x, u32 y, u32 color ) {

	if ( this->mPixelFormat != TRUECOLOR ) return;

	if ( y > this->mHeight ) return;
	if ( x > this->mWidth ) return;

	u32 pixel = ZLColor::ConvertFromRGBA ( color, this->mColorFormat );
	this->SetPixel ( x, y, pixel );
}

//----------------------------------------------------------------//
void ZLImage::SetColor ( u32 x, u32 y, u32 color, const ZLColorBlendFunc& blendMode ) {

	if ( this->mPixelFormat != TRUECOLOR ) return;
	
	if ( y > this->mHeight ) return;
	if ( x > this->mWidth ) return;

	if ( blendMode.mEquation != ZLColor::BLEND_EQ_NONE ) {
		u32 dstColor = this->GetColor ( x, y );
		color = ZLColor::Blend ( color, dstColor, blendMode );
	}
	u32 pixel = ZLColor::ConvertFromRGBA ( color, this->mColorFormat );
	this->SetPixel ( x, y, pixel );
}

//----------------------------------------------------------------//
void ZLImage::SetPaletteColor ( u32 idx, u32 rgba ) {

	u32 total = this->GetPaletteCount ();
	if ( idx < total ) {
		
		u32 color = ZLColor::ConvertFromRGBA ( rgba, this->mColorFormat );
		u32 colorDepth = ZLColor::GetDepthInBits ( this->mColorFormat );
		
		ZLBitBuffer::SetValue ( this->mPalette.Invalidate (), color, idx, colorDepth );
	}
}

//----------------------------------------------------------------//
void ZLImage::SetPixel ( u32 x, u32 y, u32 pixel ) {

	if (( x >= this->mWidth ) || ( y >= this->mHeight )) return;
	
	ZLBitBuffer::SetValue ( this->GetRowAddrMutable ( y ), pixel, x, this->GetPixelDepthInBits ());
}

//----------------------------------------------------------------//
void ZLImage::SimpleThreshold ( const ZLImage& image, float rT, float gT, float bT, float aT ) {

	if ( this != &image ) {
		this->Copy ( image );
	}

	u32 threshold = ZLColor::ConvertFromRGBA ( ZLColor::PackRGBA ( rT, gT, bT, aT ), this->mColorFormat );
	
	if ( this->mPixelFormat == TRUECOLOR ) {
		for ( u32 y = 0; y < this->mHeight; ++y ) {
			ZLColor::SimpleThreshold ( this->GetRowAddrMutable ( y ), this->mColorFormat, this->mWidth, threshold );
		}
	}
	else {
		ZLColor::SimpleThreshold ( this->mPalette.Invalidate (), this->mColorFormat, this->GetPaletteCount (), threshold );
	}
}

//----------------------------------------------------------------//
void ZLImage::Take ( ZLImage& image ) {

	this->Clear ();

	this->mColorFormat	= image.mColorFormat;
	this->mPixelFormat	= image.mPixelFormat;

	this->mWidth		= image.mWidth;
	this->mHeight		= image.mHeight;
	
	this->mBitmap		= image.mBitmap;
	this->mPalette		= image.mPalette;

	this->ZLImage_OnImageStatusChanged ( true );

	// kill the data before clear
	image.mBitmap = 0;
	image.mPalette = 0;
	image.Clear ();
}

//----------------------------------------------------------------//
void ZLImage::Transform ( u32 transform ) {
	
	if ( !transform ) return;
	
	if ( transform & ZLImageTransform::PREMULTIPLY_ALPHA ) {
		this->PremultiplyAlpha ( *this );
	}
	
	ZLColor::ColorFormat colorFormat = this->mColorFormat;
	PixelFormat pixelFormat = this->mPixelFormat;
	
	if ( transform & ZLImageTransform::QUANTIZE ) {
	
		if ( this->mColorFormat == ZLColor::RGB_888 ) {
			colorFormat = ZLColor::RGB_565;
		}
		
		if ( this->mColorFormat == ZLColor::RGBA_8888 ) {
			colorFormat = ZLColor::RGBA_4444;
		}
	}
	
	if ( transform & ZLImageTransform::TRUECOLOR ) {
		pixelFormat = TRUECOLOR;
	}
	
	this->Convert ( *this, colorFormat, pixelFormat );
	
	if ( transform & ZLImageTransform::POW_TWO ) {
		this->PadToPow2 ( *this );
	}
}

//----------------------------------------------------------------//
bool ZLImage::Write ( ZLStream& stream, cc8* formatName ) const {

	ZLImageFormat* format = ZLImageFormatMgr::Get ().FindFormat ( formatName );
	return format && format->WriteImage ( *this, stream );
}


//----------------------------------------------------------------//
ZLImage::ZLImage () :
	mPixelFormat ( PXL_FMT_UNKNOWN ),
	mColorFormat ( ZLColor::CLR_FMT_UNKNOWN ),
	mWidth ( 0 ),
	mHeight ( 0 ) {
}

//----------------------------------------------------------------//
ZLImage::~ZLImage () {

	this->Clear ();
}

//================================================================//
// ZLImage
//================================================================//

//----------------------------------------------------------------//
void ZLImage::ZLImage_OnImageStatusChanged ( bool isOK ) {
	UNUSED ( isOK );
}
