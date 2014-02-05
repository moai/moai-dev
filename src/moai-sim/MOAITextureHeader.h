// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITEXTUREHEADER_H
#define MOAITEXTUREHEADER_H

//================================================================//
// MOAITextureHeader
//================================================================//
class MOAITextureHeader {
public:
	
	//----------------------------------------------------------------//
	virtual u32				GetDataSize				() = 0;
	virtual u32				GetHeaderSize			() = 0;
	virtual void*			GetHeaderStart			() = 0;
	virtual u32				GetHeight				() = 0;
	u32						GetTotalSize			();
	virtual u32				GetWidth				() = 0;
	virtual void			Invalidate				() = 0;
	virtual bool			IsValid					() = 0;
	void					Load					( ZLStream& stream );
							MOAITextureHeader		();
	virtual					~MOAITextureHeader		();
};

#endif
