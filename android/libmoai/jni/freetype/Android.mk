#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= freetype
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS 		:= -DPIC -DFT2_BUILD_LIBRARY

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/autofit/autofit.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/bdf/bdf.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/cff/cff.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftbase.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftbitmap.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/cache/ftcache.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftfstype.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftgasp.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftglyph.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/gzip/ftgzip.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftinit.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/lzw/ftlzw.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftstroke.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftsystem.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/smooth/smooth.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftbbox.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftgxval.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftlcdfil.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftmm.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftotval.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftpatent.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftpfr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftsynth.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/fttype1.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftwinfnt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/base/ftxf86.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/pcf/pcf.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/pfr/pfr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/psaux/psaux.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/pshinter/pshinter.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/psnames/psmodule.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/raster/raster.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/sfnt/sfnt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/truetype/truetype.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/type1/type1.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/cid/type1cid.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/type42/type42.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src/winfonts/winfnt.c

	include $(BUILD_STATIC_LIBRARY)