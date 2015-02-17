// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLBitBuffer.h>
#include <zl-util/ZLColor.h>
#include <zl-util/ZLFloat.h>
#include <zl-util/ZLInterpolate.h>
#include <zl-util/ZLVec4D.h>

#define WR 0.299f
#define WG 0.587f
#define WB 0.114f

#define U_MAX 0.436f
#define V_MAX 0.615f

//================================================================//
// ZLColor
//================================================================//

//----------------------------------------------------------------//
u32 ZLColor::Add ( u32 c0, u32 c1 ) {
	
	u8* cb0 = ( u8* )&c0;
	u8* cb1 = ( u8* )&c1;
	
	u32 r32;
	u8* rb32 = ( u8* )&r32;

	rb32 [ R_BYTE ] = cb0 [ R_BYTE ] + cb1 [ R_BYTE ];
	rb32 [ G_BYTE ] = cb0 [ G_BYTE ] + cb1 [ G_BYTE ];
	rb32 [ B_BYTE ] = cb0 [ B_BYTE ] + cb1 [ B_BYTE ];
	rb32 [ A_BYTE ] = cb0 [ A_BYTE ] + cb1 [ A_BYTE ];
	
	return r32;
}

//----------------------------------------------------------------//
u32 ZLColor::AddAndClamp ( u32 c0, u32 c1 ) {
	
	u8* cb0 = ( u8* )&c0;
	u8* cb1 = ( u8* )&c1;

	u32 r = cb0 [ R_BYTE ] + cb1 [ R_BYTE ];
	u32 g = cb0 [ G_BYTE ] + cb1 [ G_BYTE ];
	u32 b = cb0 [ B_BYTE ] + cb1 [ B_BYTE ];
	u32 a = cb0 [ A_BYTE ] + cb1 [ A_BYTE ];
	
	u32 r32;
	u8* rb32 = ( u8* )&r32;
	
	rb32 [ R_BYTE ] = r > 0xff ? 0xff : r;
	rb32 [ G_BYTE ] = g > 0xff ? 0xff : g;
	rb32 [ B_BYTE ] = b > 0xff ? 0xff : b;
	rb32 [ A_BYTE ] = a > 0xff ? 0xff : a;
	
	return r32;
}

//----------------------------------------------------------------//
u32 ZLColor::Average ( u32 c0, u32 c1 ) {
	
	u32 r = (( c0 & 0xFF ) + ( c1 & 0xFF )) >> 1;
	u32 g = ((( c0 >> 0x08 ) & 0xFF ) + (( c1 >> 0x08 ) & 0xFF )) >> 1;
	u32 b = ((( c0 >> 0x10 ) & 0xFF ) + (( c1 >> 0x10 ) & 0xFF )) >> 1;
	u32 a = ((( c0 >> 0x18 ) & 0xFF ) + (( c1 >> 0x18 ) & 0xFF )) >> 1;
	
	return r + ( g << 0x08 ) + ( b << 0x10 ) + ( a << 0x18 );
}

//----------------------------------------------------------------//
u32 ZLColor::Average ( u32 c0, u32 c1, u32 c2, u32 c3 ) {
	
	u32 r = (( c0 & 0xFF ) + ( c1 & 0xFF ) + ( c2 & 0xFF ) + ( c3 & 0xFF )) >> 2;
	u32 g = ((( c0 >> 0x08 ) & 0xFF ) + (( c1 >> 0x08 ) & 0xFF ) + (( c2 >> 0x08 ) & 0xFF ) + (( c3 >> 0x08 ) & 0xFF )) >> 2;
	u32 b = ((( c0 >> 0x10 ) & 0xFF ) + (( c1 >> 0x10 ) & 0xFF ) + (( c2 >> 0x10 ) & 0xFF ) + (( c3 >> 0x10 ) & 0xFF )) >> 2;
	u32 a = ((( c0 >> 0x18 ) & 0xFF ) + (( c1 >> 0x18 ) & 0xFF ) + (( c2 >> 0x18 ) & 0xFF ) + (( c3 >> 0x18 ) & 0xFF )) >> 2;
	
	return r + ( g << 0x08 ) + ( b << 0x10 ) + ( a << 0x18 );
}

//----------------------------------------------------------------//
u32 ZLColor::BilerpFixed ( u32 c0, u32 c1, u32 c2, u32 c3, u8 xt, u8 yt ) {

	u32 s0 = ZLColor::LerpFixed ( c0, c1, xt );
	u32 s1 = ZLColor::LerpFixed ( c2, c3, xt );
	
	return ZLColor::LerpFixed ( s0, s1, yt );
}

//----------------------------------------------------------------//
u32 ZLColor::Blend ( u32 src32, u32 dst32, const ZLColorBlendFunc& blendFunc ) {

	return Blend ( src32, dst32, blendFunc.mSrcFactor, blendFunc.mDstFactor, blendFunc.mEquation );
}

//----------------------------------------------------------------//
u32 ZLColor::Blend ( u32 src32, u32 dst32, ZLColor::BlendFactor srcFactor, ZLColor::BlendFactor dstFactor, ZLColor::BlendEquation blendEq ) {
	
	if ( blendEq == BLEND_EQ_NONE ) return src32;
	
	u32 srcBlend32 = Mul ( src32, GetBlendFactor ( src32, dst32, srcFactor ));
	u32 dstBlend32 = Mul ( dst32, GetBlendFactor ( src32, dst32, dstFactor ));

	switch ( blendEq ) {

		case BLEND_EQ_ADD:
			return AddAndClamp ( srcBlend32, dstBlend32 );
			
		case BLEND_EQ_SUB:
			return SubAndClamp ( srcBlend32, dstBlend32 );
			
		case BLEND_EQ_SUB_INV:
			return SubAndClamp ( dstBlend32, srcBlend32 );
		
		// TODO: min & max
	}
	return 0;
}

//----------------------------------------------------------------//
u32 ZLColor::BlendFactorAlpha ( u32 color32 ) {
	
	u8* colorBytes = ( u8* )&color32;
	u8 a = colorBytes [ A_BYTE ];
	return PackRGBA ( a, a, a, a );
}

//----------------------------------------------------------------//
u32 ZLColor::BlendFactorOneMinusAlpha ( u32 color32 ) {
	
	u8* colorBytes = ( u8* )&color32;
	u8 a = 0xff - colorBytes [ A_BYTE ];
	return PackRGBA ( a, a, a, a );
}

//----------------------------------------------------------------//
u32 ZLColor::BlendFactorOneMinusColor ( u32 color32 ) {
	
	u8* colorBytes = ( u8* )&color32;
	
	u32 result32;
	u8* resultBytes = ( u8* )&result32;

	resultBytes [ R_BYTE ] = 0xff - colorBytes [ R_BYTE ];
	resultBytes [ G_BYTE ] = 0xff - colorBytes [ G_BYTE ];
	resultBytes [ B_BYTE ] = 0xff - colorBytes [ B_BYTE ];
	resultBytes [ A_BYTE ] = 0xff - colorBytes [ A_BYTE ];
	
	return result32;
}

//----------------------------------------------------------------//
void ZLColor::Convert ( void* dest, ColorFormat destFmt, const void* src, ColorFormat srcFmt, u32 nColors ) {

	static const u32 MAX_COLORS = 2048;

	u32 buffer [ MAX_COLORS ];
	u32* bufferPtr = buffer;
	u32 color;
	
	u32 nCopied = 0;
	
	while ( nColors ) {
	
		u32 copy = nColors;
		if ( copy > MAX_COLORS ) {
			copy = MAX_COLORS;
		}
		nColors -= copy;
	
		switch ( srcFmt ) {
			
			case A_1:
			
				for ( u32 i = 0; i < copy; ++i ) {
					color = ZLBitBuffer::GetValue ( src, nCopied + i, 1 ) ? 0xff : 0x00;
					buffer [ i ] = color << 0x18;
				}
				bufferPtr = buffer;
				break;
			
			case A_4:
			
				for ( u32 i = 0; i < copy; ++i ) {
					color = ZLBitBuffer::GetValue ( src, nCopied + i, 4 );
					buffer [ i ] = ( color << 0x1C ) | ( color << 0x18 );
				}
				bufferPtr = buffer;
				break;
			
			case A_8:
			
				for ( u32 i = 0; i < copy; ++i ) {
				
					color = *( u8* )src;
					src = ( void* )(( size_t )src + 1 );
					
					buffer [ i ] = color << 0x18;
				}
				bufferPtr = buffer;
				break;
			
			case RGB_888:
				
				for ( u32 i = 0; i < copy; ++i ) {
					
					color = *( u8* )src;
					src = ( void* )(( size_t )src + 1 );
					
					color += *( u8* )src << 0x08;
					src = ( void* )(( size_t )src + 1 );
					
					color += *( u8* )src << 0x10;
					src = ( void* )(( size_t )src + 1 );
					
					buffer [ i ]= color | 0xFF000000;
				}
				bufferPtr = buffer;
				break;
				
			case RGB_565:
				
				for ( u32 i = 0; i < copy; ++i ) {
					
					color = *( u16* )src;
					src = ( void* )(( size_t )src + 2 );
					
					u8 r = ( color >> 0x0B ) & 0x1F;
					u8 g = ( color >> 0x05 ) & 0x3F;
					u8 b = color & 0x1F;
					
					buffer [ i ] =	(( r << 0x03 ) | ( r >> 0x02 ))  +
									((( g << 0x02 ) | ( g >> 0x04 )) << 0x08 ) +
									((( b << 0x03 ) | ( b >> 0x02 )) << 0x10 ) +
									0xFF000000;
					
				}
				bufferPtr = buffer;
				break;
			
			case RGBA_5551: 
				
				for ( u32 i = 0; i < copy; ++i ) {
				
					color = *( u16* )src;
					src = ( void* )(( size_t )src + 2 );
					
					u8 r = color & 0x1F;
					u8 g = ( color >> 0x05 ) & 0x1F;
					u8 b = ( color >> 0x0A ) & 0x1F;
					
					buffer [ i ] =	(( r << 0x03 ) | ( r >> 0x02 ))  +
									((( g << 0x03 ) | ( g >> 0x02 )) << 0x08 ) +
									((( b << 0x03 ) | ( b >> 0x02 )) << 0x10 ) +
									(((( color >> 0x0F ) & 0x01 ) ? 0xFF : 0x00 ) << 0x18 );
				}
				bufferPtr = buffer;
				break;

			case RGBA_4444:
			
				for ( u32 i = 0; i < copy; ++i ) {
				
					color = *( u32* )src;
					src = ( void* )(( size_t )src + 2 );
					
					u8 r = ( color >> 0x0C ) & 0x0F;
					u8 g = ( color >> 0x08 ) & 0x0F;
					u8 b = ( color >> 0x04 ) & 0x0F;
					u8 a = color & 0x0F;
					
					buffer [ i ] =	(( r << 0x04 ) | r ) +
									(( g << 0x0C ) | ( g << 0x08 )) +
									(( b << 0x14 ) | ( b << 0x10 )) +
									(( a << 0x1C ) | ( a << 0x18 ));
				}
				bufferPtr = buffer;
				break;

			case RGBA_8888:
				bufferPtr = ( u32* )src;
				break;
			
			default:
				return;
		}
		
		switch ( destFmt ) {
		
			case A_1:
				
				for ( u32 i = 0; i < copy; ++i ) {
					color = bufferPtr [ i ];
					ZLBitBuffer::SetValue ( dest, color >> 0x1F, nCopied + i, 1 );
				}
				break;
				
			case A_4:
				
				for ( u32 i = 0; i < copy; ++i ) {
					color = bufferPtr [ i ];
					ZLBitBuffer::SetValue ( dest, color >> 0x1C, nCopied + i, 4 );
				}
				break;
		
			case A_8:
				
				for ( u32 i = 0; i < copy; ++i ) {
				
					color = bufferPtr [ i ];
					
					(( u8* )dest )[ 0 ] = ( color >> 0x18 ) & 0xFF;
					dest = ( void* )(( size_t )dest + 1 );
				}
				break;
		
			case RGB_888:
			
				for ( u32 i = 0; i < copy; ++i ) {
				
					color = bufferPtr [ i ];
					
					(( u8* )dest )[ 0 ] = color & 0xFF;
					(( u8* )dest )[ 1 ] = ( color >> 8 ) & 0xFF;
					(( u8* )dest )[ 2 ] = ( color >> 16 ) & 0xFF;
					dest = ( void* )(( size_t )dest + 3 );
				}
				break;
				
			case RGB_565:
			
				for ( u32 i = 0; i < copy; ++i ) {
				
					color = bufferPtr [ i ];
					
					*( u16* )dest =	((( color >> 0x03 ) & 0x1F ) << 0x0B ) +
									((( color >> 0x0A ) & 0x3F ) << 0x05 ) +
									((( color >> 0x13 ) & 0x1F ) << 0x00 );
					dest = ( void* )(( size_t )dest + 2 );
				}
				break;
						
			case RGBA_5551: 
				
				for ( u32 i = 0; i < copy; ++i ) {
				
					color = bufferPtr [ i ];
					
					*( u16* )dest =		((( color >> 0x03 ) & 0x1F ) << 0x00 ) +
										((( color >> 0x0B ) & 0x1F ) << 0x05 ) +
										((( color >> 0x13 ) & 0x1F ) << 0x0A ) +
										(((( color >> 0x1C ) & 0x0F ) ? 0x01 : 0x00 ) << 0x0F );
					dest = ( void* )(( size_t )dest + 2 );
				}
				break;

			case RGBA_4444:
				
				for ( u32 i = 0; i < copy; ++i ) {
				
					color = bufferPtr [ i ];
					
					*( u16* )dest =		((( color >> 0x04 ) & 0x0F ) << 0x0C ) +
										((( color >> 0x0C ) & 0x0F ) << 0x08 ) +
										((( color >> 0x14 ) & 0x0F ) << 0x04 ) +
										((( color >> 0x1C ) & 0x0F ) << 0x00 );
					dest = ( void* )(( size_t )dest + 2 );
				}
				break;

			case RGBA_8888:
				
				memcpy ( dest, bufferPtr, copy * sizeof ( u32 ));
				dest = ( void* )(( size_t )dest + ( copy * sizeof ( u32 )));
				break;
			
			default:
				break;
		}
		
		nCopied += copy;
	}
}

//----------------------------------------------------------------//
u32 ZLColor::ConvertFromRGBA ( u32 color, ColorFormat format ) {

	switch ( format ) {
		
		case A_1:
			return ( color >> 0x1F ) & 0x00000001;
		
		case A_4:
			return ( color >> 0x1C ) & 0x0000000F;
		
		case A_8:
			return ( color >> 0x18 ) & 0x000000FF;
		
		case RGB_888:
			return color & 0x00FFFFFF;
			
		case RGB_565:
		
			return	((( color >> 0x03 ) & 0x1F ) << 0x0B ) +
					((( color >> 0x0A ) & 0x3F ) << 0x05 ) +
					((( color >> 0x13 ) & 0x1F ) << 0x00 );
					
		case RGBA_5551: 
			
			return	((( color >> 0x03 ) & 0x1F ) << 0x00 ) +
					((( color >> 0x0B ) & 0x1F ) << 0x05 ) +
					((( color >> 0x13 ) & 0x1F ) << 0x0A ) +
					(((( color >> 0x1C ) & 0x0F ) ? 0x01 : 0x00 ) << 0x0F );

		case RGBA_4444:
			
			return	((( color >> 0x04 ) & 0x0F ) << 0x0C ) +
					((( color >> 0x0C ) & 0x0F ) << 0x08 ) +
					((( color >> 0x14 ) & 0x0F ) << 0x04 ) +
					((( color >> 0x1C ) & 0x0F ) << 0x00 );

		case RGBA_8888:
			return color;
		
		default:
			break;
	}
	
	return 0;
}

//----------------------------------------------------------------//
u32 ZLColor::ConvertToRGBA ( u32 color, ColorFormat format ) {

	switch ( format ) {
		
		case A_1:
			return ( color & 0x01 ) ? 0xFF000000 : 0x00000000;
			
		case A_4:
			return (( color << 0x1C ) | ( color << 0x18 )) & 0xFF000000;
		
		case A_8:
			return ( color << 0x18 ) & 0xFF000000;
		
		case RGB_888:
			return color | 0xFF000000;
			
		case RGB_565: {
			
			u8 r = ( color >> 0x0B ) & 0x1F;
			u8 g = ( color >> 0x05 ) & 0x3F;
			u8 b = color & 0x1F;
			
			return	(( r << 0x03 ) | ( r >> 0x02 ))  +
					((( g << 0x02 ) | ( g >> 0x04 )) << 0x08 ) +
					((( b << 0x03 ) | ( b >> 0x02 )) << 0x10 ) +
					0xFF000000;
		}
		
		case RGBA_5551: {
			
			u8 r = color & 0x1F;
			u8 g = ( color >> 0x05 ) & 0x1F;
			u8 b = ( color >> 0x0A ) & 0x1F;
					
			return	(( r << 0x03 ) | ( r >> 0x02 ))  +
					((( g << 0x03 ) | ( g >> 0x02 )) << 0x08 ) +
					((( b << 0x03 ) | ( b >> 0x02 )) << 0x10 ) +
					(((( color >> 0x0F ) & 0x01 ) ? 0xFF : 0x00 ) << 0x18 );
		}

		case RGBA_4444: {
		
			u8 r = ( color >> 0x0C ) & 0x0F;
			u8 g = ( color >> 0x08 ) & 0x0F;
			u8 b = ( color >> 0x04 ) & 0x0F;
			u8 a = color & 0x0F;
			
			return	(( r << 0x04 ) | r ) +
					(( g << 0x0C ) | ( g << 0x08 )) +
					(( b << 0x14 ) | ( b << 0x10 )) +
					(( a << 0x1C ) | ( a << 0x18 ));
		}
		
		case RGBA_8888:
			return color;
		
		default:
			break;
	}
	
	return 0;
}

//----------------------------------------------------------------//
void ZLColor::Desaturate ( void *colors, ZLColor::ColorFormat format, u32 nColors, float rY, float gY, float bY, float K ) {

	if ( K == 0.0f ) return;

	u32 bitDepth = ZLColor::GetDepthInBits ( format );
	
	if ( K < 1.0f ) {
	
		float oneMinusK = 1.0f - K;

		for ( u32 x = 0; x < nColors; ++x ) {
			
			ZLColorVec color ( ZLColor::ConvertToRGBA ( ZLBitBuffer::GetValue ( colors, x, bitDepth ), format ));
			
			float grayTimesK = (( color.mR * rY ) + ( color.mG * gY ) + ( color.mB * bY )) * K;
					
			color.mR = ( u8 )(( color.mR * oneMinusK ) + grayTimesK );
			color.mG = ( u8 )(( color.mG * oneMinusK ) + grayTimesK );
			color.mB = ( u8 )(( color.mB * oneMinusK ) + grayTimesK );
			
			ZLBitBuffer::SetValue ( colors, ZLColor::ConvertFromRGBA ( color.PackRGBA (), format ), x, bitDepth );
		}
	}
	else {
	
		for ( u32 x = 0; x < nColors; ++x ) {
			
			ZLColorVec color ( ZLColor::ConvertToRGBA ( ZLBitBuffer::GetValue ( colors, x, bitDepth ), format ));
			
			float gray = ( color.mR * rY ) + ( color.mG * gY ) + ( color.mB * bY );
					
			color.mR = gray;
			color.mG = gray;
			color.mB = gray;
			
			ZLBitBuffer::SetValue ( colors, ZLColor::ConvertFromRGBA ( color.PackRGBA (), format ), x, bitDepth );
		}
	}
}

//----------------------------------------------------------------//
void ZLColor::GammaCorrection ( void* colors, ColorFormat format, u32 nColors, float gamma ) {

	if ( gamma == 1.0f ) return;

	u32 bitDepth = ZLColor::GetDepthInBits ( format );
	float invGamma = 1.0f / gamma;
	
	for ( u32 x = 0; x < nColors; ++x ) {
			
		ZLColorVec color ( ZLColor::ConvertToRGBA ( ZLBitBuffer::GetValue ( colors, x, bitDepth ), format ));
		
		color.mR = powf ( color.mR, invGamma );
		color.mG = powf ( color.mG, invGamma );
		color.mB = powf ( color.mB, invGamma );
		
		ZLBitBuffer::SetValue ( colors, ZLColor::ConvertFromRGBA ( color.PackRGBA (), format ), x, bitDepth );
	}
}

//----------------------------------------------------------------//
u32 ZLColor::GetBlendFactor ( u32 src32, u32 dst32, BlendFactor factor ) {

	switch ( factor ) {
	
		case BLEND_FACTOR_0001:
			return 0xff000000;
		
		case BLEND_FACTOR_1110:
			return 0x00ffffff;
	
		case BLEND_FACTOR_ONE:
			return 0xffffffff;
			
		case BLEND_FACTOR_ZERO:
			return 0x00000000;
		
		case BLEND_FACTOR_DST_ALPHA:
			return BlendFactorAlpha ( dst32 );
		
		case BLEND_FACTOR_DST_COLOR:
			return dst32;
		
		case BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
			return BlendFactorOneMinusAlpha ( dst32 );
		
		case BLEND_FACTOR_ONE_MINUS_DST_COLOR:
			return BlendFactorOneMinusColor ( dst32 );
		
		case BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
			return BlendFactorOneMinusAlpha ( src32 );
		
		case BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
			return BlendFactorOneMinusColor ( src32 );
		
		case BLEND_FACTOR_SRC_ALPHA:
			return BlendFactorAlpha ( src32 );
		
		case BLEND_FACTOR_SRC_COLOR:
			return src32;
	}

	return 0;
}

//----------------------------------------------------------------//
u32 ZLColor::GetDepthInBits ( ColorFormat format ) {

	switch ( format ) {
		case A_1:			return 1;
		case A_4:			return 4;
		case A_8:			return 8;
		case RGB_888:		return 24;
		case RGB_565:		return 16;
		case RGBA_5551:		return 16;
		case RGBA_4444:		return 16;
		case RGBA_8888:		return 32;
		default:			break;
	}
	return 0;
}

//----------------------------------------------------------------//
u32 ZLColor::GetMask ( ColorFormat format ) {

	switch ( format ) {
		case A_1:			return 0x00000001;
		case A_4:			return 0x0000000F;
		case A_8:			return 0x000000FF;
		case RGB_888:		return 0x00ffffff;
		case RGB_565:		return 0x0000ffff;
		case RGBA_5551:		return 0x0000ffff;
		case RGBA_4444:		return 0x0000ffff;
		case RGBA_8888:		return 0xffffffff;
		default:			break;
	}
	return 0;
}

//----------------------------------------------------------------//
u32 ZLColor::LerpFixed ( u32 c0, u32 c1, u8 t ) {
	
	u32 r0 = ( c0 ) & 0xFF;
	u32 g0 = ( c0 >> 0x08 ) & 0xFF;
	u32 b0 = ( c0 >> 0x10 ) & 0xFF;
	u32 a0 = ( c0 >> 0x18 ) & 0xFF;
	
	u32 r1 = ( c1 ) & 0xFF;
	u32 g1 = ( c1 >> 0x08 ) & 0xFF;
	u32 b1 = ( c1 >> 0x10 ) & 0xFF;
	u32 a1 = ( c1 >> 0x18 ) & 0xFF;
	
	u32 r = r0 + ((( r1 - r0 ) * t ) >> 0x08 );
	u32 g = g0 + ((( g1 - g0 ) * t ) >> 0x08 );
	u32 b = b0 + ((( b1 - b0 ) * t ) >> 0x08 );
	u32 a = a0 + ((( a1 - a0 ) * t ) >> 0x08 );
	
	return r + ( g << 0x08 ) + ( b << 0x10 ) + ( a << 0x18 );
}

//----------------------------------------------------------------//
void ZLColor::Mix ( void *colors, ZLColor::ColorFormat format, u32 nColors, const ZLMatrix4x4& mtx, float K ) {

	if ( K == 0.0f ) return;

	u32 bitDepth = ZLColor::GetDepthInBits ( format );

	if ( K < 1.0f ) {

		float oneMinusK = 1.0f - K;

		for ( u32 x = 0; x < nColors; ++x ) {
			
			ZLColorVec color ( ZLColor::ConvertToRGBA ( ZLBitBuffer::GetValue ( colors, x, bitDepth ), format ));
			ZLVec4D vec ( color.mR, color.mG, color.mB, color.mA );
			
			mtx.Transform ( vec );
			
			color.mR = ( u8 )(( color.mR * oneMinusK ) + ( vec.mX * K ));
			color.mG = ( u8 )(( color.mG * oneMinusK ) + ( vec.mY * K ));
			color.mB = ( u8 )(( color.mB * oneMinusK ) + ( vec.mZ * K ));
			color.mA = ( u8 )(( color.mA * oneMinusK ) + ( vec.mW * K ));
			
			ZLBitBuffer::SetValue ( colors, ZLColor::ConvertFromRGBA ( color.PackRGBA (), format ), x, bitDepth );
		}
	}
	else {
	
		for ( u32 x = 0; x < nColors; ++x ) {
			
			ZLColorVec color ( ZLColor::ConvertToRGBA ( ZLBitBuffer::GetValue ( colors, x, bitDepth ), format ));
			ZLVec4D vec ( color.mR, color.mG, color.mB, color.mA );
			mtx.Transform ( vec );
			u32 result = ZLColor::PackRGBA ( vec.mX, vec.mY, vec.mZ, vec.mW );
			ZLBitBuffer::SetValue ( colors, ZLColor::ConvertFromRGBA ( result, format ), x, bitDepth );
		}
	}
}

//----------------------------------------------------------------//
u32 ZLColor::Mul ( u32 c0, u32 c1 ) {
	
	u8* cb0 = ( u8* )&c0;
	u8* cb1 = ( u8* )&c1;
	
	u32 r32;
	u8* rb32 = ( u8* )&r32;

	rb32 [ R_BYTE ] = (( u32 )cb0 [ R_BYTE ] * ( u32 )cb1 [ R_BYTE ]) / 0xff;
	rb32 [ G_BYTE ] = (( u32 )cb0 [ G_BYTE ] * ( u32 )cb1 [ G_BYTE ]) / 0xff;
	rb32 [ B_BYTE ] = (( u32 )cb0 [ B_BYTE ] * ( u32 )cb1 [ B_BYTE ]) / 0xff;
	rb32 [ A_BYTE ] = (( u32 )cb0 [ A_BYTE ] * ( u32 )cb1 [ A_BYTE ]) / 0xff;
	
	return r32;
}

//----------------------------------------------------------------//
u32 ZLColor::NearestNeighbor ( u32 c0, u32 c1, u32 c2, u32 c3, u8 xt, u8 yt ) {

	if ( xt < 128 ) {
		if ( yt < 128 ) {
			return c0;
		}
		return c2;
	}
	if ( yt < 128 ) {
		return c1;
	}
	return c3;
}

//----------------------------------------------------------------//
u32 ZLColor::PackRGBA ( int r, int g, int b, int a ) {

	return	( r << 0x00 ) +
			( g << 0x08 ) +
			( b << 0x10 ) +
			( a << 0x18 );
}

//----------------------------------------------------------------//
u32 ZLColor::PackRGBA ( float r, float g, float b, float a ) {

	return	(( u8 )( r * 255.0f ) << 0x00) +
			(( u8 )( g * 255.0f ) << 0x08 ) +
			(( u8 )( b * 255.0f ) << 0x10 ) +
			(( u8 )( a * 255.0f ) << 0x18 );
}

//----------------------------------------------------------------//
void ZLColor::PremultiplyAlpha ( void* colors, ColorFormat format, u32 nColors ) {

	u32 color;
	u32 alpha;
	
	switch ( format ) {
		
		case A_1:
		case A_4:
		case A_8:
		case RGB_888:
		case RGB_565:
			break;
		
		case RGBA_5551: 
			
			for ( u32 i = 0; i < nColors; ++i ) {
				color = *( u16* )colors;
				alpha = ( color >> 0x0F ) & 0x01;
				*( u16* )colors = ( u16 )(	(((( color >> 0x00 ) & 0x1F ) * alpha ) << 0x00 ) +
											(((( color >> 0x05 ) & 0x1F ) * alpha ) << 0x05 ) +
											(((( color >> 0x0A ) & 0x1F ) * alpha ) << 0x0A ) +
											( alpha << 0x0F ));
				colors = ( void* )(( size_t )colors + 2 );
			}
			break;

		case RGBA_4444:
		
			for ( u32 i = 0; i < nColors; ++i ) {
				color = *( u16* )colors;
				alpha = color & 0x0F;
				*( u16* )colors = ( u16 )(	alpha +
											((((( color >> 0x04 ) & 0x0F ) * alpha ) >> 0x04 ) << 0x04 ) +
											((((( color >> 0x08 ) & 0x0F ) * alpha ) >> 0x04 ) << 0x08 ) +
											((((( color >> 0x0c ) & 0x0F ) * alpha ) >> 0x04 ) << 0x0C ));
				colors = ( void* )(( size_t )colors + 2 );
			}
			break;

		case RGBA_8888:
		
			for ( u32 i = 0; i < nColors; ++i ) {
				color = *( u32* )colors;
				alpha = ( color >> 0x18 ) & 0xFF;
				*( u32* )colors =	((((( color >> 0x00 ) & 0xFF ) * alpha ) >> 0x08 ) << 0x00 ) +
									((((( color >> 0x08 ) & 0xFF ) * alpha ) >> 0x08 ) << 0x08 ) +
									((((( color >> 0x10 ) & 0xFF ) * alpha ) >> 0x08 ) << 0x10 ) +
									( alpha << 0x18 );
				colors = ( void* )(( size_t )colors + 4 );
			}
			break;
		
		default:
			break;
	}
}

//----------------------------------------------------------------//
u32 ZLColor::Scale ( u32 c0, u8 s ) {
	
	u8* cb0 = ( u8* )&c0;
	
	u32 r32;
	u8* rb32 = ( u8* )&r32;

	rb32 [ R_BYTE ] = (( u32 )cb0 [ R_BYTE ] * ( u32 )s ) / 0xff;
	rb32 [ G_BYTE ] = (( u32 )cb0 [ G_BYTE ] * ( u32 )s ) / 0xff;
	rb32 [ B_BYTE ] = (( u32 )cb0 [ B_BYTE ] * ( u32 )s ) / 0xff;
	rb32 [ A_BYTE ] = (( u32 )cb0 [ A_BYTE ] * ( u32 )s ) / 0xff;
	
	return r32;
}

//----------------------------------------------------------------//
ZLColorVec ZLColor::Set ( u32 c0 ) {
	
	ZLColorVec color (
		(( c0 & ( 255 << 0x00 )) >> 0x00 ) / 255.0f,
		(( c0 & ( 255 << 0x08 )) >> 0x08 ) / 255.0f,
		(( c0 & ( 255 << 0x10 )) >> 0x10 ) / 255.0f,
		(( c0 & ( 255 << 0x18 )) >> 0x18 ) / 255.0f );
	
	return color;
}

//----------------------------------------------------------------//
u32 ZLColor::Set ( u32 c0, u8 b, u8 v ) {
	
	u32 r32 = c0;
	u8* rb32 = ( u8* )&r32;
	
	return r32;
}

//----------------------------------------------------------------//
void ZLColor::SimpleThreshold ( void* colors, ColorFormat format, u32 nColors, u32 color ) {
	
	switch ( format ) {
		
		case A_1:
		case A_4:
		case A_8:
			break;
		
		case RGB_888: {
		
			u8* bytes = ( u8* )colors;
		
			u8 rT = ( u8 )(( color >> 0x00 ) & 0xFF );
			u8 gT = ( u8 )(( color >> 0x08 ) & 0xFF );
			u8 bT = ( u8 )(( color >> 0x10 ) & 0xFF );
		
			for ( u32 i = 0; i < nColors; ++i ) {
				
				bytes [ 0 ] = bytes [ 0 ] > rT ? 0xFF : 0x00;
				bytes [ 1 ] = bytes [ 1 ] > gT ? 0xFF : 0x00;
				bytes [ 2 ] = bytes [ 2 ] > bT ? 0xFF : 0x00;
				
				bytes = ( u8* )(( size_t )bytes + 3 );
			}
			break;
		}
		
		case RGB_565:
		case RGBA_5551:
		case RGBA_4444:
			break;

		case RGBA_8888: {
		
			u8* bytes = ( u8* )colors;
		
			u32 rT = ( color >> 0x00 ) & 0xFF;
			u32 gT = ( color >> 0x08 ) & 0xFF;
			u32 bT = ( color >> 0x10 ) & 0xFF;
			u32 aT = ( color >> 0x18 ) & 0xFF;
		
			for ( u32 i = 0; i < nColors; ++i ) {

				bytes [ 0 ] = bytes [ 0 ] > rT ? 0xFF : 0x00;
				bytes [ 1 ] = bytes [ 1 ] > gT ? 0xFF : 0x00;
				bytes [ 2 ] = bytes [ 2 ] > bT ? 0xFF : 0x00;
				bytes [ 3 ] = bytes [ 3 ] > aT ? 0xFF : 0x00;

				bytes = ( u8* )(( size_t )bytes + 4 );
			}
			break;
		}
		default:
			break;
	}
}

//----------------------------------------------------------------//
u32 ZLColor::Sub ( u32 c0, u32 c1 ) {
	
	u8* cb0 = ( u8* )&c0;
	u8* cb1 = ( u8* )&c1;
	
	u32 r32;
	u8* rb32 = ( u8* )&r32;

	rb32 [ R_BYTE ] = cb0 [ R_BYTE ] - cb1 [ R_BYTE ];
	rb32 [ G_BYTE ] = cb0 [ G_BYTE ] - cb1 [ G_BYTE ];
	rb32 [ B_BYTE ] = cb0 [ B_BYTE ] - cb1 [ B_BYTE ];
	rb32 [ A_BYTE ] = cb0 [ A_BYTE ] - cb1 [ A_BYTE ];
	
	return r32;
}

//----------------------------------------------------------------//
u32 ZLColor::SubAndClamp ( u32 c0, u32 c1 ) {
	
	u8* cb0 = ( u8* )&c0;
	u8* cb1 = ( u8* )&c1;

	u32 r = cb0 [ R_BYTE ] - cb1 [ R_BYTE ];
	u32 g = cb0 [ G_BYTE ] - cb1 [ G_BYTE ];
	u32 b = cb0 [ B_BYTE ] - cb1 [ B_BYTE ];
	u32 a = cb0 [ A_BYTE ] - cb1 [ A_BYTE ];
	
	u32 r32;
	u8* rb32 = ( u8* )&r32;
	
	rb32 [ R_BYTE ] = r > 0xff ? 0 : r;
	rb32 [ G_BYTE ] = g > 0xff ? 0 : g;
	rb32 [ B_BYTE ] = b > 0xff ? 0 : b;
	rb32 [ A_BYTE ] = a > 0xff ? 0 : a;
	
	return r32;
}

//----------------------------------------------------------------//
u32 ZLColor::Swizzle ( u32 c0, u32 sw ) {

	u8* cb0 = ( u8* )&c0;
	u8* swb = ( u8* )&sw;
	
	u32 r32;
	u8* rb32 = ( u8* )&r32;

	rb32 [ R_BYTE ] = cb0 [ swb [ R_BYTE & 0x02 ]];
	rb32 [ G_BYTE ] = cb0 [ swb [ G_BYTE & 0x02 ]];
	rb32 [ B_BYTE ] = cb0 [ swb [ B_BYTE & 0x02 ]];
	rb32 [ A_BYTE ] = cb0 [ swb [ A_BYTE & 0x02 ]];
	
	return r32;
}

//================================================================//
// ZLColorVec
//================================================================//

//----------------------------------------------------------------//
void ZLColorVec::Add ( const ZLColorVec& c ) {

	this->mR += c.mR;
	this->mG += c.mG;
	this->mB += c.mB;
	this->mA += c.mA;
}

//----------------------------------------------------------------//
bool ZLColorVec::Compare ( const ZLColorVec& c ) {

	if ((( mR != c.mR ) || ( mR != c.mR )) ||
		(( mG != c.mG ) || ( mG != c.mG )) ||
		(( mB != c.mB ) || ( mB != c.mB )) ||
		(( mA != c.mA ) || ( mA != c.mA ))) return false;

	return true;
}

//----------------------------------------------------------------//
bool ZLColorVec::Compare ( const ZLColorVec& c, float res ) {

	if ((( mR < ( c.mR - res )) || ( mR > ( c.mR + res ))) ||
		(( mG < ( c.mG - res )) || ( mG > ( c.mG + res ))) ||
		(( mB < ( c.mB - res )) || ( mB > ( c.mB + res ))) ||
		(( mA < ( c.mA - res )) || ( mA > ( c.mA + res )))) return false;

	return true;
}

//----------------------------------------------------------------//
void ZLColorVec::FromHSV ( float h, float s, float v ) {
	
	if( s == 0.0f ) {
		this->mR = v;
		this->mG = v;
		this->mB = v;
		return;
	}
	
	h /= 60.0f;
	
	int i = ( int )floor ( h );
	float f = h - ( float )i;
	
	float p = v * ( 1.0f - s );
	float q = v * ( 1.0f - s * f );
	float t = v * ( 1.0f - s * ( 1.0f - f ) );
	
	switch ( i ) {
	
		case 0:
			this->mR = v;
			this->mG = t;
			this->mB = p;
			break;
			
		case 1:
			this->mR = q;
			this->mG = v;
			this->mB = p;
			break;
			
		case 2:
			this->mR = p;
			this->mG = v;
			this->mB = t;
			break;
			
		case 3:
			this->mR = p;
			this->mG = q;
			this->mB = v;
			break;
			
		case 4:
			this->mR = t;
			this->mG = p;
			this->mB = v;
			break;
			
		case 5:
		default:
			this->mR = v;
			this->mG = p;
			this->mB = q;
			break;
	}
}

//----------------------------------------------------------------//
void ZLColorVec::FromYUV ( float y, float u, float v ) {

	this->mR = y + ( v * (( 1.0f - WR ) / V_MAX ));
	this->mG = y - ( u * (( WB * ( 1.0f - WB )) / ( U_MAX * WG ))) - ( v * (( WR * ( 1.0f - WR )) / ( V_MAX * WG )));
	this->mB = y + ( u * (( 1.0f - WB ) / U_MAX ));
}

//----------------------------------------------------------------//
float ZLColorVec::GetLuma () const {

	return ( WR * this->mR ) + ( WG * this->mG ) + ( WB * this->mB );
}

//----------------------------------------------------------------//
bool ZLColorVec::IsClear () const {

	return (( this->mR == 0.0f ) && ( this->mG == 0.0f ) && ( this->mB == 0.0f ) && ( this->mA == 0.0f ));
}

//----------------------------------------------------------------//
bool ZLColorVec::IsOpaque () const {

	return ( this->mA >= 1.0f );
}

//----------------------------------------------------------------//
void ZLColorVec::Lerp ( u32 mode, const ZLColorVec& v0, const ZLColorVec& v1, float t ) {

	this->mR = ZLInterpolate::Interpolate ( mode, v0.mR, v1.mR, t );
	this->mG = ZLInterpolate::Interpolate ( mode, v0.mG, v1.mG, t );
	this->mB = ZLInterpolate::Interpolate ( mode, v0.mB, v1.mB, t );
	this->mA = ZLInterpolate::Interpolate ( mode, v0.mA, v1.mA, t );
}

//----------------------------------------------------------------//
void ZLColorVec::Modulate ( const ZLColorVec& v0 ) {

	this->mR = this->mR * v0.mR;
	this->mG = this->mG * v0.mG;
	this->mB = this->mB * v0.mB;
	this->mA = this->mA * v0.mA;
}

//----------------------------------------------------------------//
u32 ZLColorVec::PackRGBA () const {

	return ZLColor::PackRGBA ( this->mR, this->mG, this->mB, this->mA );
}

//----------------------------------------------------------------//
void ZLColorVec::Scale ( float scale ) {

	this->mR *= scale;
	this->mG *= scale;
	this->mB *= scale;
	this->mA *= scale;
}

//----------------------------------------------------------------//
void ZLColorVec::SetRGBA ( u32 color ) {

	this->mR = ( float )(( color ) & 0xff ) / 255.0f;
	this->mG = ( float )(( color >> 8 ) & 0xff ) / 255.0f;
	this->mB = ( float )(( color >> 16 ) & 0xff ) / 255.0f;
	this->mA = ( float )(( color >> 24 ) & 0xff ) / 255.0f;
}

//----------------------------------------------------------------//
ZLColorVec::ZLColorVec ( u32 rgba ) {
	this->SetRGBA ( rgba );
}

//----------------------------------------------------------------//
void ZLColorVec::Set ( float r, float g, float b, float a ) {

	this->mR = r;
	this->mG = g;
	this->mB = b;
	this->mA = a;
}

//----------------------------------------------------------------//
void ZLColorVec::SetBlack () {

	this->Set ( 0.0f, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
void ZLColorVec::SetWhite () {

	this->Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
void ZLColorVec::ToHSV ( float& h, float& s, float& v ) {

	float r = this->mR;
	float g = this->mG;
	float b = this->mB;

	float min = ZLFloat::Min ( r, g, b );
	float max = ZLFloat::Min ( r, g, b );
	float delta = max - min;
	
	v = max;
	
	if ( max != 0.0f ) {
		s = delta / max;
	}
	else {
		// r = g = b = 0
		// s = 0, v is undefined
		s = 0.0f;
		h = -1.0f;
		return;
	}
	
	if ( r == max ) {
		h = ( g - b ) / delta; // between yellow & magenta
	}
	else if ( g == max ) {
		h = 2.0f + ( b - r ) / delta; // between cyan & yellow
	}
	else {
		h = 4.0f + ( r - g ) / delta; // between magenta & cyan
	}

	h *= 60.0f; // degrees
	
	if ( h < 0.0f ) {
		h += 360.0f;
	}
}

//----------------------------------------------------------------//
void ZLColorVec::ToYUV ( float& y, float& u, float& v ) {
	
	y = ( WR * this->mR ) + ( WG * this->mG ) + ( WB * this->mB );
	u = U_MAX * (( this->mB - y ) / ( 1.0f - WB ));
	v = V_MAX * (( this->mR - y ) / ( 1.0f - WR ));
}

//----------------------------------------------------------------//
ZLColorVec::ZLColorVec () {
}

//----------------------------------------------------------------//
ZLColorVec::ZLColorVec ( float r, float g, float b, float a ) {

	this->mR = r;
	this->mG = g;
	this->mB = b;
	this->mA = a;
}
