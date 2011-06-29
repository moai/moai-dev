#================================================================#
# libcontrib
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libcontrib
LOCAL_LDLIBS := -llog -lstring -lm -lz -lGLESv1_CM.so

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/contrib/utf8.c \

include $(BUILD_STATIC_LIBRARY)