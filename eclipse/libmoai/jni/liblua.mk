#================================================================#
# liblua
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := liblua
LOCAL_LDLIBS := -llog -lstring -lm -ldl

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/src \
$(SRC_PATH)/src/moai-lua-5.1.3/src \

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/src/moai-lua-5.1.3/src/lapi.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lauxlib.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lbaselib.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lcode.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/ldblib.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/ldebug.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/ldo.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/ldump.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lfunc.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lgc.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/linit.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/liolib.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lmathlib.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lmem.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/loadlib.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lobject.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lopcodes.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/loslib.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lparser.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lstate.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lstring.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lstrlib.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/ltable.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/ltablib.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/ltm.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lundump.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lvm.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/lzio.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/print.c \
$(SRC_PATH)/src/moai-lua-5.1.3/src/llex.c

include $(BUILD_STATIC_LIBRARY)