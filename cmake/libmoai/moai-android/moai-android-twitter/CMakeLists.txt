cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-android-twitter )

#tell our parent to dump our setting for ndk_build
list(APPEND MOAI_ANDROID_MODULES "moai-android-twitter")
set(MOAI_ANDROID_MODULES "${MOAI_ANDROID_MODULES}" PARENT_SCOPE)

set ( MOAI_ANDROID_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${CMAKE_BINARY_DIR}/libmoai/config"
 )

include_directories ( 
  ${MOAI_ANDROID_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-android-twitter" )

# Use all cpp files in directory as source code but don't recurse, so that host-jni.cpp is excluded
file ( GLOB MOAI_ANDROID_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_library ( moai-android-twitter STATIC ${MOAI_ANDROID_SRC} )
target_link_libraries( moai-android-twitter moai-sim moai-util moai-core )

install(TARGETS moai-android-twitter EXPORT libmoai ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION include/moai-android-twitter
          FILES_MATCHING PATTERN "*.h")

#jni
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/jni/host-jni.cpp DESTINATION ../modules/jni/moai-android-twitter)

#java
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/MoaiTwitter.java DESTINATION ../modules/moai/moai-android-twitter)

#third-party
install(DIRECTORY ${MOAI_ROOT}/3rdparty-android/twitter-3.0.3/ DESTINATION ../modules/external/moai-android-twitter)