// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	RADIXSORT16_H
#define	RADIXSORT16_H

//================================================================//
// USRadixKey16Base
//================================================================//
class USRadixKey16Base {
public:

	u16		mKey;
	
	//----------------------------------------------------------------//
	inline operator u16 () {
		return mKey;
	}
};

//================================================================//
// USRadixKey16
//================================================================//
template < typename TYPE >
class USRadixKey16 :
	public USRadixKey16Base {
public:

	TYPE	mData;
};

//================================================================//
// RadixSort16
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
TYPE* RadixSort16 ( TYPE* keyBuffer, TYPE* swapBuffer, u32 total ) {

	TYPE* bufferA = keyBuffer;
	TYPE* bufferB = swapBuffer;
	TYPE* bufferC = 0;

	// Create the offset tables on the stack
	u16 offsets0 [ 64 ];	// LSB
	u16* offsets1 = &offsets0 [ 16 ];
	u16* offsets2 = &offsets0 [ 32 ];
	u16* offsets3 = &offsets0 [ 48 ];	// MSB

	// Clear the offset tables
	memset ( offsets0, 0, 64 * ( sizeof ( u16 )));

	// Build the histograms
	for ( u32 i = 0; i < total; ++i ) {

		u16 key = bufferA [ i ];
		++offsets0 [( key >> 0x00 ) & 0x0f ];
		++offsets1 [( key >> 0x04 ) & 0x0f ];
		++offsets2 [( key >> 0x08 ) & 0x0f ];
		++offsets3 [( key >> 0x0c ) & 0x0f ];
	}

	// Check to see which passes may be skipped
	u16 anyKey = bufferA [ 0 ];
	bool pass0 = offsets0 [( anyKey >> 0x00 ) & 0x0f ] < total;
	bool pass1 = offsets1 [( anyKey >> 0x04 ) & 0x0f ] < total;
	bool pass2 = offsets2 [( anyKey >> 0x08 ) & 0x0f ] < total;
	bool pass3 = offsets3 [( anyKey >> 0x0c ) & 0x0f ] < total;

	// Build the offsets
	u16 temp;
	u16 sum0 = 0;
	u16 sum1 = 0;
	u16 sum2 = 0;
	u16 sum3 = 0;

	// Each offset is the sum of all previous histogram values
	for ( u32 i = 0; i < 16; ++i ) {

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

	if ( pass0 ) {
		for ( u32 i = 0; i < total; ++i ) {

			key = ((( bufferA [ i ]) >> 0x00 ) & 0x0f );
			offset = offsets0 [ key ]++;
			bufferB [ offset ] = bufferA [ i ];
		}
		bufferC = bufferA;
		bufferA = bufferB;
		bufferB = bufferC;
	}

	if ( pass1 ) {
		for ( u32 i = 0; i < total; ++i ) {

			key = ((( bufferA [ i ]) >> 0x04 ) & 0x0f );
			offset = offsets1 [ key ]++;
			bufferB [ offset ] = bufferA [ i ];
		}
		bufferC = bufferA;
		bufferA = bufferB;
		bufferB = bufferC;
	}

	if ( pass2 ) {
		for ( u32 i = 0; i < total; ++i ) {

			key = ((( bufferA [ i ]) >> 0x08 ) & 0x0f );
			offset = offsets2 [ key ]++;
			bufferB [ offset ] = bufferA [ i ];
		}
		bufferC = bufferA;
		bufferA = bufferB;
		bufferB = bufferC;
	}

	if ( pass3 ) {
		for ( u32 i = 0; i < total; ++i ) {

			key = ((( bufferA [ i ]) >> 0x0c ) & 0x0f );
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

#endif
