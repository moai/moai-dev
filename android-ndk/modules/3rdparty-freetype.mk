#================================================================#
# Copyright (c) 2010-2017 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 		:= freetype
LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
LOCAL_CFLAGS 		:= -DPIC -DFT2_BUILD_LIBRARY -DFT_CONFIG_OPTION_USE_ZLIB -DFT_CONFIG_OPTION_SYSTEM_ZLIB -DDARWIN_NO_CARBON
LOCAL_CFLAGS		+= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
LOCAL_CFLAGS		+= -fvisibility=hidden

LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/autofit/autofit.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/bdf/bdf.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/cff/cff.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftbase.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftbitmap.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/cache/ftcache.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftfstype.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftgasp.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftglyph.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/gzip/ftgzip.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftinit.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/lzw/ftlzw.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftstroke.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftsystem.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/smooth/smooth.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftbbox.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftgxval.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftlcdfil.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftmm.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftotval.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftpatent.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftpfr.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftsynth.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/fttype1.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/base/ftwinfnt.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/pcf/pcf.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/pfr/pfr.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/psaux/psaux.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/pshinter/pshinter.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/psnames/psmodule.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/raster/raster.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/sfnt/sfnt.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/truetype/truetype.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/type1/type1.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/cid/type1cid.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/type42/type42.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/freetype-2.7/src/winfonts/winfnt.c

include $(BUILD_STATIC_LIBRARY)