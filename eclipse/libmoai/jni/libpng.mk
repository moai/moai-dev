#================================================================#
# libpng
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libpng
LOCAL_LDLIBS := -llog -lstring -lm -lz

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/lpng140

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/lpng140/png.c \
$(SRC_PATH)/3rdparty/lpng140/pngerror.c \
$(SRC_PATH)/3rdparty/lpng140/pngget.c \
$(SRC_PATH)/3rdparty/lpng140/pngmem.c \
$(SRC_PATH)/3rdparty/lpng140/pngpread.c \
$(SRC_PATH)/3rdparty/lpng140/pngread.c \
$(SRC_PATH)/3rdparty/lpng140/pngrio.c \
$(SRC_PATH)/3rdparty/lpng140/pngrtran.c \
$(SRC_PATH)/3rdparty/lpng140/pngrutil.c \
$(SRC_PATH)/3rdparty/lpng140/pngset.c \
$(SRC_PATH)/3rdparty/lpng140/pngtest.c \
$(SRC_PATH)/3rdparty/lpng140/pngtrans.c \
$(SRC_PATH)/3rdparty/lpng140/pngwio.c \
$(SRC_PATH)/3rdparty/lpng140/pngwrite.c \
$(SRC_PATH)/3rdparty/lpng140/pngwtran.c \
$(SRC_PATH)/3rdparty/lpng140/pngwutil.c

include $(BUILD_STATIC_LIBRARY)