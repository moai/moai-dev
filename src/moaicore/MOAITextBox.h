// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTBOX_H
#define	MOAITEXTBOX_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITextFrame.h>
#include <moaicore/MOAITextLayout.h>
#include <moaicore/MOAITransform.h>

class MOAIAnimCurve;
class MOAIFont;

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

	static const u32 REVEAL_ALL = 0xffffffff;
	static const float DEFAULT_SPOOL_SPEED;

	MOAILuaSharedPtr < MOAIFont > mFont;
	float				mLineSpacing;
	
	USRect				mFrame;

	STLString			mText;
	u32					mTextLength;

	u32					mJustify;
	float				mPoints;
	
	float				mSpool;
	float				mSpeed;
	float				mThrottle;
	u32					mReveal;
	
	bool				mYFlip;
	bool				mRightToLeft;
	
	bool				mNeedsLayout;
	MOAITextCursor		mCurrentPage;
	MOAITextCursor		mNextPage;
	MOAITextLayout		mLayout;
	
	USLeanArray < MOAIAnimCurve* >	mCurves;
	
	//----------------------------------------------------------------//
	static int	_clearCurves			( lua_State* L );
	static int	_getLineSize			( lua_State* L );
	static int	_getStringBounds		( lua_State* L );
	static int	_more					( lua_State* L );
	static int	_nextPage				( lua_State* L );
	static int	_revealAll				( lua_State* L );
	static int	_reserveCurves			( lua_State* L );
	static int	_setAlignment			( lua_State* L );
	static int	_setCurve				( lua_State* L );
	static int	_setFont				( lua_State* L );
	static int	_setLineSpacing			( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setReveal				( lua_State* L );
	static int	_setRightToLeft			( lua_State* L );
	static int	_setSpeed				( lua_State* L );
	static int	_setString				( lua_State* L );
	static int	_setStringColor			( lua_State* L );
	static int	_setTextSize			( lua_State* L );
	static int	_setYFlip				( lua_State* L );
	static int	_spool					( lua_State* L );
	
	//----------------------------------------------------------------//
	void			Layout					();
	void			OnDepNodeUpdate			();
	void			Spool					( float step );
	
public:

	DECL_LUA_FACTORY ( MOAITextBox )
	
	//----------------------------------------------------------------//
	void			ClearCurves				();
	void			Draw					( int subPrimID, bool reload );
	void			DrawDebug				( int subPrimID );
	u32				GetLocalFrame			( USRect& frame );
	bool			IsDone					();
					MOAITextBox				();
					~MOAITextBox			();
	bool			More					();
	void			NextPage				( bool reveal );
	void			OnUpdate				( float step );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ReserveCurves			( u32 total );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetColor				( float r, float g, float b, float a );
	void			SetCurve				( u32 idx, MOAIAnimCurve* curve );
	void			SetFont					( MOAIFont* font );
	void			SetRect					( float left, float top, float right, float bottom );
	void			SetText					( cc8* text );
	void			SetTextSize				( float newSize );
};

#endif
