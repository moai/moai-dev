// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILAYOUTFRAME_H
#define	MOAILAYOUTFRAME_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// MOAILayoutFrame
//================================================================//
/**	@brief	Frame object for hierarchical, resizable layouts and auto-fitting.
			(Temporarily disabled).
*/
class MOAILayoutFrame :
	public MOAITransform {
private:
	
	enum {
		LAYOUT_ALIGN_MIN		= 1 << 0x00,
		LAYOUT_ALIGN_MAX		= 1 << 0x01,
		LAYOUT_ALIGN_CENTER		= 1 << 0x02,
		
		LAYOUT_JUSTIFY_MIN		= 1 << 0x03,
		LAYOUT_JUSTIFY_MAX		= 1 << 0x04,
		LAYOUT_JUSTIFY_CENTER	= 1 << 0x05,
		
		LAYOUT_ALIGN			= 0x00000007,
		LAYOUT_JUSTIFY			= 0x00000038,
	};
	
	enum {
		FIT_EXPAND,
		FIT_ABSOLUTE,
		FIT_CONTENT,
	};
	
	enum {
		X_AXIS	= 0,
		Y_AXIS	= 1,
		MIN		= 0,
		MAX		= 1,
	};
	
	typedef USLeanLink < MOAILayoutFrame* > ChildLink;
	USLeanList < MOAILayoutFrame* > mChildren;
	USLeanLink < MOAILayoutFrame* > mLinkInChildren;
	
	u32				mLayout [ 2 ];
	u32				mFit [ 2 ];
	float			mWeight [ 2 ];
	
	float			mContentSize [ 2 ];
	float			mIdealSize [ 2 ];
	float			mSize [ 2 ];
	
	float			mMargin [ 2 ][ 2 ];
	
	//----------------------------------------------------------------//
	static int		_layout					( lua_State* L );
	static int		_setFit					( lua_State* L );
	static int		_setIdealSize			( lua_State* L );
	static int		_setLayout				( lua_State* L );
	static int		_setMargin				( lua_State* L );
	static int		_setWeight				( lua_State* L );

	//----------------------------------------------------------------//
	void				ComputeContentSize		( u32 axis );
	void				Fit						( u32 axis );
	float				GetInnerSize			( u32 axis );
	MOAILayoutFrame*	GetParentWidget			();
	u32					GetPerpAxis				( u32 axis );
	float				GetSize					( u32 axis );
	void				Layout					();
	void				Layout					( u32 axis );
	void				RemoveChildren			();
	void				SetLocByAxis			( u32 axis, float loc );

public:
	
	DECL_LUA_FACTORY ( MOAILayoutFrame )
	
	//----------------------------------------------------------------//
	USRect			GetFrame				();
	USRect			GetScissorRect			();
					MOAILayoutFrame			();
					~MOAILayoutFrame		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetParent				( MOAITransformBase* parent );
};

#endif
