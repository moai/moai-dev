// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAINODE_H
#define	MOAINODE_H

#include <moai-core/MOAIEventSource.h>
#include <moai-core/MOAILua.h>

#define DECL_ATTR_HELPER(type)																								\
	class AttrID {																											\
	public:																													\
		static inline bool		Check		( ZLAttrID attrID ) { return MOAINode::CheckAttrID < type >( attrID ); }		\
		static inline ZLAttrID	Pack		( u32 rawID ) { return MOAINode::PackAttrID < type >( rawID ); }				\
	};

#define DECL_ATTR_ID(name)																									\
	static inline ZLAttrID	AttrID_##name			() { return AttrID::Pack ( name ); }

class MOAINode;
class MOAIDepLink;

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

protected:

	//----------------------------------------------------------------//
	static int		_clearAttrLink		( lua_State* L );
	static int		_clearNodeLink		( lua_State* L );
	static int		_forceUpdate		( lua_State* L );
	static int		_getAttr			( lua_State* L );
	static int		_getAttrLink		( lua_State* L );
	static int		_getNodeState		( lua_State* L );
	static int		_moveAttr			( lua_State* L ); // TODO: this should be a static method exposed by MOAIEaseDriver.
	static int		_scheduleUpdate		( lua_State* L );
	static int		_seekAttr			( lua_State* L ); // TODO: this should be a static method exposed by MOAIEaseDriver.
	static int		_setAttr			( lua_State* L );
	static int		_setAttrLink		( lua_State* L );
	static int		_setNodeLink		( lua_State* L );
	
	//----------------------------------------------------------------//
	void			ActivateOnLink		( MOAINode& srcNode );
	void			ExtendUpdate		();
	MOAIDepLink*	FindAttrLink		( ZLAttrID attrID );
	MOAIDepLink*	FindNodeLink		( MOAINode& srcNode );
	bool			IsNodeUpstream		( MOAINode* node );
	void			PullAttributes		();
	bool			PullLinkedAttr		( ZLAttrID attrID, ZLAttribute& attr );
	void			RemoveDepLink		( MOAIDepLink& link );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual bool		MOAINode_ApplyAttrOp		( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	virtual void		MOAINode_Update				();

	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE GetLinkedValue ( ZLAttrID attrID, const TYPE& value ) {
		
		ZLAttribute attr;
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
	bool			ApplyAttrOp				( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	bool			CheckAttrExists			( ZLAttrID attrID );
	void			ClearAttrLink			( ZLAttrID attrID );
	void			ClearNodeLink			( MOAINode& srcNode );
	void			DepNodeUpdate			();
	void			ForceUpdate				();
	u32				GetAttrFlags			( ZLAttrID attrID );
					MOAINode				();
					~MOAINode				();
	void			ScheduleUpdate			();
	void			SetAttrLink				( ZLAttrID attrID, MOAINode* srcNode, ZLAttrID srcAttrID );
	void			SetNodeLink				( MOAINode& srcNode );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline bool CheckAttrID ( ZLAttrID attrID ) {
	
		return (( ZLType::RawID < TYPE >()) == (( attrID.mPackedID & ZLAttribute::CLASS_ID_MASK ) >> 16 ));
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE GetAttributeValue ( ZLAttrID attrID, TYPE value ) {
		
		if ( attrID.mPackedID != ZLAttribute::NULL_ATTR ) {
			ZLAttribute getter;
			this->ApplyAttrOp ( attrID, getter, ZLAttribute::GET );
			value = getter.GetValue ( value );
		}
		return value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline ZLAttrID PackAttrID ( u32 rawID ) {
	
		ZLType::RAW_ID rawTypeID = ZLType::RawID < TYPE >();
		assert ( rawTypeID < 0xffff );
	
		return ZLAttrID::FromRaw ((( rawTypeID << 16 ) & ZLAttribute::CLASS_ID_MASK ) | ( rawID & ZLAttribute::ATTR_ID_MASK ));
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetAttributeValue ( ZLAttrID attrID, TYPE value ) {
	
		if ( attrID.mPackedID != ZLAttribute::NULL_ATTR ) {
			ZLAttribute setter;
			setter.SetValue ( value );
			this->ApplyAttrOp ( attrID, setter, ZLAttribute::SET );
		}
	}
};

#endif
