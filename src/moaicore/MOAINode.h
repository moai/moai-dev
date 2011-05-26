// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAINODE_H
#define	MOAINODE_H

class MOAINode;

//================================================================//
// MOAIAttrLink
//================================================================//
class MOAIAttrLink {
private:

	USWeak < MOAINode >			mSourceNode;
	u32							mSourceAttrID;

	MOAINode*					mDestNode;
	u32							mDestAttrID;
	bool						mDestAttrExists;

	// sibling pointers for the two singly linked lists
	MOAIAttrLink*				mNextInSource;
	MOAIAttrLink*				mNextInDest;

	//----------------------------------------------------------------//

public:

	friend class MOAINode;

	//----------------------------------------------------------------//
			MOAIAttrLink		();
			~MOAIAttrLink		();
};

//================================================================//
// MOAINode
//================================================================//
/**	@name	MOAINode
	@text	Base for all attribute bearing Moai objects and dependency
			graph nodes.
*/
class MOAINode :
	public virtual USLuaObject,
	public USAttributed {
private:

	enum {
		STATE_IDLE,			// not in list
		STATE_ACTIVE,		// in list but not scheduled
		STATE_SCHEDULED,	// in list and scheduled
	};
	
	MOAIAttrLink*	mPullAttrLinks;
	MOAIAttrLink*	mPushAttrLinks;

	u32				mState;
	MOAINode*		mPrev;
	MOAINode*		mNext;

	//----------------------------------------------------------------//
	static int	_clearAttrLink		( lua_State* L );
	static int	_clearDependency	( lua_State* L );
	static int	_forceUpdate		( lua_State* L );
	static int	_getAttr			( lua_State* L );
	static int	_moveAttr			( lua_State* L );
	static int	_scheduleUpdate		( lua_State* L );
	static int	_seekAttr			( lua_State* L );
	static int	_setAttrLink		( lua_State* L );
	static int	_setAttr			( lua_State* L );
	static int	_setDependency		( lua_State* L );
	
	//----------------------------------------------------------------//
	void			ActivateOnLink			( MOAINode& srcNode );
	MOAIAttrLink*	AffirmAttrLink			( int attrID, MOAINode* srcNode, int srcAttrID );
	MOAIAttrLink*	AffirmDependency		( MOAINode& srcNode );
	MOAIAttrLink*	AffirmPullLink			( int attrID );
	void			AffirmPushLink			( MOAIAttrLink& link, int attrID );
	void			ClearPullLink			( int attrID );
	void			ClearPullLink			( MOAIAttrLink& link );
	void			ClearPushLink			( MOAIAttrLink& link );
	void			DepNodeUpdate			();
	void			ExtendUpdate			();
	bool			IsNodeUpstream			( MOAINode* node );
	void			PullAttributes			();

protected:

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetDependentMember ( USRef < TYPE >& member, TYPE* ref ) {
		
		if ( member == ref ) return;
	
		if ( member ) {
			this->ClearDependency ( *member );
		}
		
		member = ref;
		
		if ( ref ) {
			this->SetDependency ( *ref );
		}
		
		this->ScheduleUpdate ();
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetDependentMember ( USWeak < TYPE >& member, TYPE* ref ) {
		
		if ( member == ref ) return;
	
		if ( member ) {
			this->ClearDependency ( *member );
		}
		
		member = ref;
		
		if ( ref ) {
			this->SetDependency ( *ref );
		}
		
		this->ScheduleUpdate ();
	}

	//----------------------------------------------------------------//
	virtual void	OnDepNodeUpdate		();

public:
	
	friend class MOAINodeMgr;
	
	DECL_LUA_FACTORY ( MOAINode )
	
	static const u32 NULL_ATTR = 0xffffffff;
	
	//----------------------------------------------------------------//
	void			Activate				( MOAINode& activator );
	void			ClearDependency			( MOAINode& srcNode );
	void			ClearAttrLink			( int attrID );
	void			ForceUpdate				();
					MOAINode				();
					~MOAINode				();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			ScheduleUpdate			();
	void			SetAttrLink				( int attrID, MOAINode* srcNode, int srcAttrID );
	void			SetDependency			( MOAINode& srcNode );
};

#endif
