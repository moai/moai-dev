// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTLABEL_H
#define	MOAITEXTLABEL_H

#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAITextDesigner.h>
#include <moai-sim/MOAITextLayout.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleCache.h>
#include <moai-sim/MOAITextStyleMap.h>

class MOAIAnimCurve;
class MOAIFont;

//================================================================//
// MOAITextLabel
//================================================================//
/**	@lua	MOAITextLabel
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
			values of 4 and 8 bits of precision (16 or 256 levels) respectively.
			Three and four digit numbers represent RGB and RGBA colors at
			4 bit precision. Six digits is RGB at 8 bits of precision.
			Seven digits is RGBA with 8 bits for RGB and 4 bits for A.
			Eight digits is RGBA with 8 bits for each component.</p>
			
			<p>The final text escapes ends the current escape. It takes the
			form of </>. Including any additional text in this kind of escape
			is an error.</p>
			
			<p>You may escape the '<' symbol itself by using an additional '<'.
			For example, '<<' will output '<'. '<<test>' will short circuit
			the style escape and output '<test>' in the displayed text.</p>
			
			<p>When using MOAITextLabel with MOAIFont it's important to
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
			and MOAIDynamicGlyphCache attached to it then the glyphs will be rendered
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
			
			<p>You can also attach MOAIAnimCurves to the text box. The animation curves
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
	
	@const	BASELINE_JUSTIFY
	@const	BOTTOM_JUSTIFY
	@const	LEFT_JUSTIFY
	@const	CENTER_JUSTIFY
	@const	RIGHT_JUSTIFY
	@const	TOP_JUSTIFY
	
	@const	WORD_BREAK_NONE
	@const	WORD_BREAK_CHAR
*/
class MOAITextLabel :
	public MOAIGraphicsProp,
	public MOAIAction {
private:

	bool				mNeedsLayout;

protected:

	static const u32 REVEAL_ALL = 0xffffffff;
	static const float DEFAULT_SPOOL_SPEED;
	
	float				mSpool;
	float				mSpeed;
	float				mThrottle;
	u32					mReveal;
	
	u32					mCurrentPageIdx;
	u32					mNextPageIdx;
	bool				mMore;
	
	MOAITextDesigner	mDesigner;
	MOAITextStyleCache	mStyleCache;
	MOAITextStyleMap	mStyleMap;
	MOAITextLayout		mLayout;
	
	STLString			mText;
	
	bool				mAutoFlip;
	
	//----------------------------------------------------------------//
	static int			_clearHighlights		( lua_State* L );
	static int			_getAlignment			( lua_State* L );
	static int			_getGlyphScale			( lua_State* L );
	static int			_getLineSpacing			( lua_State* L );
	static int			_getRect				( lua_State* L );
	static int			_getStyle				( lua_State* L );
	static int			_getText				( lua_State* L );
	static int			_getTextBounds			( lua_State* L );
	static int			_more					( lua_State* L );
	static int			_nextPage				( lua_State* L );
	static int			_revealAll				( lua_State* L );
	static int			_reserveCurves			( lua_State* L );
	static int			_setAlignment			( lua_State* L );
	static int			_setAutoFlip			( lua_State* L );
	static int			_setBounds				( lua_State* L );
	static int			_setCurve				( lua_State* L );
	static int			_setGlyphScale			( lua_State* L );
	static int			_setHighlight			( lua_State* L );
	static int			_setLineSnap			( lua_State* L );
	static int			_setLineSpacing			( lua_State* L );
	static int			_setRect				( lua_State* L );
	static int			_setRectLimits			( lua_State* L );
	static int			_setReveal				( lua_State* L );
	static int			_setSpeed				( lua_State* L );
	static int			_setStyle				( lua_State* L );
	static int			_setText				( lua_State* L );
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
	void				BuildLocalToWorldMtx	( ZLAffine3D& localToWorldMtx );
	ZLMatrix4x4			GetWorldDrawingMtx		();
	void				OnDepNodeUpdate			();
	u32					OnGetModelBounds		( ZLBox& bounds );
	void				OnUpdate				( double step );
	void				ResetLayout				();
	void				ScheduleLayout			();
	void				Refresh					();
	virtual void		RefreshLayout			();
	virtual void		RefreshStyleGlyphs		();

public:
	
	DECL_LUA_FACTORY ( MOAITextLabel )
	
	//----------------------------------------------------------------//
	void				Draw					( int subPrimID, float lod );
	void				DrawDebug				( int subPrimID, float lod );
	bool				IsDone					();
						MOAITextLabel			();
						~MOAITextLabel			();
	bool				More					();
	void				NextPage				( bool reveal );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void				SetText					( cc8* text );
};

#endif
