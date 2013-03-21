cmake_minimum_required ( VERSION 2.8.5 )
project(uslscore)

set ( USLSCORE_INCLUDES "../../src/;../../3rdparty/;../../src/config-default/;../../3rdparty/ooid-0.99/;" CACHE INTERNAL "doc string" )

include_directories ( ${USLSCORE_INCLUDES} ${LUA_INCLUDES} ${SSL_INCLUDES} ${EXPAT_INCLUDES} )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/uslscore/" )

if ( FLASCC_BUILD )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FLASCC__" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FLASCC__" )
endif ( FLASCC_BUILD )

add_library ( uslscore STATIC 

    ${CMAKE_CURRENT_SOURCE_DIR}/STLString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USAdapterInfo_posix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USAdapterInfo_win32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USBase64Encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USBase64Reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USBase64Writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USByteStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USCgt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USColor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USCurve.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USDeflateReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USDeflateWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USDeviceTime_apple.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USDeviceTime_nacl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USDeviceTime_posix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USDeviceTime_win32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USDirectoryItr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USDistance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USFileStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USFileSys.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USFrustum.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriterAdler32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriterCRC32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriterMD5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriterSHA1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriterSHA224.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriterSHA256.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriterSHA384.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriterSHA512.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashWriterWhirlpool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHashedString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHexDump.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHexReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USHexWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USInterpolate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USIntersect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USLexStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USLog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USMemStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USMercator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USPlane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USPolar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USPrism.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USQuad.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USQuadCoord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USQuaternion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USRhombus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USStreamReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USStreamWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USSurface2D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USSyntaxNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USSyntaxScanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USTrig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USTypedPtr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USUnique_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USUnique_win32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USXmlReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USZip.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USZipFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uslscore-pch.cpp
)