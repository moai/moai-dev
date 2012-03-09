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
// MOAITextBox
//================================================================//
/**	@name	MOAITextBox
	@text	Text box class.
	
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
	
	//----------------------------------------------------------------//
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
	static int			_setHighlight			( lua_State* L );
	static int			_setLineSpacing			( lua_State* L );
	static int			_setRect				( lua_State* L );
	static int			_setReveal				( lua_State* L );
	static int			_setSpeed				( lua_State* L );
	static int			_setString				( lua_State* L );
	static int			_setStyle				( lua_State* L );
	static int			_setYFlip				( lua_State* L );
	static int			_spool					( lua_State* L );
	
	//----------------------------------------------------------------//
	MOAITextStyle*		AddAnonymousStyle		( MOAITextStyle* source );
	bool				CheckStylesChanged		();
	void				FindSpriteSpan			( u32 idx, u32 size, u32& spanIdx, u32& spanSize );
	void				Layout					();
	void				OnDepNodeUpdate			();
	void				PushLine				( u32 start, u32 size, const USRect& rect, float ascent );
	void				PushSprite				( u32 idx, MOAIGlyph& glyph, MOAITextStyle& style, float x, float y );
	void				PushStyleSpan			( int base, int top, MOAITextStyle& style );
	void				RefreshStyleGlyphs		();
	void				ReleaseStyle			( MOAITextStyle* style );
	void				ResetLayout				();
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

	DECL_LUA_FACTORY ( MOAITextBox )
	
	//----------------------------------------------------------------//
	void				ClearCurves				();
	void				Draw					( int subPrimID, bool reload );
	void				DrawDebug				( int subPrimID );
	bool				GetBoundsForRange		( u32 idx, u32 size, USRect& rect );
	u32					GetDeckBounds			( USBox& bounds );
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
