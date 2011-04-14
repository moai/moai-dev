#================================================================#
# libexpat
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libexpat
LOCAL_LDLIBS := -llog -lstring -lm
LOCAL_CFLAGS := -DHAVE_MEMMOVE=1
# LOCAL_CFLAGS := -std=c99 -ffast-math -O3 -DNDEBUG

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/expat-2.0.1/amiga \
$(SRC_PATH)/3rdparty/expat-2.0.1/lib \
$(SRC_PATH)/3rdparty/expat-2.0.1/xmlwf \

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/expat-2.0.1/lib/xmlparse.c \
$(SRC_PATH)/3rdparty/expat-2.0.1/lib/xmlrole.c \
$(SRC_PATH)/3rdparty/expat-2.0.1/lib/xmltok.c \
$(SRC_PATH)/3rdparty/expat-2.0.1/lib/xmltok_impl.c \
$(SRC_PATH)/3rdparty/expat-2.0.1/lib/xmltok_ns.c

include $(BUILD_STATIC_LIBRARY)