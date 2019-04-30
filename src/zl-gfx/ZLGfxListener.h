// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXLISTENER_H
#define ZLGFXLISTENER_H

#include <zl-util/ZLRefCountedObject.h>
#include <zl-util/ZLWeakPtr.h>

class ZLCopyOnWrite;

//================================================================//
// ZLGfxListener
//================================================================//
class ZLGfxListener :
	public virtual ZLRefCountedObject {
private:

	GET ( ZLGfxListener*, HandleTarget, this )

public:

	//----------------------------------------------------------------//
	virtual	void			ZLGfxListener_OnGfxEvent			( u32 event, void* userdata );
	virtual void			ZLGfxListener_OnReadPixels			( const ZLCopyOnWrite& copyOnWrite, void* userdata );
	virtual void			ZLGfxListener_OnUniformLocation		( u32 addr, void* userdata );
							ZLGfxListener						();
	virtual					~ZLGfxListener						();
};

typedef ZLWeakPtr < ZLGfxListener > ZLGfxListenerHandle;

#endif
