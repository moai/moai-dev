// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLVERTEXBUFFER_H
#define	ZLVERTEXBUFFER_H

#include <moai-sim/ZLGfxBuffer.h>

class MOAIVertexFormat;

//================================================================//
// ZLVertexBuffer
//================================================================//
class ZLVertexBuffer :
	public ZLGfxBuffer {
private:
	
	friend class ZLVertexBufferWithFormat;
	
public:
		
	//----------------------------------------------------------------//
	void			PrintVertices			( MOAIVertexFormat& vertexFormat );
					ZLVertexBuffer			();
					~ZLVertexBuffer			();
};

#endif
