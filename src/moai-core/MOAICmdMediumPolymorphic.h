// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICMDMEDIUMPOLYMORPHIC_H
#define	MOAICMDMEDIUMPOLYMORPHIC_H

#include <moai-core/MOAICmdMediumAdapter.h>
#include <moai-core/MOAICmdMediumWithAPI.h>

//================================================================//
// MOAICmdMediumPolymorphic
//================================================================//
class MOAICmdMediumPolymorphic :
	public MOAICmdMediumAdapter {
protected:
	
	friend class MOAIAbstractCmdMedium;
	friend class MOAIAbstractCmdHandler;
	friend class MOAICmdMedium;
	
	ZLLeanArray < u8 >				mBuffer;
	
	//----------------------------------------------------------------//
	MOAICmdMediumPolymorphic*		MOAICmdMediumAdapter_GetPolymorphic			();
	
	//----------------------------------------------------------------//
				MOAICmdMediumPolymorphic		();
				MOAICmdMediumPolymorphic		( MOAIAbstractCmdStream& stream );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	void Morph ( MOAIAbstractCmdHandler& handler ) {
				
		typedef MOAICmdMediumWithAPI < API_TYPE > MorphWithIterface;
		
		ZLTypeID apiTypeID = ZLType::GetID < API_TYPE >();
		if (( this->mAPITypeID != apiTypeID ) || ( this->mHandler != &handler )) {
				
			this->mHandler = &handler;
			this->mAPITypeID = apiTypeID;
			this->mBuffer.Grow ( sizeof ( MorphWithIterface ));
			
			MorphWithIterface* api = new ( this->mBuffer.GetBuffer ()) MorphWithIterface ( *this );
			this->mAPI = api;
		}
	}
};

//================================================================//
// MOAIAbstractCmdHandler <>
//================================================================//

//----------------------------------------------------------------//
template < typename API_TYPE >
void MOAIAbstractCmdHandler::InitializePolymorphic ( MOAICmdMediumPolymorphic& polymorphic ) {
	polymorphic.Morph < API_TYPE >( *this );
}

#endif
