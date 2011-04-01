#================================================================#
# libTinyXml
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libTinyXml
LOCAL_LDLIBS := -llog -lstring -lm -ldl

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/tinyxml

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/tinyxml/tinyxml.cpp \
$(SRC_PATH)/3rdparty/tinyxml/tinyxmlerror.cpp \
$(SRC_PATH)/3rdparty/tinyxml/tinyxmlparser.cpp \
$(SRC_PATH)/3rdparty/tinyxml/tinystr.cpp

include $(BUILD_STATIC_LIBRARY)