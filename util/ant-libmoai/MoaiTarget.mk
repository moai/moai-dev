#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE 		:= @LIB_NAME@
LOCAL_LDLIBS 		:= -llog -lGLESv1_CM -lGLESv2

#Disable Modules Below
@AKU_PREPROCESSOR@


#remove unused libraries from this list (be sure to disable the aku flag above too)
#these libraries will never be optimised out by the linker so need to be removed from this list when not in use.
@STATIC_JNI_LIBRARIES@

LOCAL_CFLAGS		:= $(MY_LOCAL_CFLAGS) -DAKU_WITH_PLUGINS=1 -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
LOCAL_C_INCLUDES 	:= $(LOCAL_PATH)/src $(MY_HEADER_SEARCH_PATHS)

LOCAL_SRC_FILES 	+= src/jni.cpp
LOCAL_SRC_FILES 	+= $(wildcard $(LOCAL_PATH)/src/host-modules/*.cpp)
LOCAL_SRC_FILES 	+= src/aku_plugins.cpp

LOCAL_STATIC_LIBRARIES := $(LOCAL_STATIC_JNI_LIBRARIES)  @STATIC_LIBRARIES@ libmoai-util libmoai-core libzl-core libcontrib libexpat libjson liblua libsfmt libsqlite libtinyxml libzl-vfs libzlib libcpufeatures
LOCAL_WHOLE_STATIC_LIBRARIES := @WHOLE_STATIC_LIBRARIES@ libmoai-core libmoai-android

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/cpufeatures)
