//
//  RiffFile.h
//  Part of UNTZ
//
//  Created by Zach Saul (zach@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef RIFFFILE_H_
#define RIFFFILE_H_

#include "UntzSystem.h"

typedef UInt32 FourCC;

#define STR2FOURCC(str) ( *((UInt32 *)(str)) )

static void FOURCC2STR(FourCC fcc, char* str) {
	str[0] = fcc & 0xFF;
	str[1] = (fcc >>  8) & 0xFF;
	str[2] = (fcc >> 16) & 0xFF;
	str[3] = (fcc >> 24) & 0xFF;
	str[4] = 0;
}

typedef struct {			// Chunk structure
	FourCC chunkId;			// Chunk type identifier
	UInt32 chunkSize;		// Chunk size field (size of chunkData)
	UInt32 formatCode;		// Only used for RIFF and LIST chunks	
	long dataStart;
} RiffChunkHeader;

class RiffFile
{
public:
	RiffFile();
	virtual ~RiffFile();

	virtual int open(const char* path);
	virtual void close();

	void rewind();
	bool push(UInt32 chunkId);
	bool pop();
	UInt32 chunkId();
	UInt32 chunkSize();
	UInt32 formatCode();
	UInt32 dataStart();
	UInt32 readData(void *buffer, UInt32 bytes);
	UInt32 rawRead(void *buffer, UInt32 bytes);

protected:
	FILE* mpFile;
	std::vector<RiffChunkHeader> mChunkStack;
};

#endif // RIFFFILE_H_
