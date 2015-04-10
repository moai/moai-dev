// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUASTATE_H
#define MOAILUASTATE_H

class MOAILuaRef;
class MOAILuaObject;

//================================================================//
// MOAILuaState
//================================================================//
class MOAILuaState {
private:

	friend class MOAIScopedLuaState;

	lua_State*	mState;

	//----------------------------------------------------------------//
	bool			Decode					( int idx, ZLStreamAdapter& reader );
	bool			Encode					( int idx, ZLStreamAdapter& writer );
	void			ReportBadCast			( int idx, cc8* typeName );

public:

	enum {
		THREAD_UNKNOWN		= 0x00,
		THREAD_DEAD			= 0x01,
		THREAD_ERROR		= 0x02,
		THREAD_NORMAL		= 0x04,
		THREAD_RUNNING		= 0x08,
		THREAD_SUSPENDED	= 0x10,
	};
	
	static const int THREAD_VALID_MASK = THREAD_SUSPENDED | THREAD_NORMAL | THREAD_RUNNING;

	//----------------------------------------------------------------//
	int				AbsIndex					( int idx );
	bool			Base64Decode				( int idx );
	bool			Base64Encode				( int idx );
	bool			CheckParams					( int idx, cc8* format, bool verbose = true ); // "BCFLNSTU"
	bool			CheckVector					( int idx, u32 n, lua_Number match, lua_Number fallback );
	bool			CheckVector					( int idx, u32 n, lua_Number min, lua_Number max, lua_Number fallback );
	void			ClearField					( int idx, cc8* key );
	void			CloneTable					( int idx );
	void			CopyToTop					( int idx );
	int				DebugCall					( int nArgs, int nResults );
	bool			Deflate						( int idx, int level, int windowBits );
	ZLBox			GetBox						( int idx );
	ZLColorVec		GetColor					( int idx, float r, float g, float b, float a );
	u32				GetColor32					( int idx, float r, float g, float b, float a );
	void			GetField					( int idx, cc8* name );
	void			GetField					( int idx, int key );
	STLString		GetField					( int idx, cc8* key, cc8* value );
	STLString		GetField					( int idx, int key, cc8* value );
	STLString		GetField					( int idx, cc8* key, const STLString& value );
	STLString		GetField					( int idx, int key, const STLString& value );
	bool			GetFieldWithType			( int idx, cc8* name, int type );
	bool			GetFieldWithType			( int idx, int key, int type );
	bool			GetSubfieldWithType			( int idx, cc8* format, int type, ... );
	static cc8*		GetLuaTypeName				( int type );
	void*			GetPtrUserData				( int idx );
	STLString		GetStackDump				();
	STLString		GetStackTrace				( cc8* title, int level );
	int				GetLuaThreadStatus			( int idx );
	int				GetLuaThreadStatus			( lua_State* thread );
	cc8*			GetLuaThreadStatusName		( int status );
	size_t			GetTableSize				( int idx );
	int				GetTop						();
	void*			GetUserData					( int idx, void* value );
	void*			GetUserData					( int idx, cc8* name, void* value );
	STLString		GetValue					( int idx, cc8* value );
	bool			HasField					( int idx, cc8* name );
	bool			HasField					( int idx, int key );
	bool			HasField					( int idx, cc8* name, int type );
	bool			HasField					( int idx, int name, int type );
	bool			HasKeys						( int idx );
	bool			HexDecode					( int idx );
	bool			HexEncode					( int idx );
	bool			Inflate						( int idx, int windowBits );
	bool			IsNil						();
	bool			IsNil						( int idx );
	bool			IsTableOrUserdata			( int idx );
	bool			IsTrueOrNotNil				( int idx );
	bool			IsType						( int idx, int type );
	bool			IsType						( int idx, cc8* name, int type );
	void			LoadLibs					();
					MOAILuaState				();
					MOAILuaState				( lua_State* state );
	virtual			~MOAILuaState				();
	void			MoveToTop					( int idx );
	void			Pop							( int n = 1 );
	bool			PrepMemberFunc				( int idx, cc8* name );
	bool			PrintErrors					( FILE* file, int status );
	void			PrintStackDump				();
	void			PrintStackDump				( FILE* file );
	void			PrintStackTrace				( FILE* file, cc8* title, int level );
	
	void			Push						();
	void			Push						( bool value );
	void			Push						( cc8* value );
	void			Push						( double value );
	void			Push						( float value );
	void			Push						( int value );
	void			Push						( u16 value );
	void			Push						( u32 value );
	void			Push						( u64 value );
	
	void			Push						( const ZLBox& value );
	void			Push						( const ZLColorVec& value );
	void			Push						( const ZLRect& value );
	void			Push						( const ZLVec2D& value );
	void			Push						( const ZLVec3D& value );
	
	void			Push						( lua_CFunction value );
	void			Push						( MOAILuaObject* luaObject );
	void			Push						( MOAILuaRef& ref );
	void			Push						( const void* value );
	void			Push						( void* data, size_t size );
	
	void			PushPtrUserData				( void* ptr );
	int				PushTableItr				( int idx );
	void			RegisterModule				( int idx, cc8* name, bool autoLoad );
	void			RegisterModule				( lua_CFunction loader, cc8* name, bool autoload );
	void			RegisterModule				( void* data, size_t size, cc8* name, bool autoload );
	int				RelIndex					( int idx );
	int				Run							( void* data, size_t size, int nArgs, int nResults );
	void			SetPath						( cc8* path );
	void			SetTop						( int top );
	bool			TableItrNext				( int itr );
	int				YieldThread					( int nResults );
	
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
	template < typename TYPE > TYPE*					GetLuaObject		( int idx, bool verbose );
	template < typename TYPE > TYPE*					GetLuaObject		( int idx, cc8* name, bool verbose );
	template < typename TYPE > ZLMetaRect < TYPE >		GetRect				( int idx );
	template < typename TYPE > TYPE						GetValue			( int idx, TYPE value );
	template < typename TYPE > ZLMetaVec2D < TYPE >		GetVec2D			( int idx, TYPE value = 0 );
	template < typename TYPE > ZLMetaVec3D < TYPE >		GetVec3D			( int idx, TYPE value = 0 );
	template < typename TYPE > TYPE						PopValue			( TYPE value );
	template < typename TYPE > void						Push				( ZLMetaRect < TYPE >& rect );
	template < typename TYPE > void						ReadArray			( int size, TYPE* values, TYPE value );
	template < typename TYPE > void						SetField			( int idx, cc8* key, TYPE value );
	template < typename TYPE > void						SetFieldByIndex		( int idx, int key, TYPE value );
	template < typename TYPE > void						SetGlobal			( cc8* key, TYPE value );
	template < typename TYPE > void						WriteArray			( int size, TYPE* values );
};

//----------------------------------------------------------------//
template <> bool			MOAILuaState::GetValue < bool >				( int idx, const bool value );
template <> cc8*			MOAILuaState::GetValue < cc8* >				( int idx, const cc8* value );
template <> double			MOAILuaState::GetValue < double >			( int idx, const double value );
template <> float			MOAILuaState::GetValue < float >			( int idx, const float value );
template <> s8				MOAILuaState::GetValue < s8 >				( int idx, const s8 value );
template <> s16				MOAILuaState::GetValue < s16 >				( int idx, const s16 value );
template <> s32				MOAILuaState::GetValue < s32 >				( int idx, const s32 value );
template <> s64				MOAILuaState::GetValue < s64 >				( int idx, const s64 value );
template <> u8				MOAILuaState::GetValue < u8 >				( int idx, const u8 value );
template <> u16				MOAILuaState::GetValue < u16 >				( int idx, const u16 value );
template <> u32				MOAILuaState::GetValue < u32 >				( int idx, const u32 value );
template <> u64				MOAILuaState::GetValue < u64 >				( int idx, const u64 value );
template <> const void*		MOAILuaState::GetValue < const void* >		( int idx, const void* value );
template <> ZLBox			MOAILuaState::GetValue < ZLBox >			( int idx, const ZLBox value );
template <> ZLColorVec		MOAILuaState::GetValue < ZLColorVec >		( int idx, const ZLColorVec value );
template <> ZLRect			MOAILuaState::GetValue < ZLRect >			( int idx, const ZLRect value );
template <> ZLVec2D			MOAILuaState::GetValue < ZLVec2D >			( int idx, const ZLVec2D value );
template <> ZLVec3D			MOAILuaState::GetValue < ZLVec3D >			( int idx, const ZLVec3D value );

#endif
