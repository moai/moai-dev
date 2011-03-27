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
/**	@brief Base for all attribute bearing Moai objects.
*/
class MOAINode :
	public virtual USLuaData,
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
	static int	_countAttrs			( lua_State* L );
	static int	_getAttr			( lua_State* L );
	static int	_moveAttr			( lua_State* L );
	static int	_scheduleUpdate		( lua_State* L );
	static int	_seekAttr			( lua_State* L );
	static int	_setAttrLink		( lua_State* L );
	static int	_setAttr			( lua_State* L );
	static int	_setDependency		( lua_State* L );
	
	//----------------------------------------------------------------//
	void			Activate				( MOAINode& activator );
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
	virtual void	OnDepNodeUpdate		();

public:
	
	friend class MOAINodeMgr;
	
	DECL_LUA_DATA ( MOAINode )
	
	static const u32 NULL_ATTR = 0xffffffff;
	
	//----------------------------------------------------------------//
	void			ClearDependency			( MOAINode& srcNode );
	void			ClearAttrLink			( int attrID );
					MOAINode				();
					~MOAINode				();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			ScheduleUpdate			();
	void			SetAttrLink				( int attrID, MOAINode* srcNode, int srcAttrID );
	void			SetDependency			( MOAINode& srcNode );
};

#endif
