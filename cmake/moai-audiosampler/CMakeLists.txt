cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-audiosampler )

set ( MOAI_AUDIOSAMPLER_INCLUDES
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/config-default/"
 CACHE INTERNAL "doc string" )

include_directories (
  ${MOAI_AUDIOSAMPLER_INCLUDES}
)



# Use all cpp files in directory as source code
set ( MOAI_AUDIOSAMPLER_SRC
	../../src/moai-audiosampler/AKU-audiosampler.cpp
	../../src/moai-audiosampler/MOAIAudioSampler.cpp
)


add_library ( moai-audiosampler STATIC ${MOAI_AUDIOSAMPLER_SRC}  )

