// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-core/MOAIDataBuffer.h>
#include <moai-core/MOAIImage.h>
#include <moai-core/MOAIImageLoadTask.h>
#include <moai-core/MOAITaskQueue.h>
#include <moai-core/MOAITaskSubscriber.h>
#include <float.h>
#include <contrib/moai_edtaa3func.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	average
	@text	Calculates the average of each color channel.

	@in		MOAIImage self
	@out	number averageR
	@out	number averageG
	@out	number averageB
	@out	number averageA
*/
int MOAIImage::_average ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	ZLColorVec average = self->Average ();
	
	lua_pushnumber ( state, average.mR );
	lua_pushnumber ( state, average.mG );
	lua_pushnumber ( state, average.mB );
	lua_pushnumber ( state, average.mA );
	
	return 4;
}

//----------------------------------------------------------------//
/**	@lua	bleedRect
	@text	'Bleeds' the interior of the rectangle out by one pixel.

	@in		MOAIImage self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIImage::_bleedRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	ZLIntRect rect;
	
	rect.mXMin	= state.GetValue < int >( 2, 0 );
	rect.mYMin	= state.GetValue < int >( 3, 0 );
	rect.mXMax	= state.GetValue < int >( 4, 0 );
	rect.mYMax	= state.GetValue < int >( 5, 0 );
	
	self->BleedRect ( rect );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	calculateGaussianKernel
	@text	Calculate a one dimensional gaussian kernel suitable for blurring.
 	
	@in		MOAIImage   self
	@opt	number radius		Default valus is 1.0.
	@opt	number sigma		Default valie is radius / 3 (https://en.wikipedia.org/wiki/Gaussian_blur)
	@out	nil
*/
int MOAIImage::_calculateGaussianKernel ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "" )
	
	float radius	= state.GetValue < float >( 1, 1.0f );
	float sigma		= state.GetValue < float >( 2, ( radius / 3.0f ));
	
	if ( radius > 0.0f ) {
	
		int kernelWidth = ( int )MOAIImage::CalculateGaussianKernelWidth ( radius );
		float* kernel = ( float* )alloca ( kernelWidth * sizeof ( float ));
		
		MOAIImage::CalculateGaussianKernel ( radius, sigma, kernel, kernelWidth );

		lua_newtable ( state );
		for ( int i = 0; i < kernelWidth; ++i ) {
			state.SetField ( -1, i + 1, kernel [ i ]);
		}
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	compare
	@text	Compares the image to another image.
	
	@in		MOAIImage self
	@in		MOAIImage other
	@out	boolean areEqual	A value indicating whether the images are equal.
*/
int MOAIImage::_compare ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UU" )

	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2, true );

	lua_pushboolean ( state, self->Compare ( *image ));

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	convert
	@text	Return a copy of the image with a new color format. Not
			all provided formats are supported by OpenGL. 'nil' may be
			passed for either value, in which case the format will match
			the original.

	@in		MOAIImage self
	@opt	number colorFmt		One of MOAIImage.COLOR_FMT_A_1, MOAIImage.COLOR_FMT_A_4, MOAIImage.COLOR_FMT_A_8,
								MOAIImage.COLOR_FMT_RGB_888, MOAIImage.COLOR_FMT_RGB_565, MOAIImage.COLOR_FMT_RGBA_5551,
								MOAIImage.COLOR_FMT_RGBA_4444, COLOR_FMT_RGBA_8888
	@opt	number pixelFmt		One of MOAIImage.PIXEL_FMT_TRUECOLOR, MOAIImage.PIXEL_FMT_INDEX_4, MOAIImage.PIXEL_FMT_INDEX_8
	@out	MOAIImage image		Copy of the image initialized with given format.
*/
int MOAIImage::_convert ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	u32 colorFmt = state.GetValue < u32 >( 2, self->mColorFormat );
	u32 pixelFmt = state.GetValue < u32 >( 3, self->mPixelFormat );
	
	MOAIImage* image = new MOAIImage ( self->GetContext ());
	image->Convert ( *self, ( ZLColor::ColorFormat )colorFmt, ( PixelFormat )pixelFmt );
	image->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	convolve
	@text	Convolve the image using a one or two dimensional kernel. If a one-dimensional kernel is provided, the
			image will be convolved in two passes: first horizonally and then vertically using the same kernel.

	@in		MOAIImage self
	@in		table kernel		A one or two dimensional array of coefficients.
	@opt	boolean normalize	If true, the kernel will be normalized prior to the convolution. Default value is true.
	@out	MOAIImage image		The resulting image.
*/
int MOAIImage::_convolve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UT" )

	bool normalize = state.GetValue < bool >( 3, true );

	int kernelWidth = ( int )state.GetTableSize ( 2 );
	
	if ( kernelWidth ) {
	
		MOAIImage* image = new MOAIImage ( self->GetContext ());
	
		state.PushField ( 2, 1 );
	
		if ( state.IsType ( -1, LUA_TNUMBER )) {
		
			float* kernel = ( float* )alloca ( kernelWidth * sizeof ( float ));
			
			for ( int x = 0; x < kernelWidth; ++x ) {
				kernel [ x ] = state.GetFieldValue < int, float >( 2, x + 1, 0.0f );
			}
			
			if ( normalize ) {
				ZLFloat::Normalize ( kernel, kernelWidth );
			}
			image->Convolve ( *self, kernel, kernelWidth );
		}
		else if ( state.IsType ( -1, LUA_TTABLE )) {
		
			int kernelHeight = kernelWidth;
			kernelWidth = ( int )state.GetTableSize ( -1 );
			
			float* kernel = ( float* )alloca ( kernelWidth * kernelHeight * sizeof ( float ));
			
			for ( int y = 0; y < kernelHeight; ++y ) {
				
				state.PushField ( 2, y + 1 );
				for ( int x = 0; x < kernelWidth; ++x ) {
					kernel [( y * kernelWidth ) + x ] = state.GetFieldValue < int, float >( -1, x + 1, 0.0f );
				}
				state.Pop ();
			}
			
			if ( normalize ) {
				ZLFloat::Normalize ( kernel, kernelWidth * kernelHeight );
			}
			image->Convolve ( *self, kernel, kernelWidth, kernelHeight );
		}
	
		state.Pop ();
	
		image->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	convolve1D
	@text	Convolve the image using a one dimensional kernel.

	@in		MOAIImage self
	@in		table kernel		A one dimensional array of coefficients.
	@opt	boolean horizontal	If true, the image will be convolved horizontally. Otherwise the image will be convolved vertically. Devault value is true.
	@opt	boolean normalize	If true, the kernel will be normalized prior to the convolution. Default value is true.
	@out	MOAIImage image		The resulting image.
*/
int MOAIImage::_convolve1D ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UT" )

	bool horizontal		= state.GetValue < bool >( 3, true );
	bool normalize		= state.GetValue < bool >( 4, true );

	size_t kernelWidth = state.GetTableSize ( 2 );
	
	if ( kernelWidth ) {
	
		MOAIImage* image = new MOAIImage ( self->GetContext ());
	
		state.PushField ( 2, 1 );
	
		if ( state.IsType ( -1, LUA_TNUMBER )) {
		
			float* kernel = ( float* )alloca ( kernelWidth * sizeof ( float ));
			
			for ( size_t x = 0; x < kernelWidth; ++x ) {
				kernel [ x ] = state.GetFieldValue < int, float >( 2, ( int )( x + 1 ), 0.0f ); // TODO: cast
			}
			
			if ( normalize ) {
				ZLFloat::Normalize ( kernel, kernelWidth );
			}
			image->Convolve1D ( *self, kernel, kernelWidth, horizontal );
		}
	
		state.Pop ();
	
		image->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	copy
	@text	Copies an image.

	@in		MOAIImage self
	@out	MOAIImage image		Copy of the image.
*/
int MOAIImage::_copy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	MOAIImage* image = new MOAIImage ( self->GetContext ());
	image->Copy ( *self );
	image->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	copyBits
	@text	Copy a section of one image to another.

	@in		MOAIImage self
	@in		MOAIImage source	Source image.
	@in		number srcX			X location in source image.
	@in		number srcY			Y location in source image.
	@in		number destX		X location in destination image.
	@in		number destY		Y location in destination image.
	@in		number width		Width of section to copy.
	@in		number height		Height of section to copy.
	@out	nil
*/
int MOAIImage::_copyBits ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UUNNNNNN" )
	
	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2, true );
	if ( !image ) {
		return 0;
	}
	
	int srcX	= state.GetValue < int >( 3, 0 );
	int srcY	= state.GetValue < int >( 4, 0 );
	
	int destX	= state.GetValue < int >( 5, 0 );
	int destY	= state.GetValue < int >( 6, 0 );
	
	int width	= state.GetValue < int >( 7, 0 );
	int height	= state.GetValue < int >( 8, 0 );
	
	self->Blit ( *image, srcX, srcY, destX, destY, width, height );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	copyRect
	@text	Copy a section of one image to another. Accepts two rectangles.
			Rectangles may be of different size and proportion. Section of
			image may also be flipped horizontally or vertically by
			reversing min/max of either rectangle.

	@in		MOAIImage self
	@in		MOAIImage source	Source image.
	@in		number srcXMin
	@in		number srcYMin
	@in		number srcXMax
	@in		number srcYMax
	@in		number destXMin
	@in		number destYMin
	@opt	number destXMax		Default value is destXMin + srcXMax - srcXMin;
	@opt	number destYMax		Default value is destYMin + srcYMax - srcYMin;
	@opt	number filter		One of MOAIImage.FILTER_LINEAR, MOAIImage.FILTER_NEAREST.
								Default value is MOAIImage.FILTER_LINEAR.
	@opt	number srcFactor	Default value is BLEND_FACTOR_SRC_ALPHA
	@opt	number dstFactor	Default value is BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
	@opt	number equation		Default value is BLEND_EQ_ADD
	@out	nil
*/
int MOAIImage::_copyRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UUNNNNNN" )
	
	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2, true );
	if ( !image ) {
		return 0;
	}
	
	ZLIntRect srcRect;
	srcRect.mXMin = state.GetValue < int >( 3, 0 );
	srcRect.mYMin = state.GetValue < int >( 4, 0 );
	srcRect.mXMax = state.GetValue < int >( 5, 0 );
	srcRect.mYMax = state.GetValue < int >( 6, 0 );
	
	ZLIntRect destRect;
	destRect.mXMin = state.GetValue < int >( 7, 0 );
	destRect.mYMin = state.GetValue < int >( 8, 0 );
	destRect.mXMax = state.GetValue < int >( 9, destRect.mXMin + srcRect.Width ());
	destRect.mYMax = state.GetValue < int >( 10, destRect.mYMin + srcRect.Height ());
	
	u32 filter = state.GetValue < u32 >( 11, MOAIImage::FILTER_LINEAR );
	
	ZLColorBlendFunc blendMode;
	
	blendMode.mEquation		= ( ZLColor::BlendEquation )state.GetValue < u32 >( 12, ( u32 )ZLColor::BLEND_EQ_NONE );
	blendMode.mSourceFactor	= ( ZLColor::BlendFactor )state.GetValue < u32 >( 13, ( u32 )ZLColor::BLEND_FACTOR_ZERO );
	blendMode.mDestFactor	= ( ZLColor::BlendFactor )state.GetValue < u32 >( 14, ( u32 )ZLColor::BLEND_FACTOR_ZERO );

	self->CopyRect ( *image, srcRect, destRect, filter, blendMode );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	desaturate
	@text	Convert image to grayscale.

	@in		MOAIImage self
	@opt	rY
	@opt	gY
	@opt	bY
	@opt	K
	@out	nil
*/
int MOAIImage::_desaturate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	float rY	= state.GetValue < float >( 2, LUMA_709_R );
	float gY	= state.GetValue < float >( 3, LUMA_709_G );
	float bY	= state.GetValue < float >( 4, LUMA_709_B );
	float K		= state.GetValue < float >( 5, 1.0f );
	
	self->Desaturate ( *self, rY, gY, bY, K );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillCircle
	@text	Draw a filled circle.

	@in		number x
	@in		number y
	@in		number radius
	@opt	number r			Default value is 0.
	@opt	number g			Default value is 0.
	@opt	number b			Default value is 0.
	@opt	number a			Default value is 0.
	@out	nil
*/
int MOAIImage::_fillCircle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNN" )
	
	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float r		= state.GetValue < float >( 4, 0.0f );
	u32 color	= state.GetColor32 ( 5, 0.0f, 0.0f, 0.0f, 0.0f );
	
	self->FillCircle ( x0, y0, r, color );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillEllipse
	@text	Draw a filled ellipse.
 
	@in		number x
	@in		number y
	@in		number radiusX
	@in		number radiusY
	@opt	number r			Default value is 0.
	@opt	number g			Default value is 0.
	@opt	number b			Default value is 0.
	@opt	number a			Default value is 0.
	@out	nil
*/
int MOAIImage::_fillEllipse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNNN" )
	
	int x0	= state.GetValue < int >( 2, 0 );
	int y0	= state.GetValue < int >( 3, 0 );
	int rX	= state.GetValue < int >( 4, 0 );
	int rY	= state.GetValue < int >( 5, 0 );
	u32 color	= state.GetColor32 ( 6, 0.0f, 0.0f, 0.0f, 0.0f );
	
	self->FillEllipse ( x0, y0, rX, rY, color );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillRect
	@text	Fill a rectangle in the image with a solid color.

	@in		MOAIImage self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@opt	number r			Default value is 0.
	@opt	number g			Default value is 0.
	@opt	number b			Default value is 0.
	@opt	number a			Default value is 0.
	@out	nil
*/
int MOAIImage::_fillRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNNN" )

	ZLIntRect rect = state.GetRect < int >( 2 );
	u32 color = state.GetColor32 ( 6, 0.0f, 0.0f, 0.0f, 0.0f );

	self->FillRect ( rect, color );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	gammaCorrection
	@text	Apply gamma correction.

	@in		MOAIImage self
	@opt	number gamma			Default value is 1.
	@out	nil
*/
int MOAIImage::_gammaCorrection ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	float gamma = state.GetValue < float >( 2, 1 );
	
	self->GammaCorrection ( *self, gamma );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	generateOutlineFromSDF
	@text	Given a rect, and min and max distance values, transform
			to a binary image where 0 means not on the outline and
			1 means part of the outline

	@in		MOAIImage self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@opt	number distMin
	@opt	number distMax
	@opt	number r			Default value is 1.
	@opt	number g			Default value is 1.
	@opt	number b			Default value is 1.
	@opt	number a			Default value is 1.
	@out	nil
 */
int	MOAIImage::_generateOutlineFromSDF( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNNN" )
	
	ZLIntRect rect = state.GetRect < int >( 2 );
	float distMin = state.GetValue < float >( 6, 0.46f );
	float distMax = state.GetValue < float >( 7, 0.5f );
	float r		= state.GetValue < float >( 8, 1.0f );
	float g		= state.GetValue < float >( 9, 1.0f );
	float b		= state.GetValue < float >( 10, 1.0f );
	float a		= state.GetValue < float >( 11, 1.0f );
	
	self->GenerateOutlineFromSDF ( rect, distMin, distMax, r, g, b, a );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	generateSDF
	@text	Given a rect, creates a signed distance field from it
 
	@in		MOAIImage self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
 */
int MOAIImage::_generateSDF( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNNN" )
	
	ZLIntRect rect = state.GetRect < int >( 2 );
	
	self->GenerateSDF ( rect );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	generateSDFAA
	@text	Given a rect, creates a signed distance field (using alpha
			as a mask) taking into account antialiased edges. The size
			of the SDF (distance from edges) is specified in pixels.
			
			Resulting SDF is stored in the image's alpha channel.
			Distances are normalized to the given size, inverted and
			scaled so that 0.5 is at an edge with 1 at full interior
			and 0 at full exterior (edge plus size).
 
	@in		MOAIImage self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@opt	number sizeInPixels		Default is 5
	@out	nil
*/
int MOAIImage::_generateSDFAA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNNN" )
	
	ZLIntRect rect = state.GetRect <int>( 2 );
	float threshold = state.GetValue < float >( 6, 5 );
	
	self->GenerateSDFAA ( rect, threshold );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	generateSDFDeadReckoning
	@text	Given a rect, creates a signed distance field from it 
			using dead reckoning technique

	@in		MOAIImage self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@opt	number threshold	Default is 256
	@out	nil
*/
int MOAIImage::_generateSDFDeadReckoning( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNNN" )
	
	ZLIntRect rect = state.GetRect <int>( 2 );
	u32 threshold = state.GetValue < u32 >( 6, 256 );
	
	self->GenerateSDFDeadReckoning ( rect, threshold );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getColor32
	@text	Returns a 32-bit packed RGBA value from the image for a
			given pixel coordinate.

	@in		MOAIImage self
	@in		number x
	@in		number y
	@out	number color
*/
int MOAIImage::_getColor32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNN" )
	
	u32 x		= state.GetValue < u32 >( 2, 0 );
	u32 y		= state.GetValue < u32 >( 3, 0 );

	u32 color = self->GetColor ( x, y );
	lua_pushnumber ( state, color );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getContentRect
	@text	computes the content rect, not taking in account any boundary transparency
 
	@in		MOAIImage self
	@out	rect
 */
int MOAIImage::_getContentRect(lua_State *L) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	ZLIntRect contentRect = self->GetContentRect ();
	int left, right, top, bottom;
	contentRect.GetRect ( left, top, right, bottom );
	lua_pushnumber ( state,  left);
	lua_pushnumber ( state,	 top );
	lua_pushnumber ( state,  right);
	lua_pushnumber ( state,	 bottom );
	
	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getData
	@text	returns the bitmap data
 
	@in		MOAIImage self
	@out	byte array string
 */
int MOAIImage::_getData(lua_State *L) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )

	lua_pushlstring ( state, ( const char* )self->mBitmap.GetConstBuffer (), self->mBitmap.GetSize ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFormat
	@text	Returns the color format of the image.

	@in		MOAIImage self
	@out	number colorFormat
*/
int MOAIImage::_getFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	lua_pushnumber ( state, self->GetColorFormat ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getRGBA
	@text	Returns an RGBA color as four floating point values.

	@in		MOAIImage self
	@in		number x
	@in		number y
	@out	number r
	@out	number g
	@out	number b
	@out	number a
*/
int MOAIImage::_getRGBA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNN" )
	
	u32 x		= state.GetValue < u32 >( 2, 0 );
	u32 y		= state.GetValue < u32 >( 3, 0 );

	u32 color = self->GetColor ( x, y );
	
	ZLColorVec colorVec;
	colorVec.SetRGBA ( color );
	
	lua_pushnumber ( state, colorVec.mR );
	lua_pushnumber ( state, colorVec.mG );
	lua_pushnumber ( state, colorVec.mB );
	lua_pushnumber ( state, colorVec.mA );
	
	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getSize
	@text	Returns the width and height of the image.

	@in		MOAIImage self
	@opt	number scale
	@out	number width
	@out	number height
*/
int MOAIImage::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	u32 width		= self->GetWidth ();
	u32 height		= self->GetHeight ();

	float scale		= state.GetValue < float >( 2, 1.0f );

	lua_pushnumber ( state, ( float )width * scale );
	lua_pushnumber ( state, ( float )height * scale );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initializes the image with a width, height and color format.

	@in		MOAIImage self
	@in		number width
	@in		number height
	@opt	number colorFmt		One of MOAIImage.COLOR_FMT_A_1, MOAIImage.COLOR_FMT_A_4, MOAIImage.COLOR_FMT_A_8,
								MOAIImage.COLOR_FMT_RGB_888, MOAIImage.COLOR_FMT_RGB_565, MOAIImage.COLOR_FMT_RGBA_5551,
								MOAIImage.COLOR_FMT_RGBA_4444, COLOR_FMT_RGBA_8888.
								Default value is MOAIImage.COLOR_FMT_RGBA_8888.
	@out	nil
*/
int MOAIImage::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )

	MOAIImage* srcImage	= state.GetLuaObject < MOAIImage >( 2, false );
	if  ( srcImage ) {
	
		self->Init ( *srcImage );
	}
	else {

		u32 width		= state.GetValue < u32 >( 2, 0 );
		u32 height		= state.GetValue < u32 >( 3, width );
		u32 colorFmt	= state.GetValue < u32 >( 4, ZLColor::RGBA_8888 );

		self->Init ( width, height, ( ZLColor::ColorFormat )colorFmt, TRUECOLOR );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	isOpaque
	@text	false if at least one pixel is not opaque
 
	@in		MOAIImage self
	@out	bool
 */
int MOAIImage::_isOpaque(lua_State *L) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	// TODO: this is so inefficient
	
	for ( u32 y = 0; y < self->mHeight; y++ ) {
		for ( u32 x = 0; x < self->mWidth; x++ ) {
			ZLColorVec color;
			color.SetRGBA ( self->GetColor ( x, y ));
			if ( color.mA != 255 ) {
				lua_pushboolean ( L, 0 );
				return 1;
			}
		}
	}
	
	// it's opaque!
	lua_pushboolean ( L, 1 );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	load
	@text	Loads an image from an image file.
			Depending on the build configuration, the following file formats are supported:
			PNG, JPG, WebP.

	@in		MOAIImage self
	@in		string filename
	@opt	number transform	One of MOAIImage.POW_TWO, One of MOAIImage.QUANTIZE,
								One of MOAIImage.TRUECOLOR, One of MOAIImage.PREMULTIPLY_ALPHA
	@out	nil
*/
int MOAIImage::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )

	MOAIImage* srcImage	= state.GetLuaObject < MOAIImage >( 2, false );
	if  ( srcImage ) {
	
		self->Copy ( *srcImage );
	}
	else {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		u32 transform	= state.GetValue < u32 >( 3, 0 );

		self->Load ( filename, transform );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	loadAsync
	@text	Load an image asyncronously. This includes reading the file and decoding compressed data.
	
 	@overload
		@in		MOAIImage self
		@in		string filename			The path to the image file
		@in		MOAITaskQueue queue		The queue to peform operation on
		@opt	function callback		Callback that will receive loaded image
		@opt	number transform		One of MOAIImage.POW_TWO, MOAIImage.QUANTIZE,
										MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@out	nil
 
	@overload
		@in		MOAIImage self
		@in		MOAIDataBuffer data		Buffer containing the image data
		@in		MOAITaskQueue queue		The queue to peform operation on
		@opt	function callback		Callback that will receive loaded image
		@opt	number transform		One of MOAIImage.POW_TWO, MOAIImage.QUANTIZE,
										MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@out	nil
*/
int MOAIImage::_loadAsync ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	MOAIDataBuffer* buffer	= state.GetLuaObject < MOAIDataBuffer >( 2, false );
	MOAITaskQueue* queue	= state.GetLuaObject < MOAITaskQueue >( 3, true );
	u32 transform			= state.GetValue < u32 >( 5, 0 );
	
	if ( !queue ) {
		return 0;
	}
	
	MOAIImageLoadTask* task = new MOAIImageLoadTask ();
	
	if ( buffer ) {
		task->Init ( *buffer, *self, transform );
	}
	else {
		cc8* filename = state.GetValue < cc8* >( 2, "" );
		task->Init ( filename, *self, transform );
	}
	task->SetCallback ( L, 4 );
	task->Start ( *queue, self->Get < MOAIMainThreadTaskSubscriber >());
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	loadFromBuffer
	@text	Loads an image from a buffer.

	@in		MOAIImage self
	@in		MOAIDataBuffer buffer		Buffer containing the image
	@opt	number transform			One of MOAIImage.POW_TWO, One of MOAIImage.QUANTIZE,
										One of MOAIImage.TRUECOLOR, One of MOAIImage.PREMULTIPLY_ALPHA
	@out	nil
*/
int MOAIImage::_loadFromBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UU" )

	MOAIDataBuffer* buffer = state.GetLuaObject < MOAIDataBuffer >( 2, true );
	u32 transform = state.GetValue < u32 >( 3, 0 );
	if ( buffer ) {
		void* bytes = 0;
		size_t size = 0;
		ZLByteStream stream;

		buffer->Lock ( &bytes, &size );
	
		stream.SetBuffer ( bytes, size );
		stream.SetLength ( size );

		self->Load ( stream, transform );

		buffer->Unlock();
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	mix
	@text	Transforms each color by a 4x4 matrix. The default value is a
			4x4 identity matrix. The transformation 'remixes' the image's
			channels: each new channel value is given by the sum of channels
			as weighted by the corresponding row of the matrix. For example,
			to remix the blue channel: b = r*b1 + g*b2 + b*b3 + a*b4. A row value
			for b of (0, 0, 1, 0) would be the identity: b = b. A row value for
			b of (1, 0, 0, 0) would replace b with r: b=r. A row value for
			b of (0.5, 0.5, 0, 0) would replace b with an even blend of r and g:
			b = r*05 + b*0.5. In this fashion, all channels of the image
			may be rearranged or blended.
	
	@in		MOAIImage self
	
	@opt	number r1
	@opt	number r2
	@opt	number r3
	@opt	number r4
	
	@opt	number g1
	@opt	number g2
	@opt	number g3
	@opt	number g4
	
	@opt	number b1
	@opt	number b2
	@opt	number b3
	@opt	number b4
	
	@opt	number a1
	@opt	number a2
	@opt	number a3
	@opt	number a4
	
	@opt	number K					Default value is 1.
	
	@out	nil
*/
int MOAIImage::_mix ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )

	ZLMatrix4x4 mtx;

	mtx.m [ ZLMatrix4x4::C0_R0 ]		= state.GetValue < float >( 2, 1.0f );
	mtx.m [ ZLMatrix4x4::C1_R0 ]		= state.GetValue < float >( 3, 0.0f );
	mtx.m [ ZLMatrix4x4::C2_R0 ]		= state.GetValue < float >( 4, 0.0f );
	mtx.m [ ZLMatrix4x4::C3_R0 ]		= state.GetValue < float >( 5, 0.0f );
	
	mtx.m [ ZLMatrix4x4::C0_R1 ]		= state.GetValue < float >( 6, 0.0f );
	mtx.m [ ZLMatrix4x4::C1_R1 ]		= state.GetValue < float >( 7, 1.0f );
	mtx.m [ ZLMatrix4x4::C2_R1 ]		= state.GetValue < float >( 8, 0.0f );
	mtx.m [ ZLMatrix4x4::C3_R1 ]		= state.GetValue < float >( 9, 0.0f );
	
	mtx.m [ ZLMatrix4x4::C0_R2 ]		= state.GetValue < float >( 10, 0.0f );
	mtx.m [ ZLMatrix4x4::C1_R2 ]		= state.GetValue < float >( 11, 0.0f );
	mtx.m [ ZLMatrix4x4::C2_R2 ]		= state.GetValue < float >( 12, 1.0f );
	mtx.m [ ZLMatrix4x4::C3_R2 ]		= state.GetValue < float >( 13, 0.0f );
	
	mtx.m [ ZLMatrix4x4::C0_R3 ]		= state.GetValue < float >( 14, 0.0f );
	mtx.m [ ZLMatrix4x4::C1_R3 ]		= state.GetValue < float >( 15, 0.0f );
	mtx.m [ ZLMatrix4x4::C2_R3 ]		= state.GetValue < float >( 16, 0.0f );
	mtx.m [ ZLMatrix4x4::C3_R3 ]		= state.GetValue < float >( 17, 1.0f );
	
	float K								= state.GetValue < float >( 18, 1.0f );
	
	self->Mix ( *self, mtx, K );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	padToPow2
	@text	Copies an image and returns a new image padded to the next
			power of 2 along each dimension. Original image will be
			in the upper left hand corner of the new image.

	@in		MOAIImage self
	@out	MOAIImage image		Copy of the image padded to the next nearest power
								of two along each dimension.
*/
int MOAIImage::_padToPow2 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	MOAIImage* image = new MOAIImage ( self->GetContext ());
	image->PadToPow2 ( *self );
	image->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	print
	@text	Print the image colors (for debugging purposes).

	@in		MOAIImage self
	@out	nil
*/
int MOAIImage::_print ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	self->Print ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	resize
	@text	Copies the image to an image with a new size.

	@in		MOAIImage self
	@in		number width		New width of the image.
	@in		number height		New height of the image.
	@opt	number filter		One of MOAIImage.FILTER_LINEAR, MOAIImage.FILTER_NEAREST.
								Default value is MOAIImage.FILTER_LINEAR.
	@out	MOAIImage image
*/
int MOAIImage::_resize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UN" )
	
	u32 width	= state.GetValue < u32 >( 2, 0 );
	u32 height	= state.GetValue < u32 >( 3, width );
	u32 filter	= state.GetValue < u32 >( 4, MOAIImage::FILTER_LINEAR );
	
	ZLIntRect srcRect ( 0, 0, self->mWidth, self->mHeight );
	ZLIntRect destRect ( 0, 0, width, height );
	
	MOAIImage* image = new MOAIImage ( self->GetContext ());
	image->Init ( width, height, self->mColorFormat, self->mPixelFormat );
	image->CopyRect ( *self, srcRect, destRect, filter );
	image->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	resizeCanvas
	@text	Copies the image to a canvas with a new size. If the canvas
			is larger than the original image, the extra pixels will be
			initialized with 0. Pass in a new frame or just a new width
			and height. Negative values are permitted for the frame.

	@overload

		@in		MOAIImage self
		@in		number width		New width of the image.
		@in		number height		New height of the image.
		@out	MOAIImage image

	@overload

		@in		MOAIImage self
		@in		number xMin
		@in		number yMin
		@in		number xMax
		@in		number yMax
		@out	MOAIImage image
*/
int MOAIImage::_resizeCanvas ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNN" )
	
	ZLIntRect rect;
	
	if ( state.CheckParams ( 4, "NN" )) {
		rect.mXMin	= state.GetValue < int >( 2, 0 );
		rect.mYMin	= state.GetValue < int >( 3, 0 );
		rect.mXMax	= state.GetValue < int >( 4, 0 );
		rect.mYMax	= state.GetValue < int >( 5, 0 );
	}
	else {
		rect.mXMin	= 0;
		rect.mYMin	= 0;
		rect.mXMax	= state.GetValue < int >( 2, 0 );
		rect.mYMax	= state.GetValue < int >( 3, 0 );
	}
	
	MOAIImage* image = new MOAIImage ( self->GetContext ());
	image->ResizeCanvas ( *self, rect );
	image->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setColor32
	@text	Sets 32-bit the packed RGBA value for a given pixel
			coordinate. Parameter will be converted to the native format
			of the image.

	@in		MOAIImage self
	@in		number x
	@in		number y
	@in		number color
	@out	nil
*/
int MOAIImage::_setColor32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNN" )

	u32 x		= state.GetValue < u32 >( 2, 0 );
	u32 y		= state.GetValue < u32 >( 3, 0 );
	u32 color	= state.GetValue < u32 >( 4, 0 );

	self->SetColor ( x, y, color );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRGBA
	@text	Sets a color using RGBA floating point values.

	@in		MOAIImage self
	@in		number x
	@in		number y
	@in		number r
	@in		number g
	@in		number b
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIImage::_setRGBA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNN" )

	u32 x		= state.GetValue < u32 >( 2, 0 );
	u32 y		= state.GetValue < u32 >( 3, 0 );
	
	float r		= state.GetValue < float >( 4, 0.0f );
	float g		= state.GetValue < float >( 5, 0.0f );
	float b		= state.GetValue < float >( 6, 0.0f );
	float a		= state.GetValue < float >( 7, 1.0f );

	self->SetColor ( x, y, ZLColor::PackRGBA ( r, g, b, a ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	simpleThreshold
	@text	This is a 'naive' threshold implementation that forces image color channels
			to 0 or 1 based on a per-channel threshold value. The channel value must be
			entirely greater that the threshold to be promoted to a value of 1. This means
			a threshold value of 1 will always result in a channel value of 0.

	@in		MOAIImage self
	@opt	number r	Default value is 0.
	@opt	number g	Default value is 0.
	@opt	number b	Default value is 0.
	@opt	number a	Default value is 0.
	@out	nil
*/
int MOAIImage::_simpleThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )

	float r		= state.GetValue < float >( 2, 0.0f );
	float g		= state.GetValue < float >( 3, 0.0f );
	float b		= state.GetValue < float >( 4, 0.0f );
	float a		= state.GetValue < float >( 5, 0.0f );

	self->SimpleThreshold ( *self, r, g, b, a );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	subdivideRect
	@text	Convenience method. Here for now as a class method, but maybe should
			move to MOAIGrid.
			
			Subdivides a rectangle given a tile width and height. A table of tile
			rectangles will be returned. The tiles will be clipped to the original
			rect.

	@in		number tileWidth
	@in		number tileHeight
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIImage::_subdivideRect ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "NNNNNN" )

	float tileWidth		= state.GetValue < float >( 1, 1.0f );
	float tileHeight	= state.GetValue < float >( 2, 1.0f );

	tileWidth	= ABS ( tileWidth );
	tileHeight	= ABS ( tileHeight );

	ZLRect rect = state.GetRect < float >( 3 );
	
	u32 x0 = ( u32 )( rect.mXMin / tileWidth );
	u32 x1 = ( u32 )( rect.mXMax / tileWidth );
	
	u32 y0 = ( u32 )( rect.mYMin / tileHeight );
	u32 y1 = ( u32 )( rect.mYMax / tileHeight );
	
	u32 count	= 0;
	u32 xCount	= 0;
	u32 yCount	= 0;
	
	lua_newtable ( state );
	
	for ( u32 y = y0; y <= y1; ++y ) {
		for ( u32 x = x0; x <= x1; ++x ) {
		
			ZLRect tile;
		
			tile.mXMin	= x * tileWidth;
			tile.mYMin	= y * tileHeight;
			tile.mXMax	= tile.mXMin + tileWidth;
			tile.mYMax	= tile.mYMin + tileHeight;
		
			ZLRect sub;
			if ( tile.Intersect ( rect, sub )) {
			
				if (( sub.Width () > 0.0f ) && ( sub.Height () > 0.0f )) {
			
					count++;
					state.Push ( count );
				
					lua_newtable ( state );
					state.SetField < int, float >( -1, 1, sub.mXMin );
					state.SetField < int, float >( -1, 2, sub.mYMin );
					state.SetField < int, float >( -1, 3, sub.mXMax );
					state.SetField < int, float >( -1, 4, sub.mYMax );
					
					lua_settable ( state, -3 );
					
					xCount = ( x - x0 ) + 1;
					yCount = ( y - y0 ) + 1;
				}
			}
		}
	}

	state.Push ( xCount );
	state.Push ( yCount );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	write
	@text	Write image to a file.

	@in		MOAIImage self
	@in		string filename
	@opt	string format
	@out	boolean
*/
int MOAIImage::_write ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	cc8* filename = state.GetValue < cc8* >( 2, "" );
	cc8* format = state.GetValue < cc8* >( 3, "png" );
	
	ZLFileStream stream;
	stream.OpenWrite ( filename );
	ZLResultCode result = self->Write ( stream, format );
	
	state.Push ( result == ZL_OK );
	return 1;
}

//================================================================//
// MOAIImage
//================================================================//

//----------------------------------------------------------------//
MOAIImage* MOAIImage::AffirmImage ( MOAILuaState& state, int idx ) {

	MOAIImage* image = 0;
	
	if ( state.IsType ( idx, LUA_TUSERDATA )) {
		image = state.GetLuaObject < MOAIImage >( idx, false );
	}
	
	if ( state.IsType ( idx, LUA_TSTRING )) {
	
		cc8* filename = state.GetValue < cc8* >( idx, "" );
		if ( ZLFileSys::CheckFileExists ( filename )) {
	
			image = new MOAIImage ( state.GetContext ());
			if ( !image->Load ( filename )) {
				// TODO: report error
				delete image;
				image = 0;
			}
		}
	}
	return image;
}

//----------------------------------------------------------------//
bool MOAIImage::Load ( cc8* filename, u32 transform ) {

	return this->ZLImage::Load ( this->Get < ZLImageFormatMgr >(), filename, transform );
}

//----------------------------------------------------------------//
bool MOAIImage::Load ( ZLStream& stream, u32 transform ) {

	return this->ZLImage::Load ( this->Get < ZLImageFormatMgr >(), stream, transform );
}

//----------------------------------------------------------------//
MOAIImage::MOAIImage ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ) {
		
	RTTI_BEGIN ( MOAIImage )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIImage >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIImage::~MOAIImage () {
}

//----------------------------------------------------------------//
bool MOAIImage::Write ( ZLStream& stream, cc8* formatName ) {

	return this->ZLImage::Write ( this->Get < ZLImageFormatMgr >(), stream, formatName );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIImage::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	state.SetField ( -1, "FILTER_LINEAR",						( u32 )MOAIImage::FILTER_LINEAR );
	state.SetField ( -1, "FILTER_NEAREST",						( u32 )MOAIImage::FILTER_NEAREST );
	
	state.SetField ( -1, "POW_TWO",								( u32 )ZLImageTransform::POW_TWO );
	state.SetField ( -1, "QUANTIZE",							( u32 )ZLImageTransform::QUANTIZE );
	state.SetField ( -1, "TRUECOLOR",							( u32 )ZLImageTransform::TRUECOLOR );
	state.SetField ( -1, "PREMULTIPLY_ALPHA",					( u32 )ZLImageTransform::PREMULTIPLY_ALPHA );
	
	state.SetField ( -1, "PIXEL_FMT_TRUECOLOR",					( u32 )TRUECOLOR );
	state.SetField ( -1, "PIXEL_FMT_INDEX_4",					( u32 )INDEX_4 );
	state.SetField ( -1, "PIXEL_FMT_INDEX_8",					( u32 )INDEX_8 );
	
	state.SetField ( -1, "COLOR_FMT_A_1",						( u32 )ZLColor::A_1 );
	state.SetField ( -1, "COLOR_FMT_A_4",						( u32 )ZLColor::A_4 );
	state.SetField ( -1, "COLOR_FMT_A_8",						( u32 )ZLColor::A_8 );
	state.SetField ( -1, "COLOR_FMT_LA_8",						( u32 )ZLColor::LA_8 );
	state.SetField ( -1, "COLOR_FMT_RGB_888",					( u32 )ZLColor::RGB_888 );
	state.SetField ( -1, "COLOR_FMT_RGB_565",					( u32 )ZLColor::RGB_565 );
	state.SetField ( -1, "COLOR_FMT_RGBA_5551",					( u32 )ZLColor::RGBA_5551 );
	state.SetField ( -1, "COLOR_FMT_RGBA_4444",					( u32 )ZLColor::RGBA_4444 );
	state.SetField ( -1, "COLOR_FMT_RGBA_8888",					( u32 )ZLColor::RGBA_8888 );
	
	state.SetField ( -1, "BLEND_EQ_ADD",						( u32 )ZLColor::BLEND_EQ_ADD );
	state.SetField ( -1, "BLEND_EQ_NONE",						( u32 )ZLColor::BLEND_EQ_NONE );
	state.SetField ( -1, "BLEND_EQ_SUB",						( u32 )ZLColor::BLEND_EQ_SUB );
	state.SetField ( -1, "BLEND_EQ_SUB_INV",					( u32 )ZLColor::BLEND_EQ_SUB_INV );
	
	state.SetField ( -1, "BLEND_FACTOR_0001",					( u32 )ZLColor::BLEND_FACTOR_0001 );
	state.SetField ( -1, "BLEND_FACTOR_1110",					( u32 )ZLColor::BLEND_FACTOR_1110 );
	state.SetField ( -1, "BLEND_FACTOR_ONE",					( u32 )ZLColor::BLEND_FACTOR_ONE );
	state.SetField ( -1, "BLEND_FACTOR_ZERO",					( u32 )ZLColor::BLEND_FACTOR_ZERO );
	state.SetField ( -1, "BLEND_FACTOR_DST_ALPHA",				( u32 )ZLColor::BLEND_FACTOR_DST_ALPHA );
	state.SetField ( -1, "BLEND_FACTOR_DST_COLOR",				( u32 )ZLColor::BLEND_FACTOR_DST_COLOR);
	state.SetField ( -1, "BLEND_FACTOR_ONE_MINUS_DST_ALPHA",	( u32 )ZLColor::BLEND_FACTOR_ONE_MINUS_DST_ALPHA );
	state.SetField ( -1, "BLEND_FACTOR_ONE_MINUS_DST_COLOR",	( u32 )ZLColor::BLEND_FACTOR_ONE_MINUS_DST_COLOR );
	state.SetField ( -1, "BLEND_FACTOR_ONE_MINUS_SRC_ALPHA",	( u32 )ZLColor::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );
	state.SetField ( -1, "BLEND_FACTOR_ONE_MINUS_SRC_COLOR",	( u32 )ZLColor::BLEND_FACTOR_ONE_MINUS_SRC_COLOR );
	state.SetField ( -1, "BLEND_FACTOR_SRC_ALPHA",				( u32 )ZLColor::BLEND_FACTOR_SRC_ALPHA );
	state.SetField ( -1, "BLEND_FACTOR_SRC_COLOR",				( u32 )ZLColor::BLEND_FACTOR_SRC_COLOR );
	
	luaL_Reg regTable [] = {
		{ "calculateGaussianKernel",	_calculateGaussianKernel },
		{ "subdivideRect",				_subdivideRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIImage::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "average",					_average },
		{ "bleedRect",					_bleedRect },
		{ "compare",					_compare },
		{ "convert",					_convert },
		{ "convertColors",				_convert }, // back compat
		{ "convolve",					_convolve },
		{ "convolve1D",					_convolve1D },
		{ "convertToGrayScale",			_desaturate }, // back compat
		{ "copy",						_copy },
		{ "copyBits",					_copyBits },
		{ "copyRect",					_copyRect },
		{ "desaturate",					_desaturate },
		{ "fillCircle",					_fillCircle },
		{ "fillEllipse",				_fillEllipse },
		{ "fillRect",					_fillRect },
		{ "gammaCorrection",			_gammaCorrection },
		{ "generateOutlineFromSDF",		_generateOutlineFromSDF },
		{ "generateSDF",				_generateSDF },
		{ "generateSDFAA",				_generateSDFAA },
		{ "generateSDFDeadReckoning",	_generateSDFDeadReckoning },
		{ "getColor32",					_getColor32 },
		{ "getContentRect",				_getContentRect },
		{ "getData",					_getData },
		{ "getFormat",					_getFormat },
		{ "getRGBA",					_getRGBA },
		{ "getSize",					_getSize },
		{ "init",						_init },
		{ "isOpaque",					_isOpaque },
		{ "load",						_load },
		{ "loadAsync",					_loadAsync },
		{ "loadFromBuffer",				_loadFromBuffer },
		{ "mix",						_mix },
		{ "padToPow2",					_padToPow2 },
		{ "print",						_print },
		{ "resize",						_resize },
		{ "resizeCanvas",				_resizeCanvas },
		{ "setColor32",					_setColor32 },
		{ "setRGBA",					_setRGBA },
		{ "simpleThreshold",			_simpleThreshold },
		{ "write",						_write },
		{ "writePNG",					_write }, // back compat
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
