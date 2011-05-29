// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTBOX_H
#define	MOAITEXTBOX_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIProp2D.h>
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
	public MOAIProp2D,
	public MOAIAction {
private:

	static const u32 REVEAL_ALL = 0xffffffff;
	static const float DEFAULT_SPOOL_SPEED;

	USRef < MOAIFont >	mFont;
	
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
	
	bool				mNeedsLayout;
	USTextCursor		mCurrentPage;
	USTextCursor		mNextPage;
	USTextLayout		mLayout;
	
	USLeanArray < MOAIAnimCurve* >	mMOAICurves;
	USLeanArray < USAnimCurve* >	mCurves;
	
	//----------------------------------------------------------------//
	static int	_clearCurves		( lua_State* L );
	static int	_getStringBounds	( lua_State* L );
	static int	_more				( lua_State* L );
	static int	_nextPage			( lua_State* L );
	static int	_revealAll			( lua_State* L );
	static int	_reserveCurves		( lua_State* L );
	static int	_setAlignment		( lua_State* L );
	static int	_setCurve			( lua_State* L );
	static int	_setFont			( lua_State* L );
	static int	_setParent			( lua_State* L );
	static int	_setRect			( lua_State* L );
	static int	_setReveal			( lua_State* L );
	static int	_setSpeed			( lua_State* L );
	static int	_setString			( lua_State* L );
	static int	_setStringColor		( lua_State* L );
	static int	_setTextSize		( lua_State* L );
	static int	_setYFlip			( lua_State* L );
	static int	_spool				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			Layout					();
	void			OnDepNodeUpdate			();
	void			Spool					( float step );
	
public:

	enum {
		TOTAL_ATTR = MOAIProp2D::TOTAL_ATTR,
	};

	DECL_LUA_FACTORY ( MOAITextBox )
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	void			ClearCurves				();
	void			Draw					();
	void			DrawDebug				();
	u32				GetLocalFrame			( USRect& frame );
	bool			IsDone					();
					MOAITextBox				();
					~MOAITextBox			();
	bool			More					();
	void			NextPage				( bool reveal );
	void			OnUpdate				( float step );
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			ReserveCurves			( u32 total );
	void			SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void			SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	void			SetColor				( float r, float g, float b, float a );
	void			SetCurve				( u32 idx, MOAIAnimCurve* curve );
	void			SetFont					( MOAIFont* font );
	void			SetRect					( float left, float top, float right, float bottom );
	void			SetText					( cc8* text );
	void			SetTextSize				( float newSize );
	STLString		ToString				();
};

#endif
