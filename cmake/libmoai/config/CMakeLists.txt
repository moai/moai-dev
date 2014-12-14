#save our config for later
configure_file(moai_config.h.in "${CMAKE_CURRENT_BINARY_DIR}/moai_config.h")
configure_file( ${MOAI_ROOT}/src/config-default/moai_version_major.h "${CMAKE_CURRENT_BINARY_DIR}/moai_version_major.h" COPYONLY)


#author

file(READ ${MOAI_ROOT}/src/config-default/moai_version_author.h AUTHOR_FILE)
string(REGEX REPLACE "#define MOAI_SDK_VERSION_AUTHOR	\".*\"" "#define MOAI_SDK_VERSION_AUTHOR \"${AUTHOR_NAME}\"" NEW_AUTHOR_FILE ${AUTHOR_FILE} )
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/moai_version_author.h" ${NEW_AUTHOR_FILE})

#version minor
file(READ ${MOAI_ROOT}/src/config-default/moai_version_minor.h VERSION_FILE)

if (MOAI_VERSION_MINOR)
string(REGEX REPLACE "#define MOAI_SDK_VERSION_MINOR \\d+" "#define MOAI_SDK_VERSION_MINOR ${MOAI_VERSION_MINOR}" VERSION_FILE ${VERSION_FILE} )
endif (MOAI_VERSION_MINOR)

if (MOAI_VERSION_REVISION)
string(REGEX REPLACE "#define MOAI_SDK_VERSION_REVISION \\d+" "#define MOAI_SDK_VERSION_REVISION ${MOAI_VERSION_REVISION}" VERSION_FILE ${VERSION_FILE} )
endif (MOAI_VERSION_REVISION)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/moai_version_minor.h" ${VERSION_FILE})



if(MOAI_VERSION_MINOR)
  
endif(MOAI_VERSION_MINOR)



install(FILES "${CMAKE_CURRENT_BINARY_DIR}/moai_config.h" 
              "${CMAKE_CURRENT_BINARY_DIR}/moai_version_author.h" 
              "${CMAKE_CURRENT_BINARY_DIR}/moai_version_major.h"
              "${CMAKE_CURRENT_BINARY_DIR}/moai_version_minor.h"
	DESTINATION "include/config")