// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	FSMSTATEFACTORY_H
#define	FSMSTATEFACTORY_H

#include <uslsext/USFsm.h>

//================================================================//
// USAbstractStateCreator
//================================================================//
class USAbstractStateCreator {
public:

	//----------------------------------------------------------------//
	virtual						~USAbstractStateCreator		() {}
	virtual USFsmStateBase&		SetState					( USFsm& fsm, RTTIBase& self ) = 0;
};

//================================================================//
// USConcreteStateCreator
//================================================================//
template < typename STATE_TYPE >
class USConcreteStateCreator :
	public USAbstractStateCreator {
public:

	//----------------------------------------------------------------//
	USFsmStateBase& SetState ( USFsm& fsm, RTTIBase& self ) {
		return fsm.template SetState < STATE_TYPE >( self );
	}
};

//================================================================//
// USFsmStateFactory
//================================================================//
class USFsmStateFactory {
private:

	typedef STLMap < STLString, USAbstractStateCreator* >::iterator CreatorIt;
	STLMap < STLString, USAbstractStateCreator* > mCreatorTable;

public:

	//----------------------------------------------------------------//
	void Clear () {
		mCreatorTable.clear ();
	}

	//----------------------------------------------------------------//
	template < typename STATE_TYPE >
	void RegisterState ( cc8* statename ) {
	
		this->mCreatorTable [ statename ] = &USGlobalsMgr::Get ()->GetGlobal < USConcreteStateCreator < STATE_TYPE > > ();
	}
	
	//----------------------------------------------------------------//
	USFsmStateBase* SetState ( USFsm& fsm, RTTIBase& self, cc8* statename ) {
	
		CreatorIt creatorIt = this->mCreatorTable.find ( statename );
		if ( creatorIt != this->mCreatorTable.end ()) {
			USAbstractStateCreator* creator = creatorIt->second;
			return &creator->SetState ( fsm, self );
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	USFsmStateFactory () {
	}

	//----------------------------------------------------------------//
	virtual ~USFsmStateFactory () {
	}
};

#endif
