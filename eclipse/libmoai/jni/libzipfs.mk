#================================================================#
# libzipfs
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libzipfs

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/src \
$(SRC_PATH)/src/zipfs \
$(SRC_PATH)/3rdparty/zlib-1.2.3 \

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/src/zipfs/zipfs-pch.c \
$(SRC_PATH)/src/zipfs/zipfs.c \
$(SRC_PATH)/src/zipfs/zipfs_util.c \
$(SRC_PATH)/src/zipfs/zipfs_vfscanf.c \
$(SRC_PATH)/src/zipfs/ZIPFSString.c \
$(SRC_PATH)/src/zipfs/ZIPFSVirtualPath.c \
$(SRC_PATH)/src/zipfs/ZIPFSZipFile.c \

include $(BUILD_STATIC_LIBRARY)