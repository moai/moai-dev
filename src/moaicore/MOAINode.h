// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAINODE_H
#define	MOAINODE_H

#include <moaicore/MOAIAttrOp.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAISharedPtr.h>
#include <moaicore/MOAIWeakPtr.h>

#define PACK_ATTR(type,attrID)	\
	( MOAINode::PackAttrID < type >( type::attrID ))

#define UNPACK_ATTR(attrID)	\
	( attrID & MOAINode::ATTR_ID_MASK )

#define DECL_ATTR_HELPER(type)																				\
	class type##Attr {																						\
	public:																									\
		static inline bool	Check	( u32 attrID ) { return MOAINode::CheckAttrID < type >( attrID ); }		\
		static inline u32	Pack	( u32 attrID ) { return MOAINode::PackAttrID < type >( attrID ); }		\
	};

class MOAINode;
class MOAIDepLink;

//================================================================//
// MOAINode
//================================================================//
/**	@name	MOAINode
	@text	Base for all attribute bearing Moai objects and dependency
			graph nodes.
*/
class MOAINode :
	public virtual MOAILuaObject {
private:

	enum {
		STATE_IDLE,			// not in list
		STATE_ACTIVE,		// in list but not scheduled
		STATE_SCHEDULED,	// in list and scheduled
		STATE_UPDATING,		// currently being updated
	};
	
	MOAIDepLink*	mPullLinks;
	MOAIDepLink*	mPushLinks;

	u32				mState;
	MOAINode*		mPrev;
	MOAINode*		mNext;

	//----------------------------------------------------------------//
	static int		_clearAttrLink		( lua_State* L );
	static int		_clearNodeLink		( lua_State* L );
	static int		_forceUpdate		( lua_State* L );
	static int		_getAttr			( lua_State* L );
	static int		_getAttrLink		( lua_State* L );
	static int		_moveAttr			( lua_State* L );
	static int		_scheduleUpdate		( lua_State* L );
	static int		_seekAttr			( lua_State* L );
	static int		_setAttrLink		( lua_State* L );
	static int		_setAttr			( lua_State* L );
	static int		_setNodeLink		( lua_State* L );
	
	//----------------------------------------------------------------//
	void			ActivateOnLink		( MOAINode& srcNode );
	void			DepNodeUpdate		();
	void			ExtendUpdate		();
	MOAIDepLink*	FindAttrLink		( int attrID );
	MOAIDepLink*	FindNodeLink		( MOAINode& srcNode );
	bool			IsNodeUpstream		( MOAINode* node );
	void			PullAttributes		();
	void			RemoveDepLink		( MOAIDepLink& link );

protected:

	//----------------------------------------------------------------//
	virtual void	OnDepNodeUpdate		();
	bool			PullLinkedAttr		( u32 attrID, MOAIAttrOp& attrOp );

	//----------------------------------------------------------------//
	float GetLinkedValue ( u32 attrID, float value ) {
		
		MOAIAttrOp attrOp;
		if ( this->PullLinkedAttr ( attrID, attrOp )) {
			return attrOp.IsNumber () ? attrOp.GetValue () : value;
		}
		return value;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* GetLinkedValue ( u32 attrID ) {
		
		MOAIAttrOp attrOp;
		if ( this->PullLinkedAttr ( attrID, attrOp )) {
			return attrOp.GetValue < TYPE >();
		}
		return 0;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	bool GetLinkedValue ( u32 attrID, TYPE& value ) {
		
		MOAIAttrOp attrOp;
		if ( this->PullLinkedAttr ( attrID, attrOp )) {
			return attrOp.GetValue < TYPE >( value );
		}
		return false;
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

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetDependentMember ( MOAISharedPtr < TYPE >& member, TYPE* ref ) {
		
		if ( member == ref ) return;
	
		if ( member ) {
			this->ClearNodeLink ( *member );
		}
		
		member = ref;
		
		if ( ref ) {
			this->SetNodeLink ( *ref );
		}
		
		this->ScheduleUpdate ();
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetDependentMember ( MOAIWeakPtr < TYPE >& member, TYPE* ref ) {
		
		if ( member == ref ) return;
		
		if ( member ) {
			this->ClearNodeLink ( *member );
		}
		
		member = ref;
		
		if ( ref ) {
			this->SetNodeLink ( *ref );
		}
		
		this->ScheduleUpdate ();
	}

public:
	
	friend class MOAINodeMgr;
	
	DECL_LUA_FACTORY ( MOAINode )
	
	static const u32	NULL_ATTR			= 0x3fffffff;
	static const u32	ATTR_ID_MASK		= 0x0000ffff;
	static const u32	CLASS_ID_MASK		= 0x3fff0000;
	static const u32	ATTR_FLAGS_MASK		= 0xC0000000;
	
	static const u32	ATTR_READ			= 0x40000000;
	static const u32	ATTR_WRITE			= 0x80000000;
	static const u32	ATTR_READ_WRITE		= 0xC0000000;
	
	//----------------------------------------------------------------//
	void			Activate				( MOAINode& activator );
	virtual bool	ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	bool			CheckAttrExists			( u32 attrID );
	void			ClearAttrLink			( int attrID );
	void			ClearNodeLink			( MOAINode& srcNode );
	void			ForceUpdate				();
	u32				GetAttrFlags			( u32 attrID );
					MOAINode				();
					~MOAINode				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ScheduleUpdate			();
	void			SetAttrLink				( int attrID, MOAINode* srcNode, int srcAttrID );
	void			SetNodeLink				( MOAINode& srcNode );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline bool CheckAttrID ( u32 attrID ) {
	
		return (( USTypeID < TYPE >::GetID ()) == (( attrID & CLASS_ID_MASK ) >> 16 ));
	}
	
	//----------------------------------------------------------------//
	float GetAttributeValue ( u32 attrID, float value ) {
		
		if ( attrID != NULL_ATTR ) {
			MOAIAttrOp getter;
			this->ApplyAttrOp ( attrID, getter, MOAIAttrOp::GET );
			value = getter.GetValue ();
		}
		return value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE GetAttributeValue ( u32 attrID, TYPE value ) {
		
		if ( attrID != NULL_ATTR ) {
			MOAIAttrOp getter;
			this->ApplyAttrOp ( attrID, getter, MOAIAttrOp::GET );
			value = getter.GetValue < TYPE >();
		}
		return value;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline u32 PackAttrID ( u32 attrID ) {
	
		return (( USTypeID < TYPE >::GetID () << 16 ) & CLASS_ID_MASK ) | ( attrID & ATTR_ID_MASK );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetAttributeValue ( u32 attrID, TYPE value ) {
		if ( attrID != NULL_ATTR ) {
			MOAIAttrOp setter;
			setter.SetValue ( value );
			this->ApplyAttrOp ( attrID, setter, MOAIAttrOp::SET );
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static inline u32 UnpackAttrID ( u32 attrID ) {
		
		return attrID & ATTR_ID_MASK;
	}
};

#endif
