#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	WEBP_CFLAGS := -Wall -DANDROID -DHAVE_MALLOC_H -DHAVE_PTHREAD -DWEBP_USE_THREAD

	ifeq ($(APP_OPTIM),release)
	  WEBP_CFLAGS += -finline-functions -ffast-math -ffunction-sections -fdata-sections
	  ifeq ($(findstring clang,$(NDK_TOOLCHAIN_VERSION)),)
	    WEBP_CFLAGS += -frename-registers -s
	  endif
	endif

	LOCAL_MODULE		:= webp
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= $(WEBP_CFLAGS)

	ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	  LOCAL_CFLAGS += -DHAVE_NEON=1
	endif

	LOCAL_WHOLE_STATIC_LIBRARIES += cpufeatures

	ifneq ($(findstring armeabi-v7a, $(TARGET_ARCH_ABI)),)
	  # Setting LOCAL_ARM_NEON will enable -mfpu=neon which may cause illegal
	  # instructions to be generated for armv7a code. Instead target the neon code
	  # specifically.
	  NEON := c.neon
	else
	  NEON := c
	endif

	LOCAL_PATH			:= $(MOAI_SDK_HOME)/3rdparty/libwebp-0.4.1
	LOCAL_C_INCLUDES	:= $(LOCAL_PATH)/src
	LOCAL_SRC_FILES		+= src/dec/alpha.c
	LOCAL_SRC_FILES		+= src/dec/buffer.c
	LOCAL_SRC_FILES		+= src/dec/frame.c
	LOCAL_SRC_FILES		+= src/dec/idec.c
	LOCAL_SRC_FILES		+= src/dec/io.c
	LOCAL_SRC_FILES		+= src/dec/quant.c
	LOCAL_SRC_FILES		+= src/dec/tree.c
	LOCAL_SRC_FILES		+= src/dec/vp8.c
	LOCAL_SRC_FILES		+= src/dec/vp8l.c
	LOCAL_SRC_FILES		+= src/dec/webp.c
	LOCAL_SRC_FILES		+= src/dsp/alpha_processing.c
	LOCAL_SRC_FILES		+= src/dsp/cpu.c
	LOCAL_SRC_FILES		+= src/dsp/dec.c
	LOCAL_SRC_FILES		+= src/dsp/dec_clip_tables.c
	LOCAL_SRC_FILES		+= src/dsp/dec_mips32.c
	LOCAL_SRC_FILES		+= src/dsp/dec_neon.$(NEON)
	LOCAL_SRC_FILES		+= src/dsp/dec_sse2.c
	LOCAL_SRC_FILES		+= src/dsp/enc.c
	LOCAL_SRC_FILES		+= src/dsp/enc_avx2.c
	LOCAL_SRC_FILES		+= src/dsp/enc_mips32.c
	LOCAL_SRC_FILES		+= src/dsp/enc_neon.$(NEON)
	LOCAL_SRC_FILES		+= src/dsp/enc_sse2.c
	LOCAL_SRC_FILES		+= src/dsp/lossless.c
	LOCAL_SRC_FILES		+= src/dsp/lossless_mips32.c
	LOCAL_SRC_FILES		+= src/dsp/lossless_neon.$(NEON)
	LOCAL_SRC_FILES		+= src/dsp/lossless_sse2.c
	LOCAL_SRC_FILES		+= src/dsp/upsampling.c
	LOCAL_SRC_FILES		+= src/dsp/upsampling_neon.$(NEON)
	LOCAL_SRC_FILES		+= src/dsp/upsampling_sse2.c
	LOCAL_SRC_FILES		+= src/dsp/yuv.c
	LOCAL_SRC_FILES		+= src/dsp/yuv_mips32.c
	LOCAL_SRC_FILES		+= src/dsp/yuv_sse2.c
	LOCAL_SRC_FILES		+= src/enc/alpha.c
	LOCAL_SRC_FILES		+= src/enc/analysis.c
	LOCAL_SRC_FILES		+= src/enc/backward_references.c
	LOCAL_SRC_FILES		+= src/enc/config.c
	LOCAL_SRC_FILES		+= src/enc/cost.c
	LOCAL_SRC_FILES		+= src/enc/filter.c
	LOCAL_SRC_FILES		+= src/enc/frame.c
	LOCAL_SRC_FILES		+= src/enc/histogram.c
	LOCAL_SRC_FILES		+= src/enc/iterator.c
	LOCAL_SRC_FILES		+= src/enc/picture.c
	LOCAL_SRC_FILES		+= src/enc/picture_csp.c
	LOCAL_SRC_FILES		+= src/enc/picture_psnr.c
	LOCAL_SRC_FILES		+= src/enc/picture_rescale.c
	LOCAL_SRC_FILES		+= src/enc/picture_tools.c
	LOCAL_SRC_FILES		+= src/enc/quant.c
	LOCAL_SRC_FILES		+= src/enc/syntax.c
	LOCAL_SRC_FILES		+= src/enc/token.c
	LOCAL_SRC_FILES		+= src/enc/tree.c
	LOCAL_SRC_FILES		+= src/enc/vp8l.c
	LOCAL_SRC_FILES		+= src/enc/webpenc.c
	LOCAL_SRC_FILES		+= src/utils/bit_reader.c
	LOCAL_SRC_FILES		+= src/utils/bit_writer.c
	LOCAL_SRC_FILES		+= src/utils/color_cache.c
	LOCAL_SRC_FILES		+= src/utils/filters.c
	LOCAL_SRC_FILES		+= src/utils/huffman.c
	LOCAL_SRC_FILES		+= src/utils/huffman_encode.c
	LOCAL_SRC_FILES		+= src/utils/quant_levels.c
	LOCAL_SRC_FILES		+= src/utils/quant_levels_dec.c
	LOCAL_SRC_FILES		+= src/utils/random.c
	LOCAL_SRC_FILES		+= src/utils/rescaler.c
	LOCAL_SRC_FILES		+= src/utils/thread.c
	LOCAL_SRC_FILES		+= src/utils/utils.c
	LOCAL_SRC_FILES		+= src/demux/demux.c

 	include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/cpufeatures)
