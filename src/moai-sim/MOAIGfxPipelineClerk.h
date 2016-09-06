// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXPIPELINECLERK_H
#define	MOAIGFXPIPELINECLERK_H

class MOAIIndexBuffer;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;

#define GFX_PIPELINE_LOGGING_FOLDER "renderlogs"

//================================================================//
// MOAIGfxPipeline
//================================================================//
class MOAIGfxPipeline {
private:

	friend class MOAIGfxPipelineClerk;

	ZLLeanArray < ZLGfxRetained* > mDisplayLists;
	ZLLeanStack < ZLGfxRetained* > mFreeDisplayLists;
	ZLLeanStack < ZLGfxRetained* > mFinishedDisplayLists;

	enum {
		PIPELINE_CPU,			// busy on the main thread
		PIPELINE_PENDING,		// waiting for the graphics thread
		PIPELINE_GPU,			// busy on the graphics thread
		PIPELINE_TOTAL_STAGES,
	};

	enum {
		CPU_PHASE,
		GPU_PHASE,
	};

	ZLGfxRetained* mPipeline [ PIPELINE_TOTAL_STAGES ];

	size_t		mRenderCount;
	bool		mEnableLogging;

	//----------------------------------------------------------------//
	ZLGfxRetained*			GetDisplayList				();
	bool					HasContent					();
							MOAIGfxPipeline				();
							~MOAIGfxPipeline			();
	void					PhaseBegin					( u32 phase );
	void					PhaseEnd					( u32 phase );
	void					PublishAndReset				();
	void					ReleaseDisplayList			( ZLGfxRetained* list );
};

//================================================================//
// MOAIGfxPipelineClerk
//================================================================//
class MOAIGfxPipelineClerk {
public:

	enum {
		DRAWING_PIPELINE,
		LOADING_PIPELINE,
		TOTAL_PIPELINES,
	};

protected:

	ZLGfxImmediate				mGfxImmediate;

	MOAIGfxPipeline*			mPipelines [ TOTAL_PIPELINES ];
	
	ZLGfx*						mDrawingAPI;
	
	u32							mDrawCount;
	u32							mPipelineRenderCount;
	
	bool						mEnablePipelineLogging;

	//----------------------------------------------------------------//
	void				BeginPhase					( u32 list, u32 phase );
	void				EndPhase					( u32 list, u32 phase );
	void				LogPipelineRender			( ZLGfxRetained& gfx, size_t renderCount, cc8* name );

public:
	
	enum {
		LOGIC_PHASE,
		LOADING_PHASE,
		RENDER_PHASE,
	};
	
	GET ( u32, DrawCount, mDrawCount )
	GET ( ZLGfx*, DrawingAPI, mDrawingAPI )
	
	//----------------------------------------------------------------//
	void				BeginPhase					( u32 phase );
	void				EnablePipeline				( u32 pipelineID );
	void				EnablePipelineLogging		( bool enable );
	void				EndPhase					( u32 phase );
	bool				HasContent					( u32 pipelineID );
	bool				IsPipelineEnabled			( u32 pipelineID );
						MOAIGfxPipelineClerk		();
	virtual				~MOAIGfxPipelineClerk		();
	void				ProcessPipeline				( u32 pipelineID );
	void				PublishAndReset				( u32 pipelineID );
	void				ResetDrawingAPIs			();
	ZLGfx*				SelectDrawingAPI			();
	ZLGfx*				SelectDrawingAPI			( u32 pipelineID );
};

#endif
