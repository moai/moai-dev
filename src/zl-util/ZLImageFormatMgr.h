// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLIMAGEFORMATMGR_H
#define	ZLIMAGEFORMATMGR_H

#include <zl-util/ZLImageFormat.h>
#include <zl-util/STLMap.h>

//================================================================//
// ZLImageFormatMgr
//================================================================//
class ZLImageFormatMgr :
	public ZLContextClass < ZLImageFormatMgr > {
public:

	typedef STLMap < u32, ZLImageFormat* >::iterator FormatIt;
	STLMap < u32, ZLImageFormat* > mFormats;

	//----------------------------------------------------------------//
	u32						HashName					( cc8* name );

public:
	
	//----------------------------------------------------------------//
	ZLImageFormat*			FindFormat					( cc8* name );
	ZLImageFormat*			FindFormat					( ZLStream& stream, cc8* name = 0 );
							ZLImageFormatMgr			();
							~ZLImageFormatMgr			();
	
	//----------------------------------------------------------------//
	// name must be be no more than 6 characters and alphanumeric
	// names are *not* case sensitive
	template < typename TYPE >
	void RegisterFormat ( cc8* name ) {
	
		u32 hash = this->HashName ( name );

		if ( this->mFormats.contains ( hash )) {
			delete this->mFormats [ hash ];
			this->mFormats [ hash ] = 0;
		}
		this->mFormats [ hash ] = new TYPE ();
	}
};

#endif
