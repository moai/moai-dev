// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>

//================================================================//
// MOAIGfxDeleter
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDeleter::Delete () {

	switch ( this->mType ) {
		
		case DELETE_BUFFER:
			zglDeleteBuffer ( this->mResourceID );
			break;
		
		case DELETE_FRAMEBUFFER:
			zglDeleteFramebuffer ( this->mResourceID );
			break;
		
		case DELETE_PROGRAM:
			zglDeleteProgram ( this->mResourceID );
			break;
		
		case DELETE_SHADER:
			zglDeleteShader ( this->mResourceID );
			break;
		
		case DELETE_TEXTURE:
			zglDeleteTexture ( this->mResourceID );
			break;
		
		case DELETE_RENDERBUFFER:
			zglDeleteRenderbuffer ( this->mResourceID );
			break;
	}
}

//================================================================//
// MOAIGfxResourceMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::InsertGfxResource ( MOAIGfxResource& resource ) {

	this->mResources.PushBack ( resource.mLink );
}

//----------------------------------------------------------------//
MOAIGfxResourceMgr::MOAIGfxResourceMgr () {
}

//----------------------------------------------------------------//
MOAIGfxResourceMgr::~MOAIGfxResourceMgr () {
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::PurgeResources ( u32 age ) {
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::PushDeleter ( u32 type, u32 id ) {

	MOAIGfxDeleter deleter;
	deleter.mType = type;
	deleter.mResourceID = id;
	
	this->mDeleterStack.Push ( deleter );
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::RemoveGfxResource ( MOAIGfxResource& resource ) {

	this->mResources.Remove ( resource.mLink );
	this->mPending.Remove ( resource.mLink );
}

//----------------------------------------------------------------//
// this gets called when the graphics context is renewed
void MOAIGfxResourceMgr::RenewResources () {

	this->mDeleterStack.Reset ();

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Renew ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::ScheduleGPUAffirm ( MOAIGfxResource& resource ) {

	this->mPending.PushBack ( resource.mLink );
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::Update () {

	zglBegin ();
	
	u32 top = this->mDeleterStack.GetTop ();
	for ( u32 i = 0; i < top; ++i ) {
		MOAIGfxDeleter& deleter = this->mDeleterStack [ i ];
		deleter.Delete ();
	}
	this->mDeleterStack.Reset ();
	
	if ( top ) {
		zglFlush ();
	}
	
	ResourceIt resourceIt = this->mPending.Head ();
	while ( resourceIt ) {
		MOAIGfxResource* resource = resourceIt->Data ();
		resourceIt = resourceIt->Next ();
	
		resource->DoGPUAffirm ();
		this->mResources.PushBack ( resource->mLink );
	}
	
	zglEnd ();
}
