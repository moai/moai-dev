cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-plugins)

set (PLUGINS_ROOT "${MOAI_ROOT}/src/host-modules")

set ( MOAI_PLUGINS_INCLUDES 
    "${MOAI_ROOT}/src"
   "${CMAKE_BINARY_DIR}/libmoai/config"
    ${PLUGINS_ROOT}
    )

#
#  Setup our substitution vars
#

set(AKU_PLUGINS_HEADERS "// plugin headers ")
set(AKU_PLUGINS_APP_FINALIZE "// finalize here")
set(AKU_PLUGINS_APP_INITIALIZE "// initialize here")
set(AKU_PLUGINS_CONTEXT_INITIALIZE "// context initialize here")
set(AKU_PLUGINS_PAUSE "// pause here")
set(AKU_PLUGINS_UPDATE "// plugin update here")


#find our pre-installed plugins
if (PLUGIN_DIR)

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
		set(PLUGIN_VAR "AKU_WITH_PLUGIN_${UPPER_TARGET}")
		message ("Adding Plugin: ${PLUGIN_INFO}")
    set (AKU_PLUGINS_HEADERS "${AKU_PLUGINS_HEADERS}\n\n#ifdef ${PLUGIN_VAR}\n  #include <${PLUGIN}/host.h>\n#endif"  )
		set (AKU_PLUGINS_APP_INITIALIZE "${AKU_PLUGINS_APP_INITIALIZE}\n\n#ifdef ${PLUGIN_VAR}\n  ${PLUGIN_PREFIX}AppInitialize();\n#endif" )
		set (AKU_PLUGINS_CONTEXT_INITIALIZE "${AKU_PLUGINS_CONTEXT_INITIALIZE}\n\n#ifdef ${PLUGIN_VAR}\n  ${PLUGIN_PREFIX}ContextInitialize();\n#endif" )
    set (AKU_PLUGINS_PAUSE "${AKU_PLUGINS_PAUSE}\n\n#ifdef ${PLUGIN_VAR}\n  ${PLUGIN_PREFIX}Pause(pause);\n#endif" )
		set (AKU_PLUGINS_APP_FINALIZE "${AKU_PLUGINS_APP_FINALIZE}\n\n#ifdef ${PLUGIN_VAR}\n  ${PLUGIN_PREFIX}AppFinalize();\n#endif" )
		IF(PLUGIN_NEEDS_UPDATE)
			set (AKU_PLUGINS_UPDATE "${AKU_PLUGINS_UPDATE}\n\n#ifdef ${PLUGIN_VAR}\n  ${PLUGIN_PREFIX}Update();\n#endif")
		ENDIF(PLUGIN_NEEDS_UPDATE)	
		add_subdirectory("${PLUGIN_DIR}/${PLUGIN}" "${PLUGIN}")
    #install(TARGETS "${PLUGIN_TARGET}" EXPORT libmoai ARCHIVE DESTINATION plugins/${PLUGIN}/lib)
    #install(FILES "${PLUGIN_DIR}/${PLUGIN}/${PLUGIN}/host.h" DESTINATION plugins/${PLUGIN}/include/${PLUGIN})
		
    LIST(APPEND LINK_LIBS "${PLUGIN_TARGET}")
	ELSE()
		message( FATAL_ERROR "Plugin at ${PLUGIN_DIR}/${PLUGIN} does not have a plugin.CMake file" )
	ENDIF()
ENDFOREACH()

endif (PLUGIN_DIR)



#
#  Create a host.cpp
#

configure_file( "${PLUGINS_ROOT}/aku_plugins.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/aku_plugins.cpp")
install(FILES ${PLUGINS_ROOT}/aku_plugins.h DESTINATION ../src/host-modules)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/aku_plugins.cpp DESTINATION ../src/host-modules)
#
#  actually define the target
#

include_directories ( ${MOAI_PLUGINS_INCLUDES} )

set (MOAI_PLUGINS_SRC "${CMAKE_CURRENT_BINARY_DIR}/aku_plugins.cpp" )

add_library ( moai-plugins STATIC ${MOAI_PLUGINS_SRC} )
target_link_libraries ( moai-plugins ${LINK_LIBS})
target_include_directories (moai-plugins PUBLIC 
    "$<BUILD_INTERFACE:${MOAI_PLUGINS_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/moai-plugins>"
)


set_target_properties(moai-plugins PROPERTIES FOLDER Modules)  
#install(TARGETS moai-plugins EXPORT libmoai ARCHIVE DESTINATION lib)
#install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/host.h DESTINATION include/moai-plugins)