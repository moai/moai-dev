// Copyright (c) 2012 Double Fine Productions. All Rights Reserved.

#ifndef	MOAIPROFILEREPORTBOX_H
#define	MOAIPROFILEREPORTBOX_H

#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIProfiler.h>
#include <moai-sim/MOAIProfilerEntry.h>

class MOAIFont;

//================================================================//
// MOAIProfilerReportBox
//================================================================//
class MOAIProfilerReportBox :
	public MOAIGraphicsProp {
private:
	
	struct Column {

		float			mX;
		float			mWidth;
	};

	enum {

		COLUMN_NAME,
		COLUMN_HITS,
		COLUMN_PERCENTAGE,
		COLUMN_DURATION,
		COLUMN_AVGDURATION,
		COLUMN_MINDURATION,
		COLUMN_MAXDURATION,

		COLUMN_COUNT
	};
	
	//----------------------------------------------------------------//
	ZLRect				mFrame;

	MOAIFont*			mFont;
	float				mFontSize;

	u32					mBackgroundColor;
	u32					mRowColor;
	u32					mTextColor;

	u8					mOrientation;

	// a user memory value that will get displayed with the memory stats
	u32                 mUserMemory;	
	ZLVec2D				mMemoryXRange;
	float				mLineHeight;

	char				mTemp [ 128 ];
	
	ZLVec2D				mSummaryYRange;
	ZLVec2D				mOverviewXRange;

	ZLVec2D				mHeaderYRange;
	ZLVec2D				mBodyYRange;

	Column				mColumns [ COLUMN_COUNT ];

	float				mLineOffset;
	float				mCursorY;

	float				mOneOverTotalDurationMicroSec;

	float				mSampleVerts [ MOAIProfilerEntry::NUM_SAMPLES * 2 ];

	//----------------------------------------------------------------//
	static int			_getRect				( lua_State* L );
	static int			_getFont				( lua_State* L );
	static int			_getFontSize			( lua_State* L );
	static int			_getBackgroundColor		( lua_State* L );
	static int			_getRowColor			( lua_State* L );
	static int			_getTextColor			( lua_State* L );
	static int			_getOrientation			( lua_State* L );

	static int			_setRect				( lua_State* L );
	static int			_setFont				( lua_State* L );
	static int			_setFontSize			( lua_State* L );
	static int          _setUserMemory          ( lua_State* L );
	static int			_setBackgroundColor		( lua_State* L );
	static int			_setRowColor			( lua_State* L );
	static int			_setTextColor			( lua_State* L );
	static int			_setOrientation			( lua_State* L );

	static int			_enableProfiling		( lua_State* L );
	static int			_disableProfiling		( lua_State* L );
		
	//----------------------------------------------------------------//
	static u32			DrawEntryText			( MOAIProfilerEntryBase* entry, void* userData );
	static u32			DrawEntryVisuals		( MOAIProfilerEntryBase* entry, void* userData );

	bool				_DrawEntryText			( MOAIProfilerEntry* entry );
	bool				_DrawEntryVisuals		( MOAIProfilerEntry* entry );

	void				_GetMemoryStats			( u32& availableMainMem, u32& usedMainMem ) const;

	
public:

	DECL_LUA_FACTORY ( MOAIProfilerReportBox )
	
	//----------------------------------------------------------------//
	GET_SET ( MOAIFont*, Font, mFont );
	GET_SET ( float, FontSize, mFontSize );

	GET_SET ( u32, BackgroundColor, mBackgroundColor );
	GET_SET ( u32, RowColor, mRowColor );
	GET_SET ( u32, TextColor, mTextColor );
	GET_SET ( u32, UserMemory, mUserMemory );

	GET_SET ( u8, Orientation, mOrientation );

	//----------------------------------------------------------------//
						MOAIProfilerReportBox	();
						~MOAIProfilerReportBox	();
	//----------------------------------------------------------------//
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				Draw					( int subPrimID, float lod );
	u32					OnGetModelBounds		( ZLBox& bounds );
	//----------------------------------------------------------------//
	void				SetRect					( float left, float top, float right, float bottom );
};

#endif
