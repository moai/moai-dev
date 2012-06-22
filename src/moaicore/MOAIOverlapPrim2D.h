//[MOAI LICENSE]

#ifndef	MOAIOVERLAPPRIM2D_H
#define	MOAIOVERLAPPRIM2D_H

#include <moaicore/MOAIPrim.h>

class MOAIMultiShape2D;
class MOAIOverlapPrim2D;
class MOAIOverlapPair2D;

//================================================================//
// MOAIOverlapLink2D
//================================================================//
class MOAIOverlapLink2D {
private:

	friend class MOAIOverlapPrim2D;
	friend class MOAICollisionMgr;

	MOAIOverlapPrim2D*		mPrim;
	MOAIOverlapLink2D*		mNext;
	bool					mWeak;
};

//================================================================//
// MOAIOverlapPrim2D
//================================================================//
/**	@brief Partial implementation for primitives that implement overlap detection.
*/
class MOAIOverlapPrim2D :
	public virtual MOAIPrim {
private:
	
	enum {
		OVERLAP_IDLE,
		OVERLAP_SCHEDULED,
		OVERLAP_DONE,
	};
	
	static const u32 SEND_ALL		= 0xffffffff;
	static const u32 RECEIVE_ALL	= 0xffffffff;
	
	USQuad					mQuad;
	MOAIOverlapPrim2D*		mNext;
	u32						mOverlapState;
	
	u32						mSendFlags;
	u32						mReceiveFlags;
	bool					mIsActive;

	USLuaRef				mOnOverlap;
	
	bool					mCacheOverlaps;
	MOAIOverlapLink2D*		mOverlaps;

	//----------------------------------------------------------------//
	static int		_setActive				( lua_State* L );
	static int		_setCallback			( lua_State* L );
	static int		_setCacheOverlaps		( lua_State* L );
	static int		_setReceiveFlags		( lua_State* L );
	static int		_setSendFlags			( lua_State* L );

	//----------------------------------------------------------------//
	void					BreakLink				( MOAIOverlapLink2D* link );
	void					ClearLinks				();
	void					MakeLink				( MOAIOverlapPrim2D& target, USVec2D& loc );
	void					OnOverlapBegin			( u32 overlap, const USVec2D& loc );
	void					OnOverlapEnd			( u32 overlap );
	void					PurgeWeakLinks			();
	void					WeakenLinks				();

public:

	friend class MOAICollisionMgr;
	
	//----------------------------------------------------------------//
	virtual void			GetShape				( MOAIMultiShape2D& shape );
							MOAIOverlapPrim2D		();
							~MOAIOverlapPrim2D		();
	void					RegisterLuaClass		( USLuaState& state );
	void					RegisterLuaFuncs		( USLuaState& state );
	STLString				ToString				();
};

#endif
