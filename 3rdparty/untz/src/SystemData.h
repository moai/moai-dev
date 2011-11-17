//
//  SystemData.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright 2011 Retronyms. All rights reserved.
//

#ifndef SYSTEMDATA_H_
#define SYSTEMDATA_H_

#include "AudioMixer.h"
#include "Untz.h"
#include "SoundData.h"
#include <threading/Threading.h>
#include <map>

namespace UNTZ
{
	class SystemData
	{
	public:
		SystemData() : mError(0) {};

		virtual UInt32 getNumFrames() = 0;
		virtual UInt32 getNumOutputChannels() = 0;
		bool getError() const { return mError; }
		void setError(bool error) { mError = error; }
		UNTZ::Sound* getSound(const RString& url)
		{
			RScopedLock sl(&mMixer.mLock);
			for(int i=0; i<mMixer.mSounds.size(); ++i)
			{
				UNTZ::Sound *s = mMixer.mSounds[i];
				if(s->getData()->mPath == url)
					return s;
			}
			return 0;
		}
		UNTZ::Sound* getInMemorySound(const RString& url)
		{
			RScopedLock sl(&mMixer.mLock);
			for(int i=0; i<mMixer.mSounds.size(); ++i)
			{
				UNTZ::Sound *s = mMixer.mSounds[i];
				if(s->getData()->mPath == url && s->getData()->getSource()->isLoadedInMemory())
					return s;
			}
			return 0;
		}

		AudioMixer mMixer;

	protected:
		bool mError;
	};

};

#endif