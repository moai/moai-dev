// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	LUADATA_H
#define	LUADATA_H

#include <uslscore/STLString.h>
#include <uslscore/USLuaRef.h>
#include <uslscore/USObject.h>

class USLuaDataType;
class USLuaSerializer;
class USLuaState;
class USLuaStateHandle;

//================================================================//
// USLuaData
//================================================================//
/**	@brief Base class for C++ classes implementing Lua glue.
	@ingroup LuaLib
*/
class USLuaData :
	public virtual USObject {
private:

	USLuaRef		mInstance;	// strong ref to instance table stack
	USLuaRef		mSelf;		// weak ref to handle userdata 


	//----------------------------------------------------------------//
	static int		_delete		( lua_State* L );
	//static int		_tostring	( lua_State* L );

public:

	friend class USLuaDataType;

	//----------------------------------------------------------------//
	void					DebugDump			();
	virtual STLString		ToString			();
	STLString				ToStringWithType	();
	virtual USLuaDataType*	GetLuaDataType		();
	USLuaStateHandle		GetSelf				();
	bool					IsBound				();
	void					LuaUnbind			( USLuaState& state );
	void					PushLuaClass		( USLuaState& state );
	void					PushLuaInstance		( USLuaState& state );
	virtual void			RegisterLuaClass	( USLuaState& state );
	virtual void			RegisterLuaFuncs	( USLuaState& state );
	virtual	void			SerializeIn			( USLuaState& state, USLuaSerializer& serializer );
	virtual	void			SerializeOut		( USLuaState& state, USLuaSerializer& serializer );
	void					SetLuaInstance		( USLuaState& state, int idx );
							USLuaData			();
	virtual					~USLuaData			();
};

//================================================================//
// USLuaDataType
//================================================================//
/**	@brief Base class for Lua glue factory classes.
*/
class USLuaDataType :
	public USObject {
protected:

	USLuaRef mClassTable;		// global factory class for type
	USLuaRef mInstanceTable;	// metatable shared by all instances of type

	//----------------------------------------------------------------//
	void				InitLuaData				( USLuaData& data, USLuaState& state );
	void				InitLuaInstance			( USLuaData* data, USLuaState& state, int idx );
	void				InitLuaSingleton		( USLuaData& data, USLuaState& state );
	virtual void		RegisterLuaClass		( USLuaState& state ) = 0;

public:

	friend class USLuaData;

	//----------------------------------------------------------------//
	virtual USLuaData*	GetSingleton			();
	bool				IsSingleton				();
	virtual void		Register				() = 0;
						USLuaDataType			();
	virtual				~USLuaDataType			();
};

#endif
