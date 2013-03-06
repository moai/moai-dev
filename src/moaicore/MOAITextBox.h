// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTBOX_H
#define	MOAITEXTBOX_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITextStyle.h>

class MOAIAnimCurve;
class MOAIFont;

//================================================================//
// MOAITextStyleRef
//================================================================//
class MOAITextStyleRef {
private:

	friend class MOAITextBox;

	MOAITextStyle*			mStyle;
	MOAITextStyleState		mState;

public:

	//----------------------------------------------------------------//
				MOAITextStyleRef		();
				~MOAITextStyleRef		();
	bool		NeedsLayout				() const;
	void		UpdateState				();
};

//================================================================//
// MOAITextStyleSpan
//================================================================//
class MOAITextStyleSpan {
public:

	int					mBase;		// base index of first utf-8 character in span
	int					mTop;		// size of span
	MOAITextStyle*		mStyle;		// style for span
};

//================================================================//
// MOAITextSprite
//================================================================//
class MOAITextSprite {
private:

	friend class MOAITextBox;
	friend class MOAITextDesigner;
	
	MOAIGlyph*				mGlyph;
	MOAITextStyle*			mStyle;
	MOAITextureBase*		mTexture; // caching this here to avoid add'l virtual calls when drawing
	
	u32			mIdx; // index in original string
	float		mX;
	float		mY;
	float		mScale;
	u32			mRGBA;
	u32			mMask;
	
	enum {
		MASK_COLOR	= 0x01,
	};
};

//================================================================//
// MOAITextLine
//================================================================//
class MOAITextLine {
private:

	friend class MOAITextDesigner;
	friend class MOAITextBox;
	
	u32			mStart;		// index in sprite stack
	u32			mSize;		// number of sprites in line;
	USRect		mRect;		// tight bounds of line
	float		mAscent;	// offset to the text baseline

public:

};

//================================================================//
// MOAITextHighlight
//================================================================//
class MOAITextHighlight {

	friend class MOAITextBox;

	u32		mBase;
	u32		mTop;
	u32		mColor;

	MOAITextHighlight* mPrev;
	MOAITextHighlight* mNext;

public:
};

//================================================================//
// MOAITextBox
//================================================================//
/**	@name	MOAITextBox
	@text	<p>The text box manages styling, laying out and displaying text.
			You can attach named styles to the text box to be applied to
			the text using style escapes. You can also inline style
			escapes to control color. Style escapes may be nested.</p>
			
			<p>To attach a style to a text box use setStyle (). If you
			provide a name for the style then the style may be applied
			by name using a style escape. If you do not provide a name
			then the style will be used as the default style for
			the text box. The default style is the style that will be
			used when no style escapes are in effect.</p>
			
			<p>The setFont () and setSize () methods are helpers that
			operate on the text box's default style. If no default
			style exists when these methods are called, one will be
			created.</p>
			
			<p>There are three kinds of text escapes. The first takes
			the form of <styleName> where 'styleName' is the name
			of the style you provided via setStyle (). If there is no
			match for the name then the default style will be used.</p>
			
			<p>The second form of style escape sets text color. It
			takes the form of <c:XXX> where 'XXX' is a hexadecimal
			number representing a color value. The hexadecimal number
			may be have from one up to eight digits, excluding five
			digit numbers. One and two digit numbers correspond to grayscale
			values of 4 and 8 bits of precision (16 or 256 levels) respectively. Three
			and four digit numbers represent RGB and RGBA colors at
			4 bit precision. Six digits is RGB at 8 bits of precision.
			Seven digits is RGBA with 8 bits for RGB and 4 bits for A.
			Eight digits is RGBA with 8 bits for each component.</p>
			
			<p>The final text escapes ends the current escape. It takes the
			form of </>. Including any additional text in this kind of escape
			is an error.</p>
			
			<p>You may escape the '<' symbol itself by using an additional '<'.
			For example, '<<' will output '<'. '<<test>' will short circuit
			the style escape and output '<test>' in the displayed text.</p>
			
			<p>When using MOAITextBox with MOAIFont it's important to
			understand how and when glyphs are rendered. When you call
			setText () the text box's style goes to work. The entire
			string you provide is scanned and a 'style span' is created
			for each uniquely styled block of text. If you do not use
			any styles then there will be only one style span.</p>
			
			<p>Once the text style has created style spans for your text,
			the spans themselves are scanned. Each span must specify
			a font to be used. All of the characters in the span are 'affirmed'
			by the font: if the glyphs for the characters have already been
			ripped then nothing happens. If not, the characters are enqueued
			by the font to have their glyphs ripped.</p>
			
			<p>Finally, we iterate through all of the fonts used by the text
			and instruct them to load and render any pending glyphs. If
			the font is dynamic and has a valid implementation of MOAIFontReader
			and MOAIGlyphCache attached to it then the glyphs will be rendered
			and placed in the cache.</p>
			
			<p>Once the glyphs have been rendered, we know their metrics and
			(hopefully) have valid textures for them. We can now lay out
			an actual page of text. This is done by a separate subsystem known
			as the text designer. The text designer reads the style spans and
			uses the associated font, color and size information to place
			the glyphs into a layout.</p>
			
			<p>If the text associated with the textbox doesn't fit, then the
			textbox will have multiple pages. The only method that deals with
			pages at this time is nextPage (). Additional methods giving
			finer control over multi-page text boxes will be provided in a
			future release.</p>
			
			<p>There are some additional ways you can use the text box to style
			your text. The current implementation supports left, center and right
			positioning as well as top, center and bottom positioning. A future
			implementation will include justification in which words and lines
			of text will be spaced out to align with the edges of the text box.</p>
			
			<p>You can also attach MOAIAnimCurves to the text box. The anim curves
			may be used to offset characters in lines of text. Each curve may
			have any number of keyframes, but only the span between t0 and t1
			is used by the text box, regardless of its width. Curves correspond
			to lines of text. If there are more lines of text than curves,
			the curves will simply repeat.</p>
			
			<p>Once you've loaded text into the text box you can apply highlight colors.
			These colors will override any colors specified by style escapes.
			Highlight spans may be set or cleared using setHighlight ().
			clearHighlights () will remove all highlights from the text.
			Highlights will persists from page to page of text, but will be
			lost if new text is loaded by calling setText ().</p>
	
	@const	LEFT_JUSTIFY
	@const	CENTER_JUSTIFY
	@const	RIGHT_JUSTIFY
*/
class MOAITextBox :
	public MOAIProp,
	public MOAIAction {
private:

	friend class MOAITextDesigner;
	friend class MOAITextStyler;

	static const u32 REVEAL_ALL = 0xffffffff;
	static const float DEFAULT_SPOOL_SPEED;

	float				mLineSpacing;
	
	USRect				mFrame;

	STLString			mText;
	u32					mTextLength;
	
	u32					mHAlign;
	u32					mVAlign;
	
	float				mSpool;
	float				mSpeed;
	float				mThrottle;
	u32					mReveal;
	
	bool				mYFlip;
	float				mGlyphScale;
	
	int					mCurrentPageIdx;
	int					mNextPageIdx;
	bool				mNeedsLayout;
	
	USLeanArray < MOAIAnimCurve* > mCurves;
	
	// style set - these are the styles the texbox knows about
	// only need to get these during text styling, so using an STLMap for now...
	typedef STLMap < STLString, MOAITextStyleRef >::iterator StyleSetIt;
	STLMap < STLString, MOAITextStyleRef > mStyleSet;
	
	// anonymous styles - these are created on the fly as text is being styled
	USLeanStack < MOAITextStyleRef, 8 > mAnonymousStyles;
	
	// this is the style map. it is produced by analyzing the text and creating a
	// 'style span' for each styled token. this is the preprocessing step to
	// actually layout out a page of text. text is laid out based on the style spans.
	USLeanStack < MOAITextStyleSpan, 64 > mStyleMap; // each span represents a stretch of 'styled' text
	
	// this is the text page layout. these are the action sprites and lines
	// that will be rendered for the current page.
	USLeanStack < MOAITextSprite, 64 >	mSprites;
	USLeanStack < MOAITextLine, 8 >		mLines;
	bool								mMore;
	
	// list of highlight spans
	MOAITextHighlight* mHighlights;
	
	// rule for breaking words across lines
	u32 mWordBreak;
	
	//----------------------------------------------------------------//
	static int			_clearHighlights		( lua_State* L );
	static int			_getGlyphScale			( lua_State* L );
	static int			_getLineSpacing			( lua_State* L );
	static int			_getRect				( lua_State* L );
	static int			_getStringBounds		( lua_State* L );
	static int			_getStyle				( lua_State* L );
	static int			_more					( lua_State* L );
	static int			_nextPage				( lua_State* L );
	static int			_revealAll				( lua_State* L );
	static int			_reserveCurves			( lua_State* L );
	static int			_setAlignment			( lua_State* L );
	static int			_setCurve				( lua_State* L );
	static int			_setGlyphScale			( lua_State* L );
	static int			_setHighlight			( lua_State* L );
	static int			_setLineSpacing			( lua_State* L );
	static int			_setRect				( lua_State* L );
	static int			_setReveal				( lua_State* L );
	static int			_setSpeed				( lua_State* L );
	static int			_setString				( lua_State* L );
	static int			_setStyle				( lua_State* L );
	static int			_setWordBreak			( lua_State* L );
	static int			_setYFlip				( lua_State* L );
	static int			_spool					( lua_State* L );
	
	//----------------------------------------------------------------//
	#ifdef DOXYGEN
		static int		_affirmStyle			( lua_State* L );
		static int		_setFont				( lua_State* L );
		static int		_setTextSize			( lua_State* L );
	#endif
	
	//----------------------------------------------------------------//
	MOAITextStyle*		AddAnonymousStyle		( MOAITextStyle* source );
	void				AddHighlight			( u32 base, u32 top, u32 color );
	void				ApplyHighlights			();
	void				ClearHighlight			( u32 base, u32 top );
	void				ClearHighlights			();
	bool				CheckStylesChanged		();
	void				CompactHighlights		();
	void				FindSpriteSpan			( u32 idx, u32 size, u32& spanIdx, u32& spanSize );
	void				Layout					();
	void				OnDepNodeUpdate			();
	void				PushLine				( u32 start, u32 size, const USRect& rect, float ascent );
	void				PushSprite				( u32 idx, MOAIGlyph& glyph, MOAITextStyle& style, float x, float y, float scale );
	void				PushStyleSpan			( int base, int top, MOAITextStyle& style );
	void				RefreshStyleGlyphs		();
	void				ReleaseStyle			( MOAITextStyle* style );
	void				RemoveHighlight			( MOAITextHighlight& highlight );
	void				ResetLayout				();
	void				ResetHighlights			();
	void				ResetStyleMap			();
	void				ResetStyleSet			();
	void				RetainStyle				( MOAITextStyle* style );
	void				ScheduleLayout			();
	
public:

	enum {
		LEFT_JUSTIFY,
		CENTER_JUSTIFY,
		RIGHT_JUSTIFY,
	};

	enum {
		WORD_BREAK_NONE,
		WORD_BREAK_CHAR,
	};

	DECL_LUA_FACTORY ( MOAITextBox )
	
	//----------------------------------------------------------------//
	void				ClearCurves				();
	void				Draw					( int subPrimID );
	void				DrawDebug				( int subPrimID );
	bool				GetBoundsForRange		( u32 idx, u32 size, USRect& rect );
	u32					GetPropBounds			( USBox& bounds );
	MOAITextStyle*		GetStyle				();
	MOAITextStyle*		GetStyle				( cc8* styleName );
	bool				IsDone					();
						MOAITextBox				();
						~MOAITextBox			();
	bool				More					();
	void				NextPage				( bool reveal );
	void				OnUpdate				( float step );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				ReserveCurves			( u32 total );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void				SetCurve				( u32 idx, MOAIAnimCurve* curve );
	void				SetHighlight			( u32 idx, u32 size );
	void				SetHighlight			( u32 idx, u32 size, u32 color );
	void				SetRect					( float left, float top, float right, float bottom );
	void				SetStyle				( MOAITextStyle* style );
	void				SetStyle				( cc8* styleName, MOAITextStyle* style );
	void				SetText					( cc8* text );
};

#endif
