// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLINDEXBUFFERGL_H
#define	ZLINDEXBUFFERGL_H

#include <zl-gfx/ZLGfxBufferGL.h>

//================================================================//
// ZLIndexBufferGL
//================================================================//
/**	@lua	ZLIndexBufferGL
	@text	Buffer for vertex indices.
*/
class ZLIndexBufferGL :
	public ZLGfxBufferGL {
protected:

	u32				mIndexSize;
	
public:
	
	GET ( u32, IndexSize, mIndexSize )
		
	//----------------------------------------------------------------//
	u32				CountIndices			();
	void			CopyFromStream			( ZLStream& stream, size_t size, u32 srcInputSizeInBytes );
	u32				GetIndex				( u32 element );
	void			PrintIndices			();
	void			SetIndexSize			( u32 idxSize );
	void			WriteIndex				( u32 index );
					ZLIndexBufferGL			();
					~ZLIndexBufferGL		();
};

#endif
