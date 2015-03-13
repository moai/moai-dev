// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMATMGR_H
#define	MOAIIMAGEFORMATMGR_H

#include <moai-sim/MOAIImageFormat.h>

//================================================================//
// MOAIImageFormatMgr
//================================================================//
class MOAIImageFormatMgr :
	public MOAIGlobalClass < MOAIImageFormatMgr > {
public:

	typedef STLMap < u32, MOAIImageFormat* >::iterator FormatIt;
	STLMap < u32, MOAIImageFormat* > mFormats;

	//----------------------------------------------------------------//
	u32						HashName					( cc8* name );

public:
	
	//----------------------------------------------------------------//
	MOAIImageFormat*		FindFormat					( cc8* name );
	MOAIImageFormat*		FindFormat					( ZLStream& stream );
							MOAIImageFormatMgr			();
							~MOAIImageFormatMgr			();
	
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
