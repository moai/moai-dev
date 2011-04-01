#================================================================#
# libSqlite
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libSqlite
LOCAL_LDLIBS := -llog -lstring -lm 
LOCAL_CFLAGS := -w -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE \
-DSQLITE_ENABLE_COLUMN_METADATA -D_UNICODE -DUNICODE -D_LIB -DNDEBUG \
-Dfdatasync=fsync

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/sqlite-3.6.16

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/sqlite-3.6.16/sqlite3.c

include $(BUILD_STATIC_LIBRARY)