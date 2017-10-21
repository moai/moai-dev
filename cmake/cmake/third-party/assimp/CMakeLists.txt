cmake_minimum_required ( VERSION 2.8.11 )
project ( assimp )


set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/assimp/" )
set ( ASSIMP_INCLUDES 
                    "${MOAI_ROOT}/src"                    
                    "${MOAI_ROOT}/3rdparty/assimp-config"
                    "${MOAI_ROOT}/3rdparty/assimp/" 
                    "${MOAI_ROOT}/3rdparty/assimp/include" 
                    "${MOAI_ROOT}/3rdparty/assimp/contrib/openddlparser/include"
                    "${MOAI_ROOT}/3rdparty/assimp/contrib/rapidjson/include"
                    "${MOAI_ROOT}/3rdparty/assimp/contrib/irrXML"
    )

file ( GLOB_RECURSE ASSIMP_SOURCES 
                "${CMAKE_CURRENT_SOURCE_DIR}/code/*.cpp" 
                "${CMAKE_CURRENT_SOURCE_DIR}/contrib/openddlparser/code/*.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/contrib/clipper/*.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/contrib/irrXML/*.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/contrib/poly2tri/*.c*"
                "${CMAKE_CURRENT_SOURCE_DIR}/contrib/unzip/*.c"
                )
file ( GLOB_RECURSE ASSIMP_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*" )

add_library( assimp STATIC ${ASSIMP_SOURCES} ${ASSIMP_HEADERS} )

#remove forced vfs includes, assimp provides a cleaner way to do this http://www.assimp.org/lib_html/usage.html#custom_io
string(REPLACE "${VFS_INCLUDE}" " " VFS_C_FLAGS ${CMAKE_C_FLAGS} )
string(REPLACE "${VFS_INCLUDE}" " " VFS_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
set( CMAKE_C_FLAGS ${VFS_C_FLAGS})
set ( CMAKE_CXX_FLAGS "${VFS_CXX_FLAGS} -std=c++11 -DASSIMP_BUILD_NO_AMF_IMPORTER -DOPENDDL_STATIC_LIBARY -DASSIMP_BUILD_NO_C4D_IMPORTER -DASSIMP_BUILD_NO_OPENGEX_EXPORTER -DASSIMP_BUILD_NO_OPENGEX_IMPORTER -DASSIMP_BUILD_NO_OWN_ZLIB" )

if (BUILD_ANDROID)
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASSIMP_BUILD_NO_3MF_IMPORTER")
endif (BUILD_ANDROID)

if (BUILD_WINDOWS)
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DASSIMP_BUILD_NO_X3D_IMPORTER -DASSIMP_BUILD_NO_X3D_EXPORTER -DASSIMP_BUILD_NO_3MF_IMPORTER")
endif (BUILD_WINDOWS)

target_include_directories ( assimp PUBLIC "$<BUILD_INTERFACE:${ASSIMP_INCLUDES}>")
target_link_libraries( assimp zlib )

set_target_properties( assimp PROPERTIES FOLDER Third-Party )
install(TARGETS assimp EXPORT libmoai ARCHIVE DESTINATION lib)
