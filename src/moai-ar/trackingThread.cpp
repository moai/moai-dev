//
//  trackingSub.c
//  ARToolKit NFT.
//
//  Disclaimer: IMPORTANT:  This Daqri software is supplied to you by Daqri
//  LLC ("Daqri") in consideration of your agreement to the following
//  terms, and your use, installation, modification or redistribution of
//  this Daqri software constitutes acceptance of these terms.  If you do
//  not agree with these terms, please do not use, install, modify or
//  redistribute this Daqri software.
//
//  In consideration of your agreement to abide by the following terms, and
//  subject to these terms, Daqri grants you a personal, non-exclusive
//  license, under Daqri's copyrights in this original Daqri software (the
//  "Daqri Software"), to use, reproduce, modify and redistribute the Daqri
//  Software, with or without modifications, in source and/or binary forms;
//  provided that if you redistribute the Daqri Software in its entirety and
//  without modifications, you must retain this notice and the following
//  text and disclaimers in all such redistributions of the Daqri Software.
//  Neither the name, trademarks, service marks or logos of Daqri LLC may
//  be used to endorse or promote products derived from the Daqri Software
//  without specific prior written permission from Daqri.  Except as
//  expressly stated in this notice, no other rights or licenses, express or
//  implied, are granted by Daqri herein, including but not limited to any
//  patent rights that may be infringed by your derivative works or by other
//  works in which the Daqri Software may be incorporated.
//
//  The Daqri Software is provided by Daqri on an "AS IS" basis.  DAQRI
//  MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
//  THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
//  FOR A PARTICULAR PURPOSE, REGARDING THE DAQRI SOFTWARE OR ITS USE AND
//  OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
//
//  IN NO EVENT SHALL DAQRI BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
//  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
//  MODIFICATION AND/OR DISTRIBUTION OF THE DAQRI SOFTWARE, HOWEVER CAUSED
//  AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
//  STRICT LIABILITY OR OTHERWISE, EVEN IF DAQRI HAS BEEN ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
//  Copyright 2015 Daqri LLC. All Rights Reserved.
//  Copyright 2010-2015 ARToolworks, Inc. All rights reserved.
//
//  Author(s): Hirokazu Kato, Philip Lamb
//

#include "trackingThread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//================================================================//
// TrackingThreadHandle
//================================================================//
typedef struct {
	KpmHandle*		mKPMHandle;				// KPM-related data.
	ARUint8*		mImagePtr;				// Pointer to image being tracked.
	int				mImageSize;				// Bytes per image.
	
} TrackingThreadHandle;

//----------------------------------------------------------------//
void* trackingThreadMain ( THREAD_HANDLE_T* threadHandle );

//----------------------------------------------------------------//
bool trackingThreadIsBusy ( THREAD_HANDLE_T* threadHandle ) {

	if ( threadGetStatus ( threadHandle ) == 0 ) return true;
    threadEndWait ( threadHandle );
	return false;
}

//----------------------------------------------------------------//
THREAD_HANDLE_T* trackingThreadInit ( KpmHandle *mKPMHandle ) {

    TrackingThreadHandle  *trackingThreadHandle;
    THREAD_HANDLE_T     *threadHandle;

    if ( !mKPMHandle ) {
        ARLOGe ( "trackingThreadInit(): Error: NULL KpmHandle.\n" );
        return ( NULL );
    }
    
    trackingThreadHandle = ( TrackingThreadHandle* )malloc(  sizeof ( TrackingThreadHandle ));
    if( trackingThreadHandle == NULL ) return NULL;
    trackingThreadHandle->mKPMHandle	= mKPMHandle;
    trackingThreadHandle->mImageSize	= kpmHandleGetXSize ( mKPMHandle ) * kpmHandleGetYSize ( mKPMHandle ) * arUtilGetPixelSize ( kpmHandleGetPixelFormat ( mKPMHandle ));
    trackingThreadHandle->mImagePtr		= (ARUint8* )malloc ( trackingThreadHandle->mImageSize );

    threadHandle = threadInit ( 0, trackingThreadHandle, trackingThreadMain );
    return threadHandle;
}

//----------------------------------------------------------------//
void* trackingThreadMain ( THREAD_HANDLE_T *threadHandle ) {

    TrackingThreadHandle*	trackingThreadHandle;
    KpmHandle*				kpmHandle;
    KpmResult*				kpmResult = NULL;
    int						kpmResultNum;
    ARUint8*				imagePtr;
    float					err;
    int						i, j, k;

    if ( !threadHandle ) {
        ARLOGe ( "Error starting tracking thread: empty THREAD_HANDLE_T.\n" );
        return ( NULL );
    }
	
    trackingThreadHandle = ( TrackingThreadHandle* )threadGetArg ( threadHandle );
    if ( !threadHandle ) {
        ARLOGe ( "Error starting tracking thread: empty trackingThreadHandle.\n" );
        return ( NULL );
    }
	
    kpmHandle		= trackingThreadHandle->mKPMHandle;
    imagePtr		= trackingThreadHandle->mImagePtr;
    if ( !kpmHandle || !imagePtr ) {
        ARLOGe ( "Error starting tracking thread: empty mKPMHandle/mImagePtr.\n" );
        return ( NULL );
    }
    ARLOGi ( "Start tracking thread.\n" );
		
    for ( ;; ) {
        if ( threadStartWait ( threadHandle ) < 0 ) break;
        kpmMatching ( kpmHandle, imagePtr );
        threadEndSignal ( threadHandle );
    }

    ARLOGi ( "End tracking thread.\n" );
    return ( NULL );
}

//----------------------------------------------------------------//
int trackingThreadQuit ( THREAD_HANDLE_T** threadHandle_p ) {

    TrackingThreadHandle* trackingThreadHandle;

    if ( !threadHandle_p )  {
        ARLOGe ( "trackingThreadQuit(): Error: NULL threadHandle_p.\n" );
        return ( -1) ;
    }
    if ( !*threadHandle_p ) return 0;
    
    threadWaitQuit ( *threadHandle_p );
    trackingThreadHandle = ( TrackingThreadHandle* )threadGetArg ( *threadHandle_p );
    if ( trackingThreadHandle ) {
        free ( trackingThreadHandle->mImagePtr );
        free ( trackingThreadHandle );
    }
    threadFree ( threadHandle_p );
    return 0;
}

//----------------------------------------------------------------//
int trackingThreadStart ( THREAD_HANDLE_T *threadHandle, ARUint8 *mImagePtr ) {

    TrackingThreadHandle* trackingThreadHandle;

    if ( !threadHandle || !mImagePtr ) {
        ARLOGe ( "trackingThreadStart(): Error: NULL threadHandle or mImagePtr.\n" );
        return ( -1 );
    }
    
    trackingThreadHandle = ( TrackingThreadHandle* )threadGetArg ( threadHandle );
    if ( !trackingThreadHandle ) {
        ARLOGe ( "trackingThreadStart(): Error: NULL trackingThreadHandle.\n" );
        return ( -1 );
    }
    memcpy ( trackingThreadHandle->mImagePtr, mImagePtr, trackingThreadHandle->mImageSize );
    threadStartSignal ( threadHandle );

    return 0;
}
