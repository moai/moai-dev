cmake_minimum_required ( VERSION 3.0 )
project ( SDL2-static )
include(ExternalProject)

set (SDL2_SOURCE_DIR "${MOAI_ROOT}/3rdparty/sdl2-2.0.9")
set (SDL2_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/binarydir")
ExternalProject_Add(sdl2
   SOURCE_DIR "${SDL2_SOURCE_DIR}"
   CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install"  -DSDL_SHARED=OFF -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DLIBC=ON
   CMAKE_CACHE_ARGS "-DSDL2_SOURCE_DIR:string=${SDL2_SOURCE_DIR}" "-DSDL2_BINARY_DIR:string=${SDL2_BINARY_DIR}" "-DSDL_STATIC:bool=on" "-DSDL_SHARED:bool=off" 
   PREFIX "${CMAKE_CURRENT_BINARY_DIR}/sdl2-2.0.9"
   INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/install"
)

ExternalProject_Get_Property(sdl2 install_dir)

message ("sdl2 install_dir ${install_dir}")


add_library(SDL2-static-imp STATIC IMPORTED)
if (NOT BUILD_WINDOWS)
    set_property(TARGET SDL2-static-imp PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libSDL2.a)
    install(FILES ${install_dir}/lib/libSDL2.a DESTINATION lib ) 
else () 
    set_property(TARGET SDL2-static-imp PROPERTY IMPORTED_LOCATION ${install_dir}/lib/SDL2.lib)
    install(FILES ${install_dir}/lib/SDL2.lib DESTINATION lib ) 
endif ()

add_dependencies(SDL2-static-imp sdl2)


add_library(SDL2-static INTERFACE)



#todo install headers too :)
target_include_directories(SDL2-static INTERFACE "${SDL2_SOURCE_DIR}/include_moai" "${SDL2_SOURCE_DIR}/include" )
target_link_libraries(SDL2-static INTERFACE SDL2-static-imp )


install(TARGETS SDL2-static EXPORT libmoai ARCHIVE DESTINATION lib)
