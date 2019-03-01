// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/ZLIndexBuffer.h>

//================================================================//
// ZLIndexBuffer
//================================================================//

//----------------------------------------------------------------//
u32 ZLIndexBuffer::CountIndices () {

	return this->GetLength () / this->mIndexSize;
}

//----------------------------------------------------------------//
void ZLIndexBuffer::CopyFromStream ( ZLStream& stream, size_t size, u32 srcInputSizeInBytes ) {

	u32 idxSizeInBytes = this->mIndexSize;	
	u32 totalIndices = ( u32 )( size / srcInputSizeInBytes );
	
	this->Reserve ( totalIndices * idxSizeInBytes );
	
	for ( u32 i = 0; i < totalIndices; ++i ) {
	
		u32 idx = 0;
		
		if ( srcInputSizeInBytes == 4 ) {
			idx = stream.Read < u32 >( 0 );
		}
		else {
			idx = stream.Read < u16 >( 0 );
		}
	
		if ( idxSizeInBytes == 4 ) {
			this->Write < u32 >( idx );
		}
		else {
			this->Write < u16 >(( u16 )idx );
		}
	}
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
u32 ZLIndexBuffer::GetIndex ( u32 element ) {

	const void* data = this->ZLCopyOnWrite::GetBuffer ();
	return this->mIndexSize == 4 ? (( const u32* )data )[ element ] : (( const u16* )data )[ element ];
}

//----------------------------------------------------------------//
void ZLIndexBuffer::PrintIndices () {

	size_t cursor = this->GetCursor ();
	u32 length = ( u32 )( cursor / this->mIndexSize );
	
	if ( length ) {
		this->Seek ( 0, SEEK_SET );
		for ( u32 i = 0; i < length; ++i ) {
		
			u32 v = this->mIndexSize == 4 ? this->Read < u32 >( 0 ) : ( u32 )this->Read < u16 >( 0 );
			printf ( "%d: %d\n", i, v );
		}
		this->SetCursor ( cursor );
	}
}

//----------------------------------------------------------------//
void ZLIndexBuffer::SetIndexSize ( u32 idxSize ) {

	assert (( idxSize == 2 ) || ( idxSize == 4 ));
	this->mIndexSize = idxSize;
}

//----------------------------------------------------------------//
void ZLIndexBuffer::WriteIndex ( u32 index ) {

	if ( this->mIndexSize == 4 ) {
		this->Write < u32 >( index );
	}
	else {
		this->Write < u16 >(( u16 )index );
	}
}

//----------------------------------------------------------------//
ZLIndexBuffer::ZLIndexBuffer () :
	mIndexSize ( 2 ) {
	
	this->mTarget = ZGL_BUFFER_TARGET_ELEMENT_ARRAY;
}

//----------------------------------------------------------------//
ZLIndexBuffer::~ZLIndexBuffer () {
}
