// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTFINALIZABLE_H
#define	ZLABSTRACTFINALIZABLE_H

#include <zl-util/ZLRtti.h>
#include <zl-util/ZLFinalizationVisitor.h>

class ZLAbstractFinalizableImpl;

//================================================================//
// ZLAbstractFinalizable
//================================================================//
class ZLAbstractFinalizable :
	public virtual RTTIBase {
private:

	friend class ZLAbstractFinalizableImpl;
	template < typename DEPENDENCY_TYPE > friend class ZLFinalizable_DependsOn;

	ZLAbstractFinalizableImpl*		mImpl; // pimpl/impl

	//----------------------------------------------------------------//
	void 						FinalizeDependencies 				();

protected:

	//----------------------------------------------------------------//
	template < typename TYPE >
	void RegisterFinalizationVisitor () {
		this->RegisterVisitor < ZLAbstractFinalizationVisitor, ZLFinalizationVisitor < TYPE > >();
	}

	//----------------------------------------------------------------//
	void 							Destruct				();
	ZLAbstractFinalizableImpl&		AffirmImpl				();
	ZLAbstractFinalizableImpl*		GetImpl					();

public:

	//----------------------------------------------------------------//
	void 						Finalize							();
								ZLAbstractFinalizable				();
	virtual 					~ZLAbstractFinalizable				();
};

//================================================================//
// ZLAbstractFinalizableImpl
//================================================================//
class ZLAbstractFinalizableImpl {
private:

	friend class ZLAbstractFinalizable;
	template < typename DEPENDENCY_TYPE > friend class ZLFinalizable_DependsOn;

	ZLAbstractFinalizable*				mOwner;
	STLSet < ZLAbstractFinalizable* >	mDownstream;
	STLSet < ZLAbstractFinalizable* >	mUpstream;

	//----------------------------------------------------------------//
	void						AddDownstream						( ZLAbstractFinalizable& downstream );
	void						AddUpstream							( ZLAbstractFinalizable& dependency );
	ZLAbstractFinalizable&		GetOwner							();
	void						RemoveDownstream					( ZLAbstractFinalizable& downstream );
	void						RemoveUpstream						( ZLAbstractFinalizable& dependency );
								ZLAbstractFinalizableImpl			();
								ZLAbstractFinalizableImpl			( ZLAbstractFinalizable& owner );
								~ZLAbstractFinalizableImpl			();
};

#endif
