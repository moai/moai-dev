// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLLOG_H
#define ZLLOG_H

#include <zl-util/ZLContext.h>

#if !defined ( ZL_DEFAULT_LOG_LEVEL )
	#define ZL_DEFAULT_LOG_LEVEL 1 // log warnings
#endif

typedef void ( *ZLLogFunc )( u32 level, cc8* format, va_list args, void* userdata );

//================================================================//
// ZLLogContext
//================================================================//
class ZLLogContext {
private:

	friend class ZLLog;

	u32			mLogLevel;
	FILE*		mConsoleRedirect;
	ZLLogFunc	mLogFunc;
	void*		mLogFuncUserdata;
	
	//----------------------------------------------------------------//
				ZLLogContext			();
};

//================================================================//
// ZLLog
//================================================================//
class ZLLog :
	public ZLContextClass < ZLLog > {
public:

	enum {
		LOG_DEBUG,
		LOG_STATUS,
		LOG_WARNING,
		LOG_ERROR,
		LOG_FATAL,
		LOG_NONE,
		
		LOG_REPORT,
	};

	static FILE* const	CONSOLE;

private:

	u32			mLogLevel;
	FILE*		mConsoleRedirect;
	ZLLogFunc	mLogFunc;
	void*		mLogFuncUserdata;

public:

	//----------------------------------------------------------------//
	u32			GetLogLevel			();
	bool		IsEnabled			( u32 level );

	void		LogF				( u32 level, FILE* file, cc8* format, ... );
	void		LogV				( u32 level, FILE* file, cc8* format, va_list args );

	void		SetLogFunc			( ZLLogFunc logFunc, void* userdata );
	void		SetLogLevel			( u32 level );
	void		SetRedirect			( FILE* file );
				ZLLog				();
				~ZLLog				();
};

#define ZLLogF(file,format,...)				ZLLog::Get ().LogF ( ZLLog::LOG_REPORT, file, format, ##__VA_ARGS__ )
#define ZLLogV(file,format,args)			ZLLog::Get ().LogV ( ZLLog::LOG_REPORT, file, format, args )

#define ZLLog_Debug(format,...)				ZLLog::Get ().LogF ( ZLLog::LOG_DEBUG, ZLLog::CONSOLE, format, ##__VA_ARGS__ )
#define ZLLog_DebugF(file,format,...)		ZLLog::Get ().LogF ( ZLLog::LOG_DEBUG, file, format, ##__VA_ARGS__ )
#define ZLLog_DebugV(file,format,args)		ZLLog::Get ().LogV ( ZLLog::LOG_DEBUG, file, format, args )

#define ZLLog_Status(format,...)			ZLLog::Get ().LogF ( ZLLog::LOG_STATUS, ZLLog::CONSOLE, format, ##__VA_ARGS__ )
#define ZLLog_StatusF(file,format,...)		ZLLog::Get ().LogF ( ZLLog::LOG_STATUS, file, format, ##__VA_ARGS__ )
#define ZLLog_StatusV(file,format,args)		ZLLog::Get ().LogV ( ZLLog::LOG_STATUS, file, format, args )

#define ZLLog_Warning(format,...)			ZLLog::Get ().LogF ( ZLLog::LOG_WARNING, ZLLog::CONSOLE, format, ##__VA_ARGS__ )
#define ZLLog_WarningF(file,format,...)		ZLLog::Get ().LogF ( ZLLog::LOG_WARNING, file, format, ##__VA_ARGS__ )
#define ZLLog_WarningV(file,format,args)	ZLLog::Get ().LogV ( ZLLog::LOG_WARNING, file, format, args )

#define ZLLog_Error(format,...)				ZLLog::Get ().LogF ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, format, ##__VA_ARGS__ )
#define ZLLog_ErrorF(file,format,...)		ZLLog::Get ().LogF ( ZLLog::LOG_ERROR, file, format, ##__VA_ARGS__ )
#define ZLLog_ErrorV(file,format,args)		ZLLog::Get ().LogV ( ZLLog::LOG_ERROR, file, format, args )

#define ZLLog_Fatal(format,...)				ZLLog::Get ().LogF ( ZLLog::LOG_FATAL, ZLLog::CONSOLE, format, ##__VA_ARGS__ )
#define ZLLog_FatalF(file,format,...)		ZLLog::Get ().LogF ( ZLLog::LOG_FATAL, file, format, ##__VA_ARGS__ )
#define ZLLog_FatalV(file,format,args)		ZLLog::Get ().LogV ( ZLLog::LOG_FATAL, file, format, args )

#endif
