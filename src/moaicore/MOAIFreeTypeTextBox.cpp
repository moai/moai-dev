// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFreeTypeFont.h>
#include <moaicore/MOAIFreeTypeTextBox.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAIQuadBrush.h>
//#include <ft2build.h>
//#include FT_FREETYPE_H
#include FT_GLYPH_H


int MOAIFreeTypeTextBox::_getAutoFit( lua_State *L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "U" )
	state.Push ( self->mAutoFit );
	return 1;
}

int MOAIFreeTypeTextBox::_getGlyphScale( lua_State *L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "U" )
	state.Push ( self->mGlyphScale );
	return 1;
}

int MOAIFreeTypeTextBox::_getRect( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "U" )
	
	USRect rect = self->mFrame;
	rect.Bless ();
	
	lua_pushnumber ( state, rect.mXMin );
	lua_pushnumber ( state, rect.mYMin );
	lua_pushnumber ( state, rect.mXMax );
	lua_pushnumber ( state, rect.mYMax );
	
	return 4;
}

int MOAIFreeTypeTextBox::_getTexture(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "U" )
	state.Push ( self->mTexture );
	return 1;
}

int MOAIFreeTypeTextBox::_getTextSize(lua_State *L){
	MOAI_LUA_SETUP( MOAIFreeTypeTextBox, "U" )
	state.Push( self->mFontSize );
	return 1;
}


int	MOAIFreeTypeTextBox::_setAutoFit( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "UB" )
	self->mAutoFit = state.GetValue < bool >( 2, false );
	return 0;
}

/**	@name	setFont
	@text	Sets or clears the style's font.
 
	@in		MOAIFreeTypeTextBox self
	@opt	MOAIFreeTypeFont font		Default value is nil.
	@out	nil
 */

int	MOAIFreeTypeTextBox::_setFont( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "U" )
	MOAIFreeTypeFont *font = state.GetLuaObject< MOAIFreeTypeFont >(2, true);
	self->SetFont(font);
	return 0;
}

int	MOAIFreeTypeTextBox::_setGlyphScale( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "UN" )
	self->mGlyphScale = state.GetValue < float >( 2, 1.0f );
	return 0;
}


int	MOAIFreeTypeTextBox::_setRect( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "UNNNN" )
	
	float left		= state.GetValue < float >( 2, 0.0f );
	float top		= state.GetValue < float >( 3, 0.0f );
	float right		= state.GetValue < float >( 4, 0.0f );
	float bottom	= state.GetValue < float >( 5, 0.0f );
	
	// TODO: implement ScheduleLayout ();
	self->SetRect ( left, top, right, bottom );
	//self->ScheduleLayout ();
	return 0;
}

int MOAIFreeTypeTextBox::_setString( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "US" )
	
	cc8* text = state.GetValue < cc8* >( 2, "" );
	self->SetText ( text );
	
	//self->ResetStyleMap ();
	self->ScheduleLayout ();
	
	return 0;
}

int MOAIFreeTypeTextBox::_setTextSize( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "UN" )
	self->SetTextSize( state.GetValue < float >( 2, 0.0f ) );
	return 0;
}

void MOAIFreeTypeTextBox::BuildLayout(){
	
	int	pen_x, pen_y;
	int n;
	int num_chars = 1; // strlen(this->mText);
	
	FT_Error error;
	
	// initialize library
	FT_Library library;
	error = FT_Init_FreeType( &library );
	if (error){
		return;
	}
	
	// create face object
	FT_Face face = this->mFont->mFreeTypeFace;
	if (!face) {
		error = FT_New_Face(library,
							this->mFont->mFilename,
							0,
							&face);
		if (error) {
			return;
		}
		
		// set the face
		this->mFont->mFreeTypeFace = face;
	}
	
	// retrieve the glyph slot
	FT_GlyphSlot slot = face->glyph;
	//int char_height = 64 * this->mFontSize;
	
	// set character size
	error = FT_Set_Char_Size(face,					/* handle to face object           */
							 0,						/* char_width in 1/64th of points  */
							 64 * this->mFontSize,	/* char_height in 1/64th of points */
							 DPI,					/* horizontal device resolution    */
							 0);					/* vertical device resolution      */
	if (error) {
		return;
	}
	
	
	// initialize pen position
	pen_x = 300;
	pen_y = 200;
	
	for (n = 0; n < num_chars; ) {
		FT_UInt glyph_index;
		
		// retrieve the next Unicode character value and update the integer n
		u32 c = u8_nextchar(this->mText, &n);
		
		// retrieve glyph index from character code
		glyph_index = FT_Get_Char_Index(face, c);
		
		// load glyph index into slot
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
			continue;
		
		// convert to anti-aliased bitmap
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error) {
			continue;
		}
		
		// draw to target surface
		this->DrawBitmap ( &slot->bitmap,
						pen_x + slot->bitmap_left,
						pen_y - slot->bitmap_top);
		
		// increment pen position; 
		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6;
		
	}
}

void MOAIFreeTypeTextBox::Draw(int subPrimID){
	UNUSED(subPrimID);
	
	if ( !( this->mFlags & FLAGS_VISIBLE )) return;
	// TODO: implement Draw()
	
	// copied from MOAITextBox::Draw for the most part
	if (this->mTexture) {
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		
		if ( this->mUVTransform ) {
			USAffine3D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
			gfxDevice.SetUVTransform ( uvMtx );
		}
		else {
			gfxDevice.SetUVTransform ();
		}
		
		this->LoadGfxState();
		
		if ( !this->mShader ) {
			gfxDevice.SetShaderPreset ( MOAIShaderMgr::FONT_SHADER );
		}
		
		gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, this->GetLocalToWorldMtx ());
		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		
		MOAIQuadBrush::BindVertexFormat ( gfxDevice );
		
		//USColorVec baseColor = gfxDevice.GetPenColor ();
		//USColorVec blendColor;
		//u32 rgba0 = 0xffffffff;
		//u32 rgba1 = 0xffffffff;
		
		gfxDevice.SetTexture(this->mTexture);
		
		
		
		MOAIQuadBrush glQuad;
		
		// find the coordinates for glQuad.
		float x1, x2, y1, y2;
		x1 = this->mFrame.mXMin;
		x2 = this->mFrame.mXMax;
		y1 = this->mFrame.mYMin;
		y2 = this->mFrame.mYMax;
		
		glQuad.SetVerts( x1, y1, x2, y2 );
		
		float u1, u2, v1, v2;
		u1 = 0.0f;
		v1 = 0.0f;
		u2 = this->mFrame.Width();
		v2 = this->mFrame.Height();
		
		glQuad.SetUVs( u1, v1, u2, v2 );
		glQuad.Draw();
	}
	
}

void MOAIFreeTypeTextBox::DrawBitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y){
	//UNUSED(bitmap);
	UNUSED(x);
	UNUSED(y);
	
	FT_Int i, j, k;
	
	// create a buffer to use in initializing a MOAIImage 
	const int BYTES_PER_PIXEL = 4;
	
	size_t size = bitmap->width * bitmap->rows * BYTES_PER_PIXEL;
	unsigned char* imgBuffer = (unsigned char*)malloc(size);
	int idx = 0;
	u8 value;
	
	// fill the values with data from bitmap->buffer
	for (i = 0; i < bitmap->width; i++) {
		for (j = 0; j < bitmap->rows; j++) {
			idx = (j * bitmap->width + i) * BYTES_PER_PIXEL;
			value = bitmap->buffer[j * bitmap->width + i];
			for (k = 0; k < BYTES_PER_PIXEL - 1; k++){
				imgBuffer[idx+k] = value; // RGB
			}
			imgBuffer[idx+3] = 255; // alpha
		}
	}
	
	// Initialize the image with the created buffer
	MOAIImage bitmapImg;
	bitmapImg.Init(imgBuffer, bitmap->width, bitmap->rows, USColor::RGBA_8888);  // is A_8 the correct color mode?
	free(imgBuffer);
	
	// create a texture
	if (!this->mTexture) {
		this->mTexture = new MOAITexture ();
		
	}
	else{ // texture already exists
		// TODO: implement this case?
	}
	this->mTexture->Init(bitmapImg, "debug1");
	
}


void MOAIFreeTypeTextBox::Layout(){
	// TODO: implement Layout()
	
	// text is NULL
	if ( !this->mText ){
		// reset layout
		
	}
	else if (this->mNeedsLayout) {
		
		// build style map if necessary
		
		// reset layout
		
		
		// build layout (Done with MOAITextDesigner in MOAITextBox)
		this->BuildLayout();
		
		// apply highlights
		
	}
	
	this->mNeedsLayout = false;
}


MOAIFreeTypeTextBox::MOAIFreeTypeTextBox() :
mLineSpacing ( 0.0f ),
mText ( "" ),
mTextLength ( 0 ),
mHAlign ( LEFT_JUSTIFY ),
mVAlign ( LEFT_JUSTIFY ),
mYFlip ( false ),
mGlyphScale ( 1.0f ),
mWordBreak ( WORD_BREAK_NONE ),
mAutoFit( false ),
mFont( NULL ),
mFontSize(0.0f),
mNeedsLayout ( false ),
mTexture( NULL ){
	RTTI_BEGIN
		RTTI_EXTEND( MOAIProp )
	RTTI_END
	
	this->mFrame.Init( 0.0f, 0.0f, 0.0f, 0.0f);
}


MOAIFreeTypeTextBox::~MOAIFreeTypeTextBox(){
	
}

void MOAIFreeTypeTextBox::OnDepNodeUpdate(){
	
	MOAIProp::OnDepNodeUpdate ();
	
	// implement style changed?
	
	this->Layout ();
	if ( this->mYFlip ) {
		
		USAffine3D mtx;
		
		mtx.ScRoTr ( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, ( this->mFrame.mYMin + this->mFrame.mYMax ), 0.0f );
		this->mLocalToWorldMtx.Prepend ( mtx );
		
		this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
	}
}


void MOAIFreeTypeTextBox::RegisterLuaClass( MOAILuaState &state ){
	MOAIProp::RegisterLuaClass ( state );
	
	state.SetField( -1, "LEFT_JUSTIFY", ( u32 )LEFT_JUSTIFY );
	state.SetField ( -1, "CENTER_JUSTIFY", ( u32 )CENTER_JUSTIFY );
	state.SetField ( -1, "RIGHT_JUSTIFY", ( u32 )RIGHT_JUSTIFY );
	
	state.SetField ( -1, "WORD_BREAK_NONE", ( u32 )WORD_BREAK_NONE );
	state.SetField ( -1, "WORD_BREAK_CHAR", ( u32 )WORD_BREAK_CHAR );
}

void MOAIFreeTypeTextBox::RegisterLuaFuncs( MOAILuaState &state ){
	
	luaL_Reg regTable [] = {
		{ "getAutoFit",				_getAutoFit },
		{ "getGlyphScale",			_getGlyphScale },
		{ "getRect",				_getRect },
		{ "getTexture",				_getTexture },
		{ "getTextSize",			_getTextSize },
		{ "setAutoFit",				_setAutoFit },
		{ "setFont",				_setFont },
		{ "setGlyphScale",			_setGlyphScale },
		{ "setRect",				_setRect },
		{ "setString",				_setString },
		{ "setTextSize",			_setTextSize },
		{ NULL, NULL }
	};
	
	luaL_register(state, 0, regTable );
}

void MOAIFreeTypeTextBox::ScheduleLayout(){
	this->mNeedsLayout = true;
	this->ScheduleUpdate();
}

void MOAIFreeTypeTextBox::SetFont( MOAIFreeTypeFont* font ){
	if (this->mFont != font) {
		this->LuaRetain( font );
		this->LuaRelease( this->mFont );
		this->mFont = font;
		
		// get Font Size info too
		if (this->mFontSize <= 0.0f && font->mDefaultSize != this->mFontSize) {
			this->mFontSize = font->mDefaultSize;
		}
	}
}


void MOAIFreeTypeTextBox::SetRect(float left, float top, float right, float bottom){
	this->mFrame.Init(left, top, right, bottom);
}

void MOAIFreeTypeTextBox::SetText(cc8 *text){
	this->mText = text;
	
	// other set-up as necesary
}

void MOAIFreeTypeTextBox::SetTextSize(float size){
	if (this->mFontSize != size) {
		this->mFontSize = size;
	}	
}
