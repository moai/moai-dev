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

void RenderBitmapToTarget(FT_Bitmap *bitmap, u8 *target, int x, int y, int bufferPitch);

#define BYTES_PER_PIXEL 1

#define CHECK_ERROR(error) if (error != 0) { printf("freetype fail %d at __LINE__", error); exit(-1); }

// ------------------------------------------------------------------------ //

int MOAIFreeTypeTextBox::_generateLabelTexture	( lua_State* L )
{
	MOAILuaState state ( L );

	cc8* text = state.GetValue<cc8*> (1, "");
	MOAIFreeTypeFont *f = state.GetLuaObject<MOAIFreeTypeFont>(2, false);
	int size = state.GetValue < int > (3, 12);
	int width = state.GetValue < int > (4, 100);
	int height = state.GetValue < int > (5, 100);
	int alignment = state.GetValue < int > (6, MOAITextBox::CENTER_JUSTIFY );
	int wordBreak = state.GetValue < int > (7, MOAITextBox::WORD_BREAK_CHAR );

	MOAITexture *texture = MOAIFreeTypeTextBox::GenerateTexture(text, f, size, width, height, alignment, wordBreak);

	state.Push(texture);
	return 1;
}

void RenderBitmapToTarget(FT_Bitmap *bitmap, u8 *target, int x, int y, int bufferPitch)
{
	size_t offset = y*bufferPitch + x*BYTES_PER_PIXEL;
	
	for (int row = 0; row < bitmap->rows; row++)
	{
		memcpy(target + offset, bitmap->buffer + row*bitmap->pitch, bitmap->pitch);
		offset += bufferPitch;
	}
}

MOAITexture *MOAIFreeTypeTextBox::GenerateTexture( cc8 *text, MOAIFreeTypeFont *font, int size, int width, int height, int alignment, int wordbreak ) {
	UNUSED(alignment);
	UNUSED(wordbreak);

#define MAX_GLYPHS 100
	FT_Glyph      glyphs[MAX_GLYPHS];
	FT_Vector     positions[MAX_GLYPHS];
	
	FT_Error error;
	// initialize library
	FT_Library library;

	error = FT_Init_FreeType( &library );
	CHECK_ERROR(error);
	
	// create face object
	FT_Face face = font->LoadFreeTypeFace( library );

	// set character size
	error = FT_Set_Char_Size(face,					/* handle to face object           */
							 0,						/* char_width in 1/64th of points  */
							 (FT_F26Dot6)( 64 * size ),	/* char_height in 1/64th of points */
							 DPI,					/* horizontal device resolution    */
							 0);					/* vertical device resolution      */
	CHECK_ERROR(error);

	FT_GlyphSlot  slot = face->glyph;
	FT_UInt numGlyphs = 0;
	FT_UInt previousGlyphIndex = 0;

	bool useKerning = FT_HAS_KERNING(face);

	FT_Int penX = 0, penY = 0;
	for (size_t n = 0; n < strlen(text); n++) {
		FT_UInt glyphIndex;

		glyphIndex = FT_Get_Char_Index(face, text[n]);

		if (useKerning && previousGlyphIndex && glyphIndex)
		{
			FT_Vector delta;
			FT_Get_Kerning(face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta);
			penX += delta.x;
		}

		positions[numGlyphs].x = penX;
		positions[numGlyphs].y = penY;

		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
		CHECK_ERROR(error);

		error = FT_Get_Glyph( face->glyph, &glyphs[numGlyphs]);
		CHECK_ERROR(error);

		penX += slot->advance.x;

		previousGlyphIndex = glyphIndex;
		numGlyphs++;
	}

	// compute the bounding box of the glyphs
	FT_BBox  boundingBox;
	FT_BBox  glyphBoundingBox;

	boundingBox.yMin = 32000;
	boundingBox.yMax = -32000;

	for (FT_UInt n = 0; n < strlen(text); n++)
	{
		FT_Glyph_Get_CBox( glyphs[n], FT_GLYPH_BBOX_PIXELS, &glyphBoundingBox);

		glyphBoundingBox.xMin += positions[n].x;
		glyphBoundingBox.xMax += positions[n].x;
		glyphBoundingBox.yMin += positions[n].y;
		glyphBoundingBox.yMax += positions[n].y;

		if ( glyphBoundingBox.xMin < boundingBox.xMin )
		{
			boundingBox.xMin = glyphBoundingBox.xMin;
		}

		if ( glyphBoundingBox.yMin < boundingBox.yMin )
		{
			boundingBox.yMin = glyphBoundingBox.yMin;
		}

		if ( glyphBoundingBox.xMax > boundingBox.xMax )
		{
			boundingBox.xMax = glyphBoundingBox.xMax;
		}

		if ( glyphBoundingBox.yMax > boundingBox.yMax )
		{
			boundingBox.yMax = glyphBoundingBox.yMax;
		}

		if ( boundingBox.xMin > boundingBox.xMax )
		{
			boundingBox.xMax = 0;
			boundingBox.xMin = 0;
			boundingBox.yMax = 0;
			boundingBox.yMin = 0;
		}
	}

	FT_Pos stringWidth = boundingBox.xMax - boundingBox.xMin;
	FT_Pos stringHeight = boundingBox.yMax - boundingBox.yMin;

	FT_Pos startX = ((int)width*64 - stringWidth)/2;
	FT_Pos startY = ((int)height*64 - stringHeight)/2;

	const size_t bufferPitch = (size_t)width * BYTES_PER_PIXEL;
	u8 *imageBuffer = (u8 *)calloc( (size_t)width * (size_t)height, BYTES_PER_PIXEL);

	for (size_t n = 0; n < strlen(text); n++) {
		FT_Glyph image;
		FT_Vector pen;

		image = glyphs[n];

		pen.x = startX + positions[n].x;
		pen.y = startY + positions[n].y;

		error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, &pen, 0);

		if (!error)
		{
			FT_BitmapGlyph bit = (FT_BitmapGlyph)image;
			RenderBitmapToTarget(&bit->bitmap, imageBuffer, (int)bit->left, (int)(height - bit->top), bufferPitch);
			FT_Done_Glyph(image);
		}
	}
	

	MOAIImage image;
	image.Init(imageBuffer, (u32)width, (u32)height, USColor::A_8);
	image.PadToPow2(image);

	USFileStream f;
	f.Open("/tmp/image.png", USFileStream::READ_WRITE_AFFIRM);
	image.WritePNG(f);
	f.Close();
	
	MOAITexture *t = new MOAITexture();
	t->Init(image, "");

	return t;
}

MOAIFreeTypeTextBox::MOAIFreeTypeTextBox()
{

}

MOAIFreeTypeTextBox::~MOAIFreeTypeTextBox()
{

}

void MOAIFreeTypeTextBox::RegisterLuaClass( MOAILuaState &state )
{

	luaL_Reg regTable [] = {
		{ "generateLabelTexture",			_generateLabelTexture },
		{NULL, NULL}
	};

	luaL_register( state, 0, regTable );
}



