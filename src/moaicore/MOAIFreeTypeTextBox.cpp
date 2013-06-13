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


#define BYTES_PER_PIXEL 4

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

int MOAIFreeTypeTextBox::_setAlignment( lua_State* L ){
	MOAI_LUA_SETUP( MOAIFreeTypeTextBox, "UN" );
	self->mHAlign = state.GetValue < u32 >( 2, MOAIFreeTypeTextBox::LEFT_JUSTIFY );
	self->mVAlign = state.GetValue < u32 >( 3, MOAIFreeTypeTextBox::LEFT_JUSTIFY );
	
	self->ScheduleLayout();
	return 0;
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


int MOAIFreeTypeTextBox::_setWordBreak( lua_State* L ){
	MOAI_LUA_SETUP( MOAIFreeTypeTextBox, "U" )
	self->mWordBreak = state.GetValue < u32> ( 2 , MOAIFreeTypeTextBox::WORD_BREAK_NONE );
	return 0;
}

// ------------------------------------------------------------------------ //

void MOAIFreeTypeTextBox::BuildLayout(){
	
	int	pen_x, pen_y;
	int n;
	int num_chars = strlen(this->mText);
	
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
	FT_Size fontSize = face->size;
	FT_Size_Metrics sizeMetrics = fontSize->metrics;
	// initialize pen position
	FT_Int lineHeight = (sizeMetrics.height >> 6);  // find out line height
	pen_x = 0;
	pen_y = lineHeight + 1;
	
	FT_Int imgWidth = (FT_Int)this->mFrame.Width();
	FT_Int imgHeight = (FT_Int)this->mFrame.Height();
	
	
	
	// initialize the bitmap data
	if( this->mBitmapDataNeedsUpdate )
		this->InitBitmapData();
	
	// TODO: multi-line rendering
	this->GenerateLines();
	
	this->RenderLines();
	
	/*
	int last_whitespace = -1;
	this->mTokenIdx = 0;
	this->mLineIdx = 0;
	
	for (n = 0; n < num_chars; ) {
		FT_UInt glyph_index;
		
		// retrieve the next Unicode character value and update the integer n
		u32 c = u8_nextchar(this->mText, &n);
		
		// handle new-line character
		if (c == '\n') {
			this->mLineIdx = n;
			this->mTokenIdx = n;
			pen_x = 0;
			pen_y += lineHeight;
			continue;
		}
		
		if (c == ' ') {
			this->mTokenIdx = n;
			last_whitespace = n-1;
		}
		
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
		
		// check to see if character will be rendered out of bounds in X direction
		if (pen_x + (slot->advance.x >> 6) >  imgWidth) {
			
			pen_x = 0;
			pen_y += lineHeight;
			
			
		}
		// check to see if character will be rendered out of bounds in Y direction
		if (pen_y > imgHeight) {
			break;
		}
		
		
		// draw to target surface
		this->DrawBitmap ( &slot->bitmap,
						pen_x + slot->bitmap_left,
						pen_y - slot->bitmap_top);
		
		// increment pen position; 
		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6;
		
	}
	*/
	
	
	// this is where to create the texture
	this->CreateTexture();
	
	// TODO: clean-up?
}

// creates a line in m_vLines
void MOAIFreeTypeTextBox::BuildLine(wchar_t *buffer, size_t buf_len, FT_Face face, int pen_x, u32 lastChar){
	MOAIFreeTypeTextLine tempLine;
	wchar_t* text = (wchar_t*)malloc(sizeof(wchar_t) * (buf_len+1));
	memcpy(text, buffer, sizeof(wchar_t) * buf_len);
	text[buf_len] = '\0';
	tempLine.text = text;
	
	// get last glyph
	int error = FT_Load_Char(face, lastChar, FT_LOAD_DEFAULT);
	
	if (error) {
		return;
	}
	
	tempLine.lineWidth = pen_x - ((face->glyph->metrics.horiAdvance - face->glyph->metrics.horiBearingX - face->glyph->metrics.width) >> 6);
	
	m_vLines.push_back(tempLine);
}

int MOAIFreeTypeTextBox::ComputeLineStart(FT_Face face, FT_UInt unicode, int lineIndex){
	UNUSED(face);
	UNUSED(unicode);
	UNUSED(lineIndex);
	int retValue = 0;
	return retValue;
}

int MOAIFreeTypeTextBox::ComputeLineStartY(FT_Face face, int textHeight){
	UNUSED(face);
	UNUSED(textHeight);
	int retValue = 0;
	return retValue;
}

void MOAIFreeTypeTextBox::CreateTexture(){
	// Initialize the image with the created buffer
	MOAIImage bitmapImg;
	bitmapImg.Init(this->mBitmapData, this->mBitmapWidth, this->mBitmapHeight, USColor::RGBA_8888);  // is A_8 the correct color mode?
	//free(imgBuffer);
	
	// create a texture
	if (!this->mTexture) {
		this->mTexture = new MOAITexture ();
		
	}
	else{ // texture already exists
		// TODO: implement this case?
	}
	this->mTexture->Init(bitmapImg, "debug1");
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
		u2 = 1.0f;
		v2 = 1.0f;
		
		glQuad.SetUVs( u1, v1, u2, v2 );
		glQuad.Draw();
	}
	
}

void MOAIFreeTypeTextBox::DrawBitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y){
	//UNUSED(bitmap);
	
	FT_Int i, j, k, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;
	
	FT_Int imgWidth = (FT_Int)this->mFrame.Width();
	FT_Int imgHeight = (FT_Int)this->mFrame.Height();
	
	// create a buffer to use in initializing a MOAIImage 
	//const int BYTES_PER_PIXEL = 4;
	
	//size_t size = bitmap->width * bitmap->rows * BYTES_PER_PIXEL;
	//unsigned char* imgBuffer = (unsigned char*)malloc(size);
	int idx = 0;
	u8 value, formerValue;
	
	// fill the values with data from bitmap->buffer
	for (i = x, p = 0; i < x_max; i++, p++) {
		for (j = y, q = 0; j < y_max; j++, q++) {
			// compute index for bitmap data pixel red value.  Uses mBitmapWidth instead of imgWidth
			idx = (j * this->mBitmapWidth + i) * BYTES_PER_PIXEL;
			
			// retrieve value from the character bitmap
			value = bitmap->buffer[q * bitmap->width + p];
			// skip this if the location is out of bounds or the value is zero
			if (i < 0 || j < 0 || i >= imgWidth || j >= imgHeight || value == 0) {
				continue;
			}
			
			// get the former value
			formerValue = this->mBitmapData[idx+3];
			// set alpha to MAX(value, formerValue)
			if (value > formerValue) {
				this->mBitmapData[idx+3] = value; // alpha
			}
			else{
				continue;
			}
			
			// set RGB to 255
			for (k = 0; k < BYTES_PER_PIXEL - 1; k++){
				this->mBitmapData[idx+k] = value; // RGB
			}
			
			
		}
	}
	
	
	
}

// Before the rendering, the text gets broken up into lines either ending with a new-line or a space
void MOAIFreeTypeTextBox::GenerateLines(){
	FT_Face face = this->mFont->mFreeTypeFace;
	FT_Int maxWidth = (FT_Int)this->mFrame.Width();
	//FT_Int maxHeight = (FT_Int)this->mFrame.Height();
	
	FT_Error error = 0;
	FT_Int pen_x;
	
	int n = 0;
	
	u32 unicode = u8_nextchar(this->mText, &n);
	
	error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
	if (error) {
		return;
	}
	
	FT_Int pen_x_reset = 0;//-((face->glyph->metrics.horiBearingX) >> 6);
	
	pen_x = pen_x_reset; 
	u32 lastCh = 0;
	
	this->mLineIdx = 0;
	this->mTokenIdx = 0;
	
	// set n back to zero since it was advanced at the call to u8_nextchar()
	n = 0;
	
	// variable that stores the length of the text currently in the buffer
	size_t text_len = 0;
	// variable that stores text_len to last white space before final token
	size_t last_token_len = 0;
	
	wchar_t* text_buffer = (wchar_t *) malloc(sizeof(wchar_t) * strlen(this->mText));
	while ( (unicode = u8_nextchar(this->mText, &n)) ) {
		if (unicode == '\n') {
			this->BuildLine(text_buffer, text_len, face, pen_x, lastCh);
			text_len = 0;
			this->mLineIdx = this->mTokenIdx = n;
			
			error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
			if (error) {
				free(text_buffer);
				return;
			}
			pen_x = pen_x_reset; 
			continue;
		}
		else if (unicode == ' '){ // if ( MOAIFont::IsWhitespace( unicode ) )
			this->mTokenIdx = n;
			last_token_len = text_len;
		}
		
		error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
		
		if (error) {
			free(text_buffer);
			return;
		}
		
		// check its width
		// divide it when exceeding
		bool isExceeding = (maxWidth > 0
							&& pen_x + ((face->glyph->metrics.width) >> 6) > maxWidth);
		if (isExceeding) {
			if (this->mWordBreak == MOAIFreeTypeTextBox::WORD_BREAK_CHAR) {
				this->BuildLine(text_buffer, text_len, face, pen_x, lastCh);
				text_len = 0;
				this->mLineIdx = this->mTokenIdx = n;
				
				pen_x = pen_x_reset;
			}
			else{ // the default where words don't get broken up
				if (this->mTokenIdx != this->mLineIdx) {
					this->BuildLine(text_buffer, last_token_len, face, pen_x, lastCh);
					
					// set n back to token index
					n = this->mTokenIdx;
					
					// get the character after token index and update n
					unicode = u8_nextchar(this->mText, &n);
					
					// reset text_len and last_token_len
					text_len = last_token_len = 0;
					
					this->mLineIdx = this->mTokenIdx = n;
					
					pen_x = pen_x_reset;
				}
				else{ // put the rest of the token on the next line
					this->BuildLine(text_buffer, text_len, face, pen_x, lastCh);
					text_len = 0;
					
					this->mLineIdx = this->mTokenIdx = n;
					
					pen_x = pen_x_reset;
				}
			}
			
		}
		
		lastCh = unicode;
		text_buffer[text_len] = unicode;
		++text_len;
		pen_x += ((face->glyph->metrics.horiAdvance) >> 6);
		
	}
	
	this->BuildLine(text_buffer, text_len, face, pen_x, lastCh);
	free(text_buffer);
}

void MOAIFreeTypeTextBox::InitBitmapData(){
	float width = this->mFrame.Width();
	float height = this->mFrame.Height();
	
	if (width > 0.0f && height > 0.0f) {
		if (this->mBitmapData) {
			free(this->mBitmapData);
			this->mBitmapData = NULL;
		}
		// set the width and height of the texture bitmap to the next power of two
		u32 bmpW = (u32) width;
		u32 bmpH = (u32) height;
		
		u32 n;
		// set width to smallest power of two larger than bitmap's width
		if (!MOAIImage::IsPow2(bmpW)) {
			n = 1;
			// double n until it gets larger than the width of the bitmap
			while (n < bmpW) {
				n <<= 1;
			}
			bmpW = n;
		}
		this->mBitmapWidth = bmpW;
		
		// set height to smallest power of two larger than bitmap's width
		if (!MOAIImage::IsPow2(bmpH)) {
			n = 1;
			// double n until it gets larger than the height of the bitmap
			while (n < bmpH) {
				n <<= 1;
			}
			bmpH = n;
		}
		
		this->mBitmapHeight = bmpH;
		
		//const int BYTES_PER_PIXEL = 4;
		size_t bmpSize = bmpW * bmpH * BYTES_PER_PIXEL;
		// initialize mBitmapData to zero
		this->mBitmapData = (unsigned char*)calloc( bmpSize, sizeof( unsigned char ) );
		
		
		
	}
	
	this->mBitmapDataNeedsUpdate = false;
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
mTexture( NULL ),
mBitmapData( NULL ),
mBitmapWidth( 0 ),
mBitmapHeight( 0 ),
mBitmapDataNeedsUpdate( false ),
mLineIdx( 0 ),
mTokenIdx( 0 ){
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
		{ "setAlignment",			_setAlignment },
		{ "setAutoFit",				_setAutoFit },
		{ "setFont",				_setFont },
		{ "setGlyphScale",			_setGlyphScale },
		{ "setRect",				_setRect },
		{ "setString",				_setString },
		{ "setTextSize",			_setTextSize },
		{ "setWordBreak",			_setWordBreak },
		{ NULL, NULL }
	};
	
	luaL_register(state, 0, regTable );
}

// This is where the characters get rendered to mBitmapData.  Done line by line
void MOAIFreeTypeTextBox::RenderLines(){
	FT_Int pen_x, pen_y;
	
	FT_Face face = this->mFont->mFreeTypeFace;
	
	pen_y = (face->size->metrics.height >> 6) + 1;// this->ComputeLineStartY(face, alignMask, txtHeight, iMaxLineHeight);
	
	
	size_t lines = m_vLines.size();
	for (size_t i = 0; i < lines;  i++) {
		const wchar_t* text_ptr = m_vLines[i].text;
		
		// calcluate origin cursor
		pen_x = 0; //this->ComputeLineStart(face, alignMask, text_ptr[0], i)
		
		size_t text_len = wcslen(text_ptr);
		for (size_t i2 = 0; i2 < text_len; ++i2) {
			int error = FT_Load_Char(face, text_ptr[i2], FT_LOAD_RENDER);
			if (error) {
				break;
			}
			
			FT_Bitmap bitmap = face->glyph->bitmap;
			
			int yOffset = pen_y - (face->glyph->metrics.horiBearingY >> 6);
			int xOffset = pen_x + (face->glyph->metrics.horiBearingX >> 6);
			
			this->DrawBitmap(&bitmap, xOffset, yOffset);
			
			
			// step to next glyph
			pen_x += (face->glyph->metrics.horiAdvance >> 6); // + iInterval;
			
		}
		
		pen_y += (face->size->metrics.ascender >> 6) - (face->size->metrics.descender >> 6);
	}
	
	
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
	USRect oldRect;
	float oldX1 = this->mFrame.mXMin;
	float oldX2 = this->mFrame.mXMax;
	float oldY1 = this->mFrame.mYMin;
	float oldY2 = this->mFrame.mYMax;
	oldRect.Init(oldX1, oldY1, oldX2, oldY2);
	
	this->mFrame.Init(left, top, right, bottom);
	
	if (this->mFrame.Width() != oldRect.Width() ||
		this->mFrame.Height() != oldRect.Height() ) {
		this->mBitmapDataNeedsUpdate = true;
	}
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
