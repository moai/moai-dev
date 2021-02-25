// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLFINALIZATIONVISITOR_H
#define	ZLFINALIZATIONVISITOR_H

#include <zl-core/ZLRtti.h>
#include <zl-core/ZLRttiVisitorHistory.h>

#define ZL_FINALIZATION_VISITOR_FRIEND 														\
	template < typename TYPE > friend class ZLFinalizationVisitor;							\

//================================================================//
// ZLAbstractFinalizationVisitor
//================================================================//
class ZLAbstractFinalizationVisitor {
private:

	//----------------------------------------------------------------//
	virtual void	ZLAbstractFinalizationVisitor_Finalize 	( RTTIBase& self ) const = 0;

public:

	//----------------------------------------------------------------//
	virtual ~ZLAbstractFinalizationVisitor () {
	}

	//----------------------------------------------------------------//
	void Finalize ( RTTIBase& self ) const {
		this->ZLAbstractFinalizationVisitor_Finalize ( self );
	}
};

//================================================================//
// ZLFinalizationVisitor
//================================================================//
template < typename TYPE >
class ZLFinalizationVisitor :
	public ZLAbstractFinalizationVisitor {
public:

	//----------------------------------------------------------------//
	void ZLAbstractFinalizationVisitor_Finalize ( RTTIBase& self ) const {
		self.AsType < TYPE >()->TYPE::_Finalize ();
	}
};

#endif
