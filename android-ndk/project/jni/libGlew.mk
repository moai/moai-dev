#================================================================#
# libGlew
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libGlew
LOCAL_LDLIBS := -llog -lstring -lm

LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/glew-1.5.6/include \
$(SRC_PATH)/3rdparty/glew-1.5.6/src

LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/glew-1.5.6/src/glew.c 

include $(BUILD_STATIC_LIBRARY)