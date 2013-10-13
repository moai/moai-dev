cmake_minimum_required ( VERSION 2.8.5 )
project ( playhaven-ios )

set ( PLAYHAVEN_INCLUDES 
  "../../../3rdparty/playhaven-ios-1.13.1/Cache"
  "../../../3rdparty/playhaven-ios-1.13.1/src"
  "../../../3rdparty/playhaven-ios-1.13.1/JSON"
  )



file ( GLOB PLAYHAVEN_SRC "../../../3rdparty/playhaven-ios-1.13.1/src/*.m" )
file ( GLOB PLAYHAVEN_CACHE "../../../3rdparty/playhaven-ios-1.13.1/Cache/*.m" )
file ( GLOB PLAYHAVEN_JSON "../../../3rdparty/playhaven-ios-1.13.1/JSON/*.m" )


include_directories ( ${PLAYHAVEN_INCLUDES} )

add_library ( playhaven-ios STATIC 
                   ${PLAYHAVEN_SRC} 
                   ${PLAYHAVEN_CACHE}
                   ${PLAYHAVEN_JSON}  
            )

