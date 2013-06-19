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
	int alignment = state.GetValue < int > (6, MOAITextBox::CENTER_JUSTIFY ); // horizontal alignment
	int wordBreak = state.GetValue < int > (7, MOAITextBox::WORD_BREAK_CHAR );
	int vAlignment = state.GetValue <int> (8, MOAITextBox::CENTER_JUSTIFY ); // vertical alignment
	bool autoFit = state.GetValue <bool> ( 9, false );

	MOAITexture *texture = MOAIFreeTypeTextBox::GenerateTexture(text, f, size, width, height, alignment, wordBreak, vAlignment, autoFit);

	state.Push(texture);
	return 1;
}

MOAITexture *MOAIFreeTypeTextBox::GenerateTexture( cc8 *text, MOAIFreeTypeFont *font, float size, float width, float height, int alignment, int wordbreak, int vAlignment, bool autoFit  ) {
	

	int	pen_x, pen_y = 0;
	
	FT_Error error;
	
	// initialize library
	FT_Library library;
	error = FT_Init_FreeType( &library );

	CHECK_ERROR(error);
	
	// create face object
	FT_Face face = font->LoadFreeTypeFace( &library );
	
	// calculate optimal size if necessary
	if (autoFit) {
		size = MOAIFreeTypeTextBox::OptimalSizeForTexture(text, face, size, width, height, wordbreak);
	}

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
	MOAIFreeTypeImageBuffer imageBuffer = InitBitmapData(imgWidth, imgHeight);
	
	// create the lines of text
	vector<MOAIFreeTypeTextLine> lines = GenerateLines(face, imgWidth, text, wordbreak);

	// render the lines to the data buffer
	RenderLines(lines, imageBuffer.data, imgWidth, imgHeight, imageBuffer.width, imageBuffer.height, face, alignment, vAlignment);

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

int MOAIFreeTypeTextBox::ComputeLineStart(FT_Face face, FT_UInt unicode, int lineIndex, int alignment, FT_Int imgWidth, const vector<MOAIFreeTypeTextLine> &lines){
	int retValue = 0;
	int adjustmentX = -((face->glyph->metrics.horiBearingX) >> 6);
	
	int maxLineWidth = imgWidth; // * scale;
	
	int error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
	if (error) {
		return -1;
	}
	
	if ( alignment == MOAITextBox::CENTER_JUSTIFY ){
		retValue = (maxLineWidth - lines[lineIndex].lineWidth) / 2 + adjustmentX;
	}
	else if ( alignment == MOAITextBox::RIGHT_JUSTIFY ){
		retValue = (maxLineWidth - lines[lineIndex].lineWidth) + adjustmentX;
	}
	else{
		// left or other value
		retValue = adjustmentX;
	}
	
	
	return retValue;
}

int MOAIFreeTypeTextBox::ComputeLineStartY(FT_Face face, int textHeight, FT_Int imgHeight, int vAlign){

	int retValue = 0;
	int adjustmentY = ((face->size->metrics.ascender) >> 6);
	
	if ( vAlign == MOAITextBox::CENTER_JUSTIFY ) {
		// vertical center
		retValue = (imgHeight - textHeight)/2 + adjustmentY;
	}
	else if( vAlign == MOAITextBox::RIGHT_JUSTIFY ){
		// vertical bottom
		retValue = imgHeight - textHeight + adjustmentY;
	}
	else{
		// vertical top or other value
		retValue = adjustmentY;
	}
	
	
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
			formerValue = renderBitmap[idx + BYTES_PER_PIXEL - 1];
			// set alpha to MAX(value, formerValue)
			if (value > formerValue) {
				renderBitmap[idx + BYTES_PER_PIXEL - 1] = value; // alpha
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
	FT_Int last_token_x = 0;
	
	int n = 0;
	
	u32 unicode = u8_nextchar(text, &n);
	
	error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
	CHECK_ERROR(error);
	
	FT_Int pen_x_reset = 0;//-((face->glyph->metrics.horiBearingX) >> 6);
	
	pen_x = pen_x_reset; 
	u32 lastCh = 0;
	u32 lastTokenCh = 0;

	int lineIdx = 0;
	int tokenIdx = 0;
	
	// set n back to zero since it was advanced at the call to u8_nextchar()
	n = 0;
	
	// variable that stores the length of the text currently in the buffer
	size_t text_len = 0;
	// variable that stores text_len to last white space before final token
	size_t last_token_len = 0;
	
	wchar_t* text_buffer = (wchar_t *) malloc(sizeof(wchar_t) * strlen(text));
	
	// determine if font uses kerning
	bool useKerning = FT_HAS_KERNING(face);
	FT_UInt glyphIndex = 0;
	FT_UInt previousGlyphIndex = 0;
	
	
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
			last_token_x = pen_x;
			lastTokenCh = lastCh;
		}
		
		error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
		
		CHECK_ERROR(error);
		
		glyphIndex = FT_Get_Char_Index(face, unicode);
		
		// factor in kerning
		if (useKerning && previousGlyphIndex && glyphIndex) {
			FT_Vector delta;
			FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
			pen_x += delta.x >> 6;
		}
		
		
		// check its width
		// divide it when exceeding
		bool isExceeding = (maxWidth > 0
							&& pen_x + ((face->glyph->metrics.width) >> 6) > maxWidth);
		if (isExceeding) {
			if (wordBreak == MOAITextBox::WORD_BREAK_CHAR) {
				
				MOAIFreeTypeTextLine line = BuildLine(text_buffer, text_len, face, pen_x, lastCh);
				lines.push_back(line);
				
				text_len = 0;

				lineIdx = tokenIdx = n;
				
				pen_x = pen_x_reset;
			} else { // the default where words don't get broken up
				if (tokenIdx != lineIdx) {
					
					MOAIFreeTypeTextLine line = BuildLine(text_buffer, last_token_len, face, last_token_x, lastTokenCh);
					lines.push_back(line);
					
					// set n back to token index
					n = tokenIdx;
					
					// get the character after token index and update n
					unicode = u8_nextchar(text, &n);					
					// reset text_len and last_token_len
					text_len = last_token_len = 0;
					
					lineIdx = tokenIdx = n;
					
					pen_x = pen_x_reset;
				} else { // put the rest of the token on the next line
					MOAIFreeTypeTextLine line = BuildLine(text_buffer, text_len, face, pen_x, lastCh);
					lines.push_back(line);
					text_len = 0;
					
					lineIdx = tokenIdx = n;
					
					pen_x = pen_x_reset;
				}
			}
			
		}
		
		lastCh = unicode;
		previousGlyphIndex = glyphIndex;
		text_buffer[text_len] = unicode;
		++text_len;
		pen_x += ((face->glyph->metrics.horiAdvance) >> 6);
		
	}
	
	MOAIFreeTypeTextLine line = BuildLine(text_buffer, text_len, face, pen_x, lastCh);
	lines.push_back(line);

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

	unsigned char *bitmapData = (unsigned char*)calloc( bmpSize, sizeof( unsigned char ) );

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

/*
 This function calculates the optimal size for a font, given the string and dimensions of the text box. It loops until it finds the largest integral size that fits.  The loop is a binary search.  If it passes with the initial size, return the initial size.  Else, try the arithmetic mean of the initial size and the minimum size.  Adjust minimum bound upward if it passes the test.  Adjust maximum bound downward if it fails.
 
 The test here sees if the needed number of lines exceeds the maximum for the size.
 
 After the new font size is determined, gather up the positions again to see where line breaks occur and calculate the maximum number of text lines at that size.  Store value of largest working size.
 
 Why do I need to know the bounding box?  I probably do not need it.
 
 Should it try searching upward as well if the initial size is too small?  Perhaps double the font size if the initial size passes the test?
 */
float MOAIFreeTypeTextBox::OptimalSizeForTexture(cc8 *text, FT_Face face, float initialSize, float width, float height, int wordbreak){
	float retSize = initialSize;
	
	UNUSED(width);
	UNUSED(height);
	
	FT_Error error = FT_Set_Char_Size(face,
									  0,
									  (FT_F26Dot6)(64 * initialSize),
									  DPI,
									  0);
	CHECK_ERROR(error);
	
	size_t maxGlyphs = strlen(text);
	
	int numLines = 0;
	
	
	u32 unicode;
	
	
	
	
	FT_GlyphSlot slot = face->glyph;
	FT_UInt numGlyphs = 0;
	FT_UInt previousGlyphIndex = 0;
	FT_UInt glyphIndex = 0;
	
	FT_UInt lastTokenIndex = 0;
	
	bool useKerning = FT_HAS_KERNING(face);
	
	FT_Int penX = 0, penY = 0;
	
	// re-write attempt
	if (true){
		
		
		
		float minSize = 1.0; // could make this a parameter...
		
		float lowerBoundSize = minSize;
		float upperBoundSize = initialSize;
		
		// test size
		float testSize = (lowerBoundSize + upperBoundSize) / 2.0f;
		
		do{
			// set character size to test size
			error = FT_Set_Char_Size(face,
									 0,
									 (FT_F26Dot6)(64 * testSize),
									 DPI,
									 0);
			CHECK_ERROR(error);
			
			
			// compute maximum number of lines allowed at font size
			FT_Int lineHeight = (face->size->metrics.height >> 6);
			int maxLines = height / lineHeight;
			
			
			penY = lineHeight;
			
			numLines = 0;
			
			// compute actual number of lines needed to display the string
			int n = 0;
			
			int lineIdx = 0, tokenIdx = 0;
			while ( (unicode = u8_nextchar(text, &n) ) ) {
				
				// handle new line
				if (unicode == '\n'){
					numLines++;
					penX = 0;
					penY += lineHeight;
					lineIdx = tokenIdx = n;
				}
				// handle white space
				else if (' '){
					tokenIdx = n;
				}
				
				error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
				
				CHECK_ERROR(error);
				
				glyphIndex = FT_Get_Char_Index(face, unicode);
				
				numGlyphs++;
				
				// take kerning into account
				if (useKerning && previousGlyphIndex && glyphIndex) {
					FT_Vector delta;
					FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
					penX += delta.x;
				}
				
				// determine if penX is outside the bounds of the box
				bool isExceeding = penX + ((face->glyph->metrics.width) >> 6) > (FT_Int)width;
				if (isExceeding) {
					if (wordbreak == MOAITextBox::WORD_BREAK_CHAR) {
						// advance to next line
						numLines++;
						penX = 0;
						penY += lineHeight;
						lineIdx = tokenIdx = n;
					}
					else{ // WORD_BREAK_NONE
						if (tokenIdx != lineIdx) {
							// set n back to the last index
							n = lastTokenIndex;
							
							//advance to next line
							numLines++;
							penX = 0;
							penY += lineHeight;
							lineIdx = tokenIdx = n;
						}
						else{
							
						}
						
					}
				}
				
				
				// advance cursor
				penX += face->glyph->metrics.horiAdvance;
				
			}
			
			if (numLines > maxLines){ // failure case
				// adjust upper bound downward
				upperBoundSize = testSize;
			}
			else{ // success
				// adjust lower bound upward
				lowerBoundSize = testSize;
			}
			// recalculate test size
			testSize = (lowerBoundSize + upperBoundSize) / 2.0f;
			
			
		}while(upperBoundSize - lowerBoundSize >= 1.0f);
		
		return floorf(testSize);
	}
	
	
	
	
	
	
	FT_Glyph*	glyphs = new FT_Glyph[maxGlyphs];
	FT_Vector*  positions = new FT_Vector[maxGlyphs];
	
	
	
	
	
	// gather positions of glyphs
	int n = 0;
	while ( (unicode = u8_nextchar(text, &n)) ) {
		// handle line breaks and spaces
		if (unicode == '\n') {
			numLines++;
			penX = 0;
			//penY += lineHeight;
		}
		else if (unicode == ' '){
			lastTokenIndex = n;
		}
		
		glyphIndex = FT_Get_Char_Index(face, unicode);
		if (useKerning && previousGlyphIndex && glyphIndex) {
			FT_Vector delta;
			FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
			penX += delta.x;
		}
		
		positions[numGlyphs].x = penX;
		positions[numGlyphs].y = penY;
		
		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
		CHECK_ERROR(error);
		
		error = FT_Get_Glyph(face -> glyph, &glyphs[numGlyphs]);
		CHECK_ERROR(error);
		
		penX += slot->advance.x;
		// determine if penX is outside the bounds of the box and advance to the next line
		bool isExceeding = penX > (FT_Int)width;
		if (isExceeding) {
			if (wordbreak == MOAITextBox::WORD_BREAK_CHAR) {
				numLines++;
				penX = 0;
				//penY += lineHeight;
			}
			else{ // WORD_BREAK_NONE
				
				// set n back to the last index
				n = lastTokenIndex;
			}
		}
		
		previousGlyphIndex = glyphIndex;
		
		numGlyphs++;
	}
	
	
	
	delete [] glyphs;
	delete [] positions;
	
	return retSize;
}


void MOAIFreeTypeTextBox::RegisterLuaClass( MOAILuaState &state ){

	luaL_Reg regTable [] = {
		{ "generateLabelTexture",			_generateLabelTexture },
		{NULL, NULL}
	};

	luaL_register( state, 0, regTable );
}


// This is where the characters get rendered to mBitmapData.  Done line by line
void MOAIFreeTypeTextBox::RenderLines(const vector<MOAIFreeTypeTextLine> &lines, u8 *renderBitmap, FT_Int imgWidth, FT_Int imgHeight, int bitmapWidth, int bitmapHeight, FT_Face face, int hAlign, int vAlign) {
	FT_Int pen_x, pen_y;
	
	FT_Int textHeight = (face->size->metrics.height >> 6) * lines.size();

	//pen_y = (face->size->metrics.height >> 6) + 1;
	pen_y = MOAIFreeTypeTextBox::ComputeLineStartY(face, textHeight, imgHeight, vAlign);
	
	FT_UInt glyphIndex = 0;
	FT_UInt previousGlyphIndex = 0;
	bool useKerning = FT_HAS_KERNING(face);

	for (size_t i = 0; i < lines.size();  i++) {

		const wchar_t* text_ptr = lines[i].text;
		
		// calcluate origin cursor
		//pen_x = 0; //this->ComputeLineStart(face, alignMask, text_ptr[0], i)
		pen_x = MOAIFreeTypeTextBox::ComputeLineStart(face, text_ptr[0], i, hAlign, imgWidth, lines);
		
		size_t text_len = wcslen(text_ptr);
		for (size_t i2 = 0; i2 < text_len; ++i2) {
			int error = FT_Load_Char(face, text_ptr[i2], FT_LOAD_RENDER);
			if (error) {
				break;
			}
			
			FT_Bitmap bitmap = face->glyph->bitmap;
			
			glyphIndex = FT_Get_Char_Index(face, text_ptr[i2]);
			
			if (useKerning && glyphIndex && previousGlyphIndex) {
				FT_Vector delta;
				FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
				pen_x += delta.x >> 6;
			}
			
			int yOffset = pen_y - (face->glyph->metrics.horiBearingY >> 6);
			int xOffset = pen_x + (face->glyph->metrics.horiBearingX >> 6);

			//(FT_Bitmap *bitmap, FT_Int x, FT_Int y, u8 *renderBitmap, FT_Int imgWidth, FT_Int imgHeight, int bitmapWidth, int bitmapHeight);
			DrawBitmap(&bitmap, xOffset, yOffset, (u8 *)renderBitmap, imgWidth, imgHeight, bitmapWidth, bitmapHeight);
			
			
			// step to next glyph
			pen_x += (face->glyph->metrics.horiAdvance >> 6); // + iInterval;
			
			previousGlyphIndex = glyphIndex;
			
		}
		
		pen_y += (face->size->metrics.ascender >> 6) - (face->size->metrics.descender >> 6);
	}

}
