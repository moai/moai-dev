//
//  BltAudioSource.cpp
//
//
//  Created by Sylvain Rebaud (srebaud@plumzi.com) on 01/09/2014.
//  Copyright (c) 2012-2014 Plumzi, Inc. All Rights Reserved.
//

#include "BltAudioSource.h"
#include <uslscore/USFileStream.h>
#include "BltUSFileStreamWrapper.h"

#define BLT_CHECK(_result, _return) do {											   \
	BLT_Result _x = (_result);                                                         \
	if (_x != BLT_SUCCESS) {                                                           \
		MOAIPrint("%s, %d, BLT_CHECK failed, result=%d (%s) [%s]", __FILE__, __LINE__, _x, NPT_ResultText(_x), #_result); \
		return (_return);                                                              \
	}                                                                                  \
} while(0)

BltAudioSource::BltAudioSource() :
	mDecoder(NULL),
	mLastPacket(NULL),
	mSampleRate(0),
	mChannelCount(0),
	mBitsPerSample(0),
	mLength(0)
{
}

BltAudioSource::~BltAudioSource()
{
	close();
}

bool BltAudioSource::init(const RString& path, bool loadIntoMemory)
{
	BLT_Result result;

	if (mLoadedInMemory && loadIntoMemory) {
		return true;
	}

	mPath = path;

    /* create a decoder */
	if (!mDecoder) {
		result = BLT_Decoder_Create(&mDecoder);
		BLT_CHECK(result, false);
	}

    /* register builtin modules */
	result = BLT_Decoder_RegisterBuiltins(mDecoder);
	BLT_CHECK(result, false);

    ATX_InputStream* input_object = NULL;
    if (!BltUSFileStream_Create(mPath.c_str(), &input_object)) {
        MOAIPrint("[BLT] Cannot open stream : %s", mPath.c_str());
        return false;
    }

    char input_name[64];
    sprintf(input_name, "callback-input:%lld", (long long)ATX_POINTER_TO_LONG(input_object));
    MOAIPrint("[BLT] input ID : %s", input_name);

    result = BLT_Decoder_SetInput(mDecoder, input_name, "audio/mp4");
	BLT_CHECK(result, false);

    /* set the output to be a memory node */
    result = BLT_Decoder_SetOutput(mDecoder, "memory", "audio/pcm");
	BLT_CHECK(result, false);

	/* get a reference to the output node */
    BLT_MediaNode* output_node = NULL;
    BLT_Decoder_GetOutputNode(mDecoder, &output_node);

    /* get the BLT_PacketProducer interface of the output node */
    BLT_PacketProducer* packet_source = ATX_CAST(output_node, BLT_PacketProducer);

    /* determine stream sample rate and duration */
    BLT_DecoderStatus status;
    const BLT_PcmMediaType* media_type;
    do {
        result = BLT_Decoder_PumpPacket(mDecoder);
        if (BLT_FAILED(result)) {
            MOAIPrint(MOAILogMgr::LOG_ERROR, "decoding stopped (%d)", result);
            break;
        }

		result = BLT_PacketProducer_GetPacket(packet_source, &mLastPacket);
		if (BLT_SUCCEEDED(result) && mLastPacket) {
			result = BLT_MediaPacket_GetMediaType(mLastPacket, (const BLT_MediaType**)&media_type);
			if (BLT_FAILED(result)) {
				BLT_MediaPacket_Release(mLastPacket);
				mLastPacket = NULL;
			}
		}
    } while (!mLastPacket);

	/* cleanup */
    ATX_RELEASE_OBJECT(output_node);

	BLT_CHECK(result, false);

	/* Get File Info via Decoder Status */
	result = BLT_Decoder_GetStatus(mDecoder, &status);
	if (BLT_FAILED(result)) {
		if (mLastPacket) {
			BLT_MediaPacket_Release(mLastPacket);
			mLastPacket = NULL;
		}

		BLT_CHECK(result, false);
	};

    mChannelCount  = media_type->channel_count;
    mSampleRate    = media_type->sample_rate;
    mBitsPerSample = media_type->bits_per_sample;
    mLength		   = status.stream_info.duration / 1000.;

	return BufferedAudioSource::init(path, loadIntoMemory);
}

void BltAudioSource::close()
{
    BufferedAudioSource::close();

	if (mDecoder) {
		BLT_Decoder_Destroy(mDecoder);
		mDecoder = NULL;
		mSampleRate = 0;
		mBitsPerSample = 0;
		mChannelCount = 0;
	}

	if (mLastPacket) {
		BLT_MediaPacket_Release(mLastPacket);
		mLastPacket = NULL;
	}
}

void BltAudioSource::setDecoderPosition(Int64 startFrame)
{
	RScopedLock l(mDecodeLock);

    incrementPositionVersion();

	double time = (double)startFrame / (getLength() * getSampleRate());
	BLT_Decoder_SeekToTime(mDecoder, time);


	if (time < getLength()) {
		mEOF = false;
	}

	if (mLastPacket) {
		BLT_MediaPacket_Release(mLastPacket);
		mLastPacket = NULL;
	}
}

double BltAudioSource::getLength()
{
	return mLength;
}

double BltAudioSource::getSampleRate()
{
	return mSampleRate;
}

UInt32 BltAudioSource::getNumChannels()
{
	return mChannelCount;
}

UInt32 BltAudioSource::getBitsPerSample()
{
	return mBitsPerSample;
}

Int64 BltAudioSource::decodeData(float* buffer, UInt32 numFrames, int &version)
{
	RScopedLock l(mDecodeLock);

    version = getPositionVersion();

	// shortcut
	if (isEOF()) return 0;

	BLT_Result result;
    unsigned int buffer_offset = 0;
	long framesRead = 0;

	unsigned int bytes_per_sample = mBitsPerSample / 8;
	unsigned int bytes_per_frame = mChannelCount * bytes_per_sample;
	UInt32 numBytes = numFrames * bytes_per_frame;
	UInt32 bytesRead = 0;

    /* use data from last packet if any */
    if (mLastPacket != NULL) {
        unsigned int payload_size = BLT_MediaPacket_GetPayloadSize(mLastPacket);
        unsigned int bytes_to_read = payload_size <= numBytes ? payload_size : numBytes;
		BLT_UInt8* payload_buffer = (BLT_UInt8*)BLT_MediaPacket_GetPayloadBuffer(mLastPacket);
		for (int i=0; i<bytes_to_read/bytes_per_sample; i++) {
			// HACK
			BLT_Int16 value = *(BLT_Int16*)(payload_buffer + (i*bytes_per_sample));
			buffer[i] = value / 32767.;
		}

        /* update offset and release packet if empty */
        BLT_MediaPacket_SetPayloadOffset(mLastPacket, BLT_MediaPacket_GetPayloadOffset(mLastPacket) + bytes_to_read);
        if (BLT_MediaPacket_GetPayloadSize(mLastPacket) == 0) {
            BLT_MediaPacket_Release(mLastPacket);
            mLastPacket = NULL;
        }

        numBytes -= bytes_to_read;
        buffer_offset += bytes_to_read/bytes_per_sample;
        bytesRead += bytes_to_read;
    }

    /* return if no data left to read */
    if (numBytes == 0) {
        return bytesRead/bytes_per_frame;
    }

    /* get a reference to the output node */
    BLT_MediaNode* output_node = NULL;
    BLT_Decoder_GetOutputNode(mDecoder, &output_node);

    /* get the BLT_PacketProducer interface of the output node */
    BLT_PacketProducer* packet_source = ATX_CAST(output_node, BLT_PacketProducer);
    do {
        result = BLT_Decoder_PumpPacket(mDecoder);
		if (BLT_FAILED(result)) {
            if (result == BLT_ERROR_EOS) {
                MOAIPrint(MOAILogMgr::LOG_STATUS, "decoding done");
            } else {
                MOAIPrint(MOAILogMgr::LOG_WARNING, "decoding stopped (%d)", result);
            }
			mEOF = true;
            break;
        }

		BLT_MediaPacket* packet = NULL;
		result = BLT_PacketProducer_GetPacket(packet_source, &packet);
		if (BLT_SUCCEEDED(result) && packet) {
			const BLT_PcmMediaType* media_type;
			BLT_MediaPacket_GetMediaType(packet, (const BLT_MediaType**)&media_type);
			unsigned int payload_size = BLT_MediaPacket_GetPayloadSize(packet);

			MOAIPrint(MOAILogMgr::LOG_DEBUG, "PACKET: sample_rate=%d, channels=%d, bits_per_sample=%d, size=%d",
						 media_type->sample_rate,
						 media_type->channel_count,
						 media_type->bits_per_sample,
						 payload_size);
			unsigned int bytes_to_read = payload_size <= numBytes ? payload_size : numBytes;

			BLT_UInt8* payload_buffer = (BLT_UInt8*)BLT_MediaPacket_GetPayloadBuffer(packet);
			for (int i=0; i<bytes_to_read/bytes_per_sample; i++) {
				buffer[buffer_offset+i] =  *(BLT_Int16*)(payload_buffer + (i*bytes_per_sample)) / 32767.;
			}

			numBytes -= bytes_to_read;
			buffer_offset += bytes_to_read/bytes_per_sample;
			bytesRead += bytes_to_read;

			/* advance packet offset with ready data size */
			BLT_MediaPacket_SetPayloadOffset(packet, BLT_MediaPacket_GetPayloadOffset(packet) + bytes_to_read);

			/* keep packet around if we didn't read everything from it this time */
			if (BLT_MediaPacket_GetPayloadSize(packet) > 0) {
				mLastPacket = packet;
			} else {
				/* don't forget to release the packet */
				BLT_MediaPacket_Release(packet);
			}
		}
    } while (numBytes > 0);

	/* cleanup */
    ATX_RELEASE_OBJECT(output_node);

	return bytesRead/bytes_per_frame;
}
