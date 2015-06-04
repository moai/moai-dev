#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= vorbis
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
	LOCAL_CFLAGS		+= -fvisibility=hidden

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/analysis.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/bitrate.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/block.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/codebook.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/envelope.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/floor0.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/floor1.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/info.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/lookup.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/lpc.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/lsp.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/mapping0.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/mdct.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/psy.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/registry.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/res0.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/sharedbook.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/smallft.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/synthesis.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/vorbisenc.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/vorbisfile.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib/window.c

	include $(BUILD_STATIC_LIBRARY)
