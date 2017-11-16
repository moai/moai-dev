cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios )

# Extensions
add_subdirectory(moai-ios-adcolony)
add_subdirectory(moai-ios-billing)
add_subdirectory(moai-ios-chartboost)
add_subdirectory(moai-ios-crittercism)
add_subdirectory(moai-ios-facebook)
add_subdirectory(moai-ios-flurry)
add_subdirectory(moai-ios-gamecenter)
add_subdirectory(moai-ios-kontagent)
add_subdirectory(moai-ios-mobile-app-tracker)
#broken add_subdirectory(moai-ios-motion)
add_subdirectory(moai-ios-movie)
add_subdirectory(moai-ios-playhaven)
add_subdirectory(moai-ios-tapjoy)
add_subdirectory(moai-ios-twitter)
add_subdirectory(moai-ios-urban-airship)
add_subdirectory(moai-ios-vungle)


set ( MOAI_IPHONE_INCLUDES
  "${MOAI_ROOT}/src/"
  ${CMAKE_BINARY_DIR}/libmoai/config
  "${MOAI_ROOT}/src/moai-ios/"
)

# Use all mm files in directory as source code
file ( GLOB MOAI_IPHONE_SRC
  "${MOAI_ROOT}/src/moai-ios/*.mm"
  "${MOAI_ROOT}/src/moai-ios/*.m"
  "${MOAI_ROOT}/src/moai-ios/*.h"
  "${MOAI_ROOT}/src/moai-ios-camera/*.mm"
  "${MOAI_ROOT}/src/moai-ios-camera/*.h"
  "${MOAI_ROOT}/3rdparty/contrib/MOAIIso8601DateFormatter.m"
  "${MOAI_ROOT}/3rdparty/contrib/MOAIOpenUDID.m"
)


include_directories ( ${MOAI_IPHONE_INCLUDES} )
add_library ( moai-ios STATIC ${MOAI_IPHONE_SRC}  )

target_link_libraries ( moai-ios moai-sim moai-util moai-core moai-ios-facebook)

target_include_directories( moai-ios PUBLIC
  "$<BUILD_INTERFACE:${MOAI_IPHONE_INCLUDES}>"
  "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)

set_target_properties( moai-ios PROPERTIES FOLDER Modules )
install(TARGETS moai-ios EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES "${MOAI_ROOT}/src/moai-ios/host.h" DESTINATION "include/moai-ios")
