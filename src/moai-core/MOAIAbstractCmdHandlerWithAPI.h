// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTCMDHANDLERWITHAPI_H
#define	MOAIABSTRACTCMDHANDLERWITHAPI_H

#include <moai-core/MOAIAbstractCmdAPI.h>
#include <moai-core/MOAIAbstractCmdHandler.h>
#include <moai-core/MOAIAbstractCmdStream.h>
#include <moai-core/MOAICmdMedium.h>

//================================================================//
// MOAIAbstractCmdHandlerWithAPI
//================================================================//
template < typename API_TYPE >
class MOAIAbstractCmdHandlerWithAPI :
	public virtual MOAICmdMediumWithAPI < API_TYPE >,
	public virtual MOAIAbstractCmdHandler,
	public virtual MOAICmdMediumAdapter,
	public virtual MOAIAbstractCmdStream {
protected:

	//----------------------------------------------------------------//
	void MOAIAbstractCmdHandler_InitializePolymorphic ( MOAICmdMediumPolymorphic& polymorphic ) {
	
		this->template InitializePolymorphic < API_TYPE >( polymorphic );
	}

	//----------------------------------------------------------------//
	void* MOAIAbstractCmdMedium_GetMediumWithAPI ( ZLTypeID apiTypeID ) {

		assert ( this->mAPITypeID == apiTypeID );
		return ( MOAICmdMediumWithAPI < API_TYPE >* )this;
	}

	//----------------------------------------------------------------//
	void MOAIAbstractCmdMedium_RetainObject ( MOAILuaObject* object ) {
		UNUSED ( object );
	}
	
	//----------------------------------------------------------------//
	void MOAIAbstractCmdMedium_SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {
		UNUSED ( paramSize );
		this->HandleCommand ( cmd, param );
	}

	//----------------------------------------------------------------//
	void MOAIAbstractCmdStream_RetainObject ( MOAILuaObject* object ) {
		UNUSED ( object );
	}
	
	//----------------------------------------------------------------//
	void MOAIAbstractCmdStream_SubmitCommand ( MOAIAbstractCmdHandler& handler, u32 cmd, const void* param, ZLSize paramSize ) {
		UNUSED ( handler );
		UNUSED ( paramSize );
		this->HandleCommand ( cmd, param );
	}

public:

	//----------------------------------------------------------------//
	MOAIAbstractCmdHandlerWithAPI ( ZLContext& context ) :
		ZLHasContext ( context ),
		MOAILuaObject ( context ),
		MOAIAbstractCmdHandler ( context ) {

		this->mStream 		= this;
		this->mAPITypeID 	= ZLType::GetID < API_TYPE >();
		this->mHandler 		= this;
		this->mAPI 			= this;
		
		this->mAdapter		= this;
	}
	
	//----------------------------------------------------------------//
	~MOAIAbstractCmdHandlerWithAPI () {
	}
};

#endif
