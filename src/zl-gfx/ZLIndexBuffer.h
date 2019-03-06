// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLINDEXBUFFER_H
#define	ZLINDEXBUFFER_H

#include <zl-gfx/ZLGfxBuffer.h>

//================================================================//
// ZLIndexBuffer
//================================================================//
/**	@lua	ZLIndexBuffer
	@text	Buffer for vertex indices.
*/
class ZLIndexBuffer :
	public ZLGfxBuffer {
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
					ZLIndexBuffer			();
					~ZLIndexBuffer			();
};

#endif
