LOCAL_PATH := $(call my-dir)

#----------------------------------------------------------------#
# define relative path to src directory
#----------------------------------------------------------------#
SRC_PATH := ../../..

include libbox2D.mk
include libchipmunk.mk
include libexpat.mk
include libfreetype.mk
include libcurl.mk
include liblua.mk
include libsqlite.mk
include libtinyxml.mk
include libpng.mk
include libuslscore.mk
include libuslsext.mk
include libmoaicore.mk
include libssl.mk
include libcontrib.mk

#================================================================#
# Moai
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libmoai
LOCAL_LDLIBS := -lm -llog -ljnigraphics -lGLESv1_CM -ldl -lz libcrypto.a
LOCAL_CFLAGS := -DANDROID_NDK -DDISABLE_IMPORTGL
APP_STL := gnustl_static

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES :=  $(SRC_PATH) \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/Shapes \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints \
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/include \
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk \
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk/constraints \
$(SRC_PATH)/src \
$(SRC_PATH)/src/aku \
$(SRC_PATH)/src/moaicore \
$(SRC_PATH)/src/uslsext \
$(SRC_PATH)/src/uslscore \
$(SRC_PATH)/src/contrib \
$(SRC_PATH)/3rdparty/contrib \
$(SRC_PATH)/3rdparty/expat-2.0.1/amiga \
$(SRC_PATH)/3rdparty/expat-2.0.1/lib \
$(SRC_PATH)/3rdparty/expat-2.0.1/xmlwf \
$(SRC_PATH)/3rdparty/freetype-2.4.4/include \
$(SRC_PATH)/3rdparty/freetype-2.4.4/include/freetype \
$(SRC_PATH)/3rdparty/freetype-2.4.4/include/freetype2 \
$(SRC_PATH)/3rdparty/freetype-2.4.4/builds \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src \
$(SRC_PATH)/3rdparty/freetype-2.4.4/config \
$(SRC_PATH)/3rdparty/lua-5.1.3/src \
$(SRC_PATH)/3rdparty/lpng140 \
$(SRC_PATH)/3rdparty/curl-7.19.7/include-android\
$(SRC_PATH)/3rdparty/sqlite-3.6.16 \
$(SRC_PATH)/3rdparty/tinyxml \
$(SRC_PATH)/3rdparty/openssl-1.0.0d/include-android \

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := moaiJNI.cpp \
$(SRC_PATH)/src/aku/pch.cpp \
$(SRC_PATH)/src/aku/AKU.cpp

#----------------------------------------------------------------#
# libraries
#----------------------------------------------------------------#
LOCAL_STATIC_LIBRARIES += libmoaicore
LOCAL_STATIC_LIBRARIES += libuslsext
LOCAL_STATIC_LIBRARIES += libuslscore
LOCAL_STATIC_LIBRARIES += libchipmunk
LOCAL_STATIC_LIBRARIES += libbox2D
LOCAL_STATIC_LIBRARIES += libexpat
LOCAL_STATIC_LIBRARIES += liblua
LOCAL_STATIC_LIBRARIES += libfreetype
LOCAL_STATIC_LIBRARIES += libsqlite
LOCAL_STATIC_LIBRARIES += libtinyxml
LOCAL_STATIC_LIBRARIES += libpng
LOCAL_STATIC_LIBRARIES += libcurl
LOCAL_STATIC_LIBRARIES += libssl
LOCAL_STATIC_LIBRARIES += libcontrib

include $(BUILD_SHARED_LIBRARY)
