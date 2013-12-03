// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com



#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFreeTypeFont.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAITextBox.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGfxQuad2D.h>


#define BYTES_PER_PIXEL 4


#define CHECK_ERROR(error) if (error != 0) { printf("freetype fail %d at line %d", error, __LINE__); exit(-1); }

//================================================================//
// local
//================================================================//
//----------------------------------------------------------------//
/** @name	dimensionsOfLine
	@text	Returns the width and height of a single line string rendered at the given font size.
 
	@in		MOAIFont self
	@in		string	text
	@in		number	fontSize
	@opt	bool	returnGlyphTable		defaut to false
	@out	number	width
	@out	number	height
	@out	table	glyphTable				Contains data for each letter in the line.
 */
int MOAIFreeTypeFont::_dimensionsOfLine(lua_State *L){
	MOAI_LUA_SETUP( MOAIFreeTypeFont, "US" );
	cc8* text = state.GetValue < cc8* > (2, "");
	float fontSize = state.GetValue < float > (3, self->mDefaultSize);
	
	bool returnGlyphTable = state.GetValue < bool > (4, false);
	
	USRect rect = self->DimensionsOfLine(text, fontSize, returnGlyphTable, state);
	float width = rect.Width();
	float height = rect.Height();
	
	state.Push(width);
	state.Push(height);
	
	if (returnGlyphTable){
		// return the glyph bound table after the height information
		state.MoveToTop(-3);
		return 3;
	}
	
	return 2;
}

//----------------------------------------------------------------//
/** @name	dimensionsWithMaxWidth
	@text	Returns the width and height of a multiple-line string rendered at
			the given font size with the specified maximum width.
 
	@in		MOAIFont self
	@in		string	text
	@in		number	fontSize
	@in		number	maxWidth
	@opt	enum	wordBreakMode			default to MOAITextBox.WORD_BREAK_NONE
	@opt	bool	returnGlyphTable		default to false
	@out	number  width
	@out	number	height
	@out	table	glyphTable
 
 */
int MOAIFreeTypeFont::_dimensionsWithMaxWidth(lua_State *L){
	MOAI_LUA_SETUP( MOAIFreeTypeFont, "USNN");
	
	cc8* text = state.GetValue < cc8* > (2, "");
	float fontSize = state.GetValue < float > (3, self->mDefaultSize);
	float maxWidth = state.GetValue < float > (4, 320.0f);
	int	wordBreakMode = state.GetValue < int > (5, MOAITextBox::WORD_BREAK_NONE);
	bool returnGlyphTable = state.GetValue < bool > (6, false);
	
	USRect rect = self->DimensionsWithMaxWidth(text, fontSize, maxWidth, wordBreakMode, returnGlyphTable, state);
	float width = rect.Width();
	float height = rect.Height();
	
	state.Push(width);
	state.Push(height);
	if (returnGlyphTable) {
		// return the glyph bound table after the height information
		
		state.MoveToTop(-3);
		return 3;
	}
	
	return 2;
}

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
	@opt	bool		forceSingleLine		default to false
	@opt	enum		wordBreak			default to MOAITextBox.WORD_BREAK_NONE
	@opt	number		granularity			default to 1.0
	@opt	bool		roundToInteger		default to true
	@out	number		optimalSize
 */
int MOAIFreeTypeFont::_optimalSize(lua_State *L){
	MOAI_LUA_SETUP(MOAIFreeTypeFont, "USNN");
	MOAIOptimalSizeParameters params;
	params.text = state.GetValue < cc8* > (2, "");
	params.width = state.GetValue < float > (3, 1.0f);
	params.height = state.GetValue < float > (4, 1.0f);
	params.maxFontSize = state.GetValue < float > (5, self->mDefaultSize);
	params.minFontSize = state.GetValue < float > (6, 1.0f);
	params.forceSingleLine = state.GetValue < bool > (7, false);
	params.wordBreak = state.GetValue < int > (8, MOAITextBox::WORD_BREAK_NONE);
	
	params.granularity = state.GetValue < float > (9, 1.0f);
	params.roundToInteger = state.GetValue < bool > (10, true);
	
	
	
	float optimalSize = self->OptimalSize(params);
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
	@opt	bool		  returnGlyphBounds		Whether to return additional information about 
												  glyph bounds.  Default to false.
	@out	MOAITexture	  texture
	@out	table		  glyphBounds			A table containing glyph bounds for each character.
												  The table has three levels.  The top level contains
												  one or more tables, one table for each line indexed
												  by integer starting with 1.  The table for each
												  line contains one or more tables, one for each
												  glyph on the line, indexed by integer starting with
												  1, along with the entries indexed by the strings
												  'baselineY' and 'renderedCharacters'. The table for
												  each glyph contains entries indexed by the strings
												  'xMin', 'yMin', 'xMax' and 'yMax' each 
											      corresponding to the bounds of the glyph relative
												  to texture's coordinate system with the origin 
												  being in the upper left corner. Nil if 
												  returnGlyphBounds is false.
 */
int MOAIFreeTypeFont::_renderTexture(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "USNN" );
	cc8* text = state.GetValue < cc8* > (2, "");
	float width = state.GetValue < float > (3, 1.0f);
	float height = state.GetValue < float > (4, 1.0f);
	float fontSize = state.GetValue < float > (5, self->mDefaultSize);
	int horizontalAlignment = state.GetValue < int > (6, MOAITextBox::LEFT_JUSTIFY);
	int verticalAlignment = state.GetValue < int > (7, MOAITextBox::LEFT_JUSTIFY);
	int wordBreak = state.GetValue < int > (8, MOAITextBox::WORD_BREAK_NONE);
	bool returnGlyphBounds = state.GetValue < bool > (9, false);
	
	MOAITexture *texture = self->RenderTexture(text, fontSize, width, height, horizontalAlignment,
											   verticalAlignment, wordBreak, false, returnGlyphBounds,
											   state);
	state.Push(texture);
	if (returnGlyphBounds) {
		// return the glyph bound table after the texture
		
		// Move the table that would appear before the texture to the second return value.
		state.MoveToTop(-2);
		return 2;
	}
	
	return 1;
}
//----------------------------------------------------------------//
/** @name	renderTextureSingleLine
	@text	Produces a MOAITexture in RGBA_8888 format for the given string on a single line.
 
	@in		MOAIFont		self
	@in		string			text
	@in		number			fontSize
	@opt	bool			returnGlyphBounds   Whether to return additional information about
											glyph bounds.   Default to false.
	@out	MOAITexture		texture
	@out	number			width
	@out	number			height
	@out	table			glyphBounds			A table containing glyph bounds for each character.--
												  The table has two levels.  The top level contains 
												  one or more tables, one for each glyph in the 
												  string, indexed by integer beginning with one.  The
												  table for each glyph contains entries indexed by
												  the strings 'xMin', 'yMin', 'xMax', 'yMax' and
												  'baselineY', each corresponding to the bounds of
												  the glyph relative to the texture's coordinate
												  system with the origin being in the upper left
												  corner.  Nil if returnGlyphBounds is false.
 */
int MOAIFreeTypeFont::_renderTextureSingleLine(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "US" );
	cc8* text = state.GetValue < cc8* > (2, "");
	float fontSize = state.GetValue < float > (3, self->mDefaultSize);
	bool returnGlyphBounds = state.GetValue < bool > (4, false);
	
	USRect rect;
	MOAITexture *texture = self->RenderTextureSingleLine(text, fontSize, &rect, returnGlyphBounds, state);
	state.Push(texture);
	state.Push(rect.Width());
	state.Push(rect.Height());
	if (returnGlyphBounds) {
		// return the glyph bound table after the height information
		state.MoveToTop(-4);
		return 4;
	}
	
	return 3;
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

//================================================================//
// MOAIFreeTypeFont
//================================================================//

void MOAIFreeTypeFont::BuildLine(u32* buffer, size_t buf_len, int pen_x,
								 u32 lastChar, u32 startIndex){
	MOAIFreeTypeTextLine tempLine;
	
	FT_Face face = this->mFreeTypeFace;
	
	u32* text = (u32*)malloc(sizeof(u32) * (buf_len+1));
	memcpy(text, buffer, sizeof(u32) * buf_len);
	
	text[buf_len] = '\0';
	tempLine.text = text;
	
	// get last glyph
	int error = FT_Load_Char(face, lastChar, FT_LOAD_DEFAULT);
	
	CHECK_ERROR(error);
	
	tempLine.lineWidth = pen_x - (int)((face->glyph->metrics.horiAdvance - face->glyph->metrics.horiBearingX - face->glyph->metrics.width) >> 6);
	tempLine.startIndex = startIndex;
	
	this->mLineVector.push_back(tempLine);
	
}

void MOAIFreeTypeFont::BuildLine(u32 *buffer, size_t bufferLength, u32 startIndex){
	MOAIFreeTypeTextLine tempLine;
	
	u32* text = (u32*)malloc(sizeof(u32) * (bufferLength+1));
	memcpy(text, buffer, sizeof(u32) * bufferLength);
	
	text[bufferLength] = '\0';
	tempLine.text = text;
	
	tempLine.lineWidth = this->WidthOfString(text, bufferLength);
	tempLine.startIndex = startIndex;
	
	this->mLineVector.push_back(tempLine);
}

int MOAIFreeTypeFont::ComputeLineStart(FT_UInt unicode, int lineIndex, int alignment,
									   FT_Int imageWidth){
	int error = FT_Load_Char(this->mFreeTypeFace, unicode, FT_LOAD_DEFAULT);
	if (error) {
		return 0;
	}
	
	int retValue = 0;
	int adjustmentX = (int)-((this->mFreeTypeFace->glyph->metrics.horiBearingX) >> 6);
	
	int maxLineWidth = imageWidth;
	
	
	
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

int MOAIFreeTypeFont::ComputeLineStartY(int textHeight, FT_Int imageHeight, int vAlign){
	int retValue = 0;
	int adjustmentY = (int)((this->mFreeTypeFace->size->metrics.ascender) >> 6);
	
	if ( vAlign == MOAITextBox::CENTER_JUSTIFY ) {
		// vertical center
		retValue = (imageHeight - textHeight)/2 + adjustmentY;
	}
	else if( vAlign == MOAITextBox::RIGHT_JUSTIFY ){
		// vertical bottom
		retValue = imageHeight - textHeight + adjustmentY;
	}
	else{
		// vertical top or other value
		retValue = adjustmentY;
	}
	
	
	return retValue;
}

USRect MOAIFreeTypeFont::DimensionsOfLine(cc8 *text, float fontSize, bool returnGlyphBounds,
										  MOAILuaState& state){
	UNUSED(returnGlyphBounds);
	UNUSED(state);
	
	if (returnGlyphBounds) {
		FT_Vector *positions;
		FT_Glyph *glyphs;
		FT_UInt numGlyphs;
		FT_Error error;
		
		FT_Int maxDescender;
		FT_Int maxAscender;
		
		USRect rect = this->DimensionsOfLine(text, fontSize, &positions, &glyphs, &numGlyphs, &maxDescender, &maxAscender);
		u32 tableIndex;
		u32 tableSize = numGlyphs;
		
		// create main table with enough elements for the number of glyphs
		lua_createtable(state, tableSize, 0);
		
		u32 height = (u32)rect.Height();
		
		// load first glyph image to retrieve bitmap data
		FT_Glyph firstImage = glyphs[0];
		FT_Vector vec;
		vec.x = 0;
		vec.y = 0;
		error = FT_Glyph_To_Bitmap(&firstImage, FT_RENDER_MODE_NORMAL, &vec, 0);
		FT_BitmapGlyph firstBitmap = (FT_BitmapGlyph)firstImage;
		
		FT_Int leftOffset = firstBitmap->left;
		// set start position so that charaters get rendered completely
		
		// advance starting x position so that the first glyph has its left edge at zero
		FT_Pos startX = -leftOffset;
		
		
		FT_Pos startY = maxDescender;
		
		FT_Done_Glyph(firstImage);
		
		
		// iterate through the glyphs to do retrieve data
		for (size_t n = 0; n < numGlyphs; n++){
			tableIndex = (u32)n + 1;
			FT_Glyph image;
			FT_Vector pen;
			
			image = glyphs[n];
			
			FT_Pos posX = positions[n].x;
			FT_Pos posY = positions[n].y;
			
			pen.x = startX + posX;
			pen.y = startY + posY;
			
			error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, 0, 0);
			
			if (!error) {
				FT_BitmapGlyph bit = (FT_BitmapGlyph)image;
				FT_Int left = (FT_Int)pen.x + bit->left;
				FT_Int bottom = (FT_Int)pen.y + (height - bit->top);
				
				// create table with five elements
				lua_createtable(state, 5, 0);
				
				// push xMin
				int xMin = left;
				state.Push(xMin);
				lua_setfield(state, -2, "xMin");
				
				// push yMin
				int yMin = bottom;
				state.Push(yMin);
				lua_setfield(state, -2, "yMin");
				
				// push xMax
				int xMax = xMin + bit->bitmap.width;
				state.Push(xMax);
				lua_setfield(state, -2, "xMax");
				
				// push yMax
				int yMax = yMin + bit->bitmap.rows;
				state.Push(yMax);
				lua_setfield(state, -2, "yMax");
				
				// push baselineY
				int baselineY = maxAscender; 
				state.Push(baselineY);
				lua_setfield(state, -2, "baselineY");
				
				// set index for current glyph
				lua_rawseti(state, -2, tableIndex);
				
				
				FT_Done_Glyph(image);
			}
		}
		
		return rect;
	}
	else{
		return this->DimensionsOfLine(text, fontSize, NULL, NULL, NULL, NULL, NULL);
	}
}

/*
	@name		DimensionsOfLine
	@text		Calculates the bounding box for a given string at a given font size.
	@in			cc8*		text			The string to find dimensions of
	@in			float		fontSize		The font size to use
	@out		FT_Vector*	glyphPositions	An array of the locations of the glyphs
	@out		FT_Glyph*	glyphArray		An array of glyphs
	@out		FT_UInt		glyphNum		The total number of glyphs used
	@out		FT_Int		maxDescender	The minimum y-position of the glyph bounding boxes
	@out		FT_Int		maxAscender		The maximum y-position of the glyph bounding boxes
	@return		USRect		rect			The bounding box of the string
 */
USRect MOAIFreeTypeFont::DimensionsOfLine(cc8 *text, float fontSize, FT_Vector **glyphPositions, FT_Glyph **glyphArray, FT_UInt *glyphNum, FT_Int *maxDescender, FT_Int *maxAscender){
	USRect rect;
	rect.Init(0,0,0,0);
	
	// initialize library if needed
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
	error = FT_Set_Char_Size(face,							/* handle to face object           */
							 0,								/* char_width in 1/64th of points  */
							 (FT_F26Dot6)( 64 * fontSize ),	/* char_height in 1/64th of points */
							 DPI,							/* horizontal device resolution    */
							 0);							/* vertical device resolution      */
	CHECK_ERROR(error);
	
	if (maxDescender) {
		*maxDescender = 0;
	}
	
	if (maxAscender) {
		*maxAscender = 0;
	}
	
	
	FT_GlyphSlot  slot = face->glyph;
	FT_UInt numGlyphs = 0;
	FT_UInt previousGlyphIndex = 0;
	
	size_t maxGlyphs = strlen(text);
	FT_Glyph* glyphs = new FT_Glyph [maxGlyphs];
	FT_Vector* positions = new FT_Vector [maxGlyphs];
	
	bool useKerning = FT_HAS_KERNING(face);
	
	// Gather up the positions of each glyph
	FT_Int penX = 0, penY = 0;
	
	for (size_t n = 0; n < maxGlyphs; ) {
		FT_UInt glyphIndex;
		
		int idx = (int)n;
		u32 unicode = u8_nextchar(text, &idx);
		
		n = (size_t)idx;
		
		glyphIndex = FT_Get_Char_Index(face, unicode);
		
		if (useKerning && previousGlyphIndex && glyphIndex)
		{
			FT_Vector delta;
			FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
			penX += (delta.x >> 6);
		}
		
		positions[numGlyphs].x = penX;
		positions[numGlyphs].y = penY;
		
		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
		CHECK_ERROR(error);
		
		error = FT_Get_Glyph( face->glyph, &glyphs[numGlyphs]);
		CHECK_ERROR(error);
		
		penX += (slot->advance.x >> 6);
		
		previousGlyphIndex = glyphIndex;
		numGlyphs++;
	}
	
	// compute the bounding box of the glyphs
	FT_BBox  boundingBox;
	FT_BBox  glyphBoundingBox;
	
	boundingBox.xMin = 32000;
	boundingBox.xMax = -32000;
	
	FT_Size_Metrics metrics = face->size->metrics;
	
	
	boundingBox.yMin = metrics.descender >> 6;
	boundingBox.yMax = metrics.ascender >> 6;
	
	for (FT_UInt n = 0; n < numGlyphs; n++)
	{
		FT_Glyph_Get_CBox( glyphs[n], FT_GLYPH_BBOX_PIXELS, &glyphBoundingBox);
        
		if (maxDescender && glyphBoundingBox.yMin < *maxDescender) {
			*maxDescender = (FT_Int) glyphBoundingBox.yMin;
		}
		
		if (maxAscender && glyphBoundingBox.yMax > *maxAscender) {
			*maxAscender = (FT_Int) glyphBoundingBox.yMax;
		}
		
        // translate the glyph bounding box by vector in positions[n]
		glyphBoundingBox.xMin += positions[n].x;
		glyphBoundingBox.xMax += positions[n].x;
		glyphBoundingBox.yMin += positions[n].y;
		glyphBoundingBox.yMax += positions[n].y;
		
        // expand the string bounding box to include the glyph bounding box
		if ( glyphBoundingBox.xMin < boundingBox.xMin )
		{
			boundingBox.xMin = glyphBoundingBox.xMin;
		}
		
		if ( glyphBoundingBox.xMax > boundingBox.xMax )
		{
			boundingBox.xMax = glyphBoundingBox.xMax;
		}
		
		if ( boundingBox.xMin > boundingBox.xMax )
		{
			boundingBox.xMax = 0;
			boundingBox.xMin = 0;
			boundingBox.yMax = 0;
			boundingBox.yMin = 0;
		}
	}
    // calculate width and height of string
	FT_Pos stringWidth = boundingBox.xMax - boundingBox.xMin;
	FT_Pos stringHeight = boundingBox.yMax - boundingBox.yMin;
	
	rect.mXMax = stringWidth;
	rect.mYMax = stringHeight;
	
	// clean-up
	if (glyphArray) {
		*glyphArray = glyphs;
	}
	else{
		delete [] glyphs;
	}
	
	if (glyphNum) {
		*glyphNum = numGlyphs;
	}
	
	
	if (glyphPositions) {
		*glyphPositions = positions;
	}
	else{
		delete [] positions;
	}
	
	return rect;
}

USRect MOAIFreeTypeFont::DimensionsWithMaxWidth(cc8 *text, float fontSize, float width, int wordBreak, bool returnGlyphBounds,
												MOAILuaState& state){
	UNUSED(returnGlyphBounds);
	UNUSED(state);
	USRect rect;
	rect.Init(0,0,0,0);
	
	FT_Error error;
	
	// initialize the font
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
							 (FT_F26Dot6)( 64 * fontSize ),	/* char_height in 1/64th of points */
							 DPI,					/* horizontal device resolution    */
							 0);					/* vertical device resolution      */
	CHECK_ERROR(error);
	
	FT_Int pen_x, pen_y;
	
	// get the number of lines needed to display the text and populate line vector
	int numLines = this->NumberOfLinesToDisplayText(text, width, wordBreak, true);
	
	FT_Int lineHeight = ((FT_Int)face->size->metrics.height >> 6);
	
	size_t vectorSize = this->mLineVector.size();
	u32 tableIndex;
	if (returnGlyphBounds){
		lua_createtable(state, (u32)vectorSize, 0);
		
		FT_Int textHeight = lineHeight * numLines;
		FT_Int imageHeight = textHeight;
		int vAlign = MOAITextBox::LEFT_JUSTIFY;
		pen_y = this->ComputeLineStartY(textHeight, imageHeight, vAlign);
	}
	
	FT_Int imageWidth = (FT_Int)width;
	
	FT_UInt glyphIndex = 0;
	FT_UInt previousGlyphIndex = 0;
	bool useKerning = FT_HAS_KERNING(face);
	
	
	
	// find maximum line width in the line vector
	int maxLineWidth = 0;
	for (u32 i = 0; i < vectorSize; i++) {
		int lineWidth = this->mLineVector[i].lineWidth;
		if (lineWidth > maxLineWidth) {
			maxLineWidth = lineWidth;
		}
		
		if (returnGlyphBounds) {
			
			
			u32* text_ptr = this->mLineVector[i].text;
			
			tableIndex = 1 + i;
			int hAlign = MOAITextBox::LEFT_JUSTIFY;
			pen_x = this->ComputeLineStart(text_ptr[0], i, hAlign, imageWidth);
			
			
			size_t text_len = (size_t)MOAIFreeTypeFont::WideCharStringLength(text_ptr);
			// create the line sub-table with enough spaces for the glyphs in the line,
			// the baseline entry and the string of rendered characters.
			lua_createtable(state, (u32)text_len + 1, 0);
			
			for (u32 i2 = 0; i2 < text_len;  ++i2) {
				u32 lineIndex = 1 + i2;
				
				error = FT_Load_Char(face, text_ptr[i2], FT_LOAD_RENDER);
				if (error) {
					break;
				}
				FT_Bitmap bitmap = face->glyph->bitmap;
				
				glyphIndex = FT_Get_Char_Index(face, text_ptr[i2]);
				
				if (useKerning && glyphIndex && previousGlyphIndex) {
					FT_Vector delta;
					FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
					pen_x += (delta.x >> 6);
				}
				
				int yOffset = pen_y - ((u32)face->glyph->metrics.horiBearingY >> 6);
				int xOffset = pen_x + ((u32)face->glyph->metrics.horiBearingX >> 6);
				
				// create table with four elements
				lua_createtable(state, 4, 0);
				
				// push xMin
				int xMin = xOffset;
				state.Push(xMin);
				lua_setfield(state, -2, "xMin");
				
				// push yMin
				int yMin = yOffset;
				state.Push(yMin);
				lua_setfield(state, -2, "yMin");
				
				// push xMax
				int xMax = xOffset + bitmap.width;
				state.Push(xMax);
				lua_setfield(state, -2, "xMax");
				
				// push yMax
				int yMax = yOffset + bitmap.rows;
				state.Push(yMax);
				lua_setfield(state, -2, "yMax");
				
				// set index for current glyph in line
				lua_rawseti(state, -2, lineIndex);
				
				// step to next glyph
				pen_x += (face->glyph->metrics.horiAdvance >> 6);
				
				previousGlyphIndex = glyphIndex;
			} 
			
			// push baselineY to line sub-table
			int baselineY = pen_y; 
			state.Push(baselineY);
			lua_setfield(state, -2, "baselineY");
			
			// push rendered characters string to line sub-table
			u32 utfLen = MOAIFreeTypeFont::LengthOfUTF8Sequence(text_ptr) + 1;
			char *utfString = (char*)malloc(sizeof(char) * utfLen);
			
			u8_toutf8(utfString, utfLen, text_ptr, (u32)text_len);
			
			state.Push(utfString);
			lua_setfield(state, -2, "renderedCharacters");
			
			// set index for current line sub-table
			lua_rawseti(state, -2, tableIndex);
			
			pen_y += (face->size->metrics.height >> 6);
		} 
		
		
	} 
	
	// free the text lines
	for (size_t i = 0; i < this->mLineVector.size(); i++) {
		MOAIFreeTypeTextLine line = this->mLineVector[i];
		free(line.text);
	}
	// clear the line vector member variable for reuse.
	this->mLineVector.clear();
	
	rect.mXMax = maxLineWidth;
	rect.mYMax = lineHeight * numLines;
	
	return rect;
}

void MOAIFreeTypeFont::DrawBitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y, FT_Int imageWidth, FT_Int imageHeight){
	FT_Int i, j, k, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;
	
	int idx = 0;
	u8 value, formerValue;
	
	// fill the values with data from bitmap->buffer
	for (i = x, p = 0; i < x_max; i++, p++) {
		for (j = y, q = 0; j < y_max; j++, q++) {
			// compute index for bitmap data pixel red value.  Uses mBitmapWidth instead of imageWidth
			idx = (j * this->mBitmapWidth + i) * BYTES_PER_PIXEL;
			
			// retrieve value from the character bitmap
			value = bitmap->buffer[q * bitmap->width + p];
			// skip this if the location is out of bounds or the value is zero
			if (i < 0 || j < 0 || i >= imageWidth || j >= imageHeight || value == 0) {
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

void MOAIFreeTypeFont::GenerateLines(FT_Int imageWidth, cc8 *text, int wordBreak){
	this->NumberOfLinesToDisplayText(text, imageWidth, wordBreak, true);
}

float MOAIFreeTypeFont::EstimatedMaxFontSize(float height, float inputSize){
	FT_Face face = this->mFreeTypeFace;
	FT_Int lineHeight = ((u32)face->size->metrics.height >> 6);
	
	float ratio = height / lineHeight ;
	
	
	return inputSize * ratio;
}

FT_Int MOAIFreeTypeFont::GetLineHeight(){
	FT_Face face = this->mFreeTypeFace;
	
	return ((u32)face->size->metrics.height >> 6);
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
	
	size_t bmpSize = bmpW * bmpH * BYTES_PER_PIXEL;
	
	this->ResetBitmapData();
	
	this->mBitmapData = (unsigned char*)calloc( bmpSize, sizeof( unsigned char ) );
	this->mBitmapWidth = bmpW;
	this->mBitmapHeight = bmpH;
}

bool MOAIFreeTypeFont::IsFreeTypeInitialized(){
	return this->mFreeTypeFace != NULL;
}


bool MOAIFreeTypeFont::IsWordBreak(u32 character, int wordBreakMode){
	bool isWordBreak = false;
	
	switch (wordBreakMode) {
		case MOAITextBox::WORD_BREAK_NONE:
			// break for white space only
			isWordBreak = MOAIFont::IsWhitespace(character);
			break;
			
		case MOAITextBox::WORD_BREAK_HYPHEN:
			// break for white space and hyphens.
			isWordBreak = MOAIFont::IsWhitespace(character) || character == '-';
			break;
			
		default: // MOAITextBox::WORD_BREAK_CHAR and others
			break;
	}
	
	return isWordBreak;
}

// determines the length of the wide character string 'sequence' when converted to a string in UTF-8 encoding.
u32 MOAIFreeTypeFont::LengthOfUTF8Sequence(const u32 *sequence){
	u32 len = 0;
	u32 i = 0;
	u32 wc;
	
	char dest[4];
	
	while ((wc = sequence[i]) != '\0') {
		
		len += u8_wc_toutf8(dest, wc);
		i++;
	}
	
	
	return len;
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
	this->ResetBitmapData();
}

/*
	@name	NumberOfLinesToDisplayText
	@text	The body of OptimalSize() and GenerateLines().  Serves a dual purpose to return 
				the number of lines calculated given the font and text box width and fill up this->mLineVector.
				It assumes that this->mFreeTypeFace has been initialized and has had the font
				size set with FT_Set_Char_Size().
	@in		cc8* text			The string to query
	@in		FT_Int imageWidth	The width of the text box in which to render the string
	@in		int wordBreakMode	The word break mode to use.
	@in		bool generateLines	Populate the line vector member variable with lines of 
									text as well.
	@return int					The number of lines required to display the given text 
									without clipping any characters
 */
int MOAIFreeTypeFont::NumberOfLinesToDisplayText(cc8 *text, FT_Int imageWidth,
												 int wordBreakMode, bool generateLines){
	FT_Error error = 0;
	FT_Face face = this->mFreeTypeFace;
	
	bool useKerning = FT_HAS_KERNING(face);
	
	int numberOfLines = 1;
	
	u32 unicode; // the current unicode character
	u32 lastCh = 0; // the previous unicode character
	u32 lastTokenCh = 0; // the character before a word break
	u32 wordBreakCharacter = 0; // the character used in the word break
	
	FT_UInt previousGlyphIndex = 0;
	FT_UInt glyphIndex = 0;
	
	FT_Int penXReset = 0; // the value to which to reset the x-location of the cursor on a new line
	FT_Int penX = penXReset; // the current x-location of the cursor
	FT_Int lastTokenX = 0; // the x-location of the cursor at the most recent word break
	
	int lineIndex = 0; // the index of the beginning of the current line
	int tokenIndex = 0; // the index of the beginning of the current token
	
	u32 startIndex = 0; // the value for the final parameter of BuildLine()
	
	size_t textLength = 0;
	size_t lastTokenLength = 0;
	
	u32* textBuffer = NULL;
	if (generateLines) {
		textBuffer = (u32 *) calloc(strlen(text), sizeof(u32));
	}
	
	int n = 0;
	while ( (unicode = u8_nextchar(text, &n) ) ) {
		
		startIndex = (u32) lineIndex;
		
		// handle new line
		if (unicode == '\n'){
			numberOfLines++;
			penX = penXReset;
			lineIndex = tokenIndex = n;
			textLength = lastTokenLength = 0;
			if (generateLines) {
				this->BuildLine(textBuffer, textLength, startIndex);
				
				error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
				CHECK_ERROR(error);
			}
			
			continue;
		}
		// handle word breaking characters
		else if ( MOAIFreeTypeFont::IsWordBreak(unicode, wordBreakMode) ){
			tokenIndex = n;
			lastTokenLength = textLength;
			lastTokenCh = lastCh;
			lastTokenX = penX;
			wordBreakCharacter = unicode;
		}
		
		error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
		
		CHECK_ERROR(error);
		
		glyphIndex = FT_Get_Char_Index(face, unicode);
		
		// take kerning into account
		if (useKerning && previousGlyphIndex && glyphIndex) {
			FT_Vector delta;
			FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
			penX += (delta.x >> 6);
		}
		
		// test for first character of line to adjust penX
		if (textLength == 0) {
			penX += -((face->glyph->metrics.horiBearingX) >> 6);
		}
		
		// determine if penX is outside the bounds of the box
		FT_Pos glyphWidth = ((face->glyph->metrics.width) >> 6);
		FT_Pos nextPenX = penX + glyphWidth;
		bool isExceeding = (nextPenX > imageWidth);
		if (isExceeding) { 
			if (wordBreakMode == MOAITextBox::WORD_BREAK_CHAR) {
				if (generateLines) {
					this->BuildLine(textBuffer, textLength, startIndex);
				}
				
				// advance to next line
				numberOfLines++;
				textLength = 0;
				penX = penXReset;
				lineIndex = tokenIndex = n;
			}
			else{ // WORD_BREAK_NONE and other modes
				if (tokenIndex != lineIndex) {
					
					if (generateLines) {
						// include the hyphen in WORD_BREAK_HYPHEN
						if (wordBreakMode == MOAITextBox::WORD_BREAK_HYPHEN &&
							!MOAIFont::IsWhitespace(wordBreakCharacter)) {
							// add the word break character to the text buffer
							textBuffer[lastTokenLength] = wordBreakCharacter;
							lastTokenLength++;
						}
						
						this->BuildLine(textBuffer, lastTokenLength, startIndex);
					}
					else if (wordBreakMode == MOAITextBox::WORD_BREAK_HYPHEN &&
							 !MOAIFont::IsWhitespace(wordBreakCharacter)){
						// test to see if the word break character is being cut off
						if (unicode == wordBreakCharacter && n == tokenIndex) {
							return -1;
						}
						
					}
					
					// set n back to the last index
					n = tokenIndex;
					// get the character after token index and update n
					unicode = u8_nextchar(text, &n);
					
					// load the character after token index to get its width aka horiAdvance
					error = FT_Load_Char(face, unicode, FT_LOAD_DEFAULT);
					
					CHECK_ERROR(error);
					
					//advance to next line
					numberOfLines++;
					penX = penXReset;
					lineIndex = tokenIndex = n;
					
					// reset text length and last token length
					textLength = lastTokenLength = 0;
				}
				else{
					if (generateLines) {
						// put the rest of the token on the next line
						this->BuildLine(textBuffer, textLength, penX, lastCh, startIndex);
						textLength = lastTokenLength = 0;
						
						// advance to next line
						numberOfLines++;
						penX = penXReset;
						lineIndex = tokenIndex = n;
					}
					else{
						// we don't words broken up when calculating optimal size
						// return a failure code that is less than zero
						return -1;
					}
				}
			}
		}
		
		lastCh = unicode;
		previousGlyphIndex = glyphIndex;
		
		if (generateLines) {
			textBuffer[textLength] = unicode;
		}
		++textLength;
		
		// advance cursor
		penX += ((face->glyph->metrics.horiAdvance) >> 6);
		
	}
	if (generateLines) {
		this->BuildLine(textBuffer, textLength, startIndex);
		free(textBuffer);
	}
	
	return numberOfLines;
}


float MOAIFreeTypeFont::OptimalSize(const MOAIOptimalSizeParameters& params ){
	
	cc8 *text = params.text;
	float width = params.width;
	float height = params.height;
	float maxFontSize = params.maxFontSize;
	
	float minFontSize = params.minFontSize;
	int wordbreak = params.wordBreak;
	bool forceSingleLine = params.forceSingleLine;
	float granularity = params.granularity;
	bool roundToInteger = params.roundToInteger;
	
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
	
	float lowerBoundSize = minFontSize;
	float upperBoundSize = maxFontSize + 1.0f;
	
	float estimatedMaxSize = this->EstimatedMaxFontSize(height, maxFontSize);
	
	if (estimatedMaxSize < maxFontSize){
		upperBoundSize  = ceilf(estimatedMaxSize) + 1.0f;
	}
	
	
	FT_Int imageWidth = (FT_Int)width;
	
	// test size
	float testSize = (lowerBoundSize + upperBoundSize) / 2.0f;
	
	// the minimum difference between upper and lower bound sizes before the binary search stops.
	do{
		
		// set character size to test size
		error = FT_Set_Char_Size(face,
								 0,
								 (FT_F26Dot6)(64 * testSize),
								 DPI,
								 0);
		CHECK_ERROR(error);
		
		
		// compute maximum number of lines allowed at font size.
		// forceSingleLine sets this value to one if true.
		FT_Pos lineHeight = (face->size->metrics.height >> 6);
		int maxLines = (forceSingleLine && (height / lineHeight) > 1)? 1 : (height / lineHeight);
		
		numLines = this->NumberOfLinesToDisplayText(text, imageWidth, wordbreak, false);
		
		if (numLines > maxLines || numLines < 0){ // failure case
			// adjust upper bound downward
			upperBoundSize = testSize;
		}
		else{ // success
			// adjust lower bound upward
			lowerBoundSize = testSize;
		}
		// recalculate test size
		testSize = (lowerBoundSize + upperBoundSize) / 2.0f;
		
		
	}while(upperBoundSize - lowerBoundSize >= granularity);
	
	// test the proposed return value for a failure case
	if (roundToInteger) {
		testSize = floorf(lowerBoundSize);
	}
	else{
		testSize = lowerBoundSize;
	}
	
	
	// set character size to test size
	error = FT_Set_Char_Size(face,
							 0,
							 (FT_F26Dot6)(64 * testSize),
							 DPI,
							 0);
	CHECK_ERROR(error);
	// compute maximum number of lines allowed at font size.
	// forceSingleLine sets this value to one if true.
	FT_Pos lineHeight = (face->size->metrics.height >> 6);
	int maxLines = (forceSingleLine && (height / lineHeight) > 1)? 1 : (height / lineHeight);
	
	numLines = this->NumberOfLinesToDisplayText(text, imageWidth, wordbreak, false);
	
	if (numLines > maxLines || numLines < 0){ // failure case, which DOES happen rarely
		// decrement return value by one
		testSize = testSize - 1.0f;
		
		// make sure return value does not go below the minFontSize parameter
		if (testSize < minFontSize) {
			testSize = minFontSize;
		}
		
	}
	
	
	
	return testSize;
}

//----------------------------------------------------------------//

void MOAIFreeTypeFont::RegisterLuaClass ( MOAILuaState& state ) {	
	luaL_Reg regTable [] = {
		{NULL, NULL}
	};
	
	luaL_register( state, 0, regTable);	
}

//----------------------------------------------------------------//

void MOAIFreeTypeFont::RegisterLuaFuncs(MOAILuaState &state){
	luaL_Reg regTable [] = {
		{ "dimensionsOfLine",			_dimensionsOfLine },
		{ "dimensionsWithMaxWidth",		_dimensionsWithMaxWidth },
		{ "getDefaultSize",				_getDefaultSize },
		{ "getFilename",				_getFilename },
		{ "load",						_load },
		{ "optimalSize",				_optimalSize },
		{ "renderTexture",				_renderTexture },
		{ "renderTextureSingleLine",	_renderTextureSingleLine },
		{ "setDefaultSize",				_setDefaultSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

void MOAIFreeTypeFont::RenderLines(FT_Int imageWidth, FT_Int imageHeight, int hAlign, int vAlign,
								   bool returnGlyphBounds, MOAILuaState& state){
	FT_Int pen_x, pen_y;
	
	FT_Face face = this->mFreeTypeFace;
	
	FT_Int textHeight = (FT_Int) ((face->size->metrics.height >> 6) * this->mLineVector.size());
	
	pen_y = this->ComputeLineStartY(textHeight, imageHeight, vAlign);
	
	FT_UInt glyphIndex = 0;
	FT_UInt previousGlyphIndex = 0;
	bool useKerning = FT_HAS_KERNING(face);
	
	size_t vectorSize = (int) this->mLineVector.size();
	
	// set up Lua table for return
	u32 tableIndex;
	if (returnGlyphBounds) {
		
		// create the main table with enough spaces for each line
		lua_createtable(state, (int)vectorSize, 0);
		//lua_createtable(state, tableSize, 0);
		//glyphBoundTable.SetWeakRef(state, -1);
	}
	
	
	for (u32 i = 0; i < (u32)vectorSize;  i++) {
		
		u32* text_ptr = this->mLineVector[i].text;
		
		tableIndex = 1 + i; 
		
		// calcluate origin cursor
		pen_x = this->ComputeLineStart(text_ptr[0], i, hAlign, imageWidth);
		
		
		u32 text_len = MOAIFreeTypeFont::WideCharStringLength(text_ptr);
		
		if (returnGlyphBounds) {
			// create the line sub-table with enough spaces for the glyphs in the line,
			// the baseline entry and the string of rendered characters.
			lua_createtable(state, (int) text_len + 1, 0);
		}
		
		for (size_t i2 = 0; i2 < text_len; ++i2) {
			
			u32 lineIndex = (u32)(1 + i2);
			
			int error = FT_Load_Char(face, text_ptr[i2], FT_LOAD_RENDER);
			if (error) {
				break;
			}
			
			FT_Bitmap bitmap = face->glyph->bitmap;
			
			glyphIndex = FT_Get_Char_Index(face, text_ptr[i2]);
			
			if (useKerning && glyphIndex && previousGlyphIndex) {
				FT_Vector delta;
				FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
				pen_x += (delta.x >> 6);
			}
			
			int yOffset = (int)(pen_y - (face->glyph->metrics.horiBearingY >> 6));
			int xOffset = (int)(pen_x + (face->glyph->metrics.horiBearingX >> 6));
			
			this->DrawBitmap(&bitmap, xOffset, yOffset, imageWidth, imageHeight);
			
			
			if (returnGlyphBounds){
				// create table with four elements
				lua_createtable(state, 4, 0);
				
				// push xMin
				int xMin = xOffset;
				state.Push(xMin);
				lua_setfield(state, -2, "xMin");
				
				// push yMin
				int yMin = yOffset;
				state.Push(yMin);
				lua_setfield(state, -2, "yMin");
				
				// push xMax
				int xMax = xOffset + bitmap.width;
				state.Push(xMax);
				lua_setfield(state, -2, "xMax");
				
				// push yMax
				int yMax = yOffset + bitmap.rows;
				state.Push(yMax);
				lua_setfield(state, -2, "yMax");
				
				// set index for current glyph in line
				lua_rawseti(state, -2, (int) lineIndex);
				
			}
			
			// step to next glyph
			pen_x += (face->glyph->metrics.horiAdvance >> 6);
			
			previousGlyphIndex = glyphIndex;
			
		}
		
		if (returnGlyphBounds) {
			// push baselineY to line sub-table
			int baselineY = pen_y; 
			state.Push(baselineY);
			lua_setfield(state, -2, "baselineY");
			
			// push rendered characters string to line sub-table
			u32 utfLen = MOAIFreeTypeFont::LengthOfUTF8Sequence(text_ptr) + 1;
			char *utfString = (char*)malloc(sizeof(char) * utfLen);
			
			u8_toutf8(utfString, utfLen, text_ptr, text_len);
			
			state.Push(utfString);
			lua_setfield(state, -2, "renderedCharacters");
			
			// set index for current line sub-table
			lua_rawseti(state, -2, tableIndex);
		}
		
		pen_y += (face->size->metrics.height >> 6);
	}
	
	// free the text lines
	for (size_t i = 0; i < this->mLineVector.size(); i++) {
		MOAIFreeTypeTextLine line = this->mLineVector[i];
		free(line.text);
	}
	// clear the line vector member variable for reuse.
	this->mLineVector.clear();
	
}

MOAITexture* MOAIFreeTypeFont::RenderTexture(cc8 *text, float size, float width, float height,
											 int hAlignment, int vAlignment, int wordbreak,
											 bool autoFit, bool returnGlyphBounds, MOAILuaState& state){
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
	
	
	FT_Int imageWidth = (FT_Int)width;
	FT_Int imageHeight = (FT_Int)height;
	
	// create the image data buffer
	this->InitBitmapData(imageWidth, imageHeight);
	
	// create the lines of text
	this->GenerateLines(imageWidth, text, wordbreak);
	
	// render the lines to the data buffer
	this->RenderLines(imageWidth, imageHeight, hAlignment, vAlignment, returnGlyphBounds, state);
	
	// turn the data buffer into an image
	MOAIImage bitmapImg;
	bitmapImg.Init(this->mBitmapData, this->mBitmapWidth, this->mBitmapHeight, USColor::RGBA_8888);
	
	
	// create a texture from the image
	MOAITexture *texture = new MOAITexture();
	texture->Init(bitmapImg, "");
	
	return texture;
}

MOAITexture* MOAIFreeTypeFont::RenderTextureSingleLine(cc8 *text, float fontSize, USRect *rect, bool returnGlyphBounds, MOAILuaState& state){
	
	// get dimensions of the line of text
	FT_Vector *positions;
	FT_Glyph *glyphs;
	FT_UInt numGlyphs;
	FT_Error error;
	
	FT_Int maxDescender;
	FT_Int maxAscender;
	
	USRect dimensions = this->DimensionsOfLine(text, fontSize, &positions, &glyphs, &numGlyphs, &maxDescender, &maxAscender);
	
	rect->Init(0.0, 0.0, 0.0, 0.0);
	rect->Grow(dimensions);
	
	u32 width = (u32)dimensions.Width();
	u32 height = (u32)dimensions.Height();
	
	FT_Int imageWidth = (FT_Int)dimensions.Width();
	FT_Int imageHeight = (FT_Int)dimensions.Height();
	
	// create an image buffer of the proper size
	this->InitBitmapData(width, height);
	
	
	u32 tableIndex;
	u32 tableSize = numGlyphs;
	if (returnGlyphBounds) {
		// create main table with enough elements for the number of glyphs
		lua_createtable(state, tableSize, 0);
	}
	
	// load first glyph image to retrieve bitmap data
	FT_Glyph firstImage = glyphs[0];
	FT_Vector vec;
	vec.x = 0;
	vec.y = 0;
	error = FT_Glyph_To_Bitmap(&firstImage, FT_RENDER_MODE_NORMAL, &vec, 0);
	FT_BitmapGlyph firstBitmap = (FT_BitmapGlyph)firstImage;
	
	FT_Int leftOffset = firstBitmap->left;
	// set start position so that charaters get rendered completely
	
	// advance starting x position so that the first glyph has its left edge at zero
	FT_Pos startX = -leftOffset;
	
	FT_Size_Metrics metrics = this->mFreeTypeFace->size->metrics;
	FT_Pos startY = metrics.descender >> 6; 
	
	FT_Done_Glyph(firstImage);
	
	// render the glyphs to the image bufer
	for (size_t n = 0; n < numGlyphs; n++) {
		tableIndex = (u32)n + 1;
		FT_Glyph image;
		FT_Vector pen;
		
		image = glyphs[n];
		
		FT_Pos posX = positions[n].x;
		FT_Pos posY = positions[n].y;
		
		pen.x = startX + posX;
		pen.y = startY + posY;
		
		error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, 0, 0);
		
		if (!error) {
			FT_BitmapGlyph bit = (FT_BitmapGlyph)image;
			FT_Int left = (int) pen.x + bit->left;
			FT_Int bottom = (int) pen.y + (height - bit->top);//(height - bit->top);
			
			this->DrawBitmap(&bit->bitmap, left, bottom, imageWidth, imageHeight);
			
			
			if (returnGlyphBounds) {
				// create table with five elements
				lua_createtable(state, 5, 0);
				
				// push xMin
				int xMin = left;
				state.Push(xMin);
				lua_setfield(state, -2, "xMin");
				
				// push yMin
				int yMin = bottom;
				state.Push(yMin);
				lua_setfield(state, -2, "yMin");
				
				// push xMax
				int xMax = xMin + bit->bitmap.width;
				state.Push(xMax);
				lua_setfield(state, -2, "xMax");
				
				// push yMax
				int yMax = yMin + bit->bitmap.rows;
				state.Push(yMax);
				lua_setfield(state, -2, "yMax");
				
				// push baselineY
				int baselineY = maxAscender;
				state.Push(baselineY);
				lua_setfield(state, -2, "baselineY");
				
				// set index for current glyph
				lua_rawseti(state, -2, tableIndex);
			}
			
			FT_Done_Glyph(image);
		}
		
	}
	
	
	// turn the data buffer to an image
	MOAIImage bitmapImg;
	bitmapImg.Init(this->mBitmapData, this->mBitmapWidth, this->mBitmapHeight, USColor::RGBA_8888);
	
	// create a texture from the image
	MOAITexture *texture = new MOAITexture();
	texture->Init(bitmapImg, "");
	return texture;
}


void MOAIFreeTypeFont::ResetBitmapData(){
	if (this->mBitmapData) {
		free(this->mBitmapData);
		this->mBitmapData = NULL;
	}
}

void MOAIFreeTypeFont::SetCharacterSize(float fontSize){
	FT_Error error = FT_Set_Char_Size(this->mFreeTypeFace,
							 0,
							 (FT_F26Dot6)(64 * fontSize),
							 DPI,
							 0);
	CHECK_ERROR(error);
}


// this method assumes that the font size has been set by FT_Set_Size
int MOAIFreeTypeFont::WidthOfString(u32* buffer, size_t bufferLength){
	int totalWidth = 0;
	FT_Face face = this->mFreeTypeFace;
	
	FT_Error error;
	
	FT_GlyphSlot  slot = face->glyph;
	FT_UInt previousGlyphIndex = 0;
	
	FT_Glyph* glyphs = new FT_Glyph [bufferLength];
	FT_Pos* xPositions = new FT_Pos [bufferLength];
	
	FT_Pos penX = 0;
	
	bool useKerning = FT_HAS_KERNING(face);
	
	for (size_t n = 0; n < bufferLength; n ++){
		FT_UInt glyphIndex;
		
		u32 unicode = buffer[n];
		glyphIndex = FT_Get_Char_Index(face, unicode);
		
		if (useKerning && previousGlyphIndex && glyphIndex){
			FT_Vector delta;
			FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
			penX += (delta.x >> 6);
		}
		
		xPositions[n] = (FT_Pos)penX;
		
		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
		CHECK_ERROR(error);
		
		error = FT_Get_Glyph(face->glyph, &glyphs[n]);
		CHECK_ERROR(error);
		
		penX += (slot->advance.x >> 6);
		
		previousGlyphIndex = glyphIndex;
	}
	
	FT_BBox boundingBox;
	FT_BBox glyphBoundingBox;
	
	boundingBox.xMin = 32000;
	boundingBox.xMax = -32000;
	boundingBox.yMin = 0;
	boundingBox.yMax = 0;
	
	for (size_t n = 0; n < bufferLength; n++) {
		FT_Glyph_Get_CBox( glyphs[n], FT_GLYPH_BBOX_PIXELS, &glyphBoundingBox);
		
		glyphBoundingBox.xMin += xPositions[n];
		glyphBoundingBox.xMax += xPositions[n];
		
		if ( glyphBoundingBox.xMin < boundingBox.xMin )
		{
			boundingBox.xMin = glyphBoundingBox.xMin;
		}
		
		if ( glyphBoundingBox.xMax > boundingBox.xMax )
		{
			boundingBox.xMax = glyphBoundingBox.xMax;
		}
		
		if ( boundingBox.xMin > boundingBox.xMax )
		{
			boundingBox.xMax = 0;
			boundingBox.xMin = 0;
			boundingBox.yMax = 0;
			boundingBox.yMin = 0;
		}
	}
	
	totalWidth = (int)(boundingBox.xMax - boundingBox.xMin);
	
	return	totalWidth;
}

u32 MOAIFreeTypeFont::WideCharStringLength(u32 *string){
	u32 length = 0;
	u32 i = 0;
	while (string[i++] != '\0') {
		length++;
	}
	return length;
}