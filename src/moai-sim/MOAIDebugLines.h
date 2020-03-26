// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDEBUGLINES_H
#define	MOAIDEBUGLINES_H

class MOAIAbstractDrawingAPI;

//================================================================//
// MOAIDebugLineStyle
//================================================================//
class MOAIDebugLineStyle {
private:

	friend class MOAIDebugLinesMgr;

	enum {
		DISPLAY_VISIBLE,
		DISPLAY_HIDE,
		DISPLAY_PASS,
	};

	u32		mDisplay;
	float	mSize;
	u32		mColor;

public:

	//----------------------------------------------------------------//
			MOAIDebugLineStyle			();
			~MOAIDebugLineStyle			();
};

//================================================================//
// MOAIDebugLineStyleSet
//================================================================//
class MOAIDebugLineStyleSet {
private:
	
	friend class MOAIDebugLinesMgr;
	
	ZLLeanArray < MOAIDebugLineStyle > mLineStyles;
	
	bool					mShowDebugLines;

public:

	//----------------------------------------------------------------//
	MOAIDebugLineStyle*		GetStyle					( u32 styleID );
							MOAIDebugLineStyleSet		();
							~MOAIDebugLineStyleSet		();
	void					ReserveStyles				( u32 size );
};

//================================================================//
// MOAIDebugLinesMgr
//================================================================//
// TODO: doxygen
class MOAIDebugLinesMgr :
	public ZLContextClass < MOAIDebugLinesMgr >,
	public virtual MOAILuaObject {
private:

	static const u32	STYLE_SET_ID_SHIFT	= 16;
	static const u32	STYLE_SET_ID_MASK	= 0xffff0000;
	static const u32	STYLE_ID_MASK		= 0x0000ffff;

	typedef STLMap < u32, MOAIDebugLineStyleSet* >::iterator StyleSetIt;
	STLMap < u32, MOAIDebugLineStyleSet* >	mStyleSets;

	MOAIDebugLineStyleSet*	mActiveStyleSet;
	bool					mShowDebugLines;

	//----------------------------------------------------------------//
	static int				_setStyle				( lua_State* L );
	static int				_showDebugLines			( lua_State* L );

	//----------------------------------------------------------------//
	static u32				GetSetID				( u32 packedID );
	MOAIDebugLineStyle*		GetStyle				( u32 styleID );
	static u32				GetStyleID				( u32 packedID );
	void					ReserveStyleSet			( u32 setID, u32 size );
	bool					SelectStyleSet			( u32 setID );
	void					SetStyle				( u32 styleID );
	void					SetStyle				( u32 styleID, float size, u32 color );

	//----------------------------------------------------------------//
	void					_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_SINGLETON ( MOAIDebugLinesMgr )
	
	//----------------------------------------------------------------//
	bool			Bind					( u32 styleID );
	bool			Bind					( u32 styleID, MOAIAbstractDrawingAPI& draw );
	bool			IsVisible				();
	bool			IsVisible				( u32 styleID );
					MOAIDebugLinesMgr		();
					~MOAIDebugLinesMgr		();

	//----------------------------------------------------------------//
	template < typename TYPE >
	static u32 Pack ( u32 styleID ) {
	
		return ((( u32 )ZLType::RawID < TYPE >() << STYLE_SET_ID_SHIFT ) & STYLE_SET_ID_MASK ) | ( styleID & STYLE_ID_MASK );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void ReserveStyleSet ( u32 size ) {
	
		this->ReserveStyleSet (( u32 )ZLType::RawID < TYPE >(), size );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool SelectStyleSet () {
	
		return this->SelectStyleSet (( u32 )ZLType::RawID < TYPE >());
	}
};

#endif
