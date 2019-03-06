// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLVERTEXBUFFER_H
#define	ZLVERTEXBUFFER_H

#include <zl-gfx/ZLGfxBuffer.h>

class ZLVertexFormat;

//================================================================//
// ZLVertexBuffer
//================================================================//
class ZLVertexBuffer :
	public ZLGfxBuffer {
public:
	
	friend class ZLVertexBufferWithFormat;
	friend class ZLVertexArray;
		
	//----------------------------------------------------------------//
	void			PrintVertices			( ZLVertexFormat& vertexFormat );
					ZLVertexBuffer			();
					~ZLVertexBuffer			();
};

#endif
