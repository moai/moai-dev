// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LUASTATE_IMPL_H
#define LUASTATE_IMPL_H

//================================================================//
// USLuaState
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
TYPE USLuaState::GetField ( int idx, int key, TYPE value ) {

	this->GetField ( idx, key );
	value = this->GetValue ( -1, value );
	this->Pop ( 1 );
	
	return value;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE USLuaState::GetField ( int idx, cc8* key, TYPE value ) {

	this->GetField ( idx, key );
	value = this->GetValue ( -1, value );
	this->Pop ( 1 );
	
	return value;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* USLuaState::GetLuaObject ( int idx ) {
	
	if ( this->GetTop () < idx ) return 0;
	if ( !this->IsType ( idx, LUA_TUSERDATA )) return 0;
	
	USLuaObject* luaData = ( USLuaObject* )this->GetPtrUserData ( idx );
	if ( luaData ) {
		return luaData->AsType < TYPE >();
	}
	return 0;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* USLuaState::GetLuaObject ( int idx, cc8* name ) {
	
	if ( this->GetField ( idx, name, LUA_TUSERDATA )) {
		
		USLuaObject* luaData = ( USLuaObject* )this->GetPtrUserData ( idx );
		this->Pop ( 1 );
		
		if ( luaData ) {
			return luaData->AsType < TYPE >();
		}
	}
	return 0;
}

//----------------------------------------------------------------//
//template < typename TYPE >
//STLSet < TYPE* > USLuaState::GetLuaDataSet ( int idx ) {
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
USMetaRect < TYPE > USLuaState::GetRect ( int idx ) {

	USMetaRect < TYPE > rect;
	
	rect.mXMin = this->GetValue < TYPE >( idx++, 0 );
	rect.mYMin = this->GetValue < TYPE >( idx++, 0 );
	rect.mXMax = this->GetValue < TYPE >( idx++, 0 );
	rect.mYMax = this->GetValue < TYPE >( idx, 0 );
	
	return rect;
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE USLuaState::GetValue ( int idx, TYPE value ) {

	assert ( false ); // unsupported type
	return value;
}

//----------------------------------------------------------------//
template < typename TYPE >
USMetaVec2D < TYPE > USLuaState::GetVec2D ( int idx ) {

	USMetaVec2D < TYPE > vec;
	
	vec.mX = this->GetValue < TYPE >( idx++, 0 );
	vec.mY = this->GetValue < TYPE >( idx, 0 );
	
	return vec;
}

//----------------------------------------------------------------//
template < typename TYPE >
USMetaVec3D < TYPE > USLuaState::GetVec3D ( int idx ) {

	USMetaVec3D < TYPE > vec;
	
	vec.mX = this->GetValue < TYPE >( idx++, 0 );
	vec.mY = this->GetValue < TYPE >( idx++, 0 );
	vec.mZ = this->GetValue < TYPE >( idx, 0 );
	
	return vec;
}

//----------------------------------------------------------------//
template < typename TYPE >
void USLuaState::ReadArray ( int size, TYPE* values, TYPE value ) {

	for ( int i = 0; i < size; ++i ) {
		values [ i ] = this->GetField ( -1, i + 1, value );
	}
}

//----------------------------------------------------------------//
template < typename TYPE >
void USLuaState::SetField ( int idx, cc8* key, TYPE value ) {
	
	if ( this->IsTableOrUserdata ( idx )) {
		idx = this->AbsIndex ( idx );
		this->Push ( value );
		lua_setfield ( this->mState, idx, key );
	}
}

//----------------------------------------------------------------//
template < typename TYPE >
void USLuaState::SetFieldByIndex ( int idx, int key, TYPE value ) {
	
	if ( this->IsTableOrUserdata ( idx )) {
		idx = this->AbsIndex ( idx );
		this->Push ( key );
		this->Push ( value );
		lua_settable( this->mState, idx );
	}
}

//----------------------------------------------------------------//
template < typename TYPE >
void USLuaState::WriteArray ( int size, TYPE* values ) {

	for ( int i = 0; i < size; ++i ) {
		this->SetFieldByIndex ( -1, i + 1, values [ i ]);
	}
}

#endif
