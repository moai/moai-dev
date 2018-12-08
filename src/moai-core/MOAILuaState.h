// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUASTATE_H
#define MOAILUASTATE_H

class MOAILuaMemberRef;
class MOAILuaObject;
class MOAILuaRef;

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

	typedef			u32			IndexType;
	typedef			s32			IntType;
	typedef			double		RealType;
	typedef			u32			SizeType;
	typedef			u32			UIntType;

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
	bool			DumpChunk					( int idx, ZLStream& stream );
	ZLBox			GetBox						( int idx );
	ZLColorVec		GetColor					( int idx, float r, float g, float b, float a );
	u32				GetColor32					( int idx, float r, float g, float b, float a );
	static cc8*		GetLuaTypeName				( int type );
	ZLMatrix4x4		GetMatrix					( int idx, size_t size );
	void*			GetPtrUserData				( int idx );
	ZLQuad			GetQuad						( int idx );
	STLString		GetStackDump				();
	int				GetStackSize				( int idx );
	STLString		GetStackTrace				( cc8* title, int level );
	int				GetLuaThreadStatus			( int idx );
	int				GetLuaThreadStatus			( lua_State* thread );
	cc8*			GetLuaThreadStatusName		( int status );
	size_t			GetTableSize				( int idx );
	int				GetTop						();
	void*			GetUserData					( int idx, void* value );
	void*			GetUserData					( int idx, cc8* name, void* value );
	STLString		GetValue					( int idx, cc8* value );
	ZLIndex			GetValueAsIndex				( int idx, ZLIndex value = ZLIndex::ZERO );
	bool			HasField					( int idx, cc8* name );
	bool			HasField					( int idx, int key );
	bool			HasFieldWithType			( int idx, cc8* name, int type );
	bool			HasFieldWithType			( int idx, int name, int type );
	bool			HasKeys						( int idx );
	bool			HexDecode					( int idx );
	bool			HexEncode					( int idx );
	bool			Inflate						( int idx, int windowBits );
	bool			IsNil						( int idx );
	bool			IsNilOrNone					( int idx );
	bool			IsTableOrUserdata			( int idx );
	bool			IsTrueOrNotNil				( int idx );
	bool			IsType						( int idx, int type );
	bool			IsType						( int idx, cc8* name, int type );
	bool			IsValid						();
	void			LoadLibs					();
	
	bool			LogErrors					( u32 level, FILE* file, int status );
	void			LogStackDump				( u32 level, FILE* file );
	void			LogStackTrace				( u32 level, FILE* file, cc8* title, int stackLevel );
	
					MOAILuaState				();
					MOAILuaState				( lua_State* state );
	virtual			~MOAILuaState				();
	void			MoveToTop					( int idx );
	void			Pop							( int n = 1 );
	bool			PrepMemberFunc				( int idx, cc8* name );
	
	void			Push						();
	void			Push						( bool value );
	void			Push						( cc8* value );
	void			Push						( double value );
	void			Push						( float value );
	void			Push						( int value );
	void			Push						( u16 value );
	void			Push						( u32 value );
	void			Push						( u64 value );
	void			Push						( ZLSize value );
	int				Push						( const ZLAffine2D& value );
	int				Push						( const ZLAffine3D& value );
	int				Push						( const ZLBox& value );
	int				Push						( const ZLColorVec& value );
	void			Push						( const ZLIndex& value );
	int				Push						( const ZLMatrix3x3& value );
	int				Push						( const ZLMatrix4x4& value );
	int				Push						( const ZLQuad& value );
	int				Push						( const ZLRect& value );
	int				Push						( const ZLVec2D& value );
	int				Push						( const ZLVec3D& value );
	int				Push						( const ZLVec4D& value );
	
	void			Push						( lua_CFunction value );
	void			Push						( MOAILuaObject* luaObject );
	void			Push						( MOAILuaRef& ref );
	void			Push						( MOAILuaMemberRef& ref );
	void			Push						( const void* value );
	void			Push						( void* data, size_t size );
	
	void			PushPtrUserData				( void* ptr );
	
	void			PushField					( int idx, cc8* name );
	void			PushField					( int idx, int key );
	bool			PushFieldWithType			( int idx, cc8* name, int type );
	bool			PushFieldWithType			( int idx, int key, int type );
	bool			PushSubfieldWithType			( int idx, cc8* format, int type, ... );
	
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
	inline operator bool () const {
		return ( this->mState != 0 );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE > TYPE						GetFieldValue		( int idx, int key, TYPE value );
	template < typename TYPE > TYPE						GetFieldValue		( int idx, cc8* key, TYPE value );
	template < typename TYPE > TYPE*					GetLuaObject		( int idx, bool verbose );
	template < typename TYPE > TYPE*					GetLuaObject		( int idx, cc8* name, bool verbose );
	template < typename TYPE > ZLMetaRect < TYPE >		GetRect				( int idx, TYPE value = 0 );
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
template <> ZLSize			MOAILuaState::GetValue < ZLSize >			( int idx, const ZLSize value );
template <> const void*		MOAILuaState::GetValue < const void* >		( int idx, const void* value );
template <> ZLAffine2D		MOAILuaState::GetValue < ZLAffine2D >		( int idx, const ZLAffine2D value );
template <> ZLAffine3D		MOAILuaState::GetValue < ZLAffine3D >		( int idx, const ZLAffine3D value );
template <> ZLBox			MOAILuaState::GetValue < ZLBox >			( int idx, const ZLBox value );
template <> ZLColorVec		MOAILuaState::GetValue < ZLColorVec >		( int idx, const ZLColorVec value );
//template <> ZLIndex			MOAILuaState::GetValue < ZLIndex >			( int idx, const ZLIndex value );
template <> ZLMatrix3x3		MOAILuaState::GetValue < ZLMatrix3x3 >		( int idx, const ZLMatrix3x3 value );
template <> ZLMatrix4x4		MOAILuaState::GetValue < ZLMatrix4x4 >		( int idx, const ZLMatrix4x4 value );
template <> ZLQuaternion	MOAILuaState::GetValue < ZLQuaternion >		( int idx, const ZLQuaternion value );
template <> ZLRect			MOAILuaState::GetValue < ZLRect >			( int idx, const ZLRect value );
template <> ZLVec2D			MOAILuaState::GetValue < ZLVec2D >			( int idx, const ZLVec2D value );
template <> ZLVec3D			MOAILuaState::GetValue < ZLVec3D >			( int idx, const ZLVec3D value );
template <> ZLVec4D			MOAILuaState::GetValue < ZLVec4D >			( int idx, const ZLVec4D value );

#endif
