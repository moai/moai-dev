// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#ifdef _WIN32
#include <uslsext/USAdapterInfo.h>

#include <winsock2.h>
#include <iphlpapi.h>


//================================================================//
// USAdapterInfo
//================================================================//

//----------------------------------------------------------------//
void USAdapterInfo::SetNameFromMACAddress ( u8* address, u32 length ) {

	STLString result;

	for ( u32 i = 0; i < length; i++ ) {
		if ( i ) {
			result.write ( "-" );
		}
		result.write ( "%.2X", ( u32 )address [ i ]);
	}
	this->mName = result;
}

//================================================================//
// USAdapterInfoList
//================================================================//

//----------------------------------------------------------------//
void USAdapterInfoList::EnumerateAdapters () {

	this->Clear ();

	DWORD result;

	DWORD totalInterfaces;
	result = GetNumberOfInterfaces ( &totalInterfaces );
	if ( result != NO_ERROR ) return;
	
	DWORD dwBufLen = ( totalInterfaces + 1 ) * sizeof ( IP_ADAPTER_ADDRESSES );
	PIP_ADAPTER_ADDRESSES pAdapterAddress = ( PIP_ADAPTER_ADDRESSES )malloc ( dwBufLen );

	result = GetAdaptersAddresses ( AF_UNSPEC, 0, NULL, pAdapterAddress, &dwBufLen );
	
	if ( result == ERROR_BUFFER_OVERFLOW ) {
		free ( pAdapterAddress );
		pAdapterAddress = ( PIP_ADAPTER_ADDRESSES )malloc ( dwBufLen );
		
		result = GetAdaptersAddresses ( AF_UNSPEC, 0, NULL, pAdapterAddress, &dwBufLen );
	}
	
	if ( result == NO_ERROR ) {
		
		PIP_ADAPTER_ADDRESSES cursor;
		u32 count;
		
		count = 0;
		cursor = pAdapterAddress;
		for ( ; cursor; cursor = cursor->Next ) {
			if ( cursor->PhysicalAddressLength ) {
				++count;
			}
		}
		
		if ( count ) {
		
			this->Init ( count );
			
			count = 0;
			cursor = pAdapterAddress;
			for ( ; cursor; cursor = cursor->Next ) {
				if ( cursor->PhysicalAddressLength ) {
				
					USAdapterInfo& adapterInfo = ( *this )[ count ];
				
					adapterInfo.SetNameFromMACAddress (
						cursor->PhysicalAddress,
						cursor->PhysicalAddressLength
					);
					++count;
				}
			}
		}
	}
	
	free ( pAdapterAddress );
}

#endif
