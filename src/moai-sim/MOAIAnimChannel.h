// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCHANNEL_H
#define	MOAIANIMCHANNEL_H

class MOAIAttribute;

//================================================================//
// MOAIAnimChannel
//================================================================//
class MOAIAnimChannel {
private:

	friend class MOAIAnimMixer;

	u32			mType;
	void*		mBuffer;

public:

	enum {
		CHANNEL_TYPE_COLOR,
		CHANNEL_TYPE_FLOAT,
		CHANNEL_TYPE_QUAT,
		CHANNEL_TYPE_VEC,
		CHANNEL_TYPE_UNKNOWN,
	};
	
	//----------------------------------------------------------------//
	void		Accumulate			( const ZLColorVec& value, float opacity );
	void		Accumulate			( const float& value, float opacity );
	void		Accumulate			( const ZLQuaternion& value, float opacity );
	void		Accumulate			( const ZLVec3D& value, float opacity );
	size_t		GetSize				();
	bool		GetValue			( MOAIAttribute& attr );
				MOAIAnimChannel		();
				~MOAIAnimChannel	();
};

#endif
