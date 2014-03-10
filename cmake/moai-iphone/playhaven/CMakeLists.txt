cmake_minimum_required ( VERSION 2.8.5 )
project ( playhaven-ios )

set ( PLAYHAVEN_INCLUDES 
  "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/Cache"
  "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/src"
  "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/JSON"
)

file ( GLOB PLAYHAVEN_SRC "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/src/*.m" "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/src/*.h" )
file ( GLOB PLAYHAVEN_CACHE "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/Cache/*.m" "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/Cache/*.h" )
file ( GLOB PLAYHAVEN_JSON "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/JSON/*.m" "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/JSON/*.h" )

include_directories ( ${PLAYHAVEN_INCLUDES} )

add_library ( playhaven-ios STATIC 
  ${PLAYHAVEN_SRC} 
  ${PLAYHAVEN_CACHE}
  ${PLAYHAVEN_JSON}  
)

set_target_properties( playhaven-ios PROPERTIES FOLDER Third-Party )

