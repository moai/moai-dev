// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	RADIXSORT32_H
#define	RADIXSORT32_H

//================================================================//
// USRadixKey32Base
//================================================================//
class USRadixKey32Base {
public:

	u32		mKey;
	
	//----------------------------------------------------------------//
	inline operator u32 () {
		return mKey;
	}
};

//================================================================//
// USRadixKey32
//================================================================//
template < typename TYPE >
class USRadixKey32 :
	public USRadixKey32Base {
public:

	TYPE	mData;
};

//================================================================//
// RadixSort32
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* RadixSort32 ( TYPE* keyBuffer, TYPE* swapBuffer, u32 total ) {

	TYPE* bufferA = keyBuffer;
	TYPE* bufferB = swapBuffer;
	TYPE* bufferC = 0;

	if ( !total ) return bufferA;

	// Create the offset tables on the stack
	u16 offsets0 [ 1024 ];	// LSB
	u16* offsets1 = &offsets0 [ 256 ];
	u16* offsets2 = &offsets0 [ 512 ];
	u16* offsets3 = &offsets0 [ 768 ];	// MSB

	// Clear the offset tables
	memset ( offsets0, 0, 1024 * ( sizeof ( u16 )));

	// Build the histograms; check to see if sort is needed
	bool sort = false;
	u32 prevKey = bufferA [ 0 ];

	for ( u32 i = 0; i < total; ++i ) {

		u32 key = bufferA [ i ];

		++offsets0 [( key >> 0x00 ) & 0xff ];
		++offsets1 [( key >> 0x08 ) & 0xff ];
		++offsets2 [( key >> 0x10 ) & 0xff ];
		++offsets3 [( key >> 0x18 ) & 0xff ];

		if ( key < prevKey ) sort = true;
		prevKey = key;
	}

	if ( sort == false ) return bufferA;

	// Check to see which passes may be skipped
	u32 anyKey = bufferA [ 0 ];
	bool pass0 = offsets0 [( anyKey >> 0x00 ) & 0xff ] < total;
	bool pass1 = offsets1 [( anyKey >> 0x08 ) & 0xff ] < total;
	bool pass2 = offsets2 [( anyKey >> 0x10 ) & 0xff ] < total;
	bool pass3 = offsets3 [( anyKey >> 0x18 ) & 0xff ] < total;

	if ( !( pass0 || pass1 || pass2 || pass3 )) return bufferA;

	// Build the offsets
	u16 temp;
	u16 sum0 = 0;
	u16 sum1 = 0;
	u16 sum2 = 0;
	u16 sum3 = 0;

	// Each offset is the sum of all previous histogram values
	for ( u32 i = 0; i < 256; ++i ) {

		temp = sum0;
		sum0 += offsets0 [ i ];
		offsets0 [ i ] = temp;

		temp = sum1;
		sum1 += offsets1 [ i ];
		offsets1 [ i ] = temp;

		temp = sum2;
		sum2 += offsets2 [ i ];
		offsets2 [ i ] = temp;

		temp = sum3;
		sum3 += offsets3 [ i ];
		offsets3 [ i ] = temp;
	}

	// Do the sort
	u32 key;
	u16 offset;

	// Pass 0: A to B
	if ( pass0 ) {
		for ( u32 i = 0; i < total; ++i ) {

			key = (( bufferA [ i ] >> 0x00 ) & 0xff );
			offset = offsets0 [ key ]++;
			bufferB [ offset ] = bufferA [ i ];
		}
		bufferC = bufferA;
		bufferA = bufferB;
		bufferB = bufferC;
	}

	// Pass 1: B to A
	if ( pass1 ) {
		for ( u32 i = 0; i < total; ++i ) {

			key = (( bufferA [ i ] >> 0x08 ) & 0xff );
			offset = offsets1 [ key ]++;
			bufferB [ offset ] = bufferA [ i ];
		}
		bufferC = bufferA;
		bufferA = bufferB;
		bufferB = bufferC;
	}

	// Pass 2: A to B
	if ( pass2 ) {
		for ( u32 i = 0; i < total; ++i ) {

			key = (( bufferA [ i ] >> 0x10 ) & 0xff );
			offset = offsets2 [ key ]++;
			bufferB [ offset ] = bufferA [ i ];
		}
		bufferC = bufferA;
		bufferA = bufferB;
		bufferB = bufferC;
	}

	// Pass 3: B to A
	if ( pass3 ) {
		for ( u32 i = 0; i < total; ++i ) {

			key = (( bufferA [ i ] >> 0x18 ) & 0xff );
			offset = offsets3 [ key ]++;
			bufferB [ offset ] = bufferA [ i ];
		}
		bufferC = bufferA;
		bufferA = bufferB;
		bufferB = bufferC;
	}

	// And magically... we're done!
	return bufferA;
}

//----------------------------------------------------------------//
template < typename TYPE >
void RadixSort32 ( TYPE* keyBuffer, u32 total ) {

	if ( !total ) return;

	static const u32 STACK_BUFFER_SIZE = 1024;
	TYPE stackBuffer [ STACK_BUFFER_SIZE ];
	
	TYPE* buffer = 0;
	TYPE* swap = stackBuffer;
	
	if ( total > STACK_BUFFER_SIZE ) {
		buffer = ( TYPE* )malloc ( total * sizeof ( TYPE ));
		swap = buffer;
	}

	TYPE* result = RadixSort32 ( keyBuffer, swap, total );

	if ( keyBuffer != result ) {
		for ( u32 i = 0; i < total; ++i ) {
			keyBuffer [ i ] = result [ i ];
		}
	}

	if ( buffer ) {
		free ( buffer );
	}
}

#endif
