// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LUASTATE_H
#define LUASTATE_H

#include <uslscore/STLString.h>
#include <uslscore/USRect.h>
#include <uslscore/USVec2D.h>
#include <uslscore/USVec3D.h>

class USCipher;
class USLuaRef;
class USStreamFormatter;

#define LUA_SETUP(type,str) \
	USLuaState state ( L );	\
	if ( !state.CheckParams ( 1, str )) return 0; \
	type* self = state.GetLuaObject < type >( 1 ); \
	if ( !self ) return 0;

#define LUA_SETUP_STATIC(str) \
	USLuaState state ( L );	\
	if ( !state.CheckParams ( 1, str )) return 0;

//================================================================//
// USLuaState
//================================================================//
class USLuaState {
private:

	lua_State*	mState;

	//----------------------------------------------------------------//
	bool			Decode					( int idx, USCipher& cipher );
	bool			Encode					( int idx, USCipher& cipher );
	bool			Transform				( int idx, USStreamFormatter& formatter );

public:

	friend class USLuaStateHandle;

	//----------------------------------------------------------------//
	int				AbsIndex				( int idx );
	bool			Base64Decode			( int idx );
	bool			Base64Encode			( int idx );
	bool			CheckParams				( int idx, cc8* format ); // "BCFLNSTU"
	void			CopyToTop				( int idx );
	int				DebugCall				( int nArgs, int nResults );
	bool			Deflate					( int idx, int level, int windowBits );
	void			GetField				( int idx, cc8* name );
	void			GetField				( int idx, int key );
	STLString		GetField				( int idx, cc8* key, cc8* value );
	STLString		GetField				( int idx, int key, cc8* value );
	bool			GetFieldWithType		( int idx, cc8* name, int type );
	bool			GetFieldWithType		( int idx, int key, int type );
	void*			GetPtrUserData			( int idx );
	USLuaRef		GetStrongRef			( int idx );
	int				GetTop					();
	void*			GetUserData				( int idx, void* value );
	void*			GetUserData				( int idx, cc8* name, void* value );
	STLString		GetValue				( int idx, cc8* value );
	USLuaRef		GetWeakRef				( int idx );
	bool			HasField				( int idx, cc8* name );
	bool			HasField				( int idx, int key );
	bool			HasField				( int idx, cc8* name, int type );
	bool			HasField				( int idx, int name, int type );
	bool			Inflate					( int idx, int windowBits );
	bool			IsNil					();
	bool			IsTableOrUserdata		( int idx );
	bool			IsType					( int idx, int type );
	bool			IsType					( int idx, cc8* name, int type );
	void			LoadLibs				();
	void			MoveToTop				( int idx );
	void			Pop						( int n );
	bool			PrepMemberFunc			( int idx, cc8* name );
	bool			PrintErrors				( int status );
	void			PrintStackTrace			( int level );
	void			Push					( bool value );
	void			Push					( cc8* value );
	void			Push					( double value );
	void			Push					( float value );
	void			Push					( int value );
	void			Push					( u16 value );
	void			Push					( u32 value );
	void			Push					( u64 value );
	void			Push					( lua_CFunction value );
	void			Push					( USLuaRef& ref );
	void			PushPtrUserData			( void* ptr );
	int				PushTableItr			( int idx );
	void			RegisterModule			( cc8* name, lua_CFunction loader, bool autoLoad );
	int				RelIndex				( int idx );
	void			SetPath					( cc8* path );
	void			SetTop					( int top );
	bool			TableItrNext			( int itr );
					USLuaState				();
					USLuaState				( lua_State* state );
	virtual			~USLuaState				();
	int				YieldThread				( int nResults );
	
	//----------------------------------------------------------------//
	inline lua_State* operator -> () const {
		return mState;
	};

	//----------------------------------------------------------------//
	inline lua_State& operator * () const {
		return *mState;
	};

	//----------------------------------------------------------------//
	inline operator lua_State* () {
		return mState;
	};

	//----------------------------------------------------------------//
	inline operator bool () {
		return ( this->mState != 0 );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE > TYPE						GetField			( int idx, int key, TYPE value );
	template < typename TYPE > TYPE						GetField			( int idx, cc8* key, TYPE value );
	template < typename TYPE > TYPE*					GetLuaObject		( int idx );
	template < typename TYPE > TYPE*					GetLuaObject		( int idx, cc8* name );
	template < typename TYPE > USMetaRect < TYPE >		GetRect				( int idx );
	template < typename TYPE > TYPE						GetValue			( int idx, TYPE value );
	template < typename TYPE > USMetaVec2D < TYPE >		GetVec2D			( int idx );
	template < typename TYPE > USMetaVec3D < TYPE >		GetVec3D			( int idx );
	template < typename TYPE > void						ReadArray			( int size, TYPE* values, TYPE value );
	template < typename TYPE > void						SetField			( int idx, cc8* key, TYPE value );
	template < typename TYPE > void						SetFieldByIndex		( int idx, int key, TYPE value );
	template < typename TYPE > void						WriteArray			( int size, TYPE* values );
};

//----------------------------------------------------------------//
template <> bool	USLuaState::GetValue < bool >	( int idx, bool value );
template <> cc8*	USLuaState::GetValue < cc8* >	( int idx, cc8* value );
template <> double	USLuaState::GetValue < double >	( int idx, double value );
template <> float	USLuaState::GetValue < float >	( int idx, float value );
template <> int		USLuaState::GetValue < int >	( int idx, int value );
template <> uintptr	USLuaState::GetValue < uintptr >( int idx, uintptr value );
template <> u8		USLuaState::GetValue < u8 >		( int idx, u8 value );
template <> u16		USLuaState::GetValue < u16 >	( int idx, u16 value );
template <> u32		USLuaState::GetValue < u32 >	( int idx, u32 value );
template <> u64		USLuaState::GetValue < u64 >	( int idx, u64 value );

#endif
