cmake_minimum_required(VERSION 2.8.5)

project(host-html)

#build shared lib (libmoaishared.so) and moaijs.js

set ( INCLUDES_MOAIHOSTHTML 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/src/host-html/"
   )


include_directories ( ${INCLUDES_MOAIHOSTHTML} )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/host-html" )

add_library ( moaishared SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/HtmlHost.cpp
  ${MOAI_ROOT}/src/host-modules/aku_modules.cpp
)

get_target_property( MOAIHTMLOUTPUT moaishared LOCATION)

set(EXPORTED_FUNCTIONS "['_malloc','_RefreshContext','_Cleanup','_onKeyDown','_onKeyUp','_onMouseButton','_onMouseDrag','_onMouseMove','_onPaint','_onReshape','_onTimer','_AKURunScript','_AKURunString','_OpenWindowFunc','_AKUSetWorkingDirectory','_AKUGetSimStep','_AKUEnqueueKeyboardShiftEvent','_AKUEnqueueKeyboardControlEvent','_AKUEnqueueKeyboardAltEvent','_RestoreFile','_AKUInitializeUntz','_MoaiOpenWindowFunc','_MoaiSaveFunc']" )

#--closure 0 --minify 0
add_custom_target( moaijs
                    COMMAND ${EMSCRIPTEN_ROOT_PATH}/emcc${EMCC_SUFFIX} -O2 -s ASM_JS=0  -s TOTAL_MEMORY=248554432 -s FULL_ES2=1 -s LINKABLE=1 --js-library ${CMAKE_CURRENT_LIST_DIR}/../../src/host-html/moaicallbacks.js --js-library ${CMAKE_CURRENT_LIST_DIR}/../../src/host-html/library_webuntz.js 
                    --pre-js ${CMAKE_CURRENT_LIST_DIR}/../../src/host-html/libmoai-pre.js 
                    --post-js ${CMAKE_CURRENT_LIST_DIR}/../../src/host-html/libmoai-post.js
					-s EXPORTED_FUNCTIONS="${EXPORTED_FUNCTIONS}" -s INVOKE_RUN=0 ${MOAIHTMLOUTPUT} -o moaijs.js
		 
                  )

add_dependencies(moaijs moaishared )

target_link_libraries ( moaishared
  host-modules
  moai-sim
  moai-util
  moai-core
  zlcore
)

#build host template
 add_custom_target(host-html-template
     ${CMAKE_COMMAND} -E copy_directory "${MOAI_ROOT}/src/host-html/host-template" "${PROJECT_BINARY_DIR}/host-template" 
     COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/moaijs.js" "${PROJECT_BINARY_DIR}/host-template/www/moaijs.js"
 )

add_dependencies(host-html-template moaijs)


