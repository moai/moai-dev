// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USGFXDEVICE_H
#define	USGFXDEVICE_H

//================================================================//
// USGfxDevice
//================================================================//
class USGfxDevice :
	public USGlobalClass < USGfxDevice > {
private:
	
	u32			mWidth;
	u32			mHeight;
	
	//----------------------------------------------------------------//
	
public:
	
	enum {
		GL_PIPELINE_FIXED,
		GL_PIPELINE_PROGRAMMABLE,
	};
	
	//----------------------------------------------------------------//
	void		ClearErrors				();
	u32			CountErrors				();
	u32			GetHeight				();
	u32			GetPipelineMode			();
	USRect		GetRect					();
	u32			GetWidth				();
	cc8*		GetErrorString			( int error );
	u32			LogErrors				();
	u32			PrintErrors				();
	void		SetSize					( u32 width, u32 height );
				USGfxDevice				();
				~USGfxDevice			();
};

#endif
