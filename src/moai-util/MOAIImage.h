// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGE_H
#define	MOAIIMAGE_H

//================================================================//
// MOAIImage
//================================================================//
/**	@lua	MOAIImage
	@text	Image/bitmap class.
	
	@const	FILTER_LINEAR
	@const	FILTER_NEAREST
	
	@flag	POW_TWO				Adds padding at the right and bottom to make the image dimensions powers of 2.
	@flag	QUANTIZE			Uses less than 8 bits per channel to reduce memory consumption.
	@flag	TRUECOLOR			Converts palettized color formats to true color.
	@flag	PREMULTIPLY_ALPHA	Premultiplies the pixel colors with their alpha values.
	
	@flag	PIXEL_FMT_TRUECOLOR
	@flag	PIXEL_FMT_INDEX_4
	@flag	PIXEL_FMT_INDEX_8
	
	@flag	COLOR_FMT_A_1			Alpha only, 1 bit per pixel
	@flag	COLOR_FMT_A_4			Alpha only, 4 bits per pixel
	@flag	COLOR_FMT_A_8			Alpha only, 8 bits per pixel
	@flag	COLOR_FMT_LA_8			Grayscale + alpha, 16 bits per pixel
	@flag	COLOR_FMT_RGB_888		RGB, 24 bits per pixel
	@flag	COLOR_FMT_RGB_565		RGB, 16 bits per pixel
	@flag	COLOR_FMT_RGBA_5551		RGBA, 16 bits per pixel (1 bit alpha)
	@flag	COLOR_FMT_RGBA_4444		RGBA, 16 bits per pixel (4 bits per channel)
	@flag	COLOR_FMT_RGBA_8888		RGBA, 32 bits per pixel
*/
class MOAIImage :
	public virtual MOAILuaObject,
	public virtual ZLImage {
protected:

	//----------------------------------------------------------------//
	static int		_average					( lua_State* L );
	static int		_bleedRect					( lua_State* L );
	static int		_calculateGaussianKernel	( lua_State* L );
	static int		_compare					( lua_State* L );
	static int		_convert					( lua_State* L );
	static int		_convolve					( lua_State* L );
	static int		_convolve1D					( lua_State* L );
	static int		_copy						( lua_State* L );
	static int		_copyBits					( lua_State* L );
	static int		_copyRect					( lua_State* L );
	static int		_desaturate					( lua_State* L );
	static int		_fillCircle					( lua_State* L );
	static int		_fillEllipse				( lua_State* L );
	static int		_fillRect					( lua_State* L );
	static int		_gammaCorrection			( lua_State* L );
	static int		_generateOutlineFromSDF		( lua_State* L );
	static int		_generateSDF				( lua_State* L );
	static int		_generateSDFAA				( lua_State* L );
	static int		_generateSDFDeadReckoning	( lua_State* L );
	static int		_getColor32					( lua_State* L );
	static int		_getContentRect				( lua_State* L );
	static int		_getData					( lua_State* L );
	static int		_getFormat					( lua_State* L );
	static int		_getRGBA					( lua_State* L );
	static int		_getSize					( lua_State* L );
	static int		_init						( lua_State* L );
	static int		_isOpaque					( lua_State* L );
	static int		_load						( lua_State* L );
	static int		_loadAsync					( lua_State* L );
	static int		_loadFromBuffer				( lua_State* L );
	static int		_mix						( lua_State* L );
	static int		_padToPow2					( lua_State* L );
	static int		_print						( lua_State* L );
	static int		_resize						( lua_State* L );
	static int		_resizeCanvas				( lua_State* L );
	static int		_setColor32					( lua_State* L );
	static int		_setRGBA					( lua_State* L );
	static int		_simpleThreshold			( lua_State* L );
	static int		_subdivideRect				( lua_State* L );
	static int		_write						( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn				( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut				( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIImage )
	
	//----------------------------------------------------------------//
	static MOAIImage*	AffirmImage					( MOAILuaState& state, int idx );
						MOAIImage					();
						~MOAIImage					();
};

#endif
