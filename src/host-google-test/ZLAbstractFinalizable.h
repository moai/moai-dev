// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTFINALIZABLE_H
#define	ZLABSTRACTFINALIZABLE_H

#include <moai-sim/headers.h>

#define IMPLEMENT_FINALIZEABLE(TYPE,...)					\
	void ZLAbstractFinalizable_Finalize ( void* self ) {	\
		(( TYPE* )self )->~TYPE ();							\
		new ( this ) TYPE ( __VA_ARGS__ );					\
	}														\
															\
	void* ZLAbstractFinalizable_GetSelf () {				\
		( void )static_cast < TYPE* >( this );				\
		return this;										\
	}

//================================================================//
// ZLAbstractFinalizable
//================================================================//
class ZLAbstractFinalizable {
protected:

	enum ReconstructionFlag {
		RECONSTRUCTING,
	};

	//----------------------------------------------------------------//
	virtual void	ZLAbstractFinalizable_Finalize		( void* self ) = 0;
	virtual void*	ZLAbstractFinalizable_GetSelf		() = 0;

public:
	
	//----------------------------------------------------------------//
					ZLAbstractFinalizable			();
	virtual			~ZLAbstractFinalizable			();
	void			Finalize						();
};

#endif
