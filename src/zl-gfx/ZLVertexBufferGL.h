// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLVERTEXBUFFERGL_H
#define	ZLVERTEXBUFFERGL_H

#include <zl-gfx/ZLGfxBufferGL.h>

class ZLVertexFormatGL;

//================================================================//
// ZLVertexBufferGL
//================================================================//
class ZLVertexBufferGL :
	public ZLGfxBufferGL {
public:
	
	friend class ZLVertexBufferWithFormatGL;
	friend class ZLVertexArrayGL;
		
	//----------------------------------------------------------------//
	void			PrintVertices			( ZLVertexFormatGL& vertexFormat );
					ZLVertexBufferGL		();
					~ZLVertexBufferGL		();
};

#endif
