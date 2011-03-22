LOCAL_PATH := $(call my-dir)

# Chipmunk
#
include $(CLEAR_VARS)

LOCAL_MODULE := libChipmunk
LOCAL_LDLIBS := -llog
#LOCAL_CFLAGS	:= -std=c99 -ffast-math -O3 -DNDEBUG
LOCAL_CFLAGS := -std=c99
LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk \
$(LOCAL_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk/constraints

LOCAL_SRC_FILES := 3rdparty/chipmunk-5.3.4/src/chipmunk.c \
3rdparty/chipmunk-5.3.4/src/cpArbiter.c\
3rdparty/chipmunk-5.3.4/src/cpArray.c\
3rdparty/chipmunk-5.3.4/src/cpBB.c\
3rdparty/chipmunk-5.3.4/src/cpBody.c\
3rdparty/chipmunk-5.3.4/src/cpCollision.c\
3rdparty/chipmunk-5.3.4/src/cpHashSet.c\
3rdparty/chipmunk-5.3.4/src/cpPolyShape.c\
3rdparty/chipmunk-5.3.4/src/cpShape.c\
3rdparty/chipmunk-5.3.4/src/cpSpace.c\
3rdparty/chipmunk-5.3.4/src/cpSpaceComponent.c\
3rdparty/chipmunk-5.3.4/src/cpSpaceHash.c\
3rdparty/chipmunk-5.3.4/src/cpSpaceQuery.c\
3rdparty/chipmunk-5.3.4/src/cpSpaceStep.c\
3rdparty/chipmunk-5.3.4/src/cpVect.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpConstraint.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpDampedRotarySpring.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpDampedSpring.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpGearJoint.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpGrooveJoint.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpPinJoint.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpPivotJoint.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpRatchetJoint.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpRotaryLimitJoint.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpSimpleMotor.c\
3rdparty/chipmunk-5.3.4/src/constraints/cpSlideJoint.c


include $(BUILD_STATIC_LIBRARY)


# Expat
#
include $(CLEAR_VARS)

LOCAL_MODULE := libExpat
LOCAL_LDLIBS := -llog -lstring -lm
LOCAL_CFLAGS	:= -DHAVE_MEMMOVE=1
#LOCAL_CFLAGS	:= -std=c99 -ffast-math -O3 -DNDEBUG
LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rdparty/expat-2.0.1/amiga \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/lib \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/xmlwf \

LOCAL_SRC_FILES := 3rdparty/expat-2.0.1/lib/xmlparse.c \
3rdparty/expat-2.0.1/lib/xmlrole.c \
3rdparty/expat-2.0.1/lib/xmltok.c \
3rdparty/expat-2.0.1/lib/xmltok_impl.c \
3rdparty/expat-2.0.1/lib/xmltok_ns.c



include $(BUILD_STATIC_LIBRARY)

# Freetype
#
include $(CLEAR_VARS)

LOCAL_MODULE := libFreetype
LOCAL_LDLIBS := -llog -lstring -lm
LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rdparty/freetype-2.4.4/include \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype2 \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/builds \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/src \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/config
#$(LOCAL_PATH)/3rdparty/expat-2.0.1/xmlwf \

LOCAL_CFLAGS += -W -Wall
LOCAL_CFLAGS += -fPIC -DPIC
LOCAL_CFLAGS += "-DFT2_BUILD_LIBRARY"

LOCAL_SRC_FILES := 3rdparty/freetype-2.4.4/src/autofit/autofit.c \
3rdparty/freetype-2.4.4/src/bdf/bdf.c \
3rdparty/freetype-2.4.4/src/cff/cff.c \
3rdparty/freetype-2.4.4/src/base/ftbase.c \
3rdparty/freetype-2.4.4/src/base/ftbitmap.c \
3rdparty/freetype-2.4.4/src/cache/ftcache.c \
3rdparty/freetype-2.4.4/src/base/ftfstype.c \
3rdparty/freetype-2.4.4/src/base/ftgasp.c \
3rdparty/freetype-2.4.4/src/base/ftglyph.c \
3rdparty/freetype-2.4.4/src/gzip/ftgzip.c \
3rdparty/freetype-2.4.4/src/base/ftinit.c \
3rdparty/freetype-2.4.4/src/lzw/ftlzw.c \
3rdparty/freetype-2.4.4/src/base/ftstroke.c \
3rdparty/freetype-2.4.4/src/base/ftsystem.c \
3rdparty/freetype-2.4.4/src/smooth/smooth.c \
3rdparty/freetype-2.4.4/src/base/ftbbox.c \
3rdparty/freetype-2.4.4/src/base/ftgxval.c \
3rdparty/freetype-2.4.4/src/base/ftlcdfil.c \
3rdparty/freetype-2.4.4/src/base/ftmm.c \
3rdparty/freetype-2.4.4/src/base/ftotval.c \
3rdparty/freetype-2.4.4/src/base/ftpatent.c \
3rdparty/freetype-2.4.4/src/base/ftpfr.c \
3rdparty/freetype-2.4.4/src/base/ftsynth.c \
3rdparty/freetype-2.4.4/src/base/fttype1.c \
3rdparty/freetype-2.4.4/src/base/ftwinfnt.c \
3rdparty/freetype-2.4.4/src/base/ftxf86.c \
3rdparty/freetype-2.4.4/src/pcf/pcf.c \
3rdparty/freetype-2.4.4/src/pfr/pfr.c \
3rdparty/freetype-2.4.4/src/psaux/psaux.c \
3rdparty/freetype-2.4.4/src/pshinter/pshinter.c \
3rdparty/freetype-2.4.4/src/psnames/psmodule.c \
3rdparty/freetype-2.4.4/src/raster/raster.c \
3rdparty/freetype-2.4.4/src/sfnt/sfnt.c \
3rdparty/freetype-2.4.4/src/truetype/truetype.c \
3rdparty/freetype-2.4.4/src/type1/type1.c \
3rdparty/freetype-2.4.4/src/cid/type1cid.c \
3rdparty/freetype-2.4.4/src/type42/type42.c \
3rdparty/freetype-2.4.4/src/winfonts/winfnt.c \


include $(BUILD_STATIC_LIBRARY)

# Glew
#
#include $(CLEAR_VARS)

#LOCAL_MODULE := libGlew
#LOCAL_LDLIBS := -llog -lstring -lm
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rdparty/glew-1.5.6/include \
#$(LOCAL_PATH)/3rdparty/glew-1.5.6/src
#LOCAL_SRC_FILES := 3rdparty/glew-1.5.6/src/glew.c 

#include $(BUILD_STATIC_LIBRARY)

# Libcurl
#
#
common_CFLAGS := -Wpointer-arith -Wwrite-strings -Wunused -Winline \
-Wnested-externs -Wmissing-declarations -Wmissing-prototypes -Wno-long-long \
-Wfloat-equal -Wno-multichar -Wsign-compare -Wno-format-nonliteral \
-Wendif-labels -Wstrict-prototypes -Wdeclaration-after-statement \
-Wno-system-headers -DHAVE_CONFIG_H 
include $(CLEAR_VARS) 
include $(LOCAL_PATH)/3rdparty/curl-7.19.7/src/Makefile.inc 
MY_SRCS := file.c timeval.c base64.c hostip.c progress.c formdata.c \
  cookie.c http.c sendf.c ftp.c url.c dict.c if2ip.c speedcheck.c \
  ldap.c ssluse.c version.c getenv.c escape.c mprintf.c telnet.c \
  netrc.c getinfo.c transfer.c strequal.c easy.c security.c krb4.c \
  krb5.c memdebug.c http_chunks.c strtok.c connect.c llist.c hash.c \
  multi.c content_encoding.c share.c http_digest.c md5.c curl_rand.c \
  http_negotiate.c http_ntlm.c inet_pton.c strtoofft.c strerror.c \
  hostares.c hostasyn.c hostip4.c hostip6.c hostsyn.c hostthre.c \
  inet_ntop.c parsedate.c select.c gtls.c sslgen.c tftp.c splay.c \
  strdup.c socks.c ssh.c nss.c qssl.c rawstr.c curl_addrinfo.c \
  socks_gssapi.c socks_sspi.c curl_sspi.c slist.c nonblock.c \
  curl_memrchr.c 

MY_CURL_HEADERS := \
    curlbuild.h \
    curl.h \
    curlrules.h \
    curlver.h \
    easy.h \
    mprintf.h \
    multi.h \
	setup.h \
    stdcheaders.h \
    typecheck-gcc.h \
    types.h 

LOCAL_C_INCLUDES += $(LOCAL_PATH)/3rdparty/curl-7.19.7/include-android
LOCAL_SRC_FILES := $(addprefix 3rdparty/curl-7.19.7/lib/,$(MY_SRCS)) 
LOCAL_CFLAGS += $(common_CFLAGS) 
LOCAL_COPY_HEADERS_TO := libcurl 
LOCAL_COPY_HEADERS := $(addprefix 3rdparty/curl-7.19.7/include/curl/,$(MY_CURL_HEADERS)) 
LOCAL_MODULE:= libcurl 
include $(BUILD_STATIC_LIBRARY) 

# Lua-lib
#
include $(CLEAR_VARS)
LOCAL_MODULE := libLua
LOCAL_LDLIBS := -llog -lstring -lm -ldl
LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rdparty/lua-5.1.3/src_android 
LOCAL_SRC_FILES := 3rdparty/lua-5.1.3/src_android/lapi.c \
3rdparty/lua-5.1.3/src_android/lauxlib.c \
3rdparty/lua-5.1.3/src_android/lbaselib.c \
3rdparty/lua-5.1.3/src_android/lcode.c \
3rdparty/lua-5.1.3/src_android/ldblib.c \
3rdparty/lua-5.1.3/src_android/ldebug.c \
3rdparty/lua-5.1.3/src_android/ldo.c \
3rdparty/lua-5.1.3/src_android/ldump.c \
3rdparty/lua-5.1.3/src_android/lfunc.c \
3rdparty/lua-5.1.3/src_android/lgc.c \
3rdparty/lua-5.1.3/src_android/linit.c \
3rdparty/lua-5.1.3/src_android/liolib.c \
3rdparty/lua-5.1.3/src_android/lmathlib.c \
3rdparty/lua-5.1.3/src_android/lmem.c \
3rdparty/lua-5.1.3/src_android/loadlib.c \
3rdparty/lua-5.1.3/src_android/lobject.c \
3rdparty/lua-5.1.3/src_android/lopcodes.c \
3rdparty/lua-5.1.3/src_android/loslib.c \
3rdparty/lua-5.1.3/src_android/lparser.c \
3rdparty/lua-5.1.3/src_android/lstate.c \
3rdparty/lua-5.1.3/src_android/lstring.c \
3rdparty/lua-5.1.3/src_android/lstrlib.c \
3rdparty/lua-5.1.3/src_android/ltable.c \
3rdparty/lua-5.1.3/src_android/ltablib.c \
3rdparty/lua-5.1.3/src_android/ltm.c \
3rdparty/lua-5.1.3/src_android/lundump.c \
3rdparty/lua-5.1.3/src_android/lvm.c \
3rdparty/lua-5.1.3/src_android/lzio.c \
3rdparty/lua-5.1.3/src_android/print.c \
3rdparty/lua-5.1.3/src_android/llex.c

include $(BUILD_STATIC_LIBRARY)

# Soil
#

include $(CLEAR_VARS)

LOCAL_MODULE := libSoil
LOCAL_LDLIBS := -llog -lstring -lm -lGLESv1_CM.so -DANDROID_NDK=1
LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rdparty/SOIL/src
LOCAL_SRC_FILES := 3rdparty/SOIL/src/image_DXT.cpp \
3rdparty/SOIL/src/image_helper.cpp \
3rdparty/SOIL/src/SOIL.cpp \
3rdparty/SOIL/src/stb_image_aug.cpp

include $(BUILD_STATIC_LIBRARY)

# Sqlite
#
include $(CLEAR_VARS)

LOCAL_MODULE := libSqlite
LOCAL_LDLIBS := -llog -lstring -lm 
LOCAL_CFLAGS := -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE \
-DSQLITE_ENABLE_COLUMN_METADATA -D_UNICODE -DUNICODE -D_LIB -DNDEBUG \
-Dfdatasync=fsync
LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rdparty/sqlite-3.6.16
LOCAL_SRC_FILES := 3rdparty/sqlite-3.6.16/sqlite3.c

include $(BUILD_STATIC_LIBRARY)

# Tinyxml
#
include $(CLEAR_VARS)

LOCAL_MODULE := libTinyxml
LOCAL_LDLIBS := -llog -lstring -lm -ldl
LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rdparty/tinyxml
LOCAL_SRC_FILES := 3rdparty/tinyxml/tinyxml.cpp \
3rdparty/tinyxml/tinyxmlerror.cpp \
3rdparty/tinyxml/tinyxmlparser.cpp \
3rdparty/tinyxml/tinystr.cpp

include $(BUILD_STATIC_LIBRARY)

# Libpng
#
include $(CLEAR_VARS)

LOCAL_MODULE := libPng
LOCAL_LDLIBS := -llog -lstring -lm -lz
LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rdparty/lpng140
LOCAL_SRC_FILES := 3rdparty/lpng140/png.c \
3rdparty/lpng140/pngerror.c \
3rdparty/lpng140/pngget.c \
3rdparty/lpng140/pngmem.c \
3rdparty/lpng140/pngpread.c \
3rdparty/lpng140/pngread.c \
3rdparty/lpng140/pngrio.c \
3rdparty/lpng140/pngrtran.c \
3rdparty/lpng140/pngrutil.c \
3rdparty/lpng140/pngset.c \
3rdparty/lpng140/pngtest.c \
3rdparty/lpng140/pngtrans.c \
3rdparty/lpng140/pngwio.c \
3rdparty/lpng140/pngwrite.c \
3rdparty/lpng140/pngwtran.c \
3rdparty/lpng140/pngwutil.c

include $(BUILD_STATIC_LIBRARY)

# Uslscore

include $(CLEAR_VARS)

LOCAL_MODULE := libUslscore
LOCAL_LDLIBS := -llog -lstring -lm -lz
LOCAL_C_INCLUDES := $(LOCAL_PATH)/uslscore \
$(LOCAL_PATH)/3rdparty/lua-5.1.3/src 

LOCAL_SRC_FILES := src/uslscore/pch.cpp \
src/uslscore/STLString.cpp \
src/uslscore/USBase64Cipher.cpp \
src/uslscore/USByteStream.cpp \
src/uslscore/USCanary.cpp \
src/uslscore/USCipher.cpp \
src/uslscore/USCipherStream.cpp \
src/uslscore/USDeviceTime_apple.cpp \
src/uslscore/USDeviceTime_posix.cpp \
src/uslscore/USDeviceTime_win32.cpp \
src/uslscore/USDirectoryItr_posix.cpp \
src/uslscore/USDirectoryItr_win32.cpp \
src/uslscore/USFilename.cpp \
src/uslscore/USFileStream.cpp \
src/uslscore/USFileSys.cpp \
src/uslscore/USFloat.cpp \
src/uslscore/USGlobals.cpp \
src/uslscore/uslscore.cpp \
src/uslscore/USLuaData.cpp \
src/uslscore/USLuaRef.cpp \
src/uslscore/USLuaRuntime.cpp \
src/uslscore/USLuaSerializer.cpp \
src/uslscore/USLuaState.cpp \
src/uslscore/USLuaStateHandle.cpp \
src/uslscore/USMemStream.cpp \
src/uslscore/USObject.cpp \
src/uslscore/USRtti.cpp \
src/uslscore/USStream.cpp \
src/uslscore/USStreamFormatter.cpp

include $(BUILD_STATIC_LIBRARY)



# Uslsext

include $(CLEAR_VARS)

LOCAL_MODULE := libUslsext
LOCAL_LDLIBS := -llog -lstring -lm -lz -lGLESv1_CM.so
LOCAL_C_INCLUDES := $(LOCAL_PATH)/uslsext \
$(LOCAL_PATH)/src/uslscore \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/amiga \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/lib \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/xmlwf \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype2 \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/builds \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/src \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/config \
$(LOCAL_PATH)/3rdparty/lua-5.1.3/src \
$(LOCAL_PATH)/3rdparty/lpng140 \
$(LOCAL_PATH)/3rdparty/curl-7.19.7/include-android\
$(LOCAL_PATH)/3rdparty/SOIL/src \
$(LOCAL_PATH)/3rdparty/sqlite-3.6.16 \
$(LOCAL_PATH)/3rdparty/tinyxml \

LOCAL_SRC_FILES := src/uslsext/pch.cpp \
src/uslsext/USAdapterInfo_posix.cpp \
src/uslsext/USAdapterInfo_win32.cpp \
src/uslsext/USAnimCurve.cpp \
src/uslsext/USAttributed.cpp \
src/uslsext/USAttrMap.cpp \
src/uslsext/USAttrOp.cpp \
src/uslsext/USBlendMode.cpp \
src/uslsext/USBox.cpp \
src/uslsext/USCamera2D.cpp \
src/uslsext/USCamera3D.cpp \
src/uslsext/USCameraFitter.cpp \
src/uslsext/USCanvas.cpp \
src/uslsext/USColor.cpp \
src/uslsext/USCurve.cpp \
src/uslsext/USCurveSampler.cpp \
src/uslsext/USData.cpp \
src/uslsext/USDataIOTask.cpp \
src/uslsext/USDebugLines.cpp \
src/uslsext/USDeflater.cpp \
src/uslsext/USDistance.cpp \
src/uslsext/USFileCache.cpp \
src/uslsext/USFont.cpp \
src/uslsext/USFontRipper.cpp \
src/uslsext/USFrustum.cpp \
src/uslsext/USGfxDevice.cpp \
src/uslsext/USGLLine.cpp \
src/uslsext/USGLQuad.cpp \
src/uslsext/USGrid.cpp \
src/uslsext/USHitMask.cpp \
src/uslsext/USHttpTask.cpp \
src/uslsext/USHttpTask_impl.cpp \
src/uslsext/USImage.cpp \
src/uslsext/USInflater.cpp \
src/uslsext/USInterpolate.cpp \
src/uslsext/USIntersect.cpp \
src/uslsext/USLog.cpp \
src/uslsext/uslsext.cpp \
src/uslsext/USMercator.cpp \
src/uslsext/USMutex.cpp \
src/uslsext/USMutex_posix.cpp \
src/uslsext/USMutex_win32.cpp \
src/uslsext/USPartition.cpp \
src/uslsext/USPlane.cpp \
src/uslsext/USPolar.cpp \
src/uslsext/USPrettyPrint.cpp \
src/uslsext/USQuad.cpp \
src/uslsext/USQuadCoord.cpp \
src/uslsext/USSlipMapBrush.cpp \
src/uslsext/USSlipMapTile.cpp \
src/uslsext/USSlipMapTileSource.cpp \
src/uslsext/USSqlBinding.cpp \
src/uslsext/USSqlColumn.cpp \
src/uslsext/USSqlConnection.cpp \
src/uslsext/USSqlRecord.cpp \
src/uslsext/USSqlStatement.cpp \
src/uslsext/USSqlValue.cpp \
src/uslsext/USSurface2D.cpp \
src/uslsext/USTask.cpp \
src/uslsext/USTaskThread.cpp \
src/uslsext/USTexture.cpp \
src/uslsext/USThread.cpp \
src/uslsext/USThread_posix.cpp \
src/uslsext/USThread_win32.cpp \
src/uslsext/USTile.cpp \
src/uslsext/USTrig.cpp \
src/uslsext/USTypedPtr.cpp \
src/uslsext/USUrlMgr.cpp \
src/uslsext/USViewport.cpp \
src/uslsext/USViewQuad.cpp \
src/uslsext/USXmlReader.cpp \
src/uslsext/USZip.cpp \
src/uslsext/USZipFile.cpp

include $(BUILD_STATIC_LIBRARY)


# Moaicore
include $(CLEAR_VARS)

LOCAL_MODULE := libMoaicore
LOCAL_LDLIBS := -llog -lstring -lm -lz -lGLESv1_CM.so
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src/moaicore \
$(LOCAL_PATH)/src/uslsext \
$(LOCAL_PATH)/src/uslscore \
$(LOCAL_PATH)/src/contrib \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/amiga \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/lib \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/xmlwf \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype2 \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/builds \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/src \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/config \
$(LOCAL_PATH)/3rdparty/lua-5.1.3/src \
$(LOCAL_PATH)/3rdparty/lpng140 \
$(LOCAL_PATH)/3rdparty/curl-7.19.7/include-android\
$(LOCAL_PATH)/3rdparty/SOIL/src \
$(LOCAL_PATH)/3rdparty/sqlite-3.6.16 \
$(LOCAL_PATH)/3rdparty/tinyxml \
$(LOCAL_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk \
$(LOCAL_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk/constraints

LOCAL_SRC_FILES := src/moaicore/pch.cpp \
src/moaicore/MOAIAction.cpp \
src/moaicore/MOAIAnim.cpp \
src/moaicore/MOAIAnimCurve.cpp \
src/moaicore/MOAIAnimPlayer.cpp \
src/moaicore/MOAIBlocker.cpp \
src/moaicore/MOAIBrushLibrary.cpp \
src/moaicore/MOAIButtonSensor.cpp \
src/moaicore/MOAICamera2D.cpp \
src/moaicore/MOAICameraFitter.cpp \
src/moaicore/MOAICheckpoint.cpp \
src/moaicore/MOAICollisionMgr.cpp \
src/moaicore/MOAIColor.cpp \
src/moaicore/MOAICompassSensor.cpp \
src/moaicore/MOAIContentLibrary2D.cpp \
src/moaicore/moaicore.cpp \
src/moaicore/MOAICp.cpp \
src/moaicore/MOAICpArbiter.cpp \
src/moaicore/MOAICpBody.cpp \
src/moaicore/MOAICpConstraint.cpp \
src/moaicore/MOAICpShape.cpp \
src/moaicore/MOAICpSpace.cpp \
src/moaicore/MOAIData.cpp \
src/moaicore/MOAIDataIOAction.cpp \
src/moaicore/MOAIDebugLines.cpp \
src/moaicore/MOAIEulerDynamics.cpp \
src/moaicore/MOAIFeeler2D.cpp \
src/moaicore/MOAIFileCache.cpp \
src/moaicore/MOAIFmod.cpp \
src/moaicore/MOAIFmodChannel.cpp \
src/moaicore/MOAIFmodSound.cpp \
src/moaicore/MOAIFont.cpp \
src/moaicore/MOAIForceAction.cpp \
src/moaicore/MOAIFrame.cpp \
src/moaicore/MOAIGfxPrim2D.cpp \
src/moaicore/MOAIGrid.cpp \
src/moaicore/MOAIHttpTask.cpp \
src/moaicore/MOAIInputDevice.cpp \
src/moaicore/MOAIInputMgr.cpp \
src/moaicore/MOAIJoystickSensor.cpp \
src/moaicore/MOAIKeyboardSensor.cpp \
src/moaicore/MOAILayer2D.cpp \
src/moaicore/MOAILayerBridge2D.cpp \
src/moaicore/MOAILevelSensor.cpp \
src/moaicore/MOAILocationSensor.cpp \
src/moaicore/MOAILog.cpp \
src/moaicore/MOAILuaScript.cpp \
src/moaicore/MOAIMarioFsm2D.cpp \
src/moaicore/MOAIMultiShape2D.cpp \
src/moaicore/MOAINinePatch.cpp \
src/moaicore/MOAINode.cpp \
src/moaicore/MOAINodeHarness.cpp \
src/moaicore/MOAINodeMgr.cpp \
src/moaicore/MOAIOscillator.cpp \
src/moaicore/MOAIOverlapPrim2D.cpp \
src/moaicore/MOAIParticleEmitter.cpp \
src/moaicore/MOAIParticleEngine.cpp \
src/moaicore/MOAIParticleForce.cpp \
src/moaicore/MOAIParticleScript.cpp \
src/moaicore/MOAIParticleState.cpp \
src/moaicore/MOAIParticleSystem.cpp \
src/moaicore/MOAIPartition.cpp \
src/moaicore/MOAIPointerSensor.cpp \
src/moaicore/MOAIPolar.cpp \
src/moaicore/MOAIPrim.cpp \
src/moaicore/MOAIQuad.cpp \
src/moaicore/MOAIQuadLibrary.cpp \
src/moaicore/MOAIRect.cpp \
src/moaicore/MOAIScriptNode.cpp \
src/moaicore/MOAISensor.cpp \
src/moaicore/MOAISerializer.cpp \
src/moaicore/MOAIShader.cpp \
src/moaicore/MOAISim.cpp \
src/moaicore/MOAISimpleShader.cpp \
src/moaicore/MOAISlippyMap.cpp \
src/moaicore/MOAISprite2D.cpp \
src/moaicore/MOAISpriteLibrary.cpp \
src/moaicore/MOAISurfaceLibrary2D.cpp \
src/moaicore/MOAISurfaceSampler2D.cpp \
src/moaicore/MOAITextBox.cpp \
src/moaicore/MOAITextSpoolAction.cpp \
src/moaicore/MOAITexture.cpp \
src/moaicore/MOAIThread.cpp \
src/moaicore/MOAITileLibrary.cpp \
src/moaicore/MOAITilemap.cpp \
src/moaicore/MOAITimer.cpp \
src/moaicore/MOAITouchSensor.cpp \
src/moaicore/MOAITransform2D.cpp \
src/moaicore/MOAITransformBase2D.cpp \
src/moaicore/MOAITransition.cpp \
src/moaicore/MOAIViewport.cpp \
src/moaicore/MOAIXmlParser.cpp

include $(BUILD_STATIC_LIBRARY)

# Aku
#include $(CLEAR_VARS)

#LOCAL_MODULE := libAku
#LOCAL_LDLIBS := -llog -lstring -lm -lz -lGLESv1_CM.so
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/src/aku \
$(LOCAL_PATH)/moaicore \
$(LOCAL_PATH)/uslscore \
$(LOCAL_PATH)/uslsext \
$(LOCAL_PATH)/contrib \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/amiga \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/lib \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/xmlwf \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype2 \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/builds \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/src \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/config \
$(LOCAL_PATH)/3rdparty/lua-5.1.3/src \
$(LOCAL_PATH)/3rdparty/lpng140 \
$(LOCAL_PATH)/3rdparty/curl-7.19.7/include-android\
$(LOCAL_PATH)/3rdparty/SOIL/src \
$(LOCAL_PATH)/3rdparty/sqlite-3.6.16 \
$(LOCAL_PATH)/3rdparty/tinyxml \
$(LOCAL_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk \
$(LOCAL_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk/constraints

#LOCAL_SRC_FILES := aku/pch.cpp \
aku/AKU.cpp

#LOCAL_STATIC_LIBRARIES  += libMoaicore
#include $(BUILD_STATIC_LIBRARY)

# Moai JNI
include $(CLEAR_VARS)

LOCAL_MODULE    := Moai
LOCAL_C_INCLUDES :=  $(LOCAL_PATH) \
$(LOCAL_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk \
$(LOCAL_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk/constraints \
$(LOCAL_PATH)/src/aku \
$(LOCAL_PATH)/src/moaicore \
$(LOCAL_PATH)/src/uslsext \
$(LOCAL_PATH)/src/uslscore \
$(LOCAL_PATH)/src/contrib \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/amiga \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/lib \
$(LOCAL_PATH)/3rdparty/expat-2.0.1/xmlwf \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/include/freetype2 \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/builds \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/src \
$(LOCAL_PATH)/3rdparty/freetype-2.4.4/config \
$(LOCAL_PATH)/3rdparty/lua-5.1.3/src \
$(LOCAL_PATH)/3rdparty/lpng140 \
$(LOCAL_PATH)/3rdparty/curl-7.19.7/include-android\
$(LOCAL_PATH)/3rdparty/SOIL/src \
$(LOCAL_PATH)/3rdparty/sqlite-3.6.16 \
$(LOCAL_PATH)/3rdparty/tinyxml \

LOCAL_SRC_FILES := moaiJNI.cpp \
src/aku/pch.cpp \
src/aku/AKU.cpp

LOCAL_LDLIBS    := -lm -llog -ljnigraphics -lGLESv1_CM -ldl -lz
LOCAL_CFLAGS := -DANDROID_NDK -DDISABLE_IMPORTGL
LOCAL_STATIC_LIBRARIES := 
#LOCAL_STATIC_LIBRARIES += libAku
LOCAL_STATIC_LIBRARIES  += libMoaicore
LOCAL_STATIC_LIBRARIES += libUslsext
LOCAL_STATIC_LIBRARIES += libUslscore
LOCAL_STATIC_LIBRARIES += libChipmunk
LOCAL_STATIC_LIBRARIES += libExpat
#LOCAL_STATIC_LIBRARIES += libGlew
LOCAL_STATIC_LIBRARIES += libLua
LOCAL_STATIC_LIBRARIES += libFreetype
LOCAL_STATIC_LIBRARIES += libSoil
LOCAL_STATIC_LIBRARIES += libSqlite
LOCAL_STATIC_LIBRARIES += libTinyxml
LOCAL_STATIC_LIBRARIES += libPng
LOCAL_STATIC_LIBRARIES += libcurl




include $(BUILD_SHARED_LIBRARY)
