// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHCACHE_H
#define	MOAIGLYPHCACHE_H

class MOAIImage;

//================================================================//
// MOAIGlyphCache
//================================================================//
class MOAIGlyphCache {
private:


	USLeanArray < MOAIImage* > mImages;

public:
	
	//----------------------------------------------------------------//
					MOAIGlyphCache				();
					~MOAIGlyphCache				();
};

#endif
