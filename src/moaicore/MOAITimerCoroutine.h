//
//  MOAITimerCoroutine.h
//  libmoai
//
//  Created by Alexey Chernikov on 4/18/13.
//
//

#ifndef MOAITimerCoroutine_H
#define MOAITimerCoroutine_H


#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAINode.h>
#include <moaicore/MOAIAnimCurve.h>


#include <moaicore/MOAITimer.h>
//#include <moaicore/MOAICoroutine.h>

class MOAITimerCoroutine : public MOAITimer //public virtual MOAINode, public MOAIAction
{
private:
	MOAILuaLocal	mRef; // reference to the coroutine
	lua_State*		mState;
	u32				mNarg;
	STLString		mFuncName;
	
	bool			mIsUpdating;
	bool			mIsActive;
	bool			mIsFirstRun;

protected:
	MOAILuaSharedPtr < MOAIAnimCurve > mCurve;
	float	mStartTime;
	float	mEndTime;
	float	mTime;

	
public:
	
	//----------------------------------------------------------------//
	static int		_blockOnAction			( lua_State* L );
	static int		_currentThread			( lua_State* L );
	static int		_run					( lua_State* L );
	static int		_setSpan			( lua_State* L );
	static int		_setCurve			( lua_State* L );
	static int		_setTime			( lua_State* L );

	//----------------------------------------------------------------//
	bool			ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			OnUpdate			( float step );
	bool			IsDone				();
	void			SetSpan				( float span );
	void			SetSpan				( float startTime, float endTime );
	void			GenerateKeyframeCallbacks	( float t0, float t1, bool end );
	void			OnKeyframe					( u32 idx, float time, float value );
	void			SetTime				( float time );
	
	MOAITimerCoroutine			();
	~MOAITimerCoroutine			();

	DECL_LUA_FACTORY ( MOAITimerCoroutine )
	DECL_ATTR_HELPER ( MOAITimerCoroutine )
};

#endif /* defined(__libmoai__MOAITimerCoroutine__) */