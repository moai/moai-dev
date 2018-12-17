// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAINODE_H
#define	MOAINODE_H

#include <moai-sim/MOAIAttribute.h>

#define DECL_ATTR_HELPER(type)																									\
	class AttrID {																												\
	public:																														\
		static inline bool			Check		( MOAIAttrID attrID ) { return MOAINode::CheckAttrID < type >( attrID ); }		\
		static inline MOAIAttrID	Pack		( u32 rawID ) { return MOAINode::PackAttrID < type >( rawID ); }				\
	};

#define DECL_ATTR_ID(name)																										\
	static inline MOAIAttrID	AttrID_##name		() { return AttrID::Pack ( name ); }

class MOAINode;
class MOAIDepLink;

//================================================================//
// MOAIAttrID
//================================================================//
class MOAIAttrID {
private:
	friend class MOAINode;
	friend class MOAIDepLink;
	
	u32		mPackedID;

public:

	//----------------------------------------------------------------//
	static MOAIAttrID FromRaw ( u32 packedID ) {
		MOAIAttrID attrID;
		attrID.mPackedID = packedID;
		return attrID;
	}
	
	//----------------------------------------------------------------//
	u32 ToRaw () const {
		return this->mPackedID;
	}

	//----------------------------------------------------------------//
	u32 Unpack () const {
		return this->mPackedID &  MOAIAttribute::ATTR_ID_MASK;
	}
};

//================================================================//
// MOAINode
//================================================================//
/**	@lua	MOAINode
	@text	Base for all attribute bearing Moai objects and dependency
			graph nodes.
*/
class MOAINode :
	public virtual MOAIInstanceEventSource {
private:

	enum {
		STATE_IDLE,			// not in list
		STATE_ACTIVE,		// in list but not scheduled
		STATE_SCHEDULED,	// in list and scheduled
		STATE_UPDATING,		// currently being updated
	};
	
	MOAIDepLink*		mPullLinks;
	MOAIDepLink*		mPushLinks;

	u32					mState;
	MOAINode*			mPrev;
	MOAINode*			mNext;

	//----------------------------------------------------------------//
	static int		_clearAttrLink		( lua_State* L );
	static int		_clearNodeLink		( lua_State* L );
	static int		_forceUpdate		( lua_State* L );
	static int		_getAttr			( lua_State* L );
	static int		_getAttrLink		( lua_State* L );
	static int		_getNodeState		( lua_State* L );
	static int		_moveAttr			( lua_State* L );
	static int		_scheduleUpdate		( lua_State* L );
	static int		_seekAttr			( lua_State* L );
	static int		_setAttrLink		( lua_State* L );
	static int		_setAttr			( lua_State* L );
	static int		_setNodeLink		( lua_State* L );
	
	//----------------------------------------------------------------//
	void			ActivateOnLink		( MOAINode& srcNode );
	void			ExtendUpdate		();
	MOAIDepLink*	FindAttrLink		( MOAIAttrID attrID );
	MOAIDepLink*	FindNodeLink		( MOAINode& srcNode );
	bool			IsNodeUpstream		( MOAINode* node );
	void			PullAttributes		();
	void			RemoveDepLink		( MOAIDepLink& link );

	//----------------------------------------------------------------//
	virtual bool	MOAINode_ApplyAttrOp				( MOAIAttrID attrID, MOAIAttribute& attr, u32 op );
	virtual void	MOAINode_Update						();

protected:

	//----------------------------------------------------------------//
	bool			PullLinkedAttr		( MOAIAttrID attrID, MOAIAttribute& attr );

	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE GetLinkedValue ( MOAIAttrID attrID, const TYPE& value ) {
		
		MOAIAttribute attr;
		if ( this->PullLinkedAttr ( attrID, attr )) {
			return attr.GetValue ( value );
		}
		return value;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetDependentMember ( MOAILuaSharedPtr < TYPE >& member, TYPE* ref ) {
		
		if ( member == ref ) return;
	
		if ( member ) {
			this->ClearNodeLink ( *member );
		}
		
		member.Set ( *this, ref );
		
		if ( ref ) {
			this->SetNodeLink ( *ref );
		}
		
		this->ScheduleUpdate ();
	}

public:
	
	friend class MOAINodeMgr;
	
	DECL_LUA_FACTORY ( MOAINode )

	enum {
		EVENT_NODE_PRE_UPDATE,
		EVENT_NODE_POST_UPDATE,
		TOTAL_EVENTS,
	};

	//----------------------------------------------------------------//
	void			Activate				( MOAINode& activator );
	bool			ApplyAttrOp				( MOAIAttrID attrID, MOAIAttribute& attr, u32 op );
	bool			CheckAttrExists			( MOAIAttrID attrID );
	void			ClearAttrLink			( MOAIAttrID attrID );
	void			ClearNodeLink			( MOAINode& srcNode );
	void			DepNodeUpdate			();
	void			ForceUpdate				();
	u32				GetAttrFlags			( MOAIAttrID attrID );
					MOAINode				();
					~MOAINode				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ScheduleUpdate			();
	void			SetAttrLink				( MOAIAttrID attrID, MOAINode* srcNode, MOAIAttrID srcAttrID );
	void			SetNodeLink				( MOAINode& srcNode );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline bool CheckAttrID ( MOAIAttrID attrID ) {
	
		return (( ZLTypeID < TYPE >::GetID ()) == (( attrID.mPackedID & MOAIAttribute::CLASS_ID_MASK ) >> 16 ));
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE GetAttributeValue ( MOAIAttrID attrID, TYPE value ) {
		
		if ( attrID.mPackedID != MOAIAttribute::NULL_ATTR ) {
			MOAIAttribute getter;
			this->ApplyAttrOp ( attrID, getter, MOAIAttribute::GET );
			value = getter.GetValue ( value );
		}
		return value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline MOAIAttrID PackAttrID ( u32 rawID ) {
	
		return MOAIAttrID::FromRaw ((( ZLTypeID < TYPE >::GetID () << 16 ) & MOAIAttribute::CLASS_ID_MASK ) | ( rawID & MOAIAttribute::ATTR_ID_MASK ));
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetAttributeValue ( MOAIAttrID attrID, TYPE value ) {
	
		if ( attrID.mPackedID != MOAIAttribute::NULL_ATTR ) {
			MOAIAttribute setter;
			setter.SetValue ( value );
			this->ApplyAttrOp ( attrID, setter, MOAIAttribute::SET );
		}
	}
};

#endif
