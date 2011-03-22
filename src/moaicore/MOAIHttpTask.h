// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHTTPTASK_H
#define	MOAIHTTPTASK_H

class USHttpTask;
class MOAIData;

//================================================================//
// MOAIHttpTask
//================================================================//
/**	@brief HTTP task class.
*/
class MOAIHttpTask :
	public virtual USLuaData {
private:

	void*				mBuffer;
	u32					mSize;

	USLuaRef			mOnFinish;
	
	USRef < MOAIData >	mPostData;
	STLString			mPostString;

	//----------------------------------------------------------------//
	static int		_getData		( lua_State* L );
	static int		_getSize		( lua_State* L );
	static int		_getText		( lua_State* L );
	static int		_httpGet		( lua_State* L );
	static int		_httpPost		( lua_State* L );
	static int		_parseXml		( lua_State* L );
	static int		_setCallback	( lua_State* L );
	
	//----------------------------------------------------------------//
	void			OnHttpFinish	( USHttpTask* fetcher );
	
public:
	
	GET ( void*, Buffer, mBuffer )
	GET ( u32, Size, mSize )
	
	DECL_LUA_DATA ( MOAIHttpTask )
	
	//----------------------------------------------------------------//
	void			Clear					();						
					MOAIHttpTask			();
					~MOAIHttpTask			();
	void			Init					( u32 size );
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
