// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLHEXDUMP_H
#define	ZLHEXDUMP_H

class ZLStream;

//================================================================//
// ZLHexDump
//================================================================//
namespace ZLHexDump {

	//----------------------------------------------------------------//
	void	DumpAsCPPHeader		( ZLStream& stream, cc8* name, const void* data, size_t size, u32 columns );

}

#endif
