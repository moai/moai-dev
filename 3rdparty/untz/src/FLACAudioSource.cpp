//
//  FLACAudioSource.cpp
//  Part of UNTZ
//
//

#include "FLACAudioSource.h"


FLACAudioSource::FLACAudioSource()
{
    decoder = NULL;
	gotMetadata = false;
	bufferIndex = 0;
	bufferSize = 0;
	bufferFrames = 0;
	flac_buffer = NULL;
}

FLACAudioSource::~FLACAudioSource()
{
	if (flac_buffer != NULL)
		free(flac_buffer);
	close();
}

bool FLACAudioSource::init(const RString& path, bool loadIntoMemory)
{
	if(mLoadedInMemory && loadIntoMemory)
		return true;

	std::cerr << "Loading File: " << path << std::endl;
	
	// Try starting up a decoder
	if((decoder = FLAC__stream_decoder_new()) == NULL) {
		std::cerr << "Error starting flac decoder" << std::endl;
		return false;
	}
	
	FLAC__stream_decoder_set_md5_checking(decoder, true);

	FLAC__StreamDecoderInitStatus status;
	
	// Try decoding file
	status = FLAC__stream_decoder_init_file(decoder, path.c_str(), FLAC_write_callback, FLAC_metadata_callback, FLAC_error_callback, this);
	if (status != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
		std::cerr << "Error decoding file" << std::endl;
	}
	
	if (!FLAC__stream_decoder_process_until_end_of_metadata(decoder)) {
		std::cerr << "Could not read metadata" << std::endl;
		std::cerr << FLAC__stream_decoder_get_state(decoder) << std::endl;
	}
	return BufferedAudioSource::init(path, loadIntoMemory);
}

void FLACAudioSource::close()
{
    BufferedAudioSource::close();
    
	if(decoder) {
		FLAC__stream_decoder_finish(decoder);
	}
}

Int64 FLACAudioSource::setDecoderPosition(Int64 startFrame)
{
	RScopedLock l(&mDecodeLock);

	FLAC__bool status = FLAC__stream_decoder_seek_absolute(decoder, startFrame);
	if(startFrame < getLength() * getSampleRate())
		mEOF = false;
	return startFrame;
}

double FLACAudioSource::getLength() 
{
	if (gotMetadata)
		return total_samples / ((float)sample_rate);
	return 0;
}

double FLACAudioSource::getSampleRate() 
{
	if(gotMetadata)
		return sample_rate;
	return 0; 
}

UInt32 FLACAudioSource::getNumChannels()
{
	if(gotMetadata)
		return channels;
	return 0;
}

UInt32 FLACAudioSource::getBitsPerSample()
{
	if(gotMetadata)
		return bps;
	return 0;
}

Int64 FLACAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
	RScopedLock l(&mDecodeLock);

	int bitStream;
	float** data = 0;

	UInt32 framesleft = numFrames;
	UInt32 framesread = 0;

	int framesInBuffer = bufferFrames - bufferIndex;
	if (framesInBuffer > 0) {
		memcpy(buffer, flac_buffer + (bufferIndex * channels), sizeof(float) * channels * framesInBuffer);
		framesread = framesInBuffer;
		framesleft = framesleft - framesInBuffer;
	}
	
	while(framesleft > 0) {
		{
			FLAC__stream_decoder_process_single(decoder);
			int frames2read = bufferFrames;
			if (bufferFrames > framesleft) {
				frames2read = framesleft;
				bufferIndex = frames2read;
			} else {
				bufferIndex = bufferFrames;
			}
			memcpy(buffer + framesread * channels, flac_buffer, sizeof(float) * channels * frames2read);
			framesread = framesread + frames2read;
			framesleft = framesleft - frames2read;
		}
	}

	return framesread;
}

FLAC__StreamDecoderWriteStatus FLACAudioSource::flac_write(const FLAC__Frame *frame, const FLAC__int32 * const buffer[]) {
	if (flac_buffer == NULL) {
		flac_buffer = (float*)malloc(sizeof(float) * frame->header.blocksize * channels);
		bufferSize = frame->header.blocksize;
	} else if (bufferSize < (frame->header.blocksize * channels)) {
		flac_buffer = (float*)realloc(flac_buffer, sizeof(float) * frame->header.blocksize * channels);
		bufferSize = frame->header.blocksize;		
	}

	bufferFrames = frame->header.blocksize;
	
	int	bufferloc;
	float divisor = pow(2, bps);
	for(int j = 0; j < channels; ++j) {
		for(int i = 0; i < bufferFrames; ++i) {
			bufferloc = i * channels + j;
			FLAC__int32	input = (FLAC__int32)buffer[j][i];
			float output = input / divisor;
			flac_buffer[bufferloc] = output;
		}
	}
	bufferIndex = 0;
	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void FLACAudioSource::flac_metadata(const FLAC__StreamMetadata *metadata) {
	if(metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
		total_samples = metadata->data.stream_info.total_samples;
		sample_rate = metadata->data.stream_info.sample_rate;
		channels = metadata->data.stream_info.channels;
		bps = metadata->data.stream_info.bits_per_sample;
		gotMetadata = true;
	}
}

FLAC__StreamDecoderWriteStatus FLAC_write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	FLACAudioSource *reader = (FLACAudioSource*)client_data;
	return reader->flac_write(frame, buffer);

}

void FLAC_metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	FLACAudioSource *reader = (FLACAudioSource*)client_data;
	reader->flac_metadata(metadata);
}

void FLAC_error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
	fprintf(stderr, "Got error callback: %s\n", status);
}
