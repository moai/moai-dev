// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIFontReader.h>

//================================================================//
// MOAIFontFaceMetrics
//================================================================//
MOAIFontFaceMetrics::MOAIFontFaceMetrics () :
	mHeight ( 0.0f ),
	mAscent ( 0.0f ) {
};

//================================================================//
// MOAIGlyphMetrics
//================================================================//
MOAIGlyphMetrics::MOAIGlyphMetrics () :
	mAdvanceX ( 0.0f ),
	mBearingX ( 0.0f ),
	mBearingY ( 0.0f ),
	mWidth ( 0.0f ),
	mHeight ( 0.0f ) {
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
		
		ZLColorBlendFunc blendFunc;
		blendFunc.mEquation = ZLColor::BLEND_EQ_NONE;
		
		if ( state.CheckParams ( 5, "NN", false )) {
			blendFunc.mSrcFactor	= ( ZLColor::BlendFactor )state.GetValue < u32 >( 5, ( u32 )ZLColor::BLEND_FACTOR_SRC_ALPHA );
			blendFunc.mDstFactor	= ( ZLColor::BlendFactor )state.GetValue < u32 >( 6, ( u32 )ZLColor::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );
			blendFunc.mEquation		= ( ZLColor::BlendEquation )state.GetValue < u32 >( 7, ( u32 )ZLColor::BLEND_EQ_ADD );
		}
		result = self->RenderGlyph ( *image, x, y, blendFunc );
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
	
	RTTI_BEGIN
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

//----------------------------------------------------------------//
void MOAIFontReader::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFontReader::RegisterLuaFuncs ( MOAILuaState& state ) {
	
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
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
int MOAIFontReader::RenderGlyph ( MOAIImage& image, float x, float y ) {

	ZLColorBlendFunc blendFunc;
	blendFunc.mEquation = ZLColor::BLEND_EQ_NONE;
	return this->RenderGlyph ( image, x, y, blendFunc );
}

//----------------------------------------------------------------//
void MOAIFontReader::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIFontReader::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}
