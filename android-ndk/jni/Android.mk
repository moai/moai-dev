LOCAL_PATH := $(call my-dir)

#----------------------------------------------------------------#
# define relative path to src directory
#----------------------------------------------------------------#
SRC_PATH := ../..

include libBox2D.mk
include libChipmunk.mk
include libExpat.mk
include libFreetype.mk
# include libGlew.mk
include libCurl.mk
include libLua.mk
include libSqlite.mk
include libTinyXml.mk
include libPng.mk
include libUslsCore.mk
include libUslsExt.mk
include libMoaiCore.mk
# include libAku.mk
include libContrib.mk

#================================================================#
# Moai
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := Moai
LOCAL_LDLIBS := -lm -llog -ljnigraphics -lGLESv1_CM -ldl -lz
LOCAL_CFLAGS := -DANDROID_NDK -DDISABLE_IMPORTGL

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

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := moaiJNI.cpp \
$(SRC_PATH)/src/aku/pch.cpp \
$(SRC_PATH)/src/aku/AKU.cpp

#----------------------------------------------------------------#
# libraries
#----------------------------------------------------------------#
# LOCAL_STATIC_LIBRARIES += libAku
LOCAL_STATIC_LIBRARIES += libMoaiCore
LOCAL_STATIC_LIBRARIES += libUslsExt
LOCAL_STATIC_LIBRARIES += libUslsCore
LOCAL_STATIC_LIBRARIES += libChipmunk
LOCAL_STATIC_LIBRARIES += libBox2D
LOCAL_STATIC_LIBRARIES += libExpat
# LOCAL_STATIC_LIBRARIES += libGlew
LOCAL_STATIC_LIBRARIES += libLua
LOCAL_STATIC_LIBRARIES += libFreetype
LOCAL_STATIC_LIBRARIES += libSqlite
LOCAL_STATIC_LIBRARIES += libTinyXml
LOCAL_STATIC_LIBRARIES += libPng
LOCAL_STATIC_LIBRARIES += libCurl
LOCAL_STATIC_LIBRARIES += libContrib

include $(BUILD_SHARED_LIBRARY)
