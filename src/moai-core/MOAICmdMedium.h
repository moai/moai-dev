// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICMDMEDIUM_H
#define	MOAICMDMEDIUM_H

#include <moai-core/MOAIAbstractCmdHandler.h>
#include <moai-core/MOAILua.h>

class MOAIAbstractCmdAPI;
class MOAIAbstractCmdHandler;
class MOAIAbstractCmdMedium;
class MOAIAbstractCmdStream;
class MOAICmdMedium;
template < typename TYPE > class MOAIConcreteCmdMedium;

#define MOAI_LUA_CMD_SETUP(type)															\
	MOAILuaState state ( L );																\
	MOAIConcreteCmdMedium < type >* self = MOAICmdMedium::LuaSetup < type >( state );		\
	if ( !self ) return 0;

//================================================================//
// MOAIAbstractCmdMediumInternal
//================================================================//
class MOAIAbstractCmdMediumInternal {
protected:

	friend class MOAIAbstractCmdMedium;
	friend class MOAICmdMedium;

	MOAILuaStrongRef				mLuaInterface;
	
	MOAIAbstractCmdStream&			mStream;
	ZLLeanArray < u8 >				mBuffer;
	ZLTypeID						mAPITypeID;
	MOAIAbstractCmdHandler*			mHandler;
	MOAIAbstractCmdAPI*				mAPI;
	
	//----------------------------------------------------------------//
	MOAIAbstractCmdMediumInternal ( MOAIAbstractCmdStream& stream ) :
		mStream ( stream ),
		mHandler ( NULL ),
		mAPI ( NULL ) {
	}
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	void Morph ( MOAIAbstractCmdHandler& handler ) {
				
		typedef MOAIConcreteCmdMedium < API_TYPE > MorphWithIterface;
		
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
// MOAIAbstractCmdMedium
//================================================================//
class MOAIAbstractCmdMedium {
protected:

	friend class MOAIAbstractCmdHandler;

	MOAIAbstractCmdMediumInternal*	mInternal;

	//----------------------------------------------------------------//
	template < typename API_TYPE >
	void Morph ( MOAIAbstractCmdHandler& handler ) {
		
		assert ( this->mInternal );
		this->mInternal->Morph < API_TYPE >( handler );
	}

	//----------------------------------------------------------------//
						MOAIAbstractCmdMedium					();
	void				RetainObject							( MOAILuaObject* object );
	void				SubmitCommand							( u32 cmd, const void* param = NULL, ZLSize paramSize = 0 );

	//----------------------------------------------------------------//
	virtual void*		MOAIAbstractCmdMedium_GetConcrete		( ZLTypeID apiTypeID ) = 0;
	virtual void		MOAIAbstractCmdMedium_RetainObject		( MOAILuaObject* object );
	virtual void		MOAIAbstractCmdMedium_SubmitCommand		( u32 cmd, const void* param, ZLSize paramSize );

public:

	//----------------------------------------------------------------//
						MOAIAbstractCmdMedium					( MOAIAbstractCmdMediumInternal& internal );
	virtual				~MOAIAbstractCmdMedium					();
	void				SetHandler								( MOAIAbstractCmdHandler& handler );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	MOAIConcreteCmdMedium < API_TYPE >* API () {
		
//		if ( !this->mInternal ) {
//			return ( MOAIConcreteCmdMedium < API_TYPE >* )this->MOAIAbstractCmdMedium_GetConcrete ( ZLType::GetID < API_TYPE >());
//		}
//		return ( this->mInternal->mAPITypeID == ZLType::GetID < API_TYPE >()) ? ( MOAIConcreteCmdMedium < API_TYPE >* )this->mInternal->mBuffer.GetBuffer () : NULL;
		return ( MOAIConcreteCmdMedium < API_TYPE >* )this->MOAIAbstractCmdMedium_GetConcrete ( ZLType::GetID < API_TYPE >());
	}

	//----------------------------------------------------------------//
	template < typename API_TYPE >
	MOAIConcreteCmdMedium < API_TYPE >& _ ( MOAIAbstractCmdHandler& handler ) {

		this->SetHandler ( handler );
		return *this->API < API_TYPE >();
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SubmitCommand ( u32 cmd, const TYPE& param ) {
		this->SubmitCommand ( cmd, &param, sizeof ( TYPE ));
	}
};

//================================================================//
// MOAIConcreteCmdMedium
//================================================================//
template < typename API_TYPE >
class MOAIConcreteCmdMedium :
	public virtual API_TYPE,
	public virtual MOAIAbstractCmdMedium {
protected:

	//----------------------------------------------------------------//
	void* MOAIAbstractCmdMedium_GetConcrete ( ZLTypeID apiTypeID ) {
		return ( apiTypeID == ZLType::GetID < API_TYPE >()) ? this : NULL;
	}

	//----------------------------------------------------------------//
	MOAIConcreteCmdMedium () :
		MOAIAbstractCmdMedium () {
	}

public:

	//----------------------------------------------------------------//
	MOAIConcreteCmdMedium ( MOAIAbstractCmdMediumInternal& internal ) :
		MOAIAbstractCmdMedium ( internal ) {
	}
	
	//----------------------------------------------------------------//
	~MOAIConcreteCmdMedium () {
	}
};

//================================================================//
// MOAICmdMedium
//================================================================//
class MOAICmdMedium :
	public ZLRefCountedObject,
	public MOAIAbstractCmdMediumInternal,
	public MOAIAbstractCmdMedium {
protected:

	friend class MOAIAbstractCmdStream;

	//----------------------------------------------------------------//
	static int			_getInterface					( lua_State* L );
	
	//----------------------------------------------------------------//
	void				PushLuaAPI						( MOAILuaState& state );

	//----------------------------------------------------------------//
	void*				MOAIAbstractCmdMedium_GetConcrete		( ZLTypeID apiTypeID );
	virtual void		MOAICmdMedium_ExtendLuaAPI				( MOAILuaState& state );

public:

	//----------------------------------------------------------------//
						MOAICmdMedium					( MOAIAbstractCmdStream& stream );
						~MOAICmdMedium					();
	void				PushInterface					( MOAILuaState& state );
	int					PushWithHandler					( MOAILuaState& state, int idx );
	void				PushWithHandler					( MOAILuaState& state, MOAIAbstractCmdHandler& handler );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	static MOAIConcreteCmdMedium < API_TYPE >* LuaSetup ( MOAILuaState& state ) {
	
		if ( state.IsType ( 1, LUA_TUSERDATA )) {
			MOAICmdMedium* self = ( MOAICmdMedium* )state.GetPtrUserData ( 1 );
			return self->API < API_TYPE >();
		}
		return NULL;
	}
};

//================================================================//
// MOAIAbstractCmdHandler
//================================================================//

//----------------------------------------------------------------//
template < typename API_TYPE >
void MOAIAbstractCmdHandler::InitializeMedium ( MOAIAbstractCmdMedium& medium ) {
	medium.Morph < API_TYPE >( *this );
}

#endif
