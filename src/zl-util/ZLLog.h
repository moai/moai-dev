// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLLOG_H
#define ZLLOG_H

#if !defined ( ZL_DEFAULT_LOG_LEVEL )
	#define ZL_DEFAULT_LOG_LEVEL 1 // log warnings
#endif

//================================================================//
// ZLLog
//================================================================//
class ZLLog {
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

	static u32		sLogLevel;
	static FILE*	sConsoleRedirect;

	typedef void ( *LogFunc )( cc8* format, va_list args, void* userdata );
	
	static LogFunc	sLogFunc;
	static void*	sLogFuncUserdata;

public:

	//----------------------------------------------------------------//
	static u32		GetLogLevel		();
	
	static bool		IsEnabled		( u32 level );
	
	static void		LogF			( u32 level, FILE* file, cc8* format, ... );
	static void		LogV			( u32 level, FILE* file, cc8* format, va_list args );

	static void		SetLogFunc		( LogFunc logFunc, void* userdata );
	static void		SetLogLevel		( u32 level );
	static void		SetRedirect		( FILE* file );
};

#define ZLLogF(file,format,...)				ZLLog::LogF ( ZLLog::LOG_REPORT, file, format, ##__VA_ARGS__ )
#define ZLLogV(file,format,args)			ZLLog::LogV ( ZLLog::LOG_REPORT, file, format, args )

#define ZLLog_DebugF(file,format,...)		ZLLog::LogF ( ZLLog::LOG_DEBUG, file, format, ##__VA_ARGS__ )
#define ZLLog_DebugV(file,format,args)		ZLLog::LogV ( ZLLog::LOG_DEBUG, file, format, args )

#define ZLLog_StatusF(file,format,...)		ZLLog::LogF ( ZLLog::LOG_STATUS, file, format, ##__VA_ARGS__ )
#define ZLLog_StatusV(file,format,args)		ZLLog::LogV ( ZLLog::LOG_STATUS, file, format, args )

#define ZLLog_WarningF(file,format,...)		ZLLog::LogF ( ZLLog::LOG_WARNING, file, format, ##__VA_ARGS__ )
#define ZLLog_WarningV(file,format,args)	ZLLog::LogV ( ZLLog::LOG_WARNING, file, format, args )

#define ZLLog_ErrorF(file,format,...)		ZLLog::LogF ( ZLLog::LOG_ERROR, file, format, ##__VA_ARGS__ )
#define ZLLog_ErrorV(file,format,args)		ZLLog::LogV ( ZLLog::LOG_ERROR, file, format, args )

#define ZLLog_FatalF(file,format,...)		ZLLog::LogF ( ZLLog::LOG_FATAL, file, format, ##__VA_ARGS__ )
#define ZLLog_FatalV(file,format,args)		ZLLog::LogV ( ZLLog::LOG_FATAL, file, format, args )

#endif
