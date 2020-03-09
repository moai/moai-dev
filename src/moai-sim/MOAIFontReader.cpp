// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIFontReader.h>

//================================================================//
// MOAIGlyphMetrics
//================================================================//

//----------------------------------------------------------------//
/**
 * Get the rect of the glyph which includes the bearing + the size of the bounding box of the glyph.
 * 
 * @param x The x pen position when drawing this glyph
 * @param y The y pen position when drawing this glyph
 * @param scale The scale at which the glyph would be drawn
 */
ZLRect MOAIGlyphMetrics::GetGlyphRect ( float x, float y, float xScale, float yScale ) const {

	ZLRect rect;

	x += ( this->mBearingX * xScale );
	y -= ( this->mBearingY * yScale );

	rect.Init (
		x,
		y,
		x + ( this->mWidth * xScale ),
		y + ( this->mHeight * yScale )
	);

	return rect;
}

//----------------------------------------------------------------//
ZLRect MOAIGlyphMetrics::GetLogicalRect ( float x, float y, float ascent, float descent, float xScale, float yScale ) const {

	ZLRect rect;

	x += ( this->mBearingX * xScale );

	rect.Init (
		x,
		y - ( ascent * yScale ),
		x + ( this->mWidth * xScale ),
		y + ( descent * yScale )
	);

	return rect;
}

//----------------------------------------------------------------//
MOAIGlyphMetrics::MOAIGlyphMetrics () :
	mAdvanceX ( 0.0f ),
	mBearingX ( 0.0f ),
	mBearingY ( 0.0f ),
	mWidth ( 0.0f ),
	mHeight ( 0.0f ) {
};

//================================================================//
// MOAIFontFaceMetrics
//================================================================//

//----------------------------------------------------------------//
MOAIFontFaceMetrics::MOAIFontFaceMetrics () :
	mHeight ( 0.0f ),
	mAscent ( 0.0f ) {
};

//================================================================//
// MOAIFontReader lua
//================================================================//

//----------------------------------------------------------------//
int MOAIFontReader::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "U" )
	u32 result = self->CloseFontFile ();
	state.Push ( result != MOAIFontReader::OK );
	return 1;
}

//----------------------------------------------------------------//
int MOAIFontReader::_getFaceMetrics ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "U" )
	
	MOAIFontFaceMetrics faceMetrics;
	int result = self->GetFaceMetrics ( faceMetrics );
	
	if ( result == MOAIFontReader::OK ) {
		state.Push ( faceMetrics.mHeight );
		state.Push ( faceMetrics.mAscent );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIFontReader::_getGlyphMetrics ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "U" )
	
	MOAIGlyphMetrics glyphMetrics;
	int result = self->GetGlyphMetrics ( glyphMetrics );
	
	if ( result == MOAIFontReader::OK ) {
		state.Push ( glyphMetrics.mWidth );
		state.Push ( glyphMetrics.mHeight );
		state.Push ( glyphMetrics.mAdvanceX );
		state.Push ( glyphMetrics.mBearingX );
		state.Push ( glyphMetrics.mBearingY );
		return 5;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIFontReader::_getKernVec ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "UN" )
	
	u32 c = state.GetValue < u32 >( 2, MOAIFontReader::GLYPH_CODE_NULL );
	
	MOAIKernVec kernVec;
	int result = self->GetKernVec ( c, kernVec );
	
	if ( result == MOAIFontReader::OK ) {
		state.Push ( kernVec.mX );
		state.Push ( kernVec.mY );
		return 2;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIFontReader::_hasKerning ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "U" )
	state.Push ( self->HasKerning ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIFontReader::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "US" )
	u32 result = self->OpenFontFile ( state.GetValue < cc8* >( 2, NULL ));
	state.Push ( result != MOAIFontReader::OK );
	return 1;
}

//----------------------------------------------------------------//
int MOAIFontReader::_renderGlyph ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "UU" )
	
	int result = MOAIFontReader::FONT_ERROR;
	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2, true );
	
	if ( image ) {
	
		float x = state.GetValue < float >( 3, 0.0f );
		float y = state.GetValue < float >( 4, 0.0f );

		result = self->RenderGlyph ( *image, x, y );
	}
	state.Push ( result != MOAIFontReader::OK );
	return 1;
}

//----------------------------------------------------------------//
int MOAIFontReader::_selectFace ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "U" )
	u32 result = self->SelectFace ( state.GetValue < float >( 2, 0.0f ));
	state.Push ( result != MOAIFontReader::OK );
	return 1;
}

//----------------------------------------------------------------//
int MOAIFontReader::_selectGlyph ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "UN" )
	u32 result = self->SelectGlyph ( state.GetValue < u32 >( 2, MOAIFontReader::GLYPH_CODE_NULL ));
	state.Push ( result != MOAIFontReader::OK );
	return 1;
}

//----------------------------------------------------------------//
int MOAIFontReader::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFontReader, "U" )
	
	self->mBlendMode.mEquation			= ( ZLColor::BlendEquation )state.GetValue < u32 >( 2, ( u32 )ZLColor::BLEND_EQ_NONE );
	self->mBlendMode.mSourceFactor		= ( ZLColor::BlendFactor )state.GetValue < u32 >( 3, ( u32 )ZLColor::BLEND_FACTOR_ZERO );
	self->mBlendMode.mDestFactor		= ( ZLColor::BlendFactor )state.GetValue < u32 >( 4, ( u32 )ZLColor::BLEND_FACTOR_ZERO );
	
	return 0;
}

//================================================================//
// MOAIFontReader
//================================================================//

//----------------------------------------------------------------//
int MOAIFontReader::CloseFontFile () {

	return OK;
}

//----------------------------------------------------------------//
bool MOAIFontReader::HasKerning () {

	return false;
}

//----------------------------------------------------------------//
int MOAIFontReader::GetKernVec ( u32 c, MOAIKernVec& kernVec ) {
	UNUSED ( c );
	UNUSED ( kernVec );

	kernVec.mX = 0.0f;
	kernVec.mY = 0.0f;

	return NO_KERNING;
}

//----------------------------------------------------------------//
MOAIFontReader::MOAIFontReader () {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIFontReader )
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFontReader::~MOAIFontReader () {

	this->CloseFontFile ();
}

//----------------------------------------------------------------//
int MOAIFontReader::OpenFontFile ( cc8* filename ) {
	UNUSED ( filename );
	return OK;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFontReader::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIFontReader::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "getFaceMetrics",		_getFaceMetrics },
		{ "getGlyphMetrics",	_getGlyphMetrics },
		{ "getKernVec",			_getKernVec },
		{ "hasKerning",			_hasKerning },
		{ "open",				_open },
		{ "renderGlyph",		_renderGlyph },
		{ "selectFace",			_selectFace },
		{ "selectGlyph",		_selectGlyph },
		{ "setBlendMode",		_setBlendMode },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFontReader::MOAILuaObject_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIFontReader::MOAILuaObject_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
	if ( history.DidVisit ( *this )) return;
}
