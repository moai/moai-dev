cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-plugins)

set ( MOAI_PLUGINS_INCLUDES 
    "${MOAI_ROOT}/src"
    "${MOAI_ROOT}/src/config-default"
    )

set (MOAI_PLUGINS_SRC "${MOAI_ROOT}/src/moai-plugins/host.cpp" )

include_directories ( ${MOAI_PLUGINS_INCLUDES} )

add_library ( moai-plugins STATIC ${MOAI_PLUGINS_SRC} )

