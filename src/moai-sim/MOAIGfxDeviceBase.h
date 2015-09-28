// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICEBASE_H
#define	MOAIGFXDEVICEBASE_H

class MOAIIndexBuffer;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIGfxThreadPipeline
//================================================================//
class MOAIGfxThreadPipeline {
friend class MOAIGfxDeviceBase;

	ZLLeanArray < ZLGfxRetained* > mDisplayLists;
	ZLLeanStack < ZLGfxRetained* > mFreeDisplayLists;
	ZLLeanStack < ZLGfxRetained* > mFinishedDisplayLists;

	enum {
		PIPELINE_LOGIC,			// busy on the main thread
		PIPELINE_PENDING,		// waiting for the graphics thread
		PIPELINE_RENDER,		// busy for the graphics thread
		PIPELINE_TOTAL,
	};

	enum {
		LOGIC_PHASE,
		RENDER_PHASE,
	};

	ZLGfxRetained* mPipeline [ PIPELINE_TOTAL ];
	bool mHasContent;

	size_t mResetPoint;

	//----------------------------------------------------------------//
	ZLGfxRetained*		GetList						();
	bool				HasContent					();
						MOAIGfxThreadPipeline		();
						~MOAIGfxThreadPipeline		();
	void				PhaseBegin					( u32 phase );
	void				PhaseEnd					( u32 phase );
	void				PublishAndReset				();
	void				ReleaseList					( ZLGfxRetained* list );
	void				UpdateResetPoint			();
};

//================================================================//
// MOAIGfxDeviceBase
//================================================================//
class MOAIGfxDeviceBase {
public:

	enum {
		DRAWING_LIST,
		LOADING_LIST,
		TOTAL_LISTS,
	};

protected:

	ZLGfxImmediate				mGfxImmediate;

	MOAIGfxThreadPipeline*		mLists [ TOTAL_LISTS ];
	
	ZLGfx*						mDrawingAPI;
	MOAIGfxThreadPipeline*		mPipeline;
	
	bool						mShaderDirty;
	ZLRect						mViewRect;
	ZLFrustum					mViewVolume;
	
	u32							mDrawCount;

	//----------------------------------------------------------------//
	void				BeginPhase					( u32 list, u32 phase );
	void				EndPhase					( u32 list, u32 phase );
	virtual void		UpdateShaderGlobals			() = 0;

public:
	
	enum {
		LOGIC_PHASE,
		LOADING_PHASE,
		RENDER_PHASE,
	};
	
	GET ( const ZLFrustum&, ViewVolume, mViewVolume )
	GET ( const ZLRect&, ViewRect, mViewRect )
	
	GET ( u32, DrawCount, mDrawCount )
	
	//GET ( ZLGfx&, API, *mGfx )
	
	//----------------------------------------------------------------//
	void				BeginPhase					( u32 phase );
	void				EnableList					( u32 list );
	void				EndPhase					( u32 phase );
	bool				HasContent					( u32 list );
	bool				IsListEnabled				( u32 list );
						MOAIGfxDeviceBase			();
	virtual				~MOAIGfxDeviceBase			();
	void				ProcessList					( u32 list );
	void				PublishAndResetList			( u32 list );
	void				SelectList					();
	void				SelectList					( u32 list );
	void				UpdateResetPoint			();
};

#endif
