// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDSLSTREAM_H
#define	MOAIABSTRACTDSLSTREAM_H

#include <moai-core/MOAILua.h>
#include <moai-core/MOAIAbstractDSLHandler.h>

class MOAIAbstractDSLInterface;
class MOAIAbstractDSLStream;

//================================================================//
// MOAIAbstractDSLStreamBinding
//================================================================//
template < typename API_TYPE >
class MOAIAbstractDSLStreamBinding :
	public API_TYPE {
public:

	MOAIAbstractDSLStream&	mStream;
	
	//----------------------------------------------------------------//
	void MOAIAbstractDSLHandler_RetainObject ( MOAILuaObject* object ) {
		this->mStream.RetainObject ( object );
	}
	
	//----------------------------------------------------------------//
	void MOAIAbstractDSLHandler_SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {
		this->mStream.SubmitCommand ( cmd, param, paramSize );
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractDSLStreamBinding ( MOAIAbstractDSLStream& stream ) :
		mStream ( stream ) {
	}
	
	//----------------------------------------------------------------//
	void MOAIAbstractDSLInterface_PushLuaAPI ( MOAILuaState& state ) {
		UNUSED ( state );
	}
};

//================================================================//
// MOAIAbstractDSLStream
//================================================================//
// TODO: doxygen
class MOAIAbstractDSLStream :
	public virtual MOAILuaObject,
	public virtual MOAIAbstractDSLHandler {
protected:

	friend class MOAIAbstractDSLInterface;

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLTypeID						mAPITypeID;
	ZLLeanArray < u8 >				mBuffer;
	MOAILuaStrongRef				mLuaInterface;
	MOAIAbstractDSLInterface*		mDSLInterface;
	
	//----------------------------------------------------------------//
	void				PushInterface							( MOAILuaState& state );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
						MOAIAbstractDSLStream					();
						~MOAIAbstractDSLStream					();
	void				SelectAPI								( MOAIAbstractDSLInterface* handler );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	API_TYPE& GetAPI () {
		
		assert ( this->mAPITypeID == ZLType::GetID < API_TYPE >());
		return *( MOAIAbstractDSLStreamBinding < API_TYPE >* )this->mBuffer.GetBuffer ();
	}
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	void InitializeBinding () {
		
		ZLTypeID apiTypeID = ZLType::GetID < API_TYPE >();
		if ( this->mAPITypeID == apiTypeID ) return;
		this->mAPITypeID = apiTypeID;
		
		this->mBuffer.Grow ( sizeof ( MOAIAbstractDSLStreamBinding < API_TYPE >));
		new ( this->mBuffer.GetBuffer ()) MOAIAbstractDSLStreamBinding < API_TYPE >( *this );
	}
};

#endif
