// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGE_H
#define	MOAIIMAGE_H

//================================================================//
// MOAIImageTransform
//================================================================//
namespace MOAIImageTransform {
	enum Flags {
		POW_TWO				= 0x01,
		QUANTIZE			= 0x02,
		TRUECOLOR			= 0x04,
		PREMULTIPLY_ALPHA	= 0x08,
	};
}

//================================================================//
// MOAIImage
//================================================================//
/**	@name	MOAIImage
	@text	Image/bitmap class.
	
	@const	FILTER_LINEAR
	@const	FILTER_NEAREST
	
	@flag	POW_TWO
	@flag	QUANTIZE
	@flag	TRUECOLOR
	@flag	PREMULTIPLY_ALPHA
	
	@flag	PIXEL_FMT_TRUECOLOR
	@flag	PIXEL_FMT_INDEX_4
	@flag	PIXEL_FMT_INDEX_8
	
	@flag	COLOR_FMT_A_8
	@flag	COLOR_FMT_RGB_888
	@flag	COLOR_FMT_RGB_565
	@flag	COLOR_FMT_RGBA_5551
	@flag	COLOR_FMT_RGBA_4444
	@flag	COLOR_FMT_RGBA_8888
*/
class MOAIImage :
	public virtual MOAILuaObject {
private:

	USPixel::Format		mPixelFormat;
	ZLColor::Format		mColorFormat;

	u32		mWidth;
	u32		mHeight;
	
	void*	mData;
	void*	mPalette;
	void*	mBitmap;

	//----------------------------------------------------------------//
	static int		_bleedRect			( lua_State* L );
	static int		_compare			( lua_State* L );
	static int		_convertColors		( lua_State* L );
	static int		_copy				( lua_State* L );
	static int		_copyBits			( lua_State* L );
	static int		_copyRect			( lua_State* L );
	static int		_fillCircle			( lua_State* L );
	static int		_fillRect			( lua_State* L );
	static int		_getColor32			( lua_State* L );
	static int		_getFormat			( lua_State* L );
	static int		_getRGBA			( lua_State* L );
	static int		_getSize			( lua_State* L );
	static int		_init				( lua_State* L );
	static int		_load				( lua_State* L );
	static int		_loadFromBuffer		( lua_State* L );
	static int		_padToPow2			( lua_State* L );
	static int		_resize				( lua_State* L );
	static int		_resizeCanvas		( lua_State* L );
	static int		_setColor32			( lua_State* L );
	static int		_setRGBA			( lua_State* L );
	static int		_writePNG			( lua_State* L );
	static int		_convertToGrayScale ( lua_State* L );

	//----------------------------------------------------------------//
	void			Alloc				();
	static u32		GetMinPowerOfTwo	( u32 size ); // gets the smallest power of two greater than size
	void			Init				( void* bitmap, u32 width, u32 height, ZLColor::Format colorFmt, bool copy );
	static bool		IsJpg				( ZLStream& stream );
	static bool		IsPng				( ZLStream& stream );
	
	//----------------------------------------------------------------//
	#if MOAI_WITH_LIBJPG
		void			LoadJpg				( ZLStream& stream, u32 transform );
		void			LoadJpg				( void* jpgInfoParam, u32 transform );
	#endif
	
	//----------------------------------------------------------------//
	#if MOAI_WITH_LIBPNG
		void			LoadPng				( ZLStream& stream, u32 transform );
		void			LoadPng				( void* pngParam, void* pngInfoParam, u32 transform );
	#endif

public:
	
	DECL_LUA_FACTORY ( MOAIImage )
	
	GET_CONST ( USPixel::Format, PixelFormat, mPixelFormat )
	GET_CONST ( ZLColor::Format, ColorFormat, mColorFormat )

	GET_CONST ( u32, Width, mWidth )
	GET_CONST ( u32, Height, mHeight )

	GET ( void*, Data, mData )
	GET ( void*, Palette, mPalette )
	GET ( void*, Bitmap, mBitmap )
	
	enum {
		FILTER_LINEAR,
		FILTER_NEAREST,
	};
	
	//----------------------------------------------------------------//
	void				BleedRect				( int xMin, int yMin, int xMax, int yMax );
	void				Clear					();
	void				ClearBitmap				();
	void				ClearRect				( ZLIntRect rect );
	bool				Compare					( const MOAIImage& image );
	void				ConvertColors			( const MOAIImage& image, ZLColor::Format colorFmt );
	void				Copy					( const MOAIImage& image );
	void				CopyBits				( const MOAIImage& image, int srcX, int srcY, int destX, int destY, int width, int height );
	void				CopyRect				( const MOAIImage& image, ZLIntRect srcRect, ZLIntRect destRest, u32 filter );
	void				DrawLine				( int p1x, int p1y, int p2x, int p2y, u32 color );
	void				FillCircle				( float x, float y, float xRad, u32 color );
	void				FillRect				( ZLIntRect rect, u32 color );
	u32					GetBitmapSize			() const;
	ZLIntRect			GetBounds				();
	u32					GetColor				( u32 i ) const;
	u32					GetColor				( u32 x, u32 y ) const;
	u32					GetDataSize				() const;
	u32					GetPaletteCount			() const;
	u32					GetPaletteColor			( u32 idx ) const;
	u32					GetPaletteSize			() const;
	u32					GetPixel				( u32 x, u32 y ) const;
	ZLIntRect			GetRect					();
	void*				GetRowAddr				( u32 y );
	const void*			GetRowAddr				( u32 y ) const;
	u32					GetRowSize				() const;
	void				GetSubImage				( ZLIntRect rect, void* buffer );
	u32					GetSubImageSize			( ZLIntRect rect );
	void				Init					( u32 width, u32 height, ZLColor::Format colorFmt, USPixel::Format pixelFmt );
	void				Init					( void* bitmap, u32 width, u32 height, ZLColor::Format colorFmt );
	bool				IsPow2					();
	static bool			IsPow2					( u32 n );
	void				Load					( cc8* filename, u32 transform = 0 );
	void				Load					( ZLStream& stream, u32 transform = 0 );
	bool				IsOK					();
	bool				MipReduce				();
						MOAIImage				();
						~MOAIImage				();
	void				PadToPow2				( const MOAIImage& image );
	void				PremultiplyAlpha		( const MOAIImage& image );
	void				ConvertToGrayScale		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				ResizeCanvas			( const MOAIImage& image, ZLIntRect rect );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void				SetColor				( u32 x, u32 y, u32 color );
	void				SetPaletteColor			( u32 idx, u32 rgba );
	void				SetPixel				( u32 x, u32 y, u32 pixel );
	void				Take					( MOAIImage& image );
	void				ToTrueColor				( const MOAIImage& image );
	void				Transform				( u32 transform );
	
	#if MOAI_WITH_LIBPNG
		bool				WritePNG				( ZLStream& stream );
	#endif
};

#endif
