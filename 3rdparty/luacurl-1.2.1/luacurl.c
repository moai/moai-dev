/* luacurl.c
 *
 * author      : Alexander Marinov (alekmarinov@gmail.com)
 * project     : luacurl
 * description : Binds libCURL to Lua
 * copyright   : The same as Lua license (http://www.lua.org/license.html) and 
 *               curl license (http://curl.haxx.se/docs/copyright.html)
 * todo        : multipart formpost building,
 *               curl multi
 *
 * Contributors: Thomas Harning added support for tables/threads as the CURLOPT_*DATA items.
 *******************************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>

#ifndef LIBCURL_VERSION
  #include <curl/curlver.h>
#endif

#include <lauxlib.h>

#if !defined(LUA_VERSION_NUM) || (LUA_VERSION_NUM <= 500)
#define luaL_checkstring luaL_check_string 
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LUACURL_API
#define LUACURL_API	extern
#endif

#define LUACURL_LIBNAME	"curl"
#define CURLHANDLE  "curlT"

#define MAKE_VERSION_NUM(x,y,z) (z + (y << 8) + (x << 16))
#define CURL_NEWER(x,y,z) (MAKE_VERSION_NUM(x,y,z) <= LIBCURL_VERSION_NUM)
#define CURL_OLDER(x,y,z) (MAKE_VERSION_NUM(x,y,z) > LIBCURL_VERSION_NUM)

/* Fast set table macro */
#define LUA_SET_TABLE(context, key_type, key, value_type, value) \
	lua_push##key_type(context, key); \
	lua_push##value_type(context, value); \
	lua_settable(context, -3);

/* wrap curl option with simple option type */
#define C_OPT(n, t)
/* wrap curl option with string list type */
#define C_OPT_SL(n) static const char* KEY_##n = #n;
/* wrap the other curl options not included above */
#define C_OPT_SPECIAL(n)

/* describes all currently supported curl options available to curl 7.15.2 */
#define ALL_CURL_OPT \
	C_OPT_SPECIAL(WRITEDATA) \
	C_OPT(URL, string) \
	C_OPT(PORT, number) \
	C_OPT(PROXY, string) \
	C_OPT(USERPWD, string) \
	C_OPT(PROXYUSERPWD, string) \
	C_OPT(RANGE, string) \
	C_OPT_SPECIAL(READDATA) \
	C_OPT_SPECIAL(WRITEFUNCTION) \
	C_OPT_SPECIAL(READFUNCTION) \
	C_OPT(TIMEOUT, number) \
	C_OPT(INFILESIZE, number) \
	C_OPT(POSTFIELDS, string) \
	C_OPT(REFERER, string) \
	C_OPT(FTPPORT, string) \
	C_OPT(USERAGENT, string) \
	C_OPT(LOW_SPEED_LIMIT, number) \
	C_OPT(LOW_SPEED_TIME, number) \
	C_OPT(RESUME_FROM, number) \
	C_OPT(COOKIE, string) \
	C_OPT_SL(HTTPHEADER) \
	C_OPT_SL(HTTPPOST) \
	C_OPT(SSLCERT, string) \
	C_OPT(SSLKEYPASSWD, string) \
	C_OPT(CRLF, boolean) \
	C_OPT_SL(QUOTE) \
	C_OPT_SPECIAL(HEADERDATA) \
	C_OPT(COOKIEFILE, string) \
	C_OPT(SSLVERSION, number) \
	C_OPT(TIMECONDITION, number) \
	C_OPT(TIMEVALUE, number) \
	C_OPT(CUSTOMREQUEST, string) \
	C_OPT_SL(POSTQUOTE) \
	C_OPT(WRITEINFO, string) \
	C_OPT(VERBOSE, boolean) \
	C_OPT(HEADER, boolean) \
	C_OPT(NOPROGRESS, boolean) \
	C_OPT(NOBODY, boolean) \
	C_OPT(FAILONERROR, boolean) \
	C_OPT(UPLOAD, boolean) \
	C_OPT(POST, boolean) \
	C_OPT(FTPLISTONLY, boolean) \
	C_OPT(FTPAPPEND, boolean) \
	C_OPT(NETRC, number) \
	C_OPT(FOLLOWLOCATION, boolean) \
	C_OPT(TRANSFERTEXT, boolean) \
	C_OPT(PUT, boolean) \
	C_OPT_SPECIAL(PROGRESSFUNCTION) \
	C_OPT_SPECIAL(PROGRESSDATA) \
	C_OPT(AUTOREFERER, boolean) \
	C_OPT(PROXYPORT, number) \
	C_OPT(POSTFIELDSIZE, number) \
	C_OPT(HTTPPROXYTUNNEL, boolean) \
	C_OPT(INTERFACE, string) \
	C_OPT(KRB4LEVEL, string) \
	C_OPT(SSL_VERIFYPEER, boolean) \
	C_OPT(CAINFO, string) \
	C_OPT(MAXREDIRS, number) \
	C_OPT(FILETIME, boolean) \
	C_OPT_SL(TELNETOPTIONS) \
	C_OPT(MAXCONNECTS, number) \
	C_OPT(CLOSEPOLICY, number) \
	C_OPT(FRESH_CONNECT, boolean) \
	C_OPT(FORBID_REUSE, boolean) \
	C_OPT(RANDOM_FILE, string) \
	C_OPT(EGDSOCKET, string) \
	C_OPT(CONNECTTIMEOUT, number) \
	C_OPT_SPECIAL(HEADERFUNCTION) \
	C_OPT(HTTPGET, boolean) \
	C_OPT(SSL_VERIFYHOST, number) \
	C_OPT(COOKIEJAR, string) \
	C_OPT(SSL_CIPHER_LIST, string) \
	C_OPT(HTTP_VERSION, number) \
	C_OPT(FTP_USE_EPSV, boolean) \
	C_OPT(SSLCERTTYPE, string) \
	C_OPT(SSLKEY, string) \
	C_OPT(SSLKEYTYPE, string) \
	C_OPT(SSLENGINE, string) \
	C_OPT(SSLENGINE_DEFAULT, boolean) \
	C_OPT(DNS_USE_GLOBAL_CACHE, boolean) \
	C_OPT(DNS_CACHE_TIMEOUT, number) \
	C_OPT_SL(PREQUOTE) \
	C_OPT(COOKIESESSION, boolean) \
	C_OPT(CAPATH, string) \
	C_OPT(BUFFERSIZE, number) \
	C_OPT(NOSIGNAL, boolean) \
	C_OPT(PROXYTYPE, number) \
	C_OPT(ENCODING, string) \
	C_OPT_SL(HTTP200ALIASES) \
	C_OPT(UNRESTRICTED_AUTH, boolean) \
	C_OPT(FTP_USE_EPRT, boolean) \
	C_OPT(HTTPAUTH, number) \
	C_OPT(FTP_CREATE_MISSING_DIRS, boolean) \
	C_OPT(PROXYAUTH, number) \
	C_OPT(FTP_RESPONSE_TIMEOUT, number) \
	C_OPT(IPRESOLVE, number) \
	C_OPT(MAXFILESIZE, number) \
	C_OPT(INFILESIZE_LARGE, number) \
	C_OPT(RESUME_FROM_LARGE, number) \
	C_OPT(MAXFILESIZE_LARGE, number) \
	C_OPT(NETRC_FILE, string) \
	C_OPT(FTP_SSL, number) \
	C_OPT(POSTFIELDSIZE_LARGE, number) \
	C_OPT(TCP_NODELAY, boolean) \
	C_OPT(SOURCE_USERPWD, string) \
	C_OPT_SL(SOURCE_PREQUOTE) \
	C_OPT_SL(SOURCE_POSTQUOTE) \
	C_OPT(FTPSSLAUTH, number) \
	C_OPT_SPECIAL(IOCTLFUNCTION) \
	C_OPT_SPECIAL(IOCTLDATA) \
	C_OPT(SOURCE_URL, string) \
	C_OPT_SL(SOURCE_QUOTE) \
	C_OPT(FTP_ACCOUNT, string)

#if CURL_OLDER(7,12,1) || CURL_NEWER(7,16,2)
  #define CURLOPT_SOURCE_PREQUOTE      -MAKE_VERSION_NUM(1,12,7)+0
  #define CURLOPT_SOURCE_POSTQUOTE     -MAKE_VERSION_NUM(1,12,7)+1
  #define CURLOPT_SOURCE_USERPWD       -MAKE_VERSION_NUM(1,12,7)+2
#endif

#if CURL_OLDER(7,12,2)
  #define CURLOPT_FTPSSLAUTH           -MAKE_VERSION_NUM(2,12,7)+0
#endif
 
#if CURL_OLDER(7,12,3)
  #define CURLOPT_TCP_NODELAY          -MAKE_VERSION_NUM(3,12,7)+0
  #define CURLOPT_IOCTLFUNCTION        -MAKE_VERSION_NUM(3,12,7)+1
  #define CURLOPT_IOCTLDATA            -MAKE_VERSION_NUM(3,12,7)+2
  #define CURLE_SSL_ENGINE_INITFAILED  -MAKE_VERSION_NUM(3,12,7)+3
  #define CURLE_IOCTLFUNCTION          -MAKE_VERSION_NUM(3,12,7)+4
  #define CURLE_IOCTLDATA              -MAKE_VERSION_NUM(3,12,7)+5
#endif

#if CURL_OLDER(7,13,0)
  #define CURLOPT_FTP_ACCOUNT          -MAKE_VERSION_NUM(0,13,7)+2
  #define CURLE_INTERFACE_FAILED       -MAKE_VERSION_NUM(0,13,7)+3
  #define CURLE_SEND_FAIL_REWIND       -MAKE_VERSION_NUM(0,13,7)+4
#endif

#if CURL_OLDER(7,13,0) || CURL_NEWER(7,16,2)
  #define CURLOPT_SOURCE_URL           -MAKE_VERSION_NUM(0,13,7)+0
  #define CURLOPT_SOURCE_QUOTE         -MAKE_VERSION_NUM(0,13,7)+1
#endif

#if CURL_OLDER(7,13,1)
  #define CURLE_LOGIN_DENIED           -MAKE_VERSION_NUM(1,13,7)+0
#endif

/* register static the names of options of type curl_slist */
ALL_CURL_OPT

/* not supported options for any reason
	CURLOPT_STDERR (TODO)
	CURLOPT_ERRORBUFFER (all curl operations return error message via curl_easy_strerror)
	CURLOPT_SHARE (TODO)
	CURLOPT_PRIVATE (TODO)
	CURLOPT_DEBUGFUNCTION (requires curl debug mode)
	CURLOPT_DEBUGDATA
	CURLOPT_SSLCERTPASSWD (duplicated by SSLKEYPASSWD)
	CURLOPT_SSL_CTX_FUNCTION (TODO, this will make sense only if openssl is available in Lua)
	CURLOPT_SSL_CTX_DATA
*/

/* wrap values of arbitrary type */
union luaValueT
{
	struct curl_slist *slist;
	curl_read_callback rcb;
	curl_write_callback wcb;
	curl_progress_callback pcb;

#if CURL_NEWER(7,12,3)
	curl_ioctl_callback icb;
#endif

	long nval;
	char* sval;
	void* ptr;
};

/* CURL object wrapper type */
typedef struct
{
	CURL* curl;
	lua_State* L;
	int fwriterRef;	  int wudtype; union luaValueT wud;
	int freaderRef;   int rudtype; union luaValueT rud;
	int fprogressRef; int pudtype; union luaValueT pud;
	int fheaderRef;   int hudtype; union luaValueT hud;
	int fioctlRef;    int iudtype; union luaValueT iud;
} curlT;

/* push correctly luaValue to Lua stack */
static void pushLuaValueT(lua_State* L, int t, union luaValueT v)
{
	switch (t)
	{
		case LUA_TNIL: lua_pushnil(L); break;
		case LUA_TBOOLEAN: lua_pushboolean(L, v.nval); break;
		case LUA_TTABLE:
		case LUA_TFUNCTION: 
		case LUA_TTHREAD:
		case LUA_TUSERDATA: lua_rawgeti(L, LUA_REGISTRYINDEX, v.nval); break;
		case LUA_TLIGHTUSERDATA: lua_pushlightuserdata(L, v.ptr); break;
		case LUA_TNUMBER: lua_pushnumber(L, v.nval); break;
		case LUA_TSTRING: lua_pushstring(L, v.sval); break;
		default: luaL_error(L, "invalid type %s\n", lua_typename(L, t));
	}
}

/* curl callbacks connected with Lua functions */
static size_t readerCallback( void *ptr, size_t size, size_t nmemb, void *stream)
{
	const char* readBytes;
	curlT* c=(curlT*)stream;
	lua_rawgeti(c->L, LUA_REGISTRYINDEX, c->freaderRef);
	pushLuaValueT(c->L, c->rudtype, c->rud);
	lua_pushnumber(c->L, size * nmemb);
	lua_call(c->L, 2, 1);
	readBytes=lua_tostring(c->L, -1);
	if (readBytes)
	{
		memcpy(ptr, readBytes, lua_strlen(c->L, -1));
		return lua_strlen(c->L, -1);
	}
	return 0;
}

static size_t writerCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	curlT* c=(curlT*)stream;
	lua_rawgeti(c->L, LUA_REGISTRYINDEX, c->fwriterRef);
	pushLuaValueT(c->L, c->wudtype, c->wud);
	lua_pushlstring(c->L, ptr, size * nmemb);
	lua_call(c->L, 2, 1);
	return (size_t)lua_tonumber(c->L, -1);
}

int progressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	curlT* c=(curlT*)clientp;
	lua_rawgeti(c->L, LUA_REGISTRYINDEX, c->fprogressRef);
	pushLuaValueT(c->L, c->pudtype, c->pud);
	lua_pushnumber(c->L, dltotal);
	lua_pushnumber(c->L, dlnow);
	lua_pushnumber(c->L, ultotal);
	lua_pushnumber(c->L, ulnow);
	lua_call(c->L, 5, 1);
	return (int)lua_tonumber(c->L, -1);
}

static size_t headerCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	curlT* c=(curlT*)stream;
	lua_rawgeti(c->L, LUA_REGISTRYINDEX, c->fheaderRef);
	pushLuaValueT(c->L, c->hudtype, c->hud);
	lua_pushlstring(c->L, ptr, size * nmemb);
	lua_call(c->L, 2, 1);
	return (size_t)lua_tonumber(c->L, -1);
}

#if CURL_NEWER(7,12,3)
curlioerr ioctlCallback(CURL *handle, int cmd, void *clientp)
{
	curlT* c=(curlT*)clientp;
	lua_rawgeti(c->L, LUA_REGISTRYINDEX, c->fioctlRef);
	pushLuaValueT(c->L, c->iudtype, c->iud);
	lua_pushnumber(c->L, cmd);
	lua_call(c->L, 2, 1);
	return (curlioerr)lua_tonumber(c->L, -1);
}
#endif

/* Initializes CURL connection */
static int lcurl_easy_init(lua_State* L)
{
	curlT* c = (curlT*)lua_newuserdata(L, sizeof(curlT));
	c->L=L;
	c->freaderRef=c->fwriterRef=c->fprogressRef=c->fheaderRef=c->fioctlRef=LUA_REFNIL;
	c->rud.nval=c->wud.nval=c->pud.nval=c->hud.nval=c->iud.nval=0;
	c->rudtype=c->wudtype=c->pudtype=c->hudtype=c->iudtype=LUA_TNIL;
	/* open curl handle */
	c->curl = curl_easy_init();
	/* set metatable to curlT object */
	luaL_getmetatable(L, CURLHANDLE);
	lua_setmetatable(L, -2);
	return 1;
}

/* Escapes URL strings */
static int lcurl_escape(lua_State* L)
{
	if (!lua_isnil(L, 1))
	{
		const char* s=luaL_checkstring(L, 1);
		lua_pushstring(L, curl_escape(s, (int)lua_strlen(L, 1)));
		return 1;
	} else
	{
		luaL_argerror(L, 1, "string parameter expected");
	}
	return 0;
}

/* Unescapes URL encoding in strings */
static int lcurl_unescape(lua_State* L)
{
	if (!lua_isnil(L, 1))
	{
		const char* s=luaL_checkstring(L, 1);
		lua_pushstring(L, curl_unescape(s, (int)lua_strlen(L, 1)));
		return 1;
	} else
	{
		luaL_argerror(L, 1, "string parameter expected");
	}
	return 0;
}

/* Access curlT object from the Lua stack at specified position  */
static curlT* tocurl (lua_State *L, int cindex) 
{
	curlT* c=(curlT*)luaL_checkudata(L, cindex, CURLHANDLE);
	if (!c) luaL_argerror(L, cindex, "invalid curl object");
	if (!c->curl) luaL_error(L, "attempt to use closed curl object");
	return c;
}

/* Request internal information from the curl session */
static int lcurl_easy_getinfo(lua_State* L)
{
	curlT* c=tocurl(L, 1);
	CURLINFO nInfo;
	CURLcode code=-1;
	luaL_checktype(L, 2, LUA_TNUMBER);   /* accept info code number only */
	nInfo=lua_tonumber(L, 2);
	if (nInfo>CURLINFO_SLIST)
	{
		/* string list */
		struct curl_slist *slist=0;
		if (CURLE_OK == (code=curl_easy_getinfo(c->curl, nInfo, &slist)))
		{
			if (slist)
			{
				int i;
				lua_newtable(L);
				for (i=1; slist; i++, slist=slist->next)
				{
					lua_pushnumber(L, i);
					lua_pushstring(L, slist->data);
					lua_settable(L, -3);
				}
				curl_slist_free_all(slist);
			} else
			{
				lua_pushnil(L);
			}
			return 1;
		} else
		{
			/* curl_easy_getinfo returns error */
		}
	} else
	if (nInfo>CURLINFO_DOUBLE)
	{
		/* double */
		double value;
		if (CURLE_OK == (code=curl_easy_getinfo(c->curl, nInfo, &value)))
		{
			lua_pushnumber(L, value);
			return 1;
		} else
		{
			/* curl_easy_getinfo returns error */
		}
	} else
	if (nInfo>CURLINFO_LONG)
	{
		/* long */
		long value;
		if (CURLE_OK == (code=curl_easy_getinfo(c->curl, nInfo, &value)))
		{
			lua_pushinteger(L, (lua_Integer)value);
			return 1;
		} else
		{
			/* curl_easy_getinfo returns error */
		}
	} else
	if (nInfo>CURLINFO_STRING)
	{
		/* string */
		char* value;
		if (CURLE_OK == (code=curl_easy_getinfo(c->curl, nInfo, &value)))
		{
			lua_pushstring(L, value);
			return 1;
		} else
		{
			/* curl_easy_getinfo returns error */
		}
	}
/* on error */
	/* return nil, error message, error code */
	lua_pushnil(L);
	if (code>CURLE_OK)
	{
		#if CURL_NEWER(7,11,2)
			lua_pushstring(L, curl_easy_strerror(code));
		#else
			lua_pushfstring(L, "Curl error: #%d", (code));
		#endif
		lua_pushnumber(L, code);
		return 3;
	}
	else
	{
		lua_pushfstring(L, "Invalid CURLINFO number: %d", nInfo);
		return 2;
	}
}

/* convert argument n to string allowing nil values */
static union luaValueT get_string(lua_State* L, int n)
{
	union luaValueT v;
	v.sval=(char*)lua_tostring(L, 3);
	return v;
}

/* convert argument n to number allowing only number convertable values */
static union luaValueT get_number(lua_State* L, int n)
{
	union luaValueT v;
	v.nval=(int)luaL_checknumber(L, n);
	return v;
}

/* get argument n as boolean but if not boolean argument then fail with Lua error */
static union luaValueT get_boolean(lua_State* L, int n)
{
	union luaValueT v;
	if (!lua_isboolean(L, n))
	{
		luaL_argerror(L, n, "boolean value expected");
	}
	v.nval=(int)lua_toboolean(L, n);
	return v;
}

/* remove and free old slist from registry if any associated with the given key */
static void free_slist(lua_State* L, const char** key)
{
	struct curl_slist *slist; 
	lua_pushlightuserdata(L, (void *)key);
	lua_rawget(L, LUA_REGISTRYINDEX);
	slist=(struct curl_slist *)lua_topointer(L, -1);
	if (slist) 
	{
		curl_slist_free_all(slist);
	}
}

/* after argument number n combine all arguments to the curl type curl_slist */
static union luaValueT get_slist(lua_State* L, int n, const char** key)
{
	int i;
	union luaValueT v;
	struct curl_slist *slist=0; 

	/* free the previous slist if any */
	free_slist(L, key);

	/* check if all parameters are strings */
	for (i=n; i<lua_gettop(L); i++) luaL_checkstring(L, i);
	for (i=n; i<lua_gettop(L); i++)
	{
		slist = curl_slist_append(slist, lua_tostring(L, i));
	}

	/* set the new slist in registry */
	lua_pushlightuserdata(L, (void*)key);
	lua_pushlightuserdata(L, (void *)slist);
	lua_rawset(L, LUA_REGISTRYINDEX);

	v.slist=slist;
	return v;
}

/* set any supported curl option (see also ALL_CURL_OPT) */
static int lcurl_easy_setopt(lua_State* L)
{
	union luaValueT v;                   /* the result option value */
	int curlOpt;                         /* the provided option code  */
	CURLcode code;                       /* return error code from curl */
	curlT* c = tocurl(L, 1);             /* get self object */
	luaL_checktype(L, 2, LUA_TNUMBER);   /* accept only number option codes */
	if (lua_gettop(L)<3)                 /* option value is always required */
	{
		luaL_error(L, "Invalid number of arguments %d to `setopt' method", lua_gettop(L));
	}
	curlOpt=(int)lua_tonumber(L, 2);     /* get the curl option code */
	v.nval=0;

	switch (curlOpt)
	{
		case CURLOPT_PROGRESSFUNCTION:
		case CURLOPT_READFUNCTION:
		case CURLOPT_WRITEFUNCTION:
		case CURLOPT_HEADERFUNCTION:
		case CURLOPT_IOCTLFUNCTION:
			luaL_checktype(L, 3, LUA_TFUNCTION); /* callback options require Lua function value */

		case CURLOPT_READDATA:
		case CURLOPT_WRITEDATA:
		case CURLOPT_PROGRESSDATA:
		case CURLOPT_HEADERDATA:
#if CURL_NEWER(7,12,3)
		case CURLOPT_IOCTLDATA:
#endif
			switch (lua_type(L, 3))             /* handle table, userdata and funtion callback params specially */
			{
				case LUA_TTABLE:
				case LUA_TUSERDATA:
				case LUA_TTHREAD:
				case LUA_TFUNCTION:
				{
					int ref;
					lua_pushvalue(L, 3);                                  
					ref=luaL_ref(L, LUA_REGISTRYINDEX);  /* get reference to the lua object in registry */

					if (curlOpt == CURLOPT_READFUNCTION)
					{
						luaL_unref(L, LUA_REGISTRYINDEX, c->freaderRef); /* unregister previous reference to reader if any */
						c->freaderRef=ref;                               /* keep the reader function reference in self */
						v.rcb=(curl_read_callback)readerCallback;        /* redirect the option value to readerCallback */
						if (CURLE_OK != (code=curl_easy_setopt(c->curl, CURLOPT_READDATA, c))) goto on_error;
					}
					else if (curlOpt == CURLOPT_WRITEFUNCTION)
					{
						luaL_unref(L, LUA_REGISTRYINDEX, c->fwriterRef);
						c->fwriterRef=ref;
						v.wcb=(curl_write_callback)writerCallback;
						if (CURLE_OK != (code=curl_easy_setopt(c->curl, CURLOPT_WRITEDATA, c))) goto on_error;
					}
					else if (curlOpt == CURLOPT_PROGRESSFUNCTION)
					{
						luaL_unref(L, LUA_REGISTRYINDEX, c->fprogressRef);
						c->fprogressRef=ref;
						v.pcb=(curl_progress_callback)progressCallback;
						if (CURLE_OK != (code=curl_easy_setopt(c->curl, CURLOPT_PROGRESSDATA, c))) goto on_error;
					}
					else if (curlOpt == CURLOPT_HEADERFUNCTION)
					{
						luaL_unref(L, LUA_REGISTRYINDEX, c->fheaderRef);
						c->fheaderRef=ref;
						v.wcb=(curl_write_callback)headerCallback;
						if (CURLE_OK != (code=curl_easy_setopt(c->curl, CURLOPT_HEADERDATA, c))) goto on_error;
					}
#if CURL_NEWER(7,12,3)
					else if (curlOpt == CURLOPT_IOCTLFUNCTION)
					{
						luaL_unref(L, LUA_REGISTRYINDEX, c->fioctlRef);
						c->fioctlRef=ref;
						v.icb=ioctlCallback;
						if (CURLE_OK != (code=curl_easy_setopt(c->curl, CURLOPT_IOCTLDATA, c))) goto on_error;
					}
#endif
					else
					{
						/* When the option code is any of CURLOPT_xxxDATA and the argument is table, 
						/* userdata or function set the curl option value to the lua object reference */
						v.nval=ref;
					}
				}break;
			}break;

/* Handle all supported curl options differently according the specific option argument type */
#undef C_OPT
#define C_OPT(n, t) \
		case CURLOPT_##n: \
			v=get_##t(L, 3); \
			break;

#undef C_OPT_SL
#define C_OPT_SL(n) \
		case CURLOPT_##n: \
			{ \
				v=get_slist(L, 3, &KEY_##n); \
			}break;

/* Expands all the list of switch-case's here */
ALL_CURL_OPT

		default:
			luaL_error(L, "Not supported curl option %d", curlOpt);
	}

	/* additional check if the option value has compatible type with the option code */
	switch (lua_type(L, 3))
	{
		case LUA_TFUNCTION:                        /* allow function argument only for the special option codes */
			if (curlOpt == CURLOPT_READFUNCTION
				|| curlOpt == CURLOPT_WRITEFUNCTION
				|| curlOpt == CURLOPT_PROGRESSFUNCTION
				|| curlOpt == CURLOPT_HEADERFUNCTION
				|| curlOpt == CURLOPT_IOCTLFUNCTION
				)
				break;
		case LUA_TTABLE:                           /* allow table or userdata only for the callback parameter option */
		case LUA_TUSERDATA:
			if (curlOpt != CURLOPT_READDATA
				&& curlOpt != CURLOPT_WRITEDATA
				&& curlOpt != CURLOPT_PROGRESSDATA
				&& curlOpt != CURLOPT_HEADERDATA
#if CURL_NEWER(7,12,3)
				&& curlOpt != CURLOPT_IOCTLDATA
#endif
				)
				luaL_error(L, "argument #2 type %s is not compatible with this option", lua_typename(L, 3));
			break;
	}

	/* handle curl option for setting callback parameter */
	switch (curlOpt)
	{
		case CURLOPT_READDATA:
			if (c->rudtype == LUA_TFUNCTION || c->rudtype == LUA_TUSERDATA || c->rudtype == LUA_TTABLE || c->rudtype == LUA_TTHREAD)
				luaL_unref(L, LUA_REGISTRYINDEX, c->rud.nval); /* unref previously referenced read data */
			c->rudtype=lua_type(L, 3);                         /* set the read data type */
			c->rud=v;                                          /* set the read data value (it can be reference) */
			v.ptr=c;                                           /* set the real read data to curl as our self object */
			break;
		case CURLOPT_WRITEDATA:
			if (c->wudtype == LUA_TFUNCTION || c->wudtype == LUA_TUSERDATA || c->wudtype == LUA_TTABLE || c->wudtype == LUA_TTHREAD)
				luaL_unref(L, LUA_REGISTRYINDEX, c->wud.nval);
			c->wudtype=lua_type(L, 3);
			c->wud=v;
			v.ptr=c;
			break;
		case CURLOPT_PROGRESSDATA:
			if (c->pudtype == LUA_TFUNCTION || c->pudtype == LUA_TUSERDATA || c->pudtype == LUA_TTABLE || c->pudtype == LUA_TTHREAD)
				luaL_unref(L, LUA_REGISTRYINDEX, c->pud.nval);
			c->pudtype=lua_type(L, 3);
			c->pud=v;
			v.ptr=c;
			break;
		case CURLOPT_HEADERDATA:
			if (c->hudtype == LUA_TFUNCTION || c->hudtype == LUA_TUSERDATA || c->hudtype == LUA_TTABLE || c->hudtype == LUA_TTHREAD)
				luaL_unref(L, LUA_REGISTRYINDEX, c->hud.nval);
			c->hudtype=lua_type(L, 3);
			c->hud=v;
			v.ptr=c;
			break;
#if CURL_NEWER(7,12,3)
		case CURLOPT_IOCTLDATA:
			if (c->iudtype == LUA_TFUNCTION || c->iudtype == LUA_TUSERDATA || c->iudtype == LUA_TTABLE || c->iudtype == LUA_TTHREAD)
				luaL_unref(L, LUA_REGISTRYINDEX, c->iud.nval);
			c->iudtype=lua_type(L, 3);
			c->iud=v;
			v.ptr=c;
			break;
#endif
	}

	/* set easy the curl option with the processed value */
	if (CURLE_OK == (code=curl_easy_setopt(c->curl, (int)lua_tonumber(L, 2), v.nval)))
	{
		/* on success return true */
		lua_pushboolean(L, 1);
		return 1;
	} 
on_error:
	/* on fail return nil, error message, error code */
	lua_pushnil(L);
#if CURL_NEWER(7,11,2)
	lua_pushstring(L, curl_easy_strerror(code));
#else
	lua_pushfstring(L, "Curl error: #%d", (code));
#endif
	lua_pushnumber(L, code);
	return 3;
}

/* perform the curl commands */
static int lcurl_easy_perform(lua_State* L)
{
	CURLcode code;                       /* return error code from curl */
	curlT* c = tocurl(L, 1);             /* get self object */
	code = curl_easy_perform(c->curl);   /* do the curl perform */
	if (CURLE_OK == code)
	{
		/* on success return true */
		lua_pushboolean(L, 1);
		return 1;
	}
	/* on fail return nil, error message, error code */
	lua_pushnil(L);
#if CURL_NEWER(7,11,2)
	lua_pushstring(L, curl_easy_strerror(code));
#else
	lua_pushfstring(L, "Curl error: #%d", (code));
#endif
	lua_pushnumber(L, code);
	return 3;
}

/* Finalizes CURL */
static int lcurl_easy_close(lua_State* L)
{
	curlT* c=tocurl(L, 1);
	curl_easy_cleanup(c->curl);
	luaL_unref(L, LUA_REGISTRYINDEX, c->freaderRef);
	luaL_unref(L, LUA_REGISTRYINDEX, c->fwriterRef);
	luaL_unref(L, LUA_REGISTRYINDEX, c->fprogressRef);
	luaL_unref(L, LUA_REGISTRYINDEX, c->fheaderRef);
	luaL_unref(L, LUA_REGISTRYINDEX, c->fioctlRef);
	if (c->rudtype == LUA_TFUNCTION || c->rudtype == LUA_TUSERDATA || c->rudtype == LUA_TTABLE || c->rudtype == LUA_TTHREAD)
		luaL_unref(L, LUA_REGISTRYINDEX, c->rud.nval);
	if (c->wudtype == LUA_TFUNCTION || c->wudtype == LUA_TUSERDATA || c->wudtype == LUA_TTABLE || c->wudtype == LUA_TTHREAD)
		luaL_unref(L, LUA_REGISTRYINDEX, c->wud.nval);
	if (c->pudtype == LUA_TFUNCTION || c->pudtype == LUA_TUSERDATA || c->pudtype == LUA_TTABLE || c->pudtype == LUA_TTHREAD)
		luaL_unref(L, LUA_REGISTRYINDEX, c->pud.nval);
	if (c->hudtype == LUA_TFUNCTION || c->hudtype == LUA_TUSERDATA || c->hudtype == LUA_TTABLE || c->hudtype == LUA_TTHREAD)
		luaL_unref(L, LUA_REGISTRYINDEX, c->hud.nval);
	if (c->iudtype == LUA_TFUNCTION || c->iudtype == LUA_TUSERDATA || c->iudtype == LUA_TTABLE || c->iudtype == LUA_TTHREAD)
		luaL_unref(L, LUA_REGISTRYINDEX, c->iud.nval);

#undef C_OPT
#undef C_OPT_SL
#undef C_OPT_SPECIAL
#define C_OPT(n, t)
#define C_OPT_SL(n) free_slist(L, &KEY_##n);
#define C_OPT_SPECIAL(n)

	ALL_CURL_OPT

	c->curl=0;
	lua_pushboolean(L, 1);
	return 1;
}

/* garbage collect the curl object */
static int lcurl_gc(lua_State* L)
{
	curlT* c = (curlT*)luaL_checkudata(L, 1, CURLHANDLE);
	if (c && c->curl)
	{
		curl_easy_cleanup(c->curl);
	}
	return 0;
}


static const struct luaL_reg luacurl_meths[] =
{
	{"close", lcurl_easy_close},
	{"setopt", lcurl_easy_setopt},
	{"perform", lcurl_easy_perform},
	{"getinfo", lcurl_easy_getinfo},
	{"__gc", lcurl_gc},
	{0, 0}
};

static const struct luaL_reg luacurl_funcs[] =
{
	{"new", lcurl_easy_init},
	{"escape", lcurl_escape},
	{"unescape", lcurl_unescape},
	{0, 0}
};

static void createmeta (lua_State *L) 
{
	luaL_newmetatable(L, CURLHANDLE);
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -2);
	lua_rawset(L, -3);
}

/*
 * Assumes the table is on top of the stack.
 */
static void set_info (lua_State *L) 
{
	LUA_SET_TABLE(L, literal, "_COPYRIGHT", literal, "(C) 2003-2006 AVIQ Systems AG");
	LUA_SET_TABLE(L, literal, "_DESCRIPTION", literal, "LuaCurl binds the CURL easy interface to Lua");
	LUA_SET_TABLE(L, literal, "_NAME", literal, "luacurl");
	LUA_SET_TABLE(L, literal, "_VERSION", literal, "1.1.0");
	LUA_SET_TABLE(L, literal, "_CURLVERSION", string, curl_version());
	LUA_SET_TABLE(L, literal, "_SUPPORTED_CURLVERSION", literal, LIBCURL_VERSION);
}

static void setcurlerrors(lua_State* L)
{
	LUA_SET_TABLE(L, literal, "OK", number, CURLE_OK);
	LUA_SET_TABLE(L, literal, "FAILED_INIT", number, CURLE_FAILED_INIT);
	LUA_SET_TABLE(L, literal, "UNSUPPORTED_PROTOCOL", number, CURLE_UNSUPPORTED_PROTOCOL);
	LUA_SET_TABLE(L, literal, "URL_MALFORMAT", number, CURLE_URL_MALFORMAT);
	LUA_SET_TABLE(L, literal, "URL_MALFORMAT_USER", number, CURLE_URL_MALFORMAT_USER);
	LUA_SET_TABLE(L, literal, "COULDNT_RESOLVE_PROXY", number, CURLE_COULDNT_RESOLVE_PROXY);
	LUA_SET_TABLE(L, literal, "COULDNT_RESOLVE_HOST", number, CURLE_COULDNT_RESOLVE_HOST);
	LUA_SET_TABLE(L, literal, "COULDNT_CONNECT", number, CURLE_COULDNT_CONNECT);
	LUA_SET_TABLE(L, literal, "FTP_WEIRD_SERVER_REPLY", number, CURLE_FTP_WEIRD_SERVER_REPLY);
	LUA_SET_TABLE(L, literal, "FTP_ACCESS_DENIED", number, CURLE_FTP_ACCESS_DENIED);
	LUA_SET_TABLE(L, literal, "FTP_USER_PASSWORD_INCORRECT", number, CURLE_FTP_USER_PASSWORD_INCORRECT);
	LUA_SET_TABLE(L, literal, "FTP_WEIRD_PASS_REPLY", number, CURLE_FTP_WEIRD_PASS_REPLY);
	LUA_SET_TABLE(L, literal, "FTP_WEIRD_USER_REPLY", number, CURLE_FTP_WEIRD_USER_REPLY);
	LUA_SET_TABLE(L, literal, "FTP_WEIRD_PASV_REPLY", number, CURLE_FTP_WEIRD_PASV_REPLY);
	LUA_SET_TABLE(L, literal, "FTP_WEIRD_227_FORMAT", number, CURLE_FTP_WEIRD_227_FORMAT);
	LUA_SET_TABLE(L, literal, "FTP_CANT_GET_HOST", number, CURLE_FTP_CANT_GET_HOST);
	LUA_SET_TABLE(L, literal, "FTP_CANT_RECONNECT", number, CURLE_FTP_CANT_RECONNECT);
	LUA_SET_TABLE(L, literal, "FTP_COULDNT_SET_BINARY", number, CURLE_FTP_COULDNT_SET_BINARY);
	LUA_SET_TABLE(L, literal, "PARTIAL_FILE", number, CURLE_PARTIAL_FILE);
	LUA_SET_TABLE(L, literal, "FTP_COULDNT_RETR_FILE", number, CURLE_FTP_COULDNT_RETR_FILE);
	LUA_SET_TABLE(L, literal, "FTP_WRITE_ERROR", number, CURLE_FTP_WRITE_ERROR);
	LUA_SET_TABLE(L, literal, "FTP_QUOTE_ERROR", number, CURLE_FTP_QUOTE_ERROR);
	LUA_SET_TABLE(L, literal, "HTTP_RETURNED_ERROR", number, CURLE_HTTP_RETURNED_ERROR);
	LUA_SET_TABLE(L, literal, "WRITE_ERROR", number, CURLE_WRITE_ERROR);
	LUA_SET_TABLE(L, literal, "MALFORMAT_USER", number, CURLE_MALFORMAT_USER);
	LUA_SET_TABLE(L, literal, "FTP_COULDNT_STOR_FILE", number, CURLE_FTP_COULDNT_STOR_FILE);
	LUA_SET_TABLE(L, literal, "READ_ERROR", number, CURLE_READ_ERROR);
	LUA_SET_TABLE(L, literal, "OUT_OF_MEMORY", number, CURLE_OUT_OF_MEMORY);
	LUA_SET_TABLE(L, literal, "OPERATION_TIMEOUTED", number, CURLE_OPERATION_TIMEOUTED);
	LUA_SET_TABLE(L, literal, "FTP_COULDNT_SET_ASCII", number, CURLE_FTP_COULDNT_SET_ASCII);
	LUA_SET_TABLE(L, literal, "FTP_PORT_FAILED", number, CURLE_FTP_PORT_FAILED);
	LUA_SET_TABLE(L, literal, "FTP_COULDNT_USE_REST", number, CURLE_FTP_COULDNT_USE_REST);
	LUA_SET_TABLE(L, literal, "FTP_COULDNT_GET_SIZE", number, CURLE_FTP_COULDNT_GET_SIZE);
	LUA_SET_TABLE(L, literal, "HTTP_RANGE_ERROR", number, CURLE_HTTP_RANGE_ERROR);
	LUA_SET_TABLE(L, literal, "HTTP_POST_ERROR", number, CURLE_HTTP_POST_ERROR);
	LUA_SET_TABLE(L, literal, "SSL_CONNECT_ERROR", number, CURLE_SSL_CONNECT_ERROR);
	LUA_SET_TABLE(L, literal, "BAD_DOWNLOAD_RESUME", number, CURLE_BAD_DOWNLOAD_RESUME);
	LUA_SET_TABLE(L, literal, "FILE_COULDNT_READ_FILE", number, CURLE_FILE_COULDNT_READ_FILE);
	LUA_SET_TABLE(L, literal, "LDAP_CANNOT_BIND", number, CURLE_LDAP_CANNOT_BIND);
	LUA_SET_TABLE(L, literal, "LDAP_SEARCH_FAILED", number, CURLE_LDAP_SEARCH_FAILED);
	LUA_SET_TABLE(L, literal, "LIBRARY_NOT_FOUND", number, CURLE_LIBRARY_NOT_FOUND);
	LUA_SET_TABLE(L, literal, "FUNCTION_NOT_FOUND", number, CURLE_FUNCTION_NOT_FOUND);
	LUA_SET_TABLE(L, literal, "ABORTED_BY_CALLBACK", number, CURLE_ABORTED_BY_CALLBACK);
	LUA_SET_TABLE(L, literal, "BAD_FUNCTION_ARGUMENT", number, CURLE_BAD_FUNCTION_ARGUMENT);
	LUA_SET_TABLE(L, literal, "BAD_CALLING_ORDER", number, CURLE_BAD_CALLING_ORDER);
	LUA_SET_TABLE(L, literal, "INTERFACE_FAILED", number, CURLE_INTERFACE_FAILED);
	LUA_SET_TABLE(L, literal, "BAD_PASSWORD_ENTERED", number, CURLE_BAD_PASSWORD_ENTERED);
	LUA_SET_TABLE(L, literal, "TOO_MANY_REDIRECTS", number, CURLE_TOO_MANY_REDIRECTS);
	LUA_SET_TABLE(L, literal, "UNKNOWN_TELNET_OPTION", number, CURLE_UNKNOWN_TELNET_OPTION);
	LUA_SET_TABLE(L, literal, "TELNET_OPTION_SYNTAX", number, CURLE_TELNET_OPTION_SYNTAX);
	LUA_SET_TABLE(L, literal, "OBSOLETE", number, CURLE_OBSOLETE);
	LUA_SET_TABLE(L, literal, "SSL_PEER_CERTIFICATE", number, CURLE_SSL_PEER_CERTIFICATE);
	LUA_SET_TABLE(L, literal, "GOT_NOTHING", number, CURLE_GOT_NOTHING);
	LUA_SET_TABLE(L, literal, "SSL_ENGINE_NOTFOUND", number, CURLE_SSL_ENGINE_NOTFOUND);
	LUA_SET_TABLE(L, literal, "SSL_ENGINE_SETFAILED", number, CURLE_SSL_ENGINE_SETFAILED);
	LUA_SET_TABLE(L, literal, "SEND_ERROR", number, CURLE_SEND_ERROR);
	LUA_SET_TABLE(L, literal, "RECV_ERROR", number, CURLE_RECV_ERROR);
	LUA_SET_TABLE(L, literal, "SHARE_IN_USE", number, CURLE_SHARE_IN_USE);
	LUA_SET_TABLE(L, literal, "SSL_CERTPROBLEM", number, CURLE_SSL_CERTPROBLEM);
	LUA_SET_TABLE(L, literal, "SSL_CIPHER", number, CURLE_SSL_CIPHER);
	LUA_SET_TABLE(L, literal, "SSL_CACERT", number, CURLE_SSL_CACERT);
	LUA_SET_TABLE(L, literal, "BAD_CONTENT_ENCODING", number, CURLE_BAD_CONTENT_ENCODING);
	LUA_SET_TABLE(L, literal, "LDAP_INVALID_URL", number, CURLE_LDAP_INVALID_URL);
	LUA_SET_TABLE(L, literal, "FILESIZE_EXCEEDED", number, CURLE_FILESIZE_EXCEEDED);
	LUA_SET_TABLE(L, literal, "FTP_SSL_FAILED", number, CURLE_FTP_SSL_FAILED);
	LUA_SET_TABLE(L, literal, "SEND_FAIL_REWIND", number, CURLE_SEND_FAIL_REWIND);
	LUA_SET_TABLE(L, literal, "SSL_ENGINE_INITFAILED", number, CURLE_SSL_ENGINE_INITFAILED);
	LUA_SET_TABLE(L, literal, "LOGIN_DENIED", number, CURLE_LOGIN_DENIED);
}

static void setcurloptions(lua_State* L)
{
#undef C_OPT_SPECIAL
#undef C_OPT
#undef C_OPT_SL
#define C_OPT(n, t) LUA_SET_TABLE(L, literal, "OPT_"#n, number, CURLOPT_##n);
#define C_OPT_SL(n) C_OPT(n, dummy)
#define C_OPT_SPECIAL(n) C_OPT(n, dummy)

ALL_CURL_OPT
}

static void setcurlvalues(lua_State* L)
{
#if CURL_NEWER(7,12,1)
	LUA_SET_TABLE(L, literal, "READFUNC_ABORT", number, CURL_READFUNC_ABORT);
#endif

#if CURL_NEWER(7,12,3)
	/* enum curlioerr */
	LUA_SET_TABLE(L, literal, "IOE_OK", number, CURLIOE_OK);
	LUA_SET_TABLE(L, literal, "IOE_UNKNOWNCMD", number, CURLIOE_UNKNOWNCMD);
	LUA_SET_TABLE(L, literal, "IOE_FAILRESTART", number, CURLIOE_FAILRESTART);

	/* enum curliocmd */
	LUA_SET_TABLE(L, literal, "IOCMD_NOP", number, CURLIOCMD_NOP);
	LUA_SET_TABLE(L, literal, "IOCMD_RESTARTREAD", number, CURLIOCMD_RESTARTREAD);
#endif

	/* enum curl_proxytype */
	LUA_SET_TABLE(L, literal, "PROXY_HTTP", number, CURLPROXY_HTTP);
	LUA_SET_TABLE(L, literal, "PROXY_SOCKS4", number, CURLPROXY_SOCKS4);
	LUA_SET_TABLE(L, literal, "PROXY_SOCKS5", number, CURLPROXY_SOCKS5);

	/* auth types */
	LUA_SET_TABLE(L, literal, "AUTH_NONE", number, CURLAUTH_NONE);
	LUA_SET_TABLE(L, literal, "AUTH_BASIC", number, CURLAUTH_BASIC);
	LUA_SET_TABLE(L, literal, "AUTH_DIGEST", number, CURLAUTH_DIGEST);
	LUA_SET_TABLE(L, literal, "AUTH_GSSNEGOTIATE", number, CURLAUTH_GSSNEGOTIATE);
	LUA_SET_TABLE(L, literal, "AUTH_NTLM", number, CURLAUTH_NTLM);
	LUA_SET_TABLE(L, literal, "AUTH_ANY", number, CURLAUTH_ANY);
	LUA_SET_TABLE(L, literal, "AUTH_ANYSAFE", number, CURLAUTH_ANYSAFE);

	/* enum curl_ftpssl */
	LUA_SET_TABLE(L, literal, "FTPSSL_NONE", number, CURLFTPSSL_NONE);
	LUA_SET_TABLE(L, literal, "FTPSSL_TRY", number, CURLFTPSSL_TRY);
	LUA_SET_TABLE(L, literal, "FTPSSL_CONTROL", number, CURLFTPSSL_CONTROL);
	LUA_SET_TABLE(L, literal, "FTPSSL_ALL", number, CURLFTPSSL_ALL);

#if CURL_NEWER(7,12,2)
	/* enum curl_ftpauth */
	LUA_SET_TABLE(L, literal, "FTPAUTH_DEFAULT", number, CURLFTPAUTH_DEFAULT);
	LUA_SET_TABLE(L, literal, "FTPAUTH_SSL", number, CURLFTPAUTH_SSL);
	LUA_SET_TABLE(L, literal, "FTPAUTH_TLS", number, CURLFTPAUTH_TLS);
#endif

	/* ip resolve options */
	LUA_SET_TABLE(L, literal, "IPRESOLVE_WHATEVER", number, CURL_IPRESOLVE_WHATEVER);
	LUA_SET_TABLE(L, literal, "IPRESOLVE_V4", number, CURL_IPRESOLVE_V4);
	LUA_SET_TABLE(L, literal, "IPRESOLVE_V6", number, CURL_IPRESOLVE_V6);

	/* http versions */
	LUA_SET_TABLE(L, literal, "HTTP_VERSION_NONE", number, CURL_HTTP_VERSION_NONE);
	LUA_SET_TABLE(L, literal, "HTTP_VERSION_1_0", number, CURL_HTTP_VERSION_1_0);
	LUA_SET_TABLE(L, literal, "HTTP_VERSION_1_1", number, CURL_HTTP_VERSION_1_1);

	/* CURL_NETRC_OPTION */
	LUA_SET_TABLE(L, literal, "NETRC_IGNORED", number, CURL_NETRC_IGNORED);
	LUA_SET_TABLE(L, literal, "NETRC_OPTIONAL", number, CURL_NETRC_OPTIONAL);
	LUA_SET_TABLE(L, literal, "NETRC_REQUIRED", number, CURL_NETRC_REQUIRED);
	
	/* ssl version */
	LUA_SET_TABLE(L, literal, "SSLVERSION_DEFAULT", number, CURL_SSLVERSION_DEFAULT);
	LUA_SET_TABLE(L, literal, "SSLVERSION_TLSv1", number, CURL_SSLVERSION_TLSv1);
	LUA_SET_TABLE(L, literal, "SSLVERSION_SSLv2", number, CURL_SSLVERSION_SSLv2);
	LUA_SET_TABLE(L, literal, "SSLVERSION_SSLv3", number, CURL_SSLVERSION_SSLv3);

	/* CURLOPT_TIMECONDITION */
	LUA_SET_TABLE(L, literal, "TIMECOND_NONE", number, CURL_TIMECOND_NONE);
	LUA_SET_TABLE(L, literal, "TIMECOND_IFMODSINCE", number, CURL_TIMECOND_IFMODSINCE);
	LUA_SET_TABLE(L, literal, "TIMECOND_IFUNMODSINCE", number, CURL_TIMECOND_IFUNMODSINCE);
	LUA_SET_TABLE(L, literal, "TIMECOND_LASTMOD", number, CURL_TIMECOND_LASTMOD);

	/* enum CURLformoption */
	LUA_SET_TABLE(L, literal, "COPYNAME", number, CURLFORM_COPYNAME);
	LUA_SET_TABLE(L, literal, "PTRNAME", number, CURLFORM_PTRNAME);
	LUA_SET_TABLE(L, literal, "NAMELENGTH", number, CURLFORM_NAMELENGTH);
	LUA_SET_TABLE(L, literal, "COPYCONTENTS", number, CURLFORM_COPYCONTENTS);
	LUA_SET_TABLE(L, literal, "PTRCONTENTS", number, CURLFORM_PTRCONTENTS);
	LUA_SET_TABLE(L, literal, "CONTENTSLENGTH", number, CURLFORM_CONTENTSLENGTH);
	LUA_SET_TABLE(L, literal, "FILECONTENT", number, CURLFORM_FILECONTENT);
	LUA_SET_TABLE(L, literal, "ARRAY", number, CURLFORM_ARRAY);
	LUA_SET_TABLE(L, literal, "OBSOLETE", number, CURLFORM_OBSOLETE);
	LUA_SET_TABLE(L, literal, "FILE", number, CURLFORM_FILE);
	LUA_SET_TABLE(L, literal, "BUFFER", number, CURLFORM_BUFFER);
	LUA_SET_TABLE(L, literal, "BUFFERPTR", number, CURLFORM_BUFFERPTR);
	LUA_SET_TABLE(L, literal, "BUFFERLENGTH", number, CURLFORM_BUFFERLENGTH);
	LUA_SET_TABLE(L, literal, "CONTENTTYPE", number, CURLFORM_CONTENTTYPE);
	LUA_SET_TABLE(L, literal, "CONTENTHEADER", number, CURLFORM_CONTENTHEADER);
	LUA_SET_TABLE(L, literal, "FILENAME", number, CURLFORM_FILENAME);
	LUA_SET_TABLE(L, literal, "END", number, CURLFORM_END);
	LUA_SET_TABLE(L, literal, "OBSOLETE2", number, CURLFORM_OBSOLETE2);

	/* enum CURLFORMcode*/
	LUA_SET_TABLE(L, literal, "CURL_FORMADD_OK", number, CURL_FORMADD_OK);
	LUA_SET_TABLE(L, literal, "CURL_FORMADD_MEMORY", number, CURL_FORMADD_MEMORY);
	LUA_SET_TABLE(L, literal, "CURL_FORMADD_OPTION_TWICE", number, CURL_FORMADD_OPTION_TWICE);
	LUA_SET_TABLE(L, literal, "CURL_FORMADD_NULL", number, CURL_FORMADD_NULL);
	LUA_SET_TABLE(L, literal, "CURL_FORMADD_UNKNOWN_OPTION", number, CURL_FORMADD_UNKNOWN_OPTION);
	LUA_SET_TABLE(L, literal, "CURL_FORMADD_INCOMPLETE", number, CURL_FORMADD_INCOMPLETE);
	LUA_SET_TABLE(L, literal, "CURL_FORMADD_ILLEGAL_ARRAY", number, CURL_FORMADD_ILLEGAL_ARRAY);

#if CURL_NEWER(7,12,1)
	LUA_SET_TABLE(L, literal, "CURL_FORMADD_DISABLED", number, CURL_FORMADD_DISABLED);
#endif

	/* enum curl_closepolicy*/
	LUA_SET_TABLE(L, literal, "CLOSEPOLICY_OLDEST", number, CURLCLOSEPOLICY_OLDEST);
	LUA_SET_TABLE(L, literal, "CLOSEPOLICY_LEAST_RECENTLY_USED", number, CURLCLOSEPOLICY_LEAST_RECENTLY_USED);
	LUA_SET_TABLE(L, literal, "CLOSEPOLICY_LEAST_TRAFFIC", number, CURLCLOSEPOLICY_LEAST_TRAFFIC);
	LUA_SET_TABLE(L, literal, "CLOSEPOLICY_SLOWEST", number, CURLCLOSEPOLICY_SLOWEST);
	LUA_SET_TABLE(L, literal, "CLOSEPOLICY_CALLBACK", number, CURLCLOSEPOLICY_CALLBACK);
}

static void setcurlinfo(lua_State* L)
{
	LUA_SET_TABLE(L, literal, "INFO_NONE", number, CURLINFO_NONE);
	LUA_SET_TABLE(L, literal, "INFO_EFFECTIVE_URL", number, CURLINFO_EFFECTIVE_URL);
	LUA_SET_TABLE(L, literal, "INFO_RESPONSE_CODE", number, CURLINFO_RESPONSE_CODE);
	LUA_SET_TABLE(L, literal, "INFO_TOTAL_TIME", number, CURLINFO_TOTAL_TIME);
	LUA_SET_TABLE(L, literal, "INFO_NAMELOOKUP_TIME", number, CURLINFO_NAMELOOKUP_TIME);
	LUA_SET_TABLE(L, literal, "INFO_CONNECT_TIME", number, CURLINFO_CONNECT_TIME);
	LUA_SET_TABLE(L, literal, "INFO_PRETRANSFER_TIME", number, CURLINFO_PRETRANSFER_TIME);
	LUA_SET_TABLE(L, literal, "INFO_SIZE_UPLOAD", number, CURLINFO_SIZE_UPLOAD);
	LUA_SET_TABLE(L, literal, "INFO_SIZE_DOWNLOAD", number, CURLINFO_SIZE_DOWNLOAD);
	LUA_SET_TABLE(L, literal, "INFO_SPEED_DOWNLOAD", number, CURLINFO_SPEED_DOWNLOAD);
	LUA_SET_TABLE(L, literal, "INFO_SPEED_UPLOAD", number, CURLINFO_SPEED_UPLOAD);
	LUA_SET_TABLE(L, literal, "INFO_HEADER_SIZE", number, CURLINFO_HEADER_SIZE);
	LUA_SET_TABLE(L, literal, "INFO_REQUEST_SIZE", number, CURLINFO_REQUEST_SIZE);
	LUA_SET_TABLE(L, literal, "INFO_SSL_VERIFYRESULT", number, CURLINFO_SSL_VERIFYRESULT);
	LUA_SET_TABLE(L, literal, "INFO_FILETIME", number, CURLINFO_FILETIME);
	LUA_SET_TABLE(L, literal, "INFO_CONTENT_LENGTH_DOWNLOAD", number, CURLINFO_CONTENT_LENGTH_DOWNLOAD);
	LUA_SET_TABLE(L, literal, "INFO_CONTENT_LENGTH_UPLOAD", number, CURLINFO_CONTENT_LENGTH_UPLOAD);
	LUA_SET_TABLE(L, literal, "INFO_STARTTRANSFER_TIME", number, CURLINFO_STARTTRANSFER_TIME);
	LUA_SET_TABLE(L, literal, "INFO_CONTENT_TYPE", number, CURLINFO_CONTENT_TYPE);
	LUA_SET_TABLE(L, literal, "INFO_REDIRECT_TIME", number, CURLINFO_REDIRECT_TIME);
	LUA_SET_TABLE(L, literal, "INFO_REDIRECT_COUNT", number, CURLINFO_REDIRECT_COUNT);
	LUA_SET_TABLE(L, literal, "INFO_PRIVATE", number, CURLINFO_PRIVATE);
	LUA_SET_TABLE(L, literal, "INFO_HTTP_CONNECTCODE", number, CURLINFO_HTTP_CONNECTCODE);
	LUA_SET_TABLE(L, literal, "INFO_HTTPAUTH_AVAIL", number, CURLINFO_HTTPAUTH_AVAIL);
	LUA_SET_TABLE(L, literal, "INFO_PROXYAUTH_AVAIL", number, CURLINFO_PROXYAUTH_AVAIL);
	LUA_SET_TABLE(L, literal, "INFO_OS_ERRNO", number, CURLINFO_OS_ERRNO);
	LUA_SET_TABLE(L, literal, "INFO_NUM_CONNECTS", number, CURLINFO_NUM_CONNECTS);
	LUA_SET_TABLE(L, literal, "INFO_SSL_ENGINES", number, CURLINFO_SSL_ENGINES);
	LUA_SET_TABLE(L, literal, "INFO_COOKIELIST", number, CURLINFO_COOKIELIST);
#if CURL_NEWER(7,15,2)
	LUA_SET_TABLE(L, literal, "INFO_LASTSOCKET", number, CURLINFO_LASTSOCKET);
#endif
}

LUACURL_API int luaopen_luacurl (lua_State *L) 
{  
	curl_global_init(CURL_GLOBAL_ALL); /* In windows, this will init the winsock stuff */
	createmeta(L);
	luaL_openlib (L, 0, luacurl_meths, 0);
	luaL_openlib (L, LUACURL_LIBNAME, luacurl_funcs, 0);
	set_info(L);
	setcurlerrors(L);
	setcurloptions(L);
	setcurlvalues(L);
	setcurlinfo(L);
	return 1;
}

#ifdef __cplusplus
}
#endif
