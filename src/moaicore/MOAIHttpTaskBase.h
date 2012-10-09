// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHTTPTASKBASE_H
#define MOAIHTTPTASKBASE_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAIStream.h>

#define DEFAULT_MOAI_HTTP_TIMEOUT	15
#define DEFAULT_MOAI_HTTP_USERAGENT "Moai SDK beta; support@getmoai.com"

//================================================================//
// MOAIHttpTaskBase
//================================================================//
/**	@name	MOAIHttpTaskBase
	@text	Object for performing asynchronous HTTP/HTTPS tasks.
	
	@const	HTTP_GET
	@const	HTTP_HEAD
	@const	HTTP_POST
	@const	HTTP_PUT
	@const	HTTP_DELETE
*/
class MOAIHttpTaskBase :
	public virtual MOAILuaObject {
protected:

	typedef STLMap < STLString, STLString >::iterator HeaderMapIt;
	typedef STLMap<STLString, STLString> HeaderMap;

	HeaderMap			mHeaderMap;

	bool				mBusy;
	USLeanArray < u8 >	mData;
	u32					mFollowRedirects;
	float				mProgress;
	u32					mResponseCode; // set by the implementation on task completion
	HeaderMap			mResponseHeaders;
	u32					mTimeout;
		
	MOAILuaSharedPtr < MOAIStream >	mUserStream;

	MOAILuaLocal		mOnFinish;

	//----------------------------------------------------------------//
	static int		_getProgress		( lua_State* L );
	static int		_getResponseCode	( lua_State* L );
	static int		_getResponseHeader	( lua_State* L );
	static int		_getSize			( lua_State* L );
	static int		_getString			( lua_State* L );
	static int		_httpGet			( lua_State* L );
	static int		_httpPost			( lua_State* L );
	static int		_isBusy				( lua_State* L );
	static int		_parseXml			( lua_State* L );
	static int		_performAsync		( lua_State* L );
	static int		_performSync		( lua_State* L );
	static int		_setBody			( lua_State* L );
	static int		_setCallback		( lua_State* L );
	static int		_setCookieSrc		( lua_State* L );
	static int		_setCookieDst		( lua_State* L );
	static int		_setFailOnError		( lua_State* L );
	static int		_setFollowRedirects	( lua_State* L );
	static int		_setHeader			( lua_State* L );
	static int		_setStream			( lua_State* L );
	static int		_setTimeout			( lua_State* L );
	static int		_setUrl				( lua_State* L );
	static int		_setUserAgent		( lua_State* L );
	static int		_setVerb			( lua_State* L );
	static int		_setVerbose			( lua_State* L );

	//----------------------------------------------------------------//
	void			Finish				();
					MOAIHttpTaskBase	( const MOAIHttpTaskBase& task );

public:
	
	GET ( u32, ResponseCode, mResponseCode )

	enum {
		HTTP_GET,
		HTTP_HEAD,
		HTTP_POST,
		HTTP_PUT,
		HTTP_DELETE,
	};
	
	//----------------------------------------------------------------//
	virtual void		GetData					( void* buffer, u32 size );
	void				HttpGet					( cc8* url, cc8* useragent, bool verbose, bool blocking );
	void				HttpPost				( cc8* url, cc8* useragent, const void* buffer, u32 size, bool verbose, bool blocking );
	void				InitForGet				( cc8* url, cc8* useragent, bool verbose );
	void				InitForPost				( cc8* url, cc8* useragent, const void* buffer, u32 size, bool verbose );
						MOAIHttpTaskBase		();
						~MOAIHttpTaskBase		();
	virtual void		PerformAsync			() = 0;
	virtual void		PerformSync				() = 0;
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	virtual void		Reset					() = 0;
	virtual void		SetBody					( const void* buffer, u32 size ) = 0;
	virtual void		SetCookieDst			( const char *file ) = 0;
	virtual void		SetCookieSrc			( const char *file ) = 0;
	virtual void		SetFailOnError			( bool enable ) = 0;
	void				SetFollowRedirects		( u32 value );
	void				SetHeader				( cc8* key, cc8* value );
	virtual void		SetUrl					( cc8* url ) = 0;
	virtual void		SetUserAgent			( cc8* useragent ) = 0;
	virtual void		SetVerb					( u32 verb ) = 0;
	virtual void		SetVerbose				( bool verbose ) = 0;
};

#endif
