#================================================================#
# libjson
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libjson
LOCAL_LDLIBS := -llog
# LOCAL_CFLAGS := -std=c99

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/jansson-2.1/src

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/jansson-2.1/src/dump.c \
$(SRC_PATH)/3rdparty/jansson-2.1/src/error.c \
$(SRC_PATH)/3rdparty/jansson-2.1/src/hashtable.c \
$(SRC_PATH)/3rdparty/jansson-2.1/src/load.c \
$(SRC_PATH)/3rdparty/jansson-2.1/src/memory.c \
$(SRC_PATH)/3rdparty/jansson-2.1/src/pack_unpack.c \
$(SRC_PATH)/3rdparty/jansson-2.1/src/strbuffer.c \
$(SRC_PATH)/3rdparty/jansson-2.1/src/utf.c \
$(SRC_PATH)/3rdparty/jansson-2.1/src/value.c \

include $(BUILD_STATIC_LIBRARY)