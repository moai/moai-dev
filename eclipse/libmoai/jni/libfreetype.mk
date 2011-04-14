#================================================================#
# libfreetype
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libfreetype
LOCAL_LDLIBS := -llog -lstring -lm
LOCAL_CFLAGS += -W -Wall
LOCAL_CFLAGS += -fPIC -DPIC
LOCAL_CFLAGS += "-DFT2_BUILD_LIBRARY"

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/freetype-2.4.4/include \
$(SRC_PATH)/3rdparty/freetype-2.4.4/include/freetype \
$(SRC_PATH)/3rdparty/freetype-2.4.4/include/freetype2 \
$(SRC_PATH)/3rdparty/freetype-2.4.4/builds \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src \
$(SRC_PATH)/3rdparty/freetype-2.4.4/config

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/freetype-2.4.4/src/autofit/autofit.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/bdf/bdf.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/cff/cff.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftbase.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftbitmap.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/cache/ftcache.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftfstype.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftgasp.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftglyph.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/gzip/ftgzip.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftinit.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/lzw/ftlzw.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftstroke.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftsystem.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/smooth/smooth.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftbbox.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftgxval.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftlcdfil.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftmm.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftotval.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftpatent.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftpfr.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftsynth.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/fttype1.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftwinfnt.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/base/ftxf86.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/pcf/pcf.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/pfr/pfr.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/psaux/psaux.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/pshinter/pshinter.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/psnames/psmodule.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/raster/raster.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/sfnt/sfnt.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/truetype/truetype.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/type1/type1.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/cid/type1cid.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/type42/type42.c \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src/winfonts/winfnt.c \

include $(BUILD_STATIC_LIBRARY)