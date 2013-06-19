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
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAITextBox.h>
#include <ft2build.h>
//#include FT_FREETYPE_H
#include FT_GLYPH_H


#define BYTES_PER_PIXEL 4

#define CHECK_ERROR(error) if (error != 0) { printf("freetype fail %d at __LINE__", error); exit(-1); }

// ------------------------------------------------------------------------ //

int MOAIFreeTypeTextBox::_generateLabelTexture	( lua_State* L )
{
	MOAILuaState state ( L );

	cc8* text = state.GetValue<cc8*> (1, "");
	MOAIFreeTypeFont *f = state.GetLuaObject<MOAIFreeTypeFont>(2, false);
	float size = state.GetValue < float > (3, 12.0f);
	float width = state.GetValue < float > (4, 100.0f);
	float height = state.GetValue < float > (5, 100.0f);
	int alignment = state.GetValue < int > (6, MOAITextBox::CENTER_JUSTIFY );
	int wordBreak = state.GetValue < int > (7, MOAITextBox::WORD_BREAK_CHAR );

	MOAITexture *texture = MOAIFreeTypeTextBox::GenerateTexture(text, f, size, width, height, alignment, wordBreak);

	state.Push(texture);
	return 1;
}

MOAITexture *MOAIFreeTypeTextBox::GenerateTexture( cc8 *text, MOAIFreeTypeFont *font, float size, float width, float height, int alignment, int wordbreak ) {

	UNUSED(alignment);

	int	pen_x, pen_y = 0;
	
	FT_Error error;
	
	// initialize library
	FT_Library library;
	error = FT_Init_FreeType( &library );

	CHECK_ERROR(error);
	
	// create face object
	FT_Face face = font->LoadFreeTypeFace( &library );

	// set character size
	error = FT_Set_Char_Size(face,					/* handle to face object           */
							 0,						/* char_width in 1/64th of points  */
							 (FT_F26Dot6)( 64 * size ),	/* char_height in 1/64th of points */
							 DPI,					/* horizontal device resolution    */
							 0);					/* vertical device resolution      */
	CHECK_ERROR(error);

	FT_Size fontSize = face->size;
	FT_Size_Metrics sizeMetrics = fontSize->metrics;

	// initialize pen position
	FT_Int lineHeight = (sizeMetrics.height >> 6);  // find out line height
	pen_x = 0;
	pen_y = lineHeight + 1;
	
	FT_Int imgWidth = (FT_Int)width;
	FT_Int imgHeight = (FT_Int)height;

	// create the image data buffer
	MOAIFreeTypeImageBuffer imageBuffer = InitBitmapData(imgHeight, imgWidth);

	// create the lines of text
	vector<MOAIFreeTypeTextLine> lines = GenerateLines(face, imgWidth, text, wordbreak);

	// render the lines to the data buffer
	RenderLines(lines, imageBuffer.data, imgWidth, imgHeight, imageBuffer.width, imageBuffer.height, face);

	// turn that data buffer into an image
	MOAIImage bitmapImg;
	bitmapImg.Init(imageBuffer.data, imageBuffer.width, imageBuffer.height, USColor::RGBA_8888);  // is A_8 the correct color mode?

	/// send that to the GPU
	MOAITexture *texture = new MOAITexture();
	texture->Init(bitmapImg, "");

	return texture;
}

// creates a line in m_vLines
MOAIFreeTypeTextLine MOAIFreeTypeTextBox::BuildLine(wchar_t *buffer, size_t buf_len, FT_Face face, int pen_x, u32 lastChar){
	MOAIFreeTypeTextLine tempLine;

	wchar_t* text = (wchar_t*)malloc(sizeof(wchar_t) * (buf_len+1));
	memcpy(text, buffer, sizeof(wchar_t) * buf_len);
	
	text[buf_len] = '\0';
	tempLine.text = text;
	
	// get last glyph
	int error = FT_Load_Char(face, lastChar, FT_LOAD_DEFAULT);
	
	CHECK_ERROR(error);
	
	tempLine.lineWidth = pen_x - ((face->glyph->metrics.horiAdvance - face->glyph->metrics.horiBearingX - face->glyph->metrics.width) >> 6);
	
	return tempLine;
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


void MOAIFreeTypeTextBox::DrawBitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y, u8 *renderBitmap, FT_Int imgWidth, FT_Int imgHeight, int bitmapWidth, int bitmapHeight) {

	UNUSED(bitmapHeight);
	
	FT_Int i, j, k, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;
	
	// create a buffer to use in initializing a MOAIImage 
	// const int BYTES_PER_PIXEL = 4;
	
	//size_t size = bitmap->width * bitmap->rows * BYTES_PER_PIXEL;
	//unsigned char* imgBuffer = (unsigned char*)malloc(size);
	int idx = 0;
	u8 value, formerValue;
	
	// fill the values with data from bitmap->buffer
	for (i = x, p = 0; i < x_max; i++, p++) {
		for (j = y, q = 0; j < y_max; j++, q++) {
			// compute index for bitmap data pixel red value.  Uses mBitmapWidth instead of imgWidth
			idx = (j * bitmapWidth + i) * BYTES_PER_PIXEL;
			
			// retrieve value from the character bitmap
			value = bitmap->buffer[q * bitmap->width + p];
			// skip this if the location is out of bounds or the value is zero
			if (i < 0 || j < 0 || i >= imgWidth || j >= imgHeight || value == 0) {
				continue;
			}
			
			// get the former value
			formerValue = renderBitmap[idx+3];
			// set alpha to MAX(value, formerValue)
			if (value > formerValue) {
				renderBitmap[idx+3] = value; // alpha
			}
			else{
				continue;
			}
			
			// set RGB to 255
			for (k = 0; k < BYTES_PER_PIXEL - 1; k++){
				renderBitmap[idx+k] = value; // RGB
			}
			
			
		}
	}
	
	
	
}

// Before the rendering, the text gets broken up into lines either ending with a new-line or a space
vector<MOAIFreeTypeTextLine> MOAIFreeTypeTextBox::GenerateLines(FT_Face face, FT_Int maxWidth, cc8* text, int wordBreak) {

	vector<MOAIFreeTypeTextLine> lines;
	
	//FT_Int maxHeight = (FT_Int)this->mFrame.Height();
	
	FT_Error error = 0;
	FT_Int pen_x;
	
	int n = 0;
	
	u32 unicode = u8_nextchar(text, &n);
	
	error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
	CHECK_ERROR(error);
	
	FT_Int pen_x_reset = 0;//-((face->glyph->metrics.horiBearingX) >> 6);
	
	pen_x = pen_x_reset; 
	u32 lastCh = 0;

	int lineIdx = 0;
	int tokenIdx = 0;
	
	// set n back to zero since it was advanced at the call to u8_nextchar()
	n = 0;
	
	// variable that stores the length of the text currently in the buffer
	size_t text_len = 0;
	// variable that stores text_len to last white space before final token
	size_t last_token_len = 0;
	
	wchar_t* text_buffer = (wchar_t *) malloc(sizeof(wchar_t) * strlen(text));
	
	while ( (unicode = u8_nextchar(text, &n)) ) {

		if (unicode == '\n') {

			MOAIFreeTypeTextLine line = BuildLine(text_buffer, text_len, face, pen_x, lastCh);
			lines.push_back(line);

			text_len = 0;
			lineIdx = tokenIdx = n;
			
			error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
			CHECK_ERROR(error);

			pen_x = pen_x_reset; 
			continue;
		}
		else if (unicode == ' '){ // if ( MOAIFont::IsWhitespace( unicode ) )
			tokenIdx = n;
			last_token_len = text_len;
		}
		
		error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
		
		CHECK_ERROR(error);
		
		// check its width
		// divide it when exceeding
		bool isExceeding = (maxWidth > 0
							&& pen_x + ((face->glyph->metrics.width) >> 6) > maxWidth);
		if (isExceeding) {
			if (wordBreak == MOAITextBox::WORD_BREAK_CHAR) {

				lines.push_back(BuildLine(text_buffer, text_len, face, pen_x, lastCh));
				
				text_len = 0;

				lineIdx = tokenIdx = n;
				
				pen_x = pen_x_reset;
			} else { // the default where words don't get broken up
				if (tokenIdx != lineIdx) {
					lines.push_back(BuildLine(text_buffer, last_token_len, face, pen_x, lastCh));
					
					// set n back to token index
					n = tokenIdx;
					
					// get the character after token index and update n
					unicode = u8_nextchar(text, &n);
					
					// reset text_len and last_token_len
					text_len = last_token_len = 0;
					
					lineIdx = tokenIdx = n;
					
					pen_x = pen_x_reset;
				} else { // put the rest of the token on the next line
					BuildLine(text_buffer, text_len, face, pen_x, lastCh);
					text_len = 0;
					
					lineIdx = tokenIdx = n;
					
					pen_x = pen_x_reset;
				}
			}
			
		}
		
		lastCh = unicode;
		text_buffer[text_len] = unicode;
		++text_len;
		pen_x += ((face->glyph->metrics.horiAdvance) >> 6);
		
	}
	
	lines.push_back(BuildLine(text_buffer, text_len, face, pen_x, lastCh));

	free(text_buffer);
	
	return lines;

}

MOAIFreeTypeImageBuffer MOAIFreeTypeTextBox::InitBitmapData(u32 width, u32 height) {
	// initialize the bitmap data
	// set the width and height of the texture bitmap to the next power of two
	MOAIFreeTypeImageBuffer buffer;

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

	// set height to smallest power of two larger than bitmap's width
	if (!MOAIImage::IsPow2(bmpH)) {
		n = 1;
		// double n until it gets larger than the height of the bitmap
		while (n < bmpH) {
			n <<= 1;
		}
		bmpH = n;
	}

	//const int BYTES_PER_PIXEL = 4;
	size_t bmpSize = bmpW * bmpH * BYTES_PER_PIXEL;
	// initialize mBitmapData to zero

	void *bitmapData = (unsigned char*)calloc( bmpSize, sizeof( unsigned char ) );

	buffer.width = bmpW;
	buffer.height = bmpH;
	buffer.data = bitmapData;

	return buffer;
}


MOAIFreeTypeTextBox::MOAIFreeTypeTextBox()
{
	RTTI_SINGLE(MOAILuaObject)
}


MOAIFreeTypeTextBox::~MOAIFreeTypeTextBox(){
	
}


void MOAIFreeTypeTextBox::RegisterLuaClass( MOAILuaState &state ){

	luaL_Reg regTable [] = {
		{ "generateLabelTexture",			_generateLabelTexture },
		{NULL, NULL}
	};

	luaL_register( state, 0, regTable );
}


// This is where the characters get rendered to mBitmapData.  Done line by line
void MOAIFreeTypeTextBox::RenderLines(vector<MOAIFreeTypeTextLine> lines, void *renderBitmap, FT_Int imgWidth, FT_Int imgHeight, int bitmapWidth, int bitmapHeight, FT_Face face) {
	FT_Int pen_x, pen_y;

	pen_y = (face->size->metrics.height >> 6) + 1;// this->ComputeLineStartY(face, alignMask, txtHeight, iMaxLineHeight);

	for (size_t i = 0; i < lines.size();  i++) {

		const wchar_t* text_ptr = lines[i].text;
		
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

			//(FT_Bitmap *bitmap, FT_Int x, FT_Int y, u8 *renderBitmap, FT_Int imgWidth, FT_Int imgHeight, int bitmapWidth, int bitmapHeight);
			DrawBitmap(&bitmap, xOffset, yOffset, (u8 *)renderBitmap, imgWidth, imgHeight, bitmapWidth, bitmapHeight);
			
			
			// step to next glyph
			pen_x += (face->glyph->metrics.horiAdvance >> 6); // + iInterval;
			
		}
		
		pen_y += (face->size->metrics.ascender >> 6) - (face->size->metrics.descender >> 6);
	}

}
