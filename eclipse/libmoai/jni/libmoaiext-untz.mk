#================================================================#
# libmoaiext-untz
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libmoaiext-untz
LOCAL_LDLIBS := -llog

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/untz/include \
$(SRC_PATH)/3rdpary/untz/src \
$(SRC_PATH)/src \
$(SRC_PATH)/src/moaicore \
$(SRC_PATH)/src/uslsext \
$(SRC_PATH)/src/uslscore \
$(SRC_PATH)/src/contrib \
$(SRC_PATH)/src/moai-lua-5.1.3/src \
$(SRC_PATH)/3rdparty \
$(SRC_PATH)/3rdparty/expat-2.0.1/amiga \
$(SRC_PATH)/3rdparty/expat-2.0.1/lib \
$(SRC_PATH)/3rdparty/expat-2.0.1/xmlwf \
$(SRC_PATH)/3rdparty/freetype-2.4.4/include \
$(SRC_PATH)/3rdparty/freetype-2.4.4/include/freetype \
$(SRC_PATH)/3rdparty/freetype-2.4.4/include/freetype2 \
$(SRC_PATH)/3rdparty/freetype-2.4.4/builds \
$(SRC_PATH)/3rdparty/freetype-2.4.4/src \
$(SRC_PATH)/3rdparty/freetype-2.4.4/config \
$(SRC_PATH)/3rdparty/jansson-2.1/src \
$(SRC_PATH)/3rdparty/lpng140 \
$(SRC_PATH)/3rdparty/curl-7.19.7/include-android\
$(SRC_PATH)/3rdparty/sqlite-3.6.16 \
$(SRC_PATH)/3rdparty/tinyxml \
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/include \
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk \
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk/constraints \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/Shapes \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints \
$(SRC_PATH)/3rdparty/untz/include \
$(SRC_PATH)/3rdparty/untz/src \
$(SRC_PATH)/3rdparty/untz/src/native/android \
$(SRC_PATH)/3rdparty/untz/src/threading \
$(SRC_PATH)/3rdparty/libvorbis-1.3.2/include \
$(SRC_PATH)/3rdparty/libogg-1.2.2/include

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/untz/src/AudioMixer.cpp \
$(SRC_PATH)/3rdparty/untz/src/MemoryAudioSource.cpp \
$(SRC_PATH)/3rdparty/untz/src/UntzSound.cpp \
$(SRC_PATH)/3rdparty/untz/src/UserAudioSource.cpp \
$(SRC_PATH)/3rdparty/untz/src/BufferedAudioSource.cpp \
$(SRC_PATH)/3rdparty/untz/src/BufferedAudioSourceThread.cpp \
$(SRC_PATH)/3rdparty/untz/src/native/android/UntzSystem.cpp \
$(SRC_PATH)/3rdparty/untz/src/native/android/WaveFileAudioSource.cpp \
$(SRC_PATH)/3rdparty/untz/src/native/android/WaveFile.cpp \
$(SRC_PATH)/3rdparty/untz/src/native/android/RiffFile.cpp \
$(SRC_PATH)/src/moaiext-untz/MOAIUntzSound.cpp \
$(SRC_PATH)/src/moaiext-untz/MOAIUntzSystem.cpp \
#$(SRC_PATH)/3rdparty/untz/src/OggAudioSource.cpp 

include $(BUILD_STATIC_LIBRARY)
