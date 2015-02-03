cmake_minimum_required(VERSION 2.8.5)

project(host-html)

#build shared lib (libmoaishared.so) and moaijs.js
set (LUA_LIB_DIR  "")
set (MOAI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
set (HOST_ROOT "${MOAI_ROOT}/src" )
get_filename_component(MY_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake-modules" ABSOLUTE) 
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${MY_SOURCE}")

include(platform-html)
include(platform-common)
include(module-setup)



include(libmoai-setup)

add_subdirectory(../host-modules host-modules)


set ( INCLUDES_MOAIHOSTHTML 
  "${MOAI_ROOT}/src/host-html/"
   )


include_directories ( ${INCLUDES_MOAIHOSTHTML} )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/host-html" )

add_library ( moaishared SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/HtmlHost.cpp
)

set_target_properties( moaishared PROPERTIES  SUFFIX ".bc" )


set(EXPORTED_FUNCTIONS "['_malloc','_RefreshContext','_Cleanup','_onKeyDown','_onKeyUp','_onChar','_onMouseButton','_onMouseDrag','_onMouseMove','_onPaint','_onReshape','_onTimer','_AKURunScript','_AKURunString','_OpenWindowFunc','_AKUSetWorkingDirectory','_AKUGetSimStep','_AKUEnqueueKeyboardShiftEvent','_AKUEnqueueKeyboardControlEvent','_AKUEnqueueKeyboardAltEvent','_AKULoadFuncFromString','_AKULoadFuncFromFile','_AKUCallFunc','_RestoreFile','_AKUInitializeUntz','_MoaiOpenWindowFunc','_MoaiSaveFunc']" )

#--closure 0 --minify 0
add_custom_target( moaijs
                    COMMAND ${EMSCRIPTEN_ROOT_PATH}/emcc${EMCC_SUFFIX} -O2 -s ASM_JS=0  -s TOTAL_MEMORY=248554432 -s FULL_ES2=1 -s LINKABLE=1 --js-library ${CMAKE_CURRENT_SOURCE_DIR}/moaicallbacks.js --js-library ${CMAKE_CURRENT_SOURCE_DIR}/library_webuntz.js 
                    --pre-js ${CMAKE_CURRENT_SOURCE_DIR}/libmoai-pre.js 
                    --post-js ${CMAKE_CURRENT_SOURCE_DIR}/libmoai-post.js
                    --memory-init-file 0
					-s EXPORTED_FUNCTIONS="${EXPORTED_FUNCTIONS}" 
          -s INVOKE_RUN=0 $<TARGET_FILE:moaishared> -o ${CMAKE_CURRENT_BINARY_DIR}/moaijs.js
		 
                  )

add_dependencies(moaijs moaishared )
LINK_DIRECTORIES("${LUA_LIB_DIR}")
target_link_libraries ( moaishared
  host-modules
  moai-sim
  moai-util
  moai-core
  zlcore
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/moaijs.js ARCHIVE DESTINATION lib)




