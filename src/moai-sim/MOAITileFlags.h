// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITILEFLAGS_H
#define MOAITILEFLAGS_H

//================================================================//
// MOAITileFlags
//================================================================//
class MOAITileFlags {
public:
	
	static const u32 XFLIP				= 0x20000000;
	static const u32 YFLIP				= 0x40000000;
	static const u32 HIDDEN				= 0x80000000;
	
	static const u32 FLIP_MASK			= 0x60000000;
	static const u32 FLAGS_MASK			= 0xf0000000;
	static const u32 CODE_MASK			= 0x0fffffff;
};

#endif
