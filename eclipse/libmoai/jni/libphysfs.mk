#================================================================#
# libphysfs
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libphysfs
LOCAL_LDLIBS := -llog
LOCAL_CFLAGS := -std=c99 -DPHYSFS_SUPPORTS_ZIP

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/physfs-2.0.2 \
$(SRC_PATH)/3rdparty/physfs-2.0.2/archivers \
$(SRC_PATH)/3rdparty/physfs-2.0.2/platform \
$(SRC_PATH)/3rdparty/zlib-1.2.3 


#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/physfs-2.0.2/physfs.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/physfs_byteorder.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/physfs_unicode.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/archivers/dir.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/archivers/grp.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/archivers/hog.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/archivers/lzma.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/archivers/mvl.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/archivers/qpak.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/archivers/wad.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/archivers/zip.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/platform/posix.c \
$(SRC_PATH)/3rdparty/physfs-2.0.2/platform/unix.c \


include $(BUILD_STATIC_LIBRARY)