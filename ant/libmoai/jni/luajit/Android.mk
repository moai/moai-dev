include $(CLEAR_VARS)

LOCAL_MODULE            := luajit
LOCAL_ARM_MODE          := $(MY_ARM_MODE)
LOCAL_CFLAGS            := -include $(MY_MOAI_ROOT)/src/zl-vfs/zl_replace.h
LOCAL_C_INCLUDES        := $(MY_HEADER_SEARCH_PATHS)
LOCAL_SRC_FILES         := $(MY_MOAI_ROOT)/3rdparty/LuaJIT-2.0.1/android/$(TARGET_ARCH_ABI)/libluajit.a

include $(PREBUILT_STATIC_LIBRARY)
