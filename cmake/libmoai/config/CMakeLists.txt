#save our config for later
configure_file(moai_config.h.in "${CMAKE_CURRENT_BINARY_DIR}/moai_config.h")


#author
file(READ ${MOAI_ROOT}/src/config-default/moai_version_author.h AUTHOR_FILE)
string(REGEX REPLACE "#define MOAI_SDK_VERSION_AUTHOR	\".*\"" "#define MOAI_SDK_VERSION_AUTHOR \"${AUTHOR_NAME}\"" NEW_AUTHOR_FILE ${AUTHOR_FILE} )
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/moai_version_author.h" ${NEW_AUTHOR_FILE})




install(FILES "${CMAKE_CURRENT_BINARY_DIR}/moai_config.h" 
              "${CMAKE_CURRENT_BINARY_DIR}/moai_version_author.h" 
	DESTINATION "include/config")