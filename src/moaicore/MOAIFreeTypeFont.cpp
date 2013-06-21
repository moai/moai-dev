// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com



#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFreeTypeFont.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAITextBox.h>


#define BYTES_PER_PIXEL 4


#define CHECK_ERROR(error) if (error != 0) { printf("freetype fail %d at line %d", error, __LINE__); exit(-1); }

//================================================================//
// local
//================================================================//
//----------------------------------------------------------------//
/**	@name	getDefaultSize
	@text	Requests the font's default size
 
	@in		MOAIFont self
	@out	float default size
 */

int MOAIFreeTypeFont::_getDefaultSize(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	state.Push( self->mDefaultSize );
	return 1;
}
//----------------------------------------------------------------//
/**	@name	getFilename
	@text	Returns the filename of the font.
 
	@in		MOAIFont self
	@out	name
 */

int MOAIFreeTypeFont::_getFilename(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	state.Push ( self->GetFilename() );
	return 1;
}
//----------------------------------------------------------------//
/**	@name	getFlags
	@text	Returns the current flags.
 
	@in		MOAIFont self
	@out	flags
 */
int MOAIFreeTypeFont::_getFlags(lua_State *L)
{
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	state.Push ( self->mFlags );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Sets the filename of the font for use when loading glyphs.
 
	@in		MOAIFont self
	@in		string filename			The path to the font file to load.
	@out	nil
 */

int	MOAIFreeTypeFont::_load(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "US" );
	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	self->Init ( filename );
	return 0;
}
//----------------------------------------------------------------//
/** @name	optimalSize
	@text	Returns the largest integral size between minFontSize and maxFontSize inclusive that 
			  fits in a text box of the given dimensions with the given options.
 
	@in		MOAIFont	self
	@in		string		text
	@in		number		width
	@in		number		height
	@in		number      maxFontSize
	@opt	number		minFontSize			default to 1.0
	@opt	bool		forceSingleLine		defalut to false
	@opt	enum		wordBreak			default to MOAITextBox.WORD_BREAK_NONE
	@out	number		optimalSize
 */
int MOAIFreeTypeFont::_optimalSize(lua_State *L){
	MOAI_LUA_SETUP(MOAIFreeTypeFont, "USNN");
	cc8* text = state.GetValue < cc8* > (2, "");
	float width = state.GetValue < float > (3, 1.0f);
	float height = state.GetValue < float > (4, 1.0f);
	float maxFontSize = state.GetValue < float > (5, self->mDefaultSize);
	float minFontSize = state.GetValue < float > (6, 1.0f);
	bool forceSingleLine = state.GetValue < bool > (7, false);
	int wordBreak = state.GetValue < int > (8, MOAITextBox::WORD_BREAK_NONE);
	
	float optimalSize = self->OptimalSize(text, width, height, maxFontSize, minFontSize,
										  wordBreak, forceSingleLine);
	state.Push(optimalSize);
	
	return 1;
}

//----------------------------------------------------------------//
/** @name	renderTexture
	@text	Produces a MOAITexture in RGBA_8888 format for the given string, bounds and options.
 
	@in		MOAIFont      self
	@in		string        text
	@in		number		  width
	@in		number		  height
	@in		number		  fontSize
	@opt	enum		  horizontalAlignment	default to MOAITextBox.LEFT_JUSTIFY
	@opt	enum		  verticalAlignment		default to MOAITextBox.LEFT_JUSTIFY
	@opt	enum		  wordBreak				one MOAITextBox.WORD_BREAK_NONE,
												  MOAITextBox.WORD_BREAK_CHAR.  Default to
												  MOAITextBox.WORD_BREAK_NONE
	@out	MOAITexture	  texture
 */
int MOAIFreeTypeFont::_renderTexture(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "USNN" );
	// TODO: implement RenderTexture()
	cc8* text = state.GetValue < cc8* > (2, "");
	float width = state.GetValue < float > (3, 1.0f);
	float height = state.GetValue < float > (4, 1.0f);
	float fontSize = state.GetValue < float > (5, self->mDefaultSize);
	int horizontalAlignment = state.GetValue < int > (6, MOAITextBox::LEFT_JUSTIFY);
	int verticalAlignment = state.GetValue < int > (7, MOAITextBox::LEFT_JUSTIFY);
	int wordBreak = state.GetValue < int > (8, MOAITextBox::WORD_BREAK_NONE);
	
	MOAITexture *texture = self->RenderTexture(text, fontSize, width, height, horizontalAlignment,
											   verticalAlignment, wordBreak, false);
	state.Push(texture);
	return 1;
}


//----------------------------------------------------------------//
/**	@name	setDefaultSize
	@text	Selects a glyph set size to use as the default size when no
			other size is specified by objects wishing to use MOAIFont to
			render text.
 
	@in		MOAIFont self
	@in		number points			The point size to be rendered onto the internal texture.
	@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
	@out	nil
 */

int MOAIFreeTypeFont::_setDefaultSize(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	
	float points	= state.GetValue < float >( 2, 0 );
	float dpi		= state.GetValue < float >( 3, DPI );
	
	self->mDefaultSize = POINTS_TO_PIXELS ( points, dpi );
	
	return 0;
}
//----------------------------------------------------------------//
/**	@name	setFlags
 @text	Set flags to control font loading behavior. Right now the
 only supported flag is FONT_AUTOLOAD_KERNING which may be used
 to enable automatic loading of kern tables. This flag is initially
 true by default.
 
 @in		MOAIFont self
 @opt	number flags			Flags are FONT_AUTOLOAD_KERNING or DEFAULT_FLAGS. DEFAULT_FLAGS is the same as FONT_AUTOLOAD_KERNING.
 Alternatively, pass '0' to clear the flags.
 @out	nil
 */

int MOAIFreeTypeFont::_setFlags(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	self->mFlags = state.GetValue < u32 >( 2, DEFAULT_FLAGS );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setReader
	@text	Attaches or clears the MOAIFontReader associated withthe font.
			MOAIFontReader is responsible for loading and rendering glyphs from
			a font file on demand. If you are using a static font and do not
			need a reader, set this field to nil.
 
	@in		MOAIFont self
	@opt	MOAIFontReader reader		Default value is nil.
	@out	nil
 */

int	MOAIFreeTypeFont::_setReader	( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	self->mReader.Set ( *self, state.GetLuaObject < MOAIFontReader >( 2, true ));
	return 0;
}

//================================================================//
// MOAIFreeTypeFont
//================================================================//

void MOAIFreeTypeFont::BuildLine(wchar_t* buffer, size_t buf_len, FT_Face face, int pen_x,
								 u32 lastChar){
	MOAIFreeTypeTextLine tempLine;
	
	wchar_t* text = (wchar_t*)malloc(sizeof(wchar_t) * (buf_len+1));
	memcpy(text, buffer, sizeof(wchar_t) * buf_len);
	
	text[buf_len] = '\0';
	tempLine.text = text;
	
	// get last glyph
	int error = FT_Load_Char(face, lastChar, FT_LOAD_DEFAULT);
	
	CHECK_ERROR(error);
	
	tempLine.lineWidth = pen_x - ((face->glyph->metrics.horiAdvance - face->glyph->metrics.horiBearingX - face->glyph->metrics.width) >> 6);
	
	this->mLineVector.push_back(tempLine);
	
}

int MOAIFreeTypeFont::ComputeLineStart(FT_UInt unicode, int lineIndex, int alignment,
									   FT_Int imgWidth){
	int retValue = 0;
	int adjustmentX = -((this->mFreeTypeFace->glyph->metrics.horiBearingX) >> 6);
	
	int maxLineWidth = imgWidth; // * scale;
	
	int error = FT_Load_Char(this->mFreeTypeFace, unicode, FT_LOAD_DEFAULT);
	if (error) {
		return -1;
	}
	
	if ( alignment == MOAITextBox::CENTER_JUSTIFY ){
		retValue = (maxLineWidth - this->mLineVector[lineIndex].lineWidth) / 2 + adjustmentX;
	}
	else if ( alignment == MOAITextBox::RIGHT_JUSTIFY ){
		retValue = (maxLineWidth - this->mLineVector[lineIndex].lineWidth) + adjustmentX;
	}
	else{
		// left or other value
		retValue = adjustmentX;
	}
	
	
	return retValue;
}

int MOAIFreeTypeFont::ComputeLineStartY(int textHeight, FT_Int imgHeight, int vAlign){
	int retValue = 0;
	int adjustmentY = ((this->mFreeTypeFace->size->metrics.ascender) >> 6);
	
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

void MOAIFreeTypeFont::DrawBitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y, FT_Int imgWidth, FT_Int imgHeight){
	FT_Int i, j, k, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;
	
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
			formerValue = this->mBitmapData[idx + BYTES_PER_PIXEL - 1];
			// set alpha to MAX(value, formerValue)
			if (value > formerValue) {
				this->mBitmapData[idx + BYTES_PER_PIXEL - 1] = value; // alpha
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

void MOAIFreeTypeFont::GenerateLines(FT_Int imgWidth, cc8 *text, int wordBreak){
	
	
	FT_Error error = 0;
	FT_Int pen_x;
	FT_Int last_token_x = 0;
	
	FT_Face face = this->mFreeTypeFace;
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
			
			this->BuildLine(text_buffer, text_len, face, pen_x, lastCh);
			
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
		bool isExceeding = (imgWidth > 0
							&& pen_x + ((face->glyph->metrics.width) >> 6) > imgWidth);
		if (isExceeding) {
			if (wordBreak == MOAITextBox::WORD_BREAK_CHAR) {
				
				this->BuildLine(text_buffer, text_len, face, pen_x, lastCh);
				
				text_len = 0;
				
				lineIdx = tokenIdx = n;
				
				pen_x = pen_x_reset;
			} else { // the default where words don't get broken up
				if (tokenIdx != lineIdx) {
					
					this->BuildLine(text_buffer, last_token_len, face, last_token_x, lastTokenCh);
					
					// set n back to token index
					n = tokenIdx;
					
					// get the character after token index and update n
					unicode = u8_nextchar(text, &n);
					// reset text_len and last_token_len
					text_len = last_token_len = 0;
					
					lineIdx = tokenIdx = n;
					
					pen_x = pen_x_reset;
				} else { // put the rest of the token on the next line
					this->BuildLine(text_buffer, text_len, face, pen_x, lastCh);
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
	
	this->BuildLine(text_buffer, text_len, face, pen_x, lastCh);
	free(text_buffer);
	
}

void MOAIFreeTypeFont::Init(cc8 *filename) {
	if ( USFileSys::CheckFileExists ( filename ) ) {
		this->mFilename = USFileSys::GetAbsoluteFilePath ( filename );
	}
}

void MOAIFreeTypeFont::InitBitmapData(u32 width, u32 height){
	u32 bmpW = width;
	u32 bmpH = height;
	
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
	
	this->mBitmapData = (unsigned char*)calloc( bmpSize, sizeof( unsigned char ) );
	this->mBitmapWidth = bmpW;
	this->mBitmapHeight = bmpH;
}

FT_Face MOAIFreeTypeFont::LoadFreeTypeFace ( FT_Library *library )
{
	if (this->mFreeTypeFace) return this->mFreeTypeFace;

	int error = FT_New_Face(*library,
						this->GetFilename(),
						0,
						&this->mFreeTypeFace);

	if (error) return NULL;
	else return this->mFreeTypeFace;
}


MOAIFreeTypeFont::MOAIFreeTypeFont():
	mFlags( DEFAULT_FLAGS ),
	mDefaultSize( 0.0f ),
	mFreeTypeFace( NULL ),
    mBitmapData( NULL ),
	mBitmapWidth(0.0f),
	mBitmapHeight(0.0f){
	
	RTTI_BEGIN
		RTTI_EXTEND( MOAILuaObject )
	RTTI_END
}

MOAIFreeTypeFont::~MOAIFreeTypeFont(){
	
}


float MOAIFreeTypeFont::OptimalSize(cc8 *text, float width, float height, float maxFontSize,
									float minFontSize, int wordbreak, bool forceSingleLine){
	
	FT_Error error;
	// initialize library and face
	FT_Face face;
	if (!this->mFreeTypeFace) {
		FT_Library library;
		error = FT_Init_FreeType( &library );
		face = this->LoadFreeTypeFace( &library );
	}
	else{
		face = this->mFreeTypeFace;
	}
	
	error = FT_Set_Char_Size(face,
									  0,
									  (FT_F26Dot6)(64 * maxFontSize),
									  DPI,
									  0);
	CHECK_ERROR(error);
	
	int numLines = 0;
	
	
	bool useKerning = FT_HAS_KERNING(face);
	
	float lowerBoundSize = minFontSize;
	float upperBoundSize = maxFontSize + 1.0;
	
	// test size
	float testSize = (lowerBoundSize + upperBoundSize) / 2.0f;
	
	do{
		u32 unicode;
		u32 lastCh = 0;
		u32 lastTokenCh = 0;
		
		FT_UInt numGlyphs = 0;
		FT_UInt previousGlyphIndex = 0;
		FT_UInt glyphIndex = 0;
		
		// set character size to test size
		error = FT_Set_Char_Size(face,
								 0,
								 (FT_F26Dot6)(64 * testSize),
								 DPI,
								 0);
		CHECK_ERROR(error);
		
		
		// compute maximum number of lines allowed at font size.
		// forceSingleLine sets this value to one if true.
		FT_Int lineHeight = (face->size->metrics.height >> 6);
		int maxLines = forceSingleLine ? 1 : (height / lineHeight);
		FT_Int penXReset = 0;
		FT_Int penX = penXReset, penY = lineHeight;
		FT_Int lastTokenX = 0;
		
		numLines = 1;
		
		// compute actual number of lines needed to display the string
		int n = 0;
		
		int lineIdx = 0, tokenIdx = 0;
		while ( (unicode = u8_nextchar(text, &n) ) ) {
			
			// handle new line
			if (unicode == '\n'){
				numLines++;
				penX = penXReset;
				penY += lineHeight;
				lineIdx = tokenIdx = n;
				
				continue;
			}
			// handle white space
			else if (unicode == ' '){
				tokenIdx = n;
				lastTokenCh = lastCh;
				lastTokenX = penX;
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
					penX = penXReset;
					penY += lineHeight;
					lineIdx = tokenIdx = n;
				}
				else{ // WORD_BREAK_NONE
					if (tokenIdx != lineIdx) {
						// set n back to the last index
						n = tokenIdx;
						// get the character after token index and update n
						unicode = u8_nextchar(text, &n);
						
						//advance to next line
						numLines++;
						penX = penXReset;
						penY += lineHeight;
						lineIdx = tokenIdx = n;
					}
					else{
						// advance to next line
						numLines++;
						penX = penXReset;
						penY += lineHeight;
						lineIdx = tokenIdx = n;
					}
					
				}
			}
			
			lastCh = unicode;
			previousGlyphIndex = glyphIndex;
			
			// advance cursor
			penX += (face->glyph->metrics.horiAdvance >> 6);
			
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
	
	
	return floorf(lowerBoundSize);
}

//----------------------------------------------------------------//

void MOAIFreeTypeFont::RegisterLuaClass ( MOAILuaState& state ) {
	state.SetField ( -1, "DEFAULT_FLAGS",			( u32 )DEFAULT_FLAGS );
	state.SetField ( -1, "FONT_AUTOLOAD_KERNING",	( u32 )FONT_AUTOLOAD_KERNING );
}

//----------------------------------------------------------------//

void MOAIFreeTypeFont::RegisterLuaFuncs(MOAILuaState &state){
	luaL_Reg regTable [] = {
		{ "getDefaultSize",				_getDefaultSize },
		{ "getFilename",				_getFilename },
		{ "getFlags",					_getFlags },
		{ "load",						_load },
		{ "optimalSize",				_optimalSize },
		{ "renderTexture",				_renderTexture },
		{ "setDefaultSize",				_setDefaultSize },
		{ "setFlags",					_setFlags },
		{ "setReader",					_setReader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

void MOAIFreeTypeFont::RenderLines(FT_Int imgWidth, FT_Int imgHeight, int hAlign, int vAlign){
	FT_Int pen_x, pen_y;
	
	FT_Face face = this->mFreeTypeFace;
	
	FT_Int textHeight = (face->size->metrics.height >> 6) * this->mLineVector.size();
	
	//pen_y = (face->size->metrics.height >> 6) + 1;
	pen_y = this->ComputeLineStartY(textHeight, imgHeight, vAlign);
	
	FT_UInt glyphIndex = 0;
	FT_UInt previousGlyphIndex = 0;
	bool useKerning = FT_HAS_KERNING(face);
	
	for (size_t i = 0; i < this->mLineVector.size();  i++) {
		
		const wchar_t* text_ptr = this->mLineVector[i].text;
		
		// calcluate origin cursor
		//pen_x = 0; //this->ComputeLineStart(face, alignMask, text_ptr[0], i)
		pen_x = this->ComputeLineStart(text_ptr[0], i, hAlign, imgWidth);
		
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
			this->DrawBitmap(&bitmap, xOffset, yOffset, imgWidth, imgHeight);
			
			
			// step to next glyph
			pen_x += (face->glyph->metrics.horiAdvance >> 6); // + iInterval;
			
			previousGlyphIndex = glyphIndex;
			
		}
		
		pen_y += (face->size->metrics.ascender >> 6) - (face->size->metrics.descender >> 6);
	}
}

MOAITexture* MOAIFreeTypeFont::RenderTexture(cc8 *text, float size, float width, float height,
											 int hAlignment, int vAlignment, int wordbreak,
											 bool autoFit){
	UNUSED(autoFit);
	
	FT_Error error;
	
	// initialize library and face
	FT_Face face;
	if (!this->mFreeTypeFace) {
		FT_Library library;
		error = FT_Init_FreeType( &library );
		face = this->LoadFreeTypeFace( &library );
	}
	else{
		face = this->mFreeTypeFace;
	}
	
	// set character size
	error = FT_Set_Char_Size(face,					/* handle to face object           */
							 0,						/* char_width in 1/64th of points  */
							 (FT_F26Dot6)( 64 * size ),	/* char_height in 1/64th of points */
							 DPI,					/* horizontal device resolution    */
							 0);					/* vertical device resolution      */
	CHECK_ERROR(error);
	
	
	FT_Int imgWidth = (FT_Int)width;
	FT_Int imgHeight = (FT_Int)height;
	
	// create the image data buffer
	this->InitBitmapData(imgWidth, imgHeight);
	
	// create the lines of text
	this->GenerateLines(imgWidth, text, wordbreak);
	
	// render the lines to the data buffer
	this->RenderLines(imgWidth, imgHeight, hAlignment, vAlignment);
	
	// turn that data buffer into an image
	MOAIImage bitmapImg;
	bitmapImg.Init(this->mBitmapData, this->mBitmapWidth, this->mBitmapHeight, USColor::RGBA_8888);
	
	
	// send that to the GPU
	MOAITexture *texture = new MOAITexture();
	texture->Init(bitmapImg, "");
	
	return texture;
}
