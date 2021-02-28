// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLTILEFLAGS_H
#define ZLTILEFLAGS_H

//================================================================//
// ZLTileFlags
//================================================================//
class ZLTileFlags {
public:
	
	static const u32 ROT_90				= 0x10000000;
	static const u32 XFLIP				= 0x20000000;
	static const u32 YFLIP				= 0x40000000;
	static const u32 HIDDEN				= 0x80000000;
	
	static const u32 FLIP_MASK			= 0x70000000;
	static const u32 FLAGS_MASK			= 0xf0000000;
	static const u32 CODE_MASK			= 0x0fffffff;
};

#endif
