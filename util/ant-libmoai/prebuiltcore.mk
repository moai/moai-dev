LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libmoai-util
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libmoai-util.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libexpat
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libexpat.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libsfmt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libsfmt.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcontrib
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libcontrib.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libsqlite
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libsqlite.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libtinyxml
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libtinyxml.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libzl-core
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libzl-core.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libzl-vfs
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libzl-vfs.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libzlib
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libzlib.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libmoai-core
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libmoai-core.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libjson
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libjson.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := liblua
LOCAL_SRC_FILES := $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/liblua.a
include $(PREBUILT_STATIC_LIBRARY)

