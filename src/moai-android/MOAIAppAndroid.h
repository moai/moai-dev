// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPPANDROID_H
#define	MOAIAPPANDROID_H

#include <moai-sim/host.h>
#include <moai-core/headers.h>

#include <pthread.h>

//================================================================//
// MOAIAppAndroid
//================================================================//
/**	@name	MOAIAppAndroid
	@text	Wrapper for base application class on Android devices.
			Exposed to Lua via MOAIApp on all mobile platforms.

	@const	SESSION_START			Event code indicating the beginning of an app session.
	@const	SESSION_END				Event code indicating the end of an app sessions.
	@const	BACK_BUTTON_PRESSED		Event code indicating that the physical device back button was pressed.
*/
class MOAIAppAndroid :
	public MOAIGlobalClass < MOAIAppAndroid, MOAILuaObject > {
private:

	enum {
		SESSION_START,
		SESSION_END,
		BACK_BUTTON_PRESSED,
		PICTURE_TAKEN,
		TOTAL,
	};

	MOAILuaRef	mListeners [ TOTAL ];

	//----------------------------------------------------------------//
	static int	_getUTCTime			( lua_State* L );
	static int 	_getStatusBarHeight ( lua_State* L );
	static int	_sendMail			( lua_State* L );
	static int	_setListener		( lua_State* L );
	static int	_share				( lua_State* L );


	// ## CAMERA SUPPORT
	class MutexLocker {
	public:
		MutexLocker( pthread_mutex_t& mutex ) : _mutex(mutex) {
			lock();
		}
		~MutexLocker() {
			pthread_mutex_unlock( &_mutex );
		}
		void lock() {
			if( !_locked ) {
				pthread_mutex_lock( &_mutex );
				_locked = true;
			}
		}
		void unlock() {
			if( !_locked ) {
				pthread_mutex_unlock( &_mutex );
				_locked = false;
			}
		}
		bool isLocked() { return _locked; }
	protected:
	private:
		pthread_mutex_t& _mutex;
		bool _locked;
		MutexLocker();
		MutexLocker( const MutexLocker& src);
		MutexLocker& operator=( const MutexLocker& src);
	};

	static bool 			_pictureTakenFlag;
	static pthread_mutex_t 	_pictureTakenMutex;
	static int _code;

	static int  _takePicture		( lua_State* L );
	// ## /CAMERA SUPPORT

public:

	DECL_LUA_SINGLETON ( MOAIAppAndroid )

			MOAIAppAndroid				();
			~MOAIAppAndroid				();
	bool	NotifyBackButtonPressed		();
	void	NotifyDidStartSession		( bool resumed );
	void	NotifyWillEndSession		();
	void	NotifyPictureTaken			( int code, cc8* path );
	void	NotifyPictureTaken			();
	void	RegisterLuaClass			( MOAILuaState& state );

	void getCamDir();
};

#endif
