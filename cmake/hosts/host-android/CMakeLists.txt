cmake_minimum_required ( VERSION 2.8.8 )
project ( moaiandroid )
#mirror the old build.sh file

set ( ANDROID_FORCE_ARM_BUILD ON)

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -DNDEBUG -DANDROID_NDK -DDISABLE_IMPORTGL" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -DNDEBUG -DANDROID_NDK -DDISABLE_IMPORTGL" )

set ( INCLUDES_MOAIANDROID
  "${MOAI_ROOT}/src/"
  "${CMAKE_BINARY_DIR}/libmoai/config"
  "${MOAI_ROOT}/3rdparty"
 )

include_directories (
    ${INCLUDES_MOAIANDROID}
)

LINK_DIRECTORIES("${LUA_LIB_DIR}")
add_library ( moai SHARED
    ${MOAI_ROOT}/ant/libmoai/jni/src/moai.cpp
)


target_link_libraries ( moai
   host-modules
   moai-sim
   moai-util
   moai-android
   moai-core
   zlcore
   log
   GLESv1_CM
   GLESv2
)

#emulate the android ndk build script to help with debugging
set(LIBRARY_NAME moai)


# 2. generate gdb.setup
get_directory_property(INCLUDE_DIRECTORIES DIRECTORY . INCLUDE_DIRECTORIES)
string(REGEX REPLACE ";" " " INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}")
file(WRITE ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_NDK_ABI_NAME}/gdb.setup "set solib-search-path ${LIBRARY_OUTPUT_PATH_ROOT}/obj/local/${ANDROID_NDK_ABI_NAME}\n")
file(APPEND ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_NDK_ABI_NAME}/gdb.setup "directory ${INCLUDE_DIRECTORIES}\n")

# 3. copy gdbserver executable
file(COPY ${ANDROID_NDK}/prebuilt/android-${ANDROID_ARCH_NAME}/gdbserver/gdbserver DESTINATION ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_NDK_ABI_NAME}/)

# 4. copy lib to obj
add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBRARY_OUTPUT_PATH_ROOT}/obj/local/${ANDROID_NDK_ABI_NAME}/)
add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_NDK_ABI_NAME}/lib${LIBRARY_NAME}.so ${LIBRARY_OUTPUT_PATH_ROOT}/obj/local/${ANDROID_NDK_ABI_NAME}/)

# 5. strip symbols
add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_NDK_ABI_NAME}/lib${LIBRARY_NAME}.so)
 