cmake_minimum_required ( VERSION 2.8.8 )
project ( moaiandroid )
#mirror the old build.sh file

set ( ANDROID_FORCE_ARM_BUILD ON)

FOREACH(DISABLED ${DISABLED_EXT} )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_${DISABLED}" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_${DISABLED}" CACHE INTERNAL "doc string" )
ENDFOREACH()

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -DNDEBUG -DANDROID_NDK -DDISABLE_IMPORTGL" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -DNDEBUG -DANDROID_NDK -DDISABLE_IMPORTGL" )

set ( INCLUDES_MOAIANDROID
  "${MOAI_DIR}/src/"
  "${MOAI_DIR}/src/config-default/"
  "${MOAI_DIR}/3rdparty"
  "${MOAI_DIR}/3rdparty/lua-5.1.3/src"
  "${MOAI_DIR}/3rdparty/expat-2.0.1/lib"
  "${MOAI_DIR}/3rdparty/tinyxml"
  "${MOAI_DIR}/3rdparty/freetype-2.4.4/include"
    CACHE INTERNAL "doc string" )

include_directories (

    ${INCLUDES_MOAIANDROID}
)


add_library ( moai SHARED
    ${MOAI_DIR}/ant/libmoai/jni/src/moai.cpp
    ${MOAI_DIR}/src/host-modules/aku_modules.cpp
 )


target_link_libraries ( moai
   moai-sim
   moai-util
   moai-android
   moai-core
   zlcore
   third-party
   plugins
   log
   GLESv1_CM
   GLESv2
)

if (MOAI_LUAEXT )
    target_link_libraries( moai  moai-luaext )
endif( MOAI_LUAEXT )

if ( MOAI_HTTP_CLIENT)
    target_link_libraries( moai moai-http-client)
endif (MOAI_HTTP_CLIENT)

if ( MOAI_UNTZ )
   target_link_libraries( moai moai-untz )
endif (MOAI_UNTZ)

if ( MOAI_CHIPMUNK )
  target_link_libraries ( moai moai-chipmunk )
endif ( MOAI_CHIPMUNK )

if ( MOAI_BOX2D )
  target_link_libraries ( moai moai-box2d )
endif ( MOAI_BOX2D )

#emulate the android ndk build script to help with debugging
set(LIBRARY_NAME moai)


# 2. generate gdb.setup
get_directory_property(INCLUDE_DIRECTORIES DIRECTORY . INCLUDE_DIRECTORIES)
string(REGEX REPLACE ";" " " INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/libs/${ANDROID_NDK_ABI_NAME}/gdb.setup "set solib-search-path ./obj/local/${ANDROID_NDK_ABI_NAME}\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/libs/${ANDROID_NDK_ABI_NAME}/gdb.setup "directory ${INCLUDE_DIRECTORIES}\n")

# 3. copy gdbserver executable
file(COPY ${ANDROID_NDK}/prebuilt/android-${ANDROID_ARCH_NAME}/gdbserver/gdbserver DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/libs/${ANDROID_NDK_ABI_NAME}/)

# 4. copy lib to obj
add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ./obj/local/${ANDROID_NDK_ABI_NAME}/)
add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ./libs/${ANDROID_NDK_ABI_NAME}/lib${LIBRARY_NAME}.so ./obj/local/${ANDROID_NDK_ABI_NAME}/)

# 5. strip symbols
add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} ./libs/${ANDROID_NDK_ABI_NAME}/lib${LIBRARY_NAME}.so)
 