#include "OggAudioSource.h"

//#define fmin(a,b) (a < b) ? a : b
//#define fmax(a,b) (b < a) ? a : b

OggAudioSource::OggAudioSource()
{
    mpOggInfo = NULL;
	mInFile = 0;
}

OggAudioSource::~OggAudioSource()
{
}

bool OggAudioSource::init(const RString& path, bool loadIntoMemory)
{
	mPath = path;
	mInFile = fopen(mPath.c_str(), "rb");

	if(mInFile == NULL)
	{
		std::cerr << "Cannot open " << mPath.c_str() << " for reading..." << std::endl;
		return false;
	}

	// Try opening the given file
	if(ov_open(mInFile, &mOggFile, NULL, 0) != 0)
	{
		std::cerr << "Error opening " << mPath.c_str() << " for decoding..." << std::endl;
		return false;
	}

	// Get some information about the OGG file
	mpOggInfo = ov_info(&mOggFile, -1);

	return BufferedAudioSource::init(path, loadIntoMemory);
}

void OggAudioSource::close()
{
    BufferedAudioSource::close();
    
	ov_clear(&mOggFile);
    
	if(mInFile)
		fclose(mInFile);    
}

void OggAudioSource::setDecoderPosition(Int64 startFrame)
{
//	ov_time_seek(&mOggFile, position);
	int status = ov_pcm_seek(&mOggFile, startFrame * getNumChannels());
	if(startFrame < getLength() * getSampleRate())
		mEOF = false;
}

double OggAudioSource::getLength() 
{ 
	return ov_time_total(&mOggFile, -1);
}

double OggAudioSource::getSampleRate() 
{
	if(mpOggInfo)
		return mpOggInfo->rate;
	return 0; 
}

UInt32 OggAudioSource::getNumChannels()
{
	if(mpOggInfo)
		return mpOggInfo->channels;
	return 0; 
}

Int64 OggAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
	int bitStream;
	float** data = 0;

	long framesRead = ov_read_float(&mOggFile, &data, numFrames, &bitStream);	
	if(framesRead > 0)
	{
		for(int i = 0; i < getNumChannels(); ++i)
		{
			int channels = getNumChannels();
			float *pTemp = &buffer[i];
			for(int j = 0; j < framesRead; ++j)
			{
				*pTemp = data[i][j];
				pTemp += channels;
			}
		}
	}
	else
		mEOF = true;

	return framesRead;
}
