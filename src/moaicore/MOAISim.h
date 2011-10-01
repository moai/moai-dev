// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISIM_H
#define	MOAISIM_H

#include <moaicore/MOAIEaseDriver.h>

class MOAIProp2D;

//================================================================//
// MOAISim
//================================================================//
/**	@name	MOAISim
	@text	Sim timing and settings class.
	
	@const	EVENT_FINALIZE
*/
class MOAISim :
	public USGlobalClass < MOAISim, MOAIEventSource > {
private:

	// timer state
	enum {
		RUNNING,
		PAUSED,
		START,
	};

	// events
	enum {
		EVENT_FINALIZE,
	};

	u32				mLoopState;

	double			mStep;			// simulation step size
	double			mSimTime;		// elapsed simulation running time (in seconds)
	double			mBaseTime;		// subtracted from real time
	double			mFrameTime;		// time last frame time was measured (in seconds)
	
	u32				mRenderCounter;	// increments every render
	
	static const u32 FPS_BUFFER_SIZE = 30;
	float			mFrameRate;
	float			mFrameRateBuffer [ FPS_BUFFER_SIZE ];
	u32				mFrameRateIdx;

	typedef USList < MOAIProp2D* >::Iterator RenderPassIt;
	USList < MOAIProp2D* > mRenderPasses;
	
	USCallback < int > mLoadingScreenCallback;
	USCallback < int > mTextInputCallback;
	
	bool			mNeedsInit;
	STLString		mInitFromFilename;
	STLString		mInitFromString;
	
	GLbitfield		mClearFlags;
	u32				mClearColor;
	
	USTaskThread	mDataIOThread;
	
	u32				mLoopFlags;
	double			mBoostThreshold;
	
	bool			mLongLoadFlag;
	u32				mFramesToDo;
	
	//----------------------------------------------------------------//
	static int		_clearRenderStack			( lua_State* L );
	static int		_enterFullscreenMode		( lua_State* L );
	static int		_exitFullscreenMode			( lua_State* L );
	static int		_flagLongLoad				( lua_State* L );
	static int		_forceGarbageCollection		( lua_State* L );
	static int		_framesToTime				( lua_State* L );
	static int		_getDeviceSize				( lua_State* L );
	static int		_getDeviceTime				( lua_State* L );
	static int		_getElapsedFrames			( lua_State* L );
	static int		_getElapsedTime				( lua_State* L );
	static int		_getFrameSize				( lua_State* L );
	static int		_getLuaObjectCount			( lua_State* L );
	static int		_getMemoryUsage				( lua_State* L );
	static int		_getNetworkStatus			( lua_State* L );
	static int		_getPerformance				( lua_State* L );
	static int		_openWindow					( lua_State* L );
	static int		_pauseTimer					( lua_State* L );
	static int		_popRenderPass				( lua_State* L );
	static int		_pushRenderPass				( lua_State* L );
	static int		_reportHistogram			( lua_State* L );
	static int		_reportLeaks				( lua_State* L );
	static int		_setBoostThreshold			( lua_State* L );
	static int		_setClearColor				( lua_State* L );
	static int		_setClearDepth				( lua_State* L );
	static int		_setFrameSize				( lua_State* L );
	static int		_setFramesToDo				( lua_State* L );
	static int		_setHistogramEnabled		( lua_State* L );
	static int		_setLeakTrackingEnabled		( lua_State* L );
	static int		_setLoopFlags				( lua_State* L );
	static int		_setLuaAllocLogEnabled		( lua_State* L );
	static int		_timeToFrames				( lua_State* L );

	//----------------------------------------------------------------//
	void			MeasureFrameRate			();
	double			StepSim						( double step );

public:
	
	enum {
		SIM_LOOP_ALLOW_BOOST		= 0x01,		// allow a variable time step 'boost' if sim time falls behind
		SIM_LOOP_ALLOW_SPIN			= 0x02,		// spins the update loop to use up any excess time available
	};
	
	DECL_LUA_SINGLETON ( MOAISim )
	
	GET ( USTaskThread&, DataIOThread, mDataIOThread )
	GET ( u32, RenderCounter, mRenderCounter )
	GET ( double, Step, mStep )
	
	static const u32 DEFAULT_LOOP_FLAGS = SIM_LOOP_ALLOW_SPIN;
	static const double DEFAULT_BOOST_THRESHOLD;
	
	//----------------------------------------------------------------//
	void			Clear						();
					MOAISim						();
					~MOAISim					();
	void			PauseMOAI					();
	void			PopRenderPass				();
	void			PushRenderPass				( MOAIProp2D* prop );
	void			RegisterLuaClass			( USLuaState& state );
	void			RegisterLuaFuncs			( USLuaState& state );
	void			Render						();
	void			ResumeMOAI					();
	void			RunFile						( cc8* filename );
	void			RunString					( cc8* script );
	void			SendFinalizeEvent			();
	void			Update						();
	STLString		ToString					();
};

#endif
