#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= moaiext-untz
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -include $(MY_MOAI_ROOT)/src/zipfs/zipfs_replace.h

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/AudioMixer.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/MemoryAudioSource.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/UntzSound.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/UserAudioSource.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/BufferedAudioSource.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/BufferedAudioSourceThread.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/OggAudioSource.cpp 
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/native/android/UntzSystem.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/native/android/WaveFileAudioSource.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/native/android/WaveFile.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/untz/src/native/android/RiffFile.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-untz/MOAIUntzSound.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-untz/MOAIUntzSystem.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-untz/MOAIUntzSampleBuffer.cpp

	include $(BUILD_STATIC_LIBRARY)