#include <moaicore/moaicore.h>

#include <contrib/utf8.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#define MAX_TEXTURE_SIZE 1024
#define DPI 300

//================================================================//
// RenderParams
//================================================================//
class RenderParams {
public:

	MOAIImage*	mImage;
	int			mPenX;
	int			mPenY;
};

//----------------------------------------------------------------//
static void _renderSpan ( const int y, const int count, const FT_Span* const spans, void* const user ) {

	if ( !user ) return;
	
	RenderParams* render = ( RenderParams* )user;

	int line = render->mPenY - y - 1;
	int offset = render->mPenX;

	for ( int i = 0; i < count; ++i ) {
		
		const FT_Span& span = spans [ i ];
		
		int x = offset + span.x;
		int len = span.len;
		
		u32 alpha = ( u32 )span.coverage;
		
		for ( int j = 0; j < len; ++j ) {
			render->mImage->SetPixel ( x + j, line, alpha );
		}
	}
}

//================================================================//
// MOAIGlyphPage
//================================================================//
class MOAIGlyphPage {
private:

	friend class MOAIGlyphCache;

	typedef MOAISpanList < MOAIGlyph* > GlyphList;
	typedef MOAISpanList < MOAIGlyph* >::Span GlyphSpan;
	
	typedef MOAISpanList < GlyphList > RowList;
	typedef MOAISpanList < GlyphList >::Span RowSpan;

	RowList mRows;

	MOAIImage mImage;
	
	MOAIGlyphPage* mNext;

	float mThreshold;

	//----------------------------------------------------------------//
	void AffirmCanvas () {
		
		if ( !this->mImage.GetHeight ()) {
			this->mImage.Init ( MAX_TEXTURE_SIZE, MAX_TEXTURE_SIZE, USColor::A_8, USPixel::TRUECOLOR );
			this->mImage.ClearBitmap ();
		}
		
		//if ( this->mImage.GetHeight () < this->mRows.mSize ) {
		//	
		//	USIntRect rect;
		//	rect.Init ( 0, 0, MAX_TEXTURE_SIZE, this->mRows.mSize );
		//	this->mImage.ResizeCanvas ( this->mImage, rect );
		//}
	}

	//----------------------------------------------------------------//
	RowSpan* AllocRow ( u32 height ) {
	
		RowSpan* rowIt = this->mRows.Alloc ( height );
			
		// if alloc succeeded, initialize the new row
		if ( rowIt ) {
			//u32 maxTextureSize = MOAIGfxDevice::Get ().GetMaxTextureSize ();
			u32 maxTextureSize = MAX_TEXTURE_SIZE;
			rowIt->mData.Expand ( maxTextureSize );
		}
		return rowIt;
	}

	//----------------------------------------------------------------//
	bool ExpandToNextPowerofTwo () {
	
		//u32 maxTextureSize = MOAIGfxDevice::Get ().GetMaxTextureSize ();
		u32 maxTextureSize = MAX_TEXTURE_SIZE;
		if ( this->mRows.mSize >= maxTextureSize ) return false;
		
		size_t size = this->mRows.mSize ? this->mRows.mSize << 1 : 8;
		this->mRows.Expand ( size );
		
		return true;
	}

public:
	
	//----------------------------------------------------------------//
	GlyphSpan* Alloc ( MOAIGlyph& glyph ) {
		
		u32 width = glyph.mWidth + 2;
		u32 height = glyph.mHeight + 2;
		
		RowSpan* rowIt = this->mRows.mHead;
		RowSpan* bestRowIt = 0;
		RowSpan* backupRowIt = 0;
		
		// find the very shortest row that can still accomodate the glyph
		for ( ; rowIt; rowIt = rowIt->mNext ) {
			if ( rowIt->mOccupied && ( height <= rowIt->mSize ) && rowIt->mData.HasRoom ( width )){
				if ( !bestRowIt || ( bestRowIt && ( rowIt->mSize < bestRowIt->mSize ))) {
					bestRowIt = rowIt;
				}
			}
		}
		
		// save a ref to the best row in case we need to fall back on it
		backupRowIt = bestRowIt;
		
		// if the best row is still too tall for us, throw it away
		// 'too tall' means that the row is going to be too loose a fit as
		// given by mThreshold
		if ( bestRowIt && (( u32 )( bestRowIt->mSize * this->mThreshold ) > height )) {
			bestRowIt = 0;
		}
		
		// if no suitable match was found or the best row wasn't best enough, try to alloc a new row
		if ( !bestRowIt ) {
			
			bestRowIt = this->AllocRow ( height );
			
			// alloc failed and no fall back available
			// only choice is to try and expand...
			if ( !bestRowIt ) {
				
				// expand page to next power of two and try again
				// keep trying until the alloc succeeds or we max out
				while ( !bestRowIt && this->ExpandToNextPowerofTwo ()) {
					bestRowIt = this->AllocRow ( height );
				}
				
				// if expand/alloc failed, fall back on original match
				if ( !bestRowIt ) {
					bestRowIt = backupRowIt;
				}
				
				// if we're still failing, give up
				if ( !bestRowIt ) return 0;
			}
		}
		
		GlyphSpan* glyphSpan = bestRowIt->mData.Alloc ( width );
		if ( glyphSpan ) {
			glyph.mPage = this;
			glyph.mSrcX = glyphSpan->mBase;
			glyph.mSrcY = bestRowIt->mBase;
		}
		return glyphSpan;
	}
	
	//----------------------------------------------------------------//
	MOAIGlyphPage () :
		mNext ( 0 ),
		mThreshold ( 0.8f ) {
	}
	
	//----------------------------------------------------------------//
	~MOAIGlyphPage () {
	}
};

//================================================================//
// MOAIGlyphSet
//================================================================//
class MOAIGlyphSet {
private:

	friend class MOAIGlyphCache;
	
	float mPoints;
	
	// for now
	STLMap < u32, MOAIGlyph > mGlyphMap;
	
	MOAIGlyph* mPending;
	
	//----------------------------------------------------------------//
	void AffirmGlyph ( u32 c ) {
	
		if ( !this->mGlyphMap.contains ( c )) {
		
			MOAIGlyph& glyph = this->mGlyphMap [ c ];
			glyph.mNext = this->mPending;
			this->mPending = &glyph;
			glyph.SetCode ( c );
		}
	}

public:

	//----------------------------------------------------------------//
	MOAIGlyphSet () :
		mPending ( 0 ) {
	}
};

//================================================================//
// MOAIGlyphCache
//================================================================//
class MOAIGlyphCache {
private:

	MOAIGlyphPage* mPages;

	STLString	mFilename;
	
	// for now
	typedef STLMap < float, MOAIGlyphSet >::iterator GlyphSetsIt;
	STLMap < float, MOAIGlyphSet > mGlyphSets;

public:
	
	//----------------------------------------------------------------//
	void AffirmGlyphs ( float points, cc8* chars ) {
	
		MOAIGlyphSet& glyphSet = this->mGlyphSets [ points ];
		glyphSet.mPoints = points;
		
		int i = 0;
		while ( chars [ i ]) {
			u32 c = u8_nextchar ( chars, &i );
			glyphSet.AffirmGlyph ( c );
		}
	}
	
	//----------------------------------------------------------------//
	MOAISpan < MOAIGlyph* >* Alloc ( MOAIGlyph& glyph ) {
		
		glyph.mPage = 0;
		
		MOAIGlyphPage* page = this->mPages;
		for ( ; page; page = page->mNext ) {
		
			MOAISpan < MOAIGlyph* >* span = page->Alloc ( glyph );
			if ( span ) {
				return span;
			}
		}
		
		page = new MOAIGlyphPage ();
		page->mNext = this->mPages;
		this->mPages = page;
		
		return page->Alloc ( glyph );
	}
	
	//----------------------------------------------------------------//
	void Init ( cc8* filename ) {
	
		this->mFilename = filename;
	}
	
	//----------------------------------------------------------------//
	MOAIGlyph* GetGlyphForChar ( u32 c ) {
	
		return 0;
	}
	
	//----------------------------------------------------------------//
	MOAIGlyphCache () :
		mPages ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~MOAIGlyphCache () {
	}
	
	//----------------------------------------------------------------//
	void RenderGlyphs () {
		
		RenderParams render;
		FT_Raster_Params params;
		memset ( &params, 0, sizeof ( params ));
		params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
		params.gray_spans = _renderSpan;
		params.user = &render;
		
		FT_Library library;
		FT_Init_FreeType( &library );

		FT_Face face;
		if ( FT_New_Face( library, this->mFilename, 0, &face )) {
			FT_Done_FreeType ( library );
			fprintf ( stderr, "Error loading font: %s\n", this->mFilename );
			return;
		}
		
		GlyphSetsIt glyphSetsIt = this->mGlyphSets.begin ();
		for ( ; glyphSetsIt != this->mGlyphSets.end (); ++glyphSetsIt ) {
			MOAIGlyphSet& glyphSet = glyphSetsIt->second;
		
			FT_Set_Char_Size ( face, 0, ( u32 )( glyphSet.mPoints * 64.0f ), DPI, DPI );
			float pixelSize = face->size->metrics.y_ppem;
			
			int yMin = FT_MulFix ( face->bbox.yMin, face->size->metrics.y_scale ) >> 6;
			int yMax = FT_MulFix ( face->bbox.yMax, face->size->metrics.y_scale ) >> 6;
			
			int faceHeight = yMax - yMin;
			
			//u32 imageWidth	= image.GetWidth ();
			//u32 imageHeight	= image.GetHeight ();
			
			float scale = 1.0f / pixelSize;
			//float uScale = 1.0f / image.GetWidth ();
			//float vScale = 1.0f / image.GetHeight ();
			
			//float lineSpacing = ( float )( face->size->metrics.height >> 6 ) * scale;
			
			//u32 x = 0;
			//u32 y = 0;
			
			MOAIGlyph* glyphIt = glyphSet.mPending;
			glyphSet.mPending = 0;
			
			for ( ; glyphIt; glyphIt = glyphIt->mNext ) {
				MOAIGlyph& glyph = *glyphIt;
				
				u32 index = FT_Get_Char_Index ( face, glyph.mCode );
				FT_Load_Glyph ( face, index, FT_LOAD_NO_BITMAP );
				
				if ( face->glyph->format!= FT_GLYPH_FORMAT_OUTLINE ) continue;
				
				int glyphWidth = face->glyph->metrics.width >> 6;
				//int glyphHeight = face->glyph->metrics.height >> 6;
				
				int advanceX = face->glyph->metrics.horiAdvance >> 6;
				int bearingX = face->glyph->metrics.horiBearingX >> 6;
				
				//u32 width = ( u32 )( glyphWidth * scale );
				//u32 height = ( u32 )( faceHeight * scale );
				
				//glyph.SetScreenRect (( float )glyphWidth * scale, ( float )faceHeight * scale, 0.0f );
				//glyph.SetAdvanceX (( float )advanceX * scale );
				//glyph.SetBearingX (( float )bearingX * scale );
				
				glyph.mWidth = glyphWidth;
				glyph.mHeight = faceHeight;
				
				this->Alloc ( glyph );
				if ( glyph.mPage ) {
					
					render.mImage = &glyph.mPage->mImage;
					render.mPenX = glyph.mSrcX - bearingX;
					render.mPenY = glyph.mSrcY + yMax;

					glyph.mPage->AffirmCanvas ();
					FT_Outline_Render ( library, &face->glyph->outline, &params );
				}
			}
		}
		
		FT_Done_Face ( face );
		FT_Done_FreeType ( library );
	}
	
	//----------------------------------------------------------------//
	void WritePages () {
		
		char buffer [ 256 ];

		MOAIGlyphPage* page = this->mPages;
		for ( int i = 0; page; page = page->mNext, ++i ) {
		
			sprintf ( buffer, "page%d.png", i );
			
			USFileStream stream;
			stream.OpenWrite ( buffer );
			page->mImage.WritePNG ( stream );
		}
	}
};

//----------------------------------------------------------------//
int work_glyph_cache ( int argc, char **argv ) {
	
	AKUCreateContext ();
	
	MOAIGlyphCache glyphCache;
	glyphCache.Init ( "arial-rounded.TTF" );
	glyphCache.AffirmGlyphs ( 12.0f, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-" );
	glyphCache.AffirmGlyphs ( 6.0f, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-" );
	glyphCache.AffirmGlyphs ( 24.0f, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-" );
	glyphCache.AffirmGlyphs ( 8.0f, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-" );
	glyphCache.AffirmGlyphs ( 16.0f, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-" );
	glyphCache.AffirmGlyphs ( 21.0f, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-" );
	glyphCache.AffirmGlyphs ( 18.0f, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-" );
	glyphCache.RenderGlyphs ();
	glyphCache.WritePages ();
	
	return 0;
}
