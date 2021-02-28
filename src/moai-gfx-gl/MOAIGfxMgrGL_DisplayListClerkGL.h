// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_DISPLAYLISTCLERKGL_H
#define	MOAIGFXMGRGL_DISPLAYLISTCLERKGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>

#define GFX_PIPELINE_LOGGING_FOLDER "renderlogs"

//================================================================//
// MOAIDisplayListQueueGL
//================================================================//
class MOAIDisplayListQueueGL {
private:

	friend class MOAIGfxMgrGL_DisplayListClerkGL;

	ZLLeanArray < ZLGfxRetained* > mDisplayLists;
	ZLLeanStack < ZLGfxRetained* > mFreeDisplayLists;
	ZLLeanStack < ZLGfxRetained* > mFinishedDisplayLists;

	enum {
		QUEUE_FOR_CPU,			// busy on the main thread
		QUEUE_PENDING,			// waiting for the graphics thread
		QUEUE_FOR_GPU,			// busy on the graphics thread
		QUEUE_TOTAL_STAGES,
	};

	enum {
		CPU_PHASE,
		GPU_PHASE,
	};

	ZLGfxRetained* mQueue [ QUEUE_TOTAL_STAGES ];

	size_t		mRenderCount;
	bool		mEnableLogging;

	//----------------------------------------------------------------//
	ZLGfxRetained*			GetDisplayList				();
	bool					HasContent					();
	void					PhaseBegin					( u32 phase );
	void					PhaseEnd					( u32 phase );
	void					PublishAndReset				();
	void					ReleaseDisplayList			( ZLGfxRetained* list );
							MOAIDisplayListQueueGL		();
							~MOAIDisplayListQueueGL		();
};

//================================================================//
// MOAIGfxMgrGL_DisplayListClerkGL
//================================================================//
class MOAIGfxMgrGL_DisplayListClerkGL :
	public virtual MOAIGfxMgrGLComponents {
public:

	enum {
		DRAWING_QUEUE,
		LOADING_QUEUE,
		TOTAL_QUEUES,
	};

protected:

	ZLGfxImmediate		mGfxImmediate;

	MOAIDisplayListQueueGL*	mQueues [ TOTAL_QUEUES ];
	
	ZLGfx*				mDrawingAPI;
	
	u32					mDrawCount;
	u32					mQueueRenderCount;
	
	bool				mEnableQueueLogging;

	//----------------------------------------------------------------//
	void				BeginPhase					( u32 list, u32 phase );
	void				EndPhase					( u32 list, u32 phase );
	void				LogQueueRender				( ZLGfxRetained& gfx, size_t renderCount, cc8* name );

public:
	
	enum {
		LOGIC_PHASE,
		LOADING_PHASE,
		RENDER_PHASE,
	};
	
	GET ( u32, DrawCount, mDrawCount )
	GET ( ZLGfx&, DrawingAPI, mDrawingAPI ? *mDrawingAPI : mGfxImmediate )
	
	//----------------------------------------------------------------//
	void				BeginPhase							( u32 phase );
	void				EnableQueue							( u32 queueID );
	void				EnableQueueLogging					( bool enable );
	void				EndPhase							( u32 phase );
	bool				HasContent							( u32 queueID );
	bool				IsQueueEnabled						( u32 queueID );
						MOAIGfxMgrGL_DisplayListClerkGL		( ZLContext& context );
	virtual				~MOAIGfxMgrGL_DisplayListClerkGL	();
	void				ProcessQueue						( u32 queueID );
	void				PublishAndReset						( u32 queueID );
	void				ResetDrawingAPIs					();
	ZLGfx*				SelectDrawingAPI					();
	ZLGfx*				SelectDrawingAPI					( u32 queueID );
};

#endif
