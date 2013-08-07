cmake_minimum_required(VERSION 2.8.5)


project(host-html)

# these must be set *after* project()
    SET(CMAKE_AR                  "${EMSCRIPTEN_TOOLS}/emar")
    SET(CMAKE_RANLIB              "${EMSCRIPTEN_TOOLS}/emranlib")




#build shared lib (libmoaishared.so) and moaijs.js


set ( INCLUDES_MOAIHOSTHTML 
  "../../src/"
  "../../src/config-default/"
  "../../src/host-html/"
  ${OPENGL_INCLUDE_DIRS}
  CACHE INTERNAL "doc string" )


include_directories ( ${INCLUDES_MOAIHOSTHTML} )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/host-html" )

add_library ( moaishared SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/HtmlHost.cpp
)

get_target_property( MOAIHTMLOUTPUT moaishared LOCATION)

set(EXPORTED_FUNCTIONS "['_malloc','_RefreshContext','_Cleanup','_onKeyDown','_onKeyUp','_onMouseButton','_onMouseDrag','_onMouseMove','_onPaint','_onReshape','_onTimer','_AKURunScript','_AKURunString','_OpenWindowFunc','_AKUSetWorkingDirectory','_AKUGetSimStep','_AKUEnqueueKeyboardShiftEvent','_AKUEnqueueKeyboardControlEvent','_AKUEnqueueKeyboardAltEvent','_AKUInitializeUntz','_RestoreFile']" )


add_custom_target( moaijs
                    COMMAND ${EMSCRIPTEN_HOME}/emcc -O2 -s ASM_JS=0 --closure 0 --minify 0 -s TOTAL_MEMORY=33554432 -s FULL_ES2=1 -s LINKABLE=1 --js-library ../../../src/host-html/moaicallbacks.js --js-library ../../../src/host-html/library_webuntz.js 
					-s EXPORTED_FUNCTIONS="${EXPORTED_FUNCTIONS}" -s INVOKE_RUN=0 ${MOAIHTMLOUTPUT} -o moaijs.js
		 
                  )

add_dependencies(moaijs moaishared )

target_link_libraries ( moaishared
  moai-sim
  moai-util
  moai-core
  zlcore
)

if (MOAI_UNTZ)
  target_link_libraries ( moaishared moai-untz )
endif (MOAI_UNTZ)

if ( MOAI_BOX2D )
  target_link_libraries ( moaishared moai-box2d )
endif ( MOAI_BOX2D )

if ( MOAI_CHIPMUNK )
  target_link_libraries ( moaishared moai-chipmunk )
endif ( MOAI_CHIPMUNK )

target_link_libraries ( moaishared
  third-party 
)

#build host template
 add_custom_target(host-html-template
     ${CMAKE_COMMAND} -E copy_directory "../../../src/host-html/host-template" "${PROJECT_BINARY_DIR}/host-template" 
     COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/moaijs.js" "${PROJECT_BINARY_DIR}/host-template/www/moaijs.js"
 )

add_dependencies(host-html-template moaijs)


