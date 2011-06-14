# src/CMakeLists.txt

FIND_LIBRARY(LIB_LUA lua PATHS
	/usr/local/lib
	/usr/lib
)

IF(NOT LIB_LUA)
	FIND_LIBRARY(LIB_LUA lua5.1 PATHS
		/usr/local/lib
		/usr/lib
	)
ENDIF(NOT LIB_LUA)

FIND_PATH( CURL_INCLUDE_DIR curl/curl.h
  /usr/include
  /usr/local/include
)

FIND_LIBRARY( LIB_CURL curl
  /usr/lib
  /usr/local/lib
)

INCLUDE_DIRECTORIES(
	${${PROJECT_NAME}_SOURCE_DIR}/src
	${CURL_INCLUDE_DIR}
	/usr/local/include/lua5.1
	/usr/include/lua5.1
)

SET(${PROJECT_NAME}_LIBRARIES
	${LIB_LUA}
	${LIB_CURL}
)

SET(${PROJECT_NAME}_SOURCES
	luacurl.c
)

LINK_LIBRARIES(${${PROJECT_NAME}_LIBRARIES} m)
ADD_LIBRARY(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
INSTALL_TARGETS(/lib ${PROJECT_NAME})
