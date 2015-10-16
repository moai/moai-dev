// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISPINESKELETON_H
#define MOAISPINESKELETON_H

#include <spine/spine.h>

class MOAISpineBone;
class MOAISpineSlot;
class MOAISpineSkeletonData;


//================================================================//
// MOAISpineSkeleton
//================================================================//
/**	@name	MOAISpineSkeleton
	@text	Special prop for drawing animated spine characters
*/
class MOAISpineSkeleton :
	public MOAIGraphicsProp,
	public MOAIAction {
private:
	
	bool			mComputeBounds;
	ZLBox			mSkeletonBounds;
		
	spSkeleton*			mSkeleton;
	spAnimationState*	mAnimationState;
	MOAISpineBone*		mRootBone;
		
	MOAILuaSharedPtr < MOAISpineSkeletonData > mSkeletonData;
	ZLLeanStack < float, 32 > mVertices;
		
	typedef STLMap < STLString, MOAISpineBone* >::iterator BoneTransformIt;
	STLMap < STLString, MOAISpineBone* > mBoneTransformMap;
	
	typedef STLMap < STLString, MOAISpineSlot* >::iterator SlotColorIt;
	STLMap < STLString, MOAISpineSlot* > mSlotColorMap;
	
	//----------------------------------------------------------------//
	static int		_addAnimation			( lua_State* L );
	static int		_clearAllTracks			( lua_State* L );
	static int		_clearTrack				( lua_State* L );
	static int		_getAttachmentVertices	( lua_State* L );
	static int		_getBone				( lua_State* L );
    static int      _getDuration            ( lua_State* L );
	static int		_getSlot				( lua_State* L );
	static int		_init					( lua_State* L );
	static int		_initAnimationState		( lua_State* L );
	static int		_setAnimation			( lua_State* L );
	static int		_setAttachment			( lua_State* L );
	static int		_setBonesToSetupPose	( lua_State* L );
	static int		_setComputeBounds		( lua_State* L );
	static int		_setFlip				( lua_State* L );
	static int		_setMix					( lua_State* L );
	static int		_setSkin				( lua_State* L );
	static int		_setSlotsToSetupPose	( lua_State* L );
	static int		_setSlotColor			( lua_State* L );
	static int		_setTime				( lua_State* L );
	static int		_setToSetupPose			( lua_State* L );
        
protected:
	//----------------------------------------------------------------//
	void			OnDepNodeUpdate			();
    u32				OnGetModelBounds		( ZLBox& bounds );
    void			OnUpdate				( double step );
        
public:
	
	DECL_LUA_FACTORY ( MOAISpineSkeleton )
			
	enum {
		EVENT_ANIMATION_START = MOAIAction::TOTAL_EVENTS,
		EVENT_ANIMATION_END,
		EVENT_ANIMATION_COMPLETE,
		EVENT_ANIMATION_EVENT,
		TOTAL_EVENTS
	};

	//----------------------------------------------------------------//
	void			AddAnimation			( int trackId, cc8* name, bool loop, float delay );
	void			AffirmBoneHierarchy		( spBone* bone );
	void			ClearAllTracks			();
	void			ClearTrack				( int trackId );
	void			Draw					( int subPrimID, float lod );
	void			DrawDebug				( int subPrimID, float lod );
	void			Init					( spSkeletonData* skeletonData );
	void			InitAnimationState		( spAnimationStateData* animData );
	bool			IsDone					();
					MOAISpineSkeleton		();
					~MOAISpineSkeleton		();
	
	void			OnAnimationEvent		( int trackIndex, spEventType type, spEvent* event, int loopCount );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetAnimation			( int trackId, cc8* name, bool loop );
	void			SetMix					( cc8* fromName, cc8* toName, float duration );
	void			UpdateBounds            ();
	void			UpdateSkeleton			();
};

#endif
