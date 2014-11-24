// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBITBUFFER_H
#define ZLBITBUFFER_H

//================================================================//
// ZLBitBuffer
//================================================================//
// legal bit depths are 1, 4, 8, 16, 24, and 32
namespace ZLBitBuffer {
	
	//----------------------------------------------------------------//
	void				Blit						( void* dest, size_t destX, const void* src, size_t srcX, size_t n, u32 bitDepth );
	size_t				CalculateSize				( u32 bitDepth, size_t n );
	void				Clear						( void* buffer, u32 bitDepth, size_t x, size_t n );
	u32					GetMask						( u32 bitDepth );
	u32					GetValue					( const void* buffer, size_t x, u32 bitDepth );
	bool				IsLegalBitDepth				( u32 bitDepth );
	void				SetValue					( void* buffer, u32 value, size_t x, u32 bitDepth );
	void				Test						();
};

#endif
