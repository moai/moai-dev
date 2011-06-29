#================================================================#
# liblua
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := liblua
LOCAL_LDLIBS := -llog -lstring -lm -ldl

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/lua-5.1.3/src_android \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android \

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lapi.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lauxlib.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lbaselib.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lcode.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/ldblib.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/ldebug.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/ldo.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/ldump.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lfunc.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lgc.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/linit.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/liolib.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lmathlib.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lmem.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/loadlib.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lobject.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lopcodes.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/loslib.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lparser.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lstate.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lstring.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lstrlib.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/ltable.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/ltablib.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/ltm.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lundump.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lvm.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/lzio.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/print.c \
$(SRC_PATH)/3rdparty/lua-5.1.3/src_android/llex.c

include $(BUILD_STATIC_LIBRARY)