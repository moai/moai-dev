cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-plugins)

set (PLUGINS_ROOT "${MOAI_ROOT}/src/moai-plugins")

set ( MOAI_PLUGINS_INCLUDES 
    "${MOAI_ROOT}/src"
    "${MOAI_ROOT}/src/config-default"
    ${PLUGINS_ROOT}
    )

#
#  Setup our substitution vars
#

set(PLUGIN_HEADERS "// headers here")
set(PLUGIN_FINALIZE "// finalize here")
set(PLUGIN_APPINITIALIZE "// initialize here")
set(PLUGIN_CONTEXTINITIALIZE "// context initialize here")


#
#  Create a host.cpp
#

configure_file( "${PLUGINS_ROOT}/host.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/host.cpp" 
                 [COPYONLY] [ESCAPE_QUOTES] [@ONLY] 
                 [NEWLINE_STYLE [UNIX|DOS|WIN32|LF|CRLF] ])

#
#  actually define the target
#

include_directories ( ${MOAI_PLUGINS_INCLUDES} )

set (MOAI_PLUGINS_SRC "${CMAKE_CURRENT_BINARY_DIR}/host.cpp" )

add_library ( moai-plugins STATIC ${MOAI_PLUGINS_SRC} )

