#================================================================#
# libuslsext
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libuslsext
LOCAL_LDLIBS := -llog -lstring -lm -lz -lGLESv1_CM.so

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/src/uslsext \
$(SRC_PATH)/src \
$(SRC_PATH)/src/uslscore \
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
$(SRC_PATH)/3rdparty/lua-5.1.3/src \
$(SRC_PATH)/3rdparty/lpng140 \
$(SRC_PATH)/3rdparty/curl-7.19.7 \
$(SRC_PATH)/3rdparty/curl-7.19.7/lib \
$(SRC_PATH)/3rdparty/curl-7.19.7/include-android \
$(SRC_PATH)/3rdparty/curl-7.19.7/include-android/curl \
$(SRC_PATH)/3rdparty/sqlite-3.6.16 \
$(SRC_PATH)/3rdparty/tinyxml \
$(SRC_PATH)/3rdparty/openssl-1.0.0d/include-android \

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/src/uslsext/pch.cpp \
$(SRC_PATH)/src/uslsext/USAdapterInfo_posix.cpp \
$(SRC_PATH)/src/uslsext/USAdapterInfo_win32.cpp \
$(SRC_PATH)/src/uslsext/USAnimCurve.cpp \
$(SRC_PATH)/src/uslsext/USAttributed.cpp \
$(SRC_PATH)/src/uslsext/USAttrMap.cpp \
$(SRC_PATH)/src/uslsext/USAttrOp.cpp \
$(SRC_PATH)/src/uslsext/USBlendMode.cpp \
$(SRC_PATH)/src/uslsext/USBox.cpp \
$(SRC_PATH)/src/uslsext/USCanvas.cpp \
$(SRC_PATH)/src/uslsext/USCgt.cpp \
$(SRC_PATH)/src/uslsext/USColor.cpp \
$(SRC_PATH)/src/uslsext/USCurve.cpp \
$(SRC_PATH)/src/uslsext/USData.cpp \
$(SRC_PATH)/src/uslsext/USDataIOTask.cpp \
$(SRC_PATH)/src/uslsext/USDebugLines.cpp \
$(SRC_PATH)/src/uslsext/USDistance.cpp \
$(SRC_PATH)/src/uslsext/USDrawBuffer.cpp \
$(SRC_PATH)/src/uslsext/USFont.cpp \
$(SRC_PATH)/src/uslsext/USFontRipper.cpp \
$(SRC_PATH)/src/uslsext/USGfxDevice.cpp \
$(SRC_PATH)/src/uslsext/USGLLine.cpp \
$(SRC_PATH)/src/uslsext/USGLQuad.cpp \
$(SRC_PATH)/src/uslsext/USGrid.cpp \
$(SRC_PATH)/src/uslsext/USHttpTask.cpp \
$(SRC_PATH)/src/uslsext/USHttpTask_impl.cpp \
$(SRC_PATH)/src/uslsext/USImage.cpp \
$(SRC_PATH)/src/uslsext/USInterpolate.cpp \
$(SRC_PATH)/src/uslsext/USIntersect.cpp \
$(SRC_PATH)/src/uslsext/USLexStream.cpp \
$(SRC_PATH)/src/uslsext/USLog.cpp \
$(SRC_PATH)/src/uslsext/uslsext.cpp \
$(SRC_PATH)/src/uslsext/USMercator.cpp \
$(SRC_PATH)/src/uslsext/USMutex.cpp \
$(SRC_PATH)/src/uslsext/USMutex_posix.cpp \
$(SRC_PATH)/src/uslsext/USMutex_win32.cpp \
$(SRC_PATH)/src/uslsext/USParser.cpp \
$(SRC_PATH)/src/uslsext/USPlane.cpp \
$(SRC_PATH)/src/uslsext/USPolar.cpp \
$(SRC_PATH)/src/uslsext/USPrettyPrint.cpp \
$(SRC_PATH)/src/uslsext/USQuad.cpp \
$(SRC_PATH)/src/uslsext/USQuadCoord.cpp \
$(SRC_PATH)/src/uslsext/USSqlBinding.cpp \
$(SRC_PATH)/src/uslsext/USSqlColumn.cpp \
$(SRC_PATH)/src/uslsext/USSqlConnection.cpp \
$(SRC_PATH)/src/uslsext/USSqlRecord.cpp \
$(SRC_PATH)/src/uslsext/USSqlStatement.cpp \
$(SRC_PATH)/src/uslsext/USSqlValue.cpp \
$(SRC_PATH)/src/uslsext/USSurface2D.cpp \
$(SRC_PATH)/src/uslsext/USSyntaxNode.cpp \
$(SRC_PATH)/src/uslsext/USSyntaxScanner.cpp \
$(SRC_PATH)/src/uslsext/USTask.cpp \
$(SRC_PATH)/src/uslsext/USTaskThread.cpp \
$(SRC_PATH)/src/uslsext/USTextFrame.cpp \
$(SRC_PATH)/src/uslsext/USTextLayout.cpp \
$(SRC_PATH)/src/uslsext/USTexture.cpp \
$(SRC_PATH)/src/uslsext/USThread.cpp \
$(SRC_PATH)/src/uslsext/USThread_posix.cpp \
$(SRC_PATH)/src/uslsext/USThread_win32.cpp \
$(SRC_PATH)/src/uslsext/USTile.cpp \
$(SRC_PATH)/src/uslsext/USTrig.cpp \
$(SRC_PATH)/src/uslsext/USTypedPtr.cpp \
$(SRC_PATH)/src/uslsext/USUrlMgr.cpp \
$(SRC_PATH)/src/uslsext/USVertexFormat.cpp \
$(SRC_PATH)/src/uslsext/USVertexFormatMgr.cpp \
$(SRC_PATH)/src/uslsext/USViewport.cpp \
$(SRC_PATH)/src/uslsext/USViewQuad.cpp \
$(SRC_PATH)/src/uslsext/USXmlReader.cpp \

include $(BUILD_STATIC_LIBRARY)