// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUASTATE_IMPL_H
#define MOAILUASTATE_IMPL_H

//================================================================//
// MOAILuaState
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
TYPE MOAILuaState::GetField ( int idx, int key, TYPE value ) {

	this->GetField ( idx, key );
	value = this->GetValue < TYPE >( -1, value );
	this->Pop ( 1 );
	
	return value;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE MOAILuaState::GetField ( int idx, cc8* key, TYPE value ) {

	this->GetField ( idx, key );
	value = this->GetValue < TYPE >( -1, value );
	this->Pop ( 1 );
	
	return value;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* MOAILuaState::GetLuaObject ( int idx, bool verbose ) {
	
	if ( this->GetTop () < idx ) return 0;
	if ( !this->IsType ( idx, LUA_TUSERDATA )) return 0;
	
	MOAILuaObject* luaData = ( MOAILuaObject* )this->GetPtrUserData ( idx );
	if ( luaData ) {
		TYPE* type = luaData->AsType < TYPE >();
		if (( type == 0 ) && verbose ) {
			this->ReportBadCast ( this->AbsIndex ( idx ), luaData->TypeName ());
		}
		return type;
	}
	return 0;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* MOAILuaState::GetLuaObject ( int idx, cc8* name, bool verbose ) {
	
	if ( this->GetField ( idx, name, LUA_TUSERDATA )) {
		
		MOAILuaObject* luaData = ( MOAILuaObject* )this->GetPtrUserData ( idx );
		this->Pop ( 1 );
		
		if ( luaData ) {
			TYPE* type = luaData->AsType < TYPE >();
			if (( type == 0 ) && verbose ) {
				this->ReportBadCast ( this->AbsIndex ( idx ), luaData->TypeName ());
			}
			return type;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
//template < typename TYPE >
//STLSet < TYPE* > MOAILuaState::GetLuaDataSet ( int idx ) {
//	
//	idx = this->AbsIndex ( idx );
//	
//	STLSet < TYPE* > luaDataSet;
//	
//	this->PushTableItr ( idx );
//	while ( this->TableItrNext ( idx )) {
//		
//		TYPE* type = this->GetLuaObject < TYPE >( -1 );
//		if ( type ) {
//			luaDataSet.Affirm ( type );
//		}
//	}
//	
//	return luaDataSet;
//}

//----------------------------------------------------------------//
template < typename TYPE >
USMetaRect < TYPE > MOAILuaState::GetRect ( int idx ) {

	USMetaRect < TYPE > rect;
	
	rect.mXMin = this->GetValue < TYPE >( idx++, 0 );
	rect.mYMin = this->GetValue < TYPE >( idx++, 0 );
	rect.mXMax = this->GetValue < TYPE >( idx++, 0 );
	rect.mYMax = this->GetValue < TYPE >( idx, 0 );
	
	return rect;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE MOAILuaState::GetValue ( int idx, TYPE value ) {
	UNUSED ( idx );

	assert ( false ); // unsupported type
	return value;
}

//----------------------------------------------------------------//
template < typename TYPE >
USMetaVec2D < TYPE > MOAILuaState::GetVec2D ( int idx ) {

	USMetaVec2D < TYPE > vec;
	
	vec.mX = this->GetValue < TYPE >( idx++, 0 );
	vec.mY = this->GetValue < TYPE >( idx, 0 );
	
	return vec;
}

//----------------------------------------------------------------//
template < typename TYPE >
USMetaVec3D < TYPE > MOAILuaState::GetVec3D ( int idx ) {

	USMetaVec3D < TYPE > vec;
	
	vec.mX = this->GetValue < TYPE >( idx++, 0 );
	vec.mY = this->GetValue < TYPE >( idx++, 0 );
	vec.mZ = this->GetValue < TYPE >( idx, 0 );
	
	return vec;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE MOAILuaState::PopValue ( TYPE value ) {

	value = this->GetValue < TYPE >( -1, value );
	this->Pop ( 1 );
	return value;
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaState::Push ( USMetaRect < TYPE >& rect ) {

	this->Push ( rect.mXMin );
	this->Push ( rect.mYMin );
	this->Push ( rect.mXMax );
	this->Push ( rect.mYMax );
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaState::ReadArray ( int size, TYPE* values, TYPE value ) {

	for ( int i = 0; i < size; ++i ) {
		values [ i ] = this->GetField ( -1, i + 1, value );
	}
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaState::SetField ( int idx, cc8* key, TYPE value ) {
	
	if ( this->IsTableOrUserdata ( idx )) {
		idx = this->AbsIndex ( idx );
		this->Push ( value );
		lua_setfield ( this->mState, idx, key );
	}
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaState::SetFieldByIndex ( int idx, int key, TYPE value ) {
	
	if ( this->IsTableOrUserdata ( idx )) {
		idx = this->AbsIndex ( idx );
		this->Push ( key );
		this->Push ( value );
		lua_settable( this->mState, idx );
	}
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaState::WriteArray ( int size, TYPE* values ) {

	for ( int i = 0; i < size; ++i ) {
		this->SetFieldByIndex ( -1, i + 1, values [ i ]);
	}
}

#endif
