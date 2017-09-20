// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLSample.h>
#include <zl-util/ZLStream.h>

//================================================================//
// ZLSample
//================================================================//

//----------------------------------------------------------------//
size_t ZLSample::Convert ( u32 sourceType, const void* source, u32 destType, void* dest ) {

	assert (( sourceType != SAMPLE_UNKNOWN ) && ( destType != SAMPLE_UNKNOWN ));

	size_t sourceSize = ZLSample::GetSize ( sourceType );
	
	if ( sourceType == destType ) {
		memcpy ( dest, source, sourceSize );
		return sourceSize;
	}
	
	float sample = 0.0f;
	
	switch ( sourceType ) {
		
		case SAMPLE_S8:
			sample = (( float )*( s8* )source ) / ( float )0x7f;
			break;
		
		case SAMPLE_U8:
			sample = ((( float )*( u8* )source ) / ( float )0x7f ) - 1.0f;
			break;

		case SAMPLE_S16:
			sample = (( float )*( s16* )source ) / ( float )0x7fff;
			break;
		
		case SAMPLE_U16:
			sample = ((( float )*( u16* )source ) / ( float )0x7fff ) - 1.0f;
			break;
	
		case SAMPLE_S32:
			sample = (( float )*( s32* )source ) / ( float )0x7fffffff;
			break;
		
		case SAMPLE_U32:
			sample = ((( float )*( u32* )source ) / ( float )0x7fffffff ) - 1.0f;
			break;
		
		case SAMPLE_FLOAT:
			sample = *( float* )source;
			break;
	}
	
	switch ( destType ) {
		
		case SAMPLE_S8:
			*( s8* )dest = ( s8 )( sample * ( float )0x7f );
			return 1;
		
		case SAMPLE_U8:
			*( u8* )dest = ( u8 )(( sample + 1.0f ) * ( float )0x7f );
			return 1;

		case SAMPLE_S16:
			*( s16* )dest = ( s16 )( sample * ( float )0x7fff );
			return 2;
		
		case SAMPLE_U16:
			*( u16* )dest = ( u16 )(( sample + 1.0f ) * ( float )0x7fff );
			return 2;
	
		case SAMPLE_S32:
			*( s32* )dest = ( s32 )( sample * ( float )0x7fffffff );
			return 4;
		
		case SAMPLE_U32:
			*( u32* )dest = ( u32 )(( sample + 1.0f ) * ( float )0x7fffffff );
			return 4;
		
		case SAMPLE_FLOAT:
			*( float* )dest = sample;
			return 4;
	}

	return 0;
}

//----------------------------------------------------------------//
size_t ZLSample::GetSize ( u32 type ) {

	switch ( type ) {
	
		case SAMPLE_S8:
		case SAMPLE_U8:
			return 8;
		
		case SAMPLE_S16:
		case SAMPLE_U16:
			return 2;
		
		case SAMPLE_S32:
		case SAMPLE_U32:
		case SAMPLE_FLOAT:
			return 4;
		
		case SAMPLE_UNKNOWN:
		default:
			return 0;
	}
}

//----------------------------------------------------------------//
size_t ZLSample::ReadSample ( ZLStream& stream, u32 streamType, void* buffer, u32 bufferType ) {

	size_t result = stream.ReadBytes ( buffer, ZLSample::GetSize ( streamType ));
	if ( result ) {
		ZLSample::Convert ( streamType, buffer, bufferType, buffer );
	}
	return result;
}

//----------------------------------------------------------------//
size_t ZLSample::WriteSample ( ZLStream& stream, u32 streamType, const void* buffer, u32 bufferType ) {

	if ( streamType == bufferType ) {
		return stream.WriteBytes ( buffer, ZLSample::GetSize ( streamType ));
	}
	u8 temp [ SAMPLE_BUFFER_SIZE ];
	size_t size = ZLSample::Convert ( bufferType, buffer, streamType, temp );
	return stream.WriteBytes ( temp, size );
}
