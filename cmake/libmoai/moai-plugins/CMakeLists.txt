cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-plugins)

set (PLUGINS_ROOT "${MOAI_ROOT}/src/moai-plugins")

set ( MOAI_PLUGINS_INCLUDES 
    "${MOAI_ROOT}/src"
   "${CMAKE_BINARY_DIR}/libmoai/config"
    ${PLUGINS_ROOT}
    )

#
#  Setup our substitution vars
#

set(PLUGIN_HEADERS "// plugin headers ")
set(PLUGIN_FINALIZE "// finalize here")
set(PLUGIN_APPINITIALIZE "// initialize here")
set(PLUGIN_CONTEXTINITIALIZE "// context initialize here")
set(PLUGIN_UPDATE "// plugin update here")


#find our plugins
if (NOT PLUGIN_DIR)
	 message( FATAL_ERROR "MOAI_PLUGINS was specified but no PLUGIN_DIR was given" )
endif (NOT PLUGIN_DIR)

include_directories(${PLUGIN_DIR})

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(PLUGINS ${PLUGIN_DIR} )
set(LINK_LIBS "")
FOREACH(PLUGIN ${PLUGINS})
	IF(EXISTS ${PLUGIN_DIR}/${PLUGIN}/plugin.CMake)
		include(${PLUGIN_DIR}/${PLUGIN}/plugin.CMake)
		string(TOUPPER ${PLUGIN_TARGET} UPPER_TARGET)
		set(PLUGIN_VAR "PLUGIN_${UPPER_TARGET}")
		IF(${PLUGIN_VAR})
			message ("Adding Plugin: ${PLUGIN_INFO}")
			set (PLUGIN_HEADERS "${PLUGIN_HEADERS}\n#include <${PLUGIN}/host.h>" )
			set (PLUGIN_APPINITIALIZE "${PLUGIN_APPINITIALIZE}\n${PLUGIN_PREFIX}AppInitialize();" )
			set (PLUGIN_CONTEXTINITIALIZE "${PLUGIN_CONTEXTINITIALIZE}\n${PLUGIN_PREFIX}ContextInitialize();" )
			set (PLUGIN_FINALIZE "${PLUGIN_FINALIZE}\n${PLUGIN_PREFIX}AppFinalize();" )
			IF(PLUGIN_NEEDS_UPDATE)
				set (PLUGIN_UPDATE "${PLUGIN_UPDATE}\n${PLUGIN_PREFIX}Update();")
			ENDIF(PLUGIN_NEEDS_UPDATE)	
			add_subdirectory("${PLUGIN_DIR}/${PLUGIN}" "${PLUGIN}")
			LIST(APPEND LINK_LIBS "${PLUGIN_TARGET}")
		ELSE(${PLUGIN_VAR})
			message ("Skipping plugin ${UPPER_TARGET}: ${PLUGIN_INFO}")
		ENDIF(${PLUGIN_VAR})
	ELSE()
		message( FATAL_ERROR "Plugin at ${PLUGIN_DIR}/${PLUGIN} does not have a plugin.CMake file" )
	ENDIF()
ENDFOREACH()




#
#  Create a host.cpp
#

configure_file( "${PLUGINS_ROOT}/host.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/host.cpp")

#
#  actually define the target
#

include_directories ( ${MOAI_PLUGINS_INCLUDES} )

set (MOAI_PLUGINS_SRC "${CMAKE_CURRENT_BINARY_DIR}/host.cpp" )

add_library ( moai-plugins STATIC ${MOAI_PLUGINS_SRC} )
target_link_libraries ( moai-plugins ${LINK_LIBS})
target_include_directories (moai-plugins PUBLIC 
    "$<BUILD_INTERFACE:${MOAI_PLUGINS_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/moai-plugins>"
)


set_target_properties(moai-plugins PROPERTIES FOLDER Modules)  
install(TARGETS moai-plugins EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/host.h DESTINATION include/moai-plugins)