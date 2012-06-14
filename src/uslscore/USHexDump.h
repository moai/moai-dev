// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USHEXDUMP_H
#define	USHEXDUMP_H

class USStream;

//================================================================//
// USHexDump
//================================================================//
namespace USHexDump {

	//----------------------------------------------------------------//
	void	DumpAsCPPHeader		( USStream& stream, cc8* name, const void* data, size_t size, u32 columns );

}

#endif
