cmake_minimum_required ( VERSION 2.8.5 )
project ( zlcore )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/zlcore/" )
set ( ZLCORE_INCLUDES "../../src/;../../3rdparty/tlsf-2.0/" CACHE INTERNAL "doc string" )

if ( FLASCC_BUILD )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FLASCC__" )
endif ( FLASCC_BUILD )


include_directories ( ${ZLCORE_INCLUDES} )

add_library ( zlcore STATIC 

    ${CMAKE_CURRENT_SOURCE_DIR}/ZLDirectoryItr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLFileSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLVirtualPath.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLZipArchive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLZipStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zl_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zl_util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zl_vfscanf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zlcore-pch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zlcore.cpp
	  ${CMAKE_CURRENT_SOURCE_DIR}/zgl_flascc.cpp
)