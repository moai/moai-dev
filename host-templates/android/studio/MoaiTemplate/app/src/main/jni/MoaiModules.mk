
MOAI_LIBS := $(MOAI_LIB_ROOT)/lib
include $(CLEAR_VARS)


LOCAL_MODULE    := sfmt 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libsfmt.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := pvr 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libpvr.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := moai-core 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-core.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := curl 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libcurl.a

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := ssl 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libssl.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := tess
LOCAL_SRC_FILES := $(MOAI_LIBS)/libtess.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := crypto 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libcrypto.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := tlsf 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libtlsf.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := moai-http-client 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-http-client.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := luacrypto 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libluacrypto.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := tinyxml 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libtinyxml.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := mongoose 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmongoose.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := sqlite3 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libsqlite3.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := ogg 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libogg.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := vorbis 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libvorbis.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := untz 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libuntz.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := moai-luaext 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-luaext.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := moai-untz 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-untz.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := moai-box2d 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-box2d.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := moai-crypto
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-crypto.a

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := moai-util 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-util.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := moai-sim 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-sim.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := liblua-static 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libliblua-static.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := luafilesystem 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libluafilesystem.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := jansson 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libjansson.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := cares 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libcares.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := jpg 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libjpg.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := luasocket 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libluasocket.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := zlcore 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libzlcore.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := zlcrypto
LOCAL_SRC_FILES := $(MOAI_LIBS)/libzlcrypto.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := zlib 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libzlib.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := box2d 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libbox2d.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := moai-android 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-android.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := png 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libpng.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := freetype 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libfreetype.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := expat 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libexpat.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := contrib 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libcontrib.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := luasql 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libluasql.a

include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE    := zlvfs 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libzlvfs.a

include $(PREBUILT_STATIC_LIBRARY)	

include $(CLEAR_VARS)

LOCAL_MODULE    := luacurl 
LOCAL_SRC_FILES := $(MOAI_LIBS)/libluacurl.a

include $(PREBUILT_STATIC_LIBRARY)
