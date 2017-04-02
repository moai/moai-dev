// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMMIXER_H
#define	MOAIANIMMIXER_H

#include <moai-sim/MOAIAnimChannel.h>
#include <moai-sim/MOAIAnimLoop.h>
#include <moai-sim/MOAITimer.h>

class MOAIAnimClip;
class MOAIAnimTrack;
class MOAIAnimCurveBase;
class MOAINode;

//================================================================//
// MOAIAnimChannelLink
//================================================================//
class MOAIAnimChannelLink {
public:

	ZLStrongPtr < MOAINode > mTarget;
	u32 mAttrID;
};

//================================================================//
// MOAIAnimTransition
//================================================================//
class MOAIAnimTransition {
public:

	ZLLeanLink < MOAIAnimTransition* > mLink;

	u32			mCurve;
	float		mStartTime;
	float		mDuration;
	float		mStartValue;
	float		mEndValue;
	bool		mPopOnEnd;
	
	MOAIAnimTrack*		mTarget;
	float*				mAttribute;
};

//================================================================//
// MOAIAnimTrack
//================================================================//
class MOAIAnimTrack {
protected:

	friend class MOAIAnimMixer;

	enum {
		ANIM_TRACK_SPEED,
		ANIM_TRACK_WEIGHT,
		TOTAL_ATTRIBUTES,
	};

	bool		mActive;		// clip is active in a stack
	u32			mIdentifier;
	
	union {
		struct {
			// order must match enum above!
			float	mSpeed;
			float	mWeight;
		};
		float mAttributes [ TOTAL_ATTRIBUTES ];
	};
	
	MOAIAnimTransition* mTransitions [ TOTAL_ATTRIBUTES ]; // one transition per attribute
};

//================================================================//
// MOAIAnimClip
//================================================================//
// this is a section of an anim loop that may be added to a cycle
class MOAIAnimClip :
	public MOAIAnimTrack {
private:

	friend class MOAIAnimMixer;

	ZLLeanLink < MOAIAnimClip* >		mLink;			// link in list of free clips or in stack
	
	ZLStrongPtr < MOAIAnimLoop >		mLoop;			// the loop to take the clip from
	float								mClipStart;		// start time of the clip
	float								mClipEnd;		// end time of the clip
	float								mClipLength;	// absolute length of clip in seconds
};

//================================================================//
// MOAIAnimCycle
//================================================================//
// a stack of anim clips
class MOAIAnimCycle :
	public MOAIAnimTrack {
private:

	friend class MOAIAnimMixer;

	ZLLeanLink < MOAIAnimCycle* >		mLink;				// link in list of free clips or in stack
	ZLLeanList < MOAIAnimClip* >		mStack;				// stack of clips
	float								mNormalizedTime;	// local time in the cycle (normalized: 0.0 to 1.0)
};

//================================================================//
// MOAIAnimMixer
//================================================================//
// TODO: doxygen
class MOAIAnimMixer :
	public virtual MOAIAction {
private:

	static const u32 INDEX_MASK				= 0x7fffffff;
	static const u32 CYCLE_FLAG				= 0x80000000;

	ZLLeanList < MOAIAnimTransition* >		mTransitionPool;
	ZLLeanList < MOAIAnimTransition* >		mActiveTransitions;

	ZLLeanArray < MOAIAnimChannel >			mChannels;				// channels (accumulators)
	ZLLeanArray < u8 >						mChannelBuffer;			// storage for accumulators
	ZLLeanArray < MOAIAnimChannelLink >		mLinks;					// maps channels onto node attributes

	ZLLeanList < MOAIAnimClip* >			mClipPool;				// pool of free clips
	ZLLeanList < MOAIAnimCycle* >			mCyclePool;				// pool of free cycles

	ZLLeanArray < MOAIAnimClip* >			mClipsByIndex;			// fast lookup for clips
	ZLLeanArray < MOAIAnimCycle* >			mCyclesByIndex;			// fast lookup for cycles

	ZLLeanList < MOAIAnimCycle* >			mStack;					// the stack of cycles

	float									mTime;

	//----------------------------------------------------------------//
	static int			_apply							( lua_State* L );
	static int			_buildChannels					( lua_State* L );
	static int			_insertClip						( lua_State* L );
	static int			_insertCycle					( lua_State* L );
	static int			_popClip						( lua_State* L );
	static int			_popCycle						( lua_State* L );
	static int			_pushClip						( lua_State* L );
	static int			_pushCycle						( lua_State* L );
	static int			_remove							( lua_State* L );
	static int			_reserveChannels				( lua_State* L );
	static int			_setChannelLink					( lua_State* L );
	static int			_setChannelType					( lua_State* L );
	static int			_setSpeed						( lua_State* L );
	static int			_setWeight						( lua_State* L );
	static int			_transition						( lua_State* L );
	
	//----------------------------------------------------------------//
	MOAIAnimClip&				AffirmClip				( MOAIAnimLoop& loop, float clipStart, float clipEnd );
	MOAIAnimCycle&				AffirmCycle				();
	MOAIAnimTransition&			AffirmTransition		();
	void						DeactivateClip			( MOAIAnimClip& clip );
	void						DeactivateCycle			( MOAIAnimCycle& cycle );
	MOAIAnimClip*				GetClip					( u32 identifier );
	MOAIAnimCycle*				GetCycle				( u32 identifier );
	MOAIAnimTrack*				GetTrack				( u32 identifier );
	
	//----------------------------------------------------------------//
	bool				MOAIAction_IsDone	();
	void				MOAIAction_Update	( double step );
	
public:
	
	DECL_LUA_FACTORY ( MOAIAnimMixer )
	
	//----------------------------------------------------------------//
	void				Apply				( float t );
	void				BuildChannels		();
	MOAIAnimChannel*	GetChannel			( u32 channelID );
	ZLResult < u32 >	InsertClip			( u32 identifier, MOAIAnimLoop& loop, float clipStart, float clipEnd );
	ZLResult < u32 >	InsertCycle			( u32 identifier );
						MOAIAnimMixer		();
						~MOAIAnimMixer		();
	void				PopClip				();
	void				PopCycle			();
	ZLResult < u32 >	PushClip			( MOAIAnimLoop& loop, float clipStart, float clipEnd );
	ZLResult < u32 >	PushCycle			();
	void				RegisterLuaClass	( MOAILuaState& state );
	void				RegisterLuaFuncs	( MOAILuaState& state );
	void				Remove				( u32 identifier );
	void				ReserveChannels		( size_t nChannels );
	void				SetAttribute		( u32 identifier, u32 attrID, float value );
	void				SetChannelType		( u32 channelID, u32 type );
	void				SetChannelLink		( u32 channelID, MOAINode* target, u32 attrID );
	void				Transition			( u32 identifier, u32 attrID, float value, float duration, u32 curve, bool popOnEnd );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void Accumulate ( u32 channelID, const TYPE& value, float weight ) {
		
		assert ( channelID < this->mChannels.Size ());
		this->mChannels [ channelID ].Accumulate ( value, weight );
	}
};

#endif
