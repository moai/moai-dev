cmake_minimum_required ( VERSION 2.8.11 )
project ( harfbuzz )


set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/harfbuzz-1.0.6/" )
set ( HARFBUZZ_INCLUDES "${MOAI_ROOT}/3rdparty/harfbuzz-1.0.6" )

file ( GLOB_RECURSE HARFBUZZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

list ( REMOVE_ITEM HARFBUZZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/hb-coretext.cc" )
list ( REMOVE_ITEM HARFBUZZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/hb-glib.cc" )
list ( REMOVE_ITEM HARFBUZZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/hb-gobject.cc" )
list ( REMOVE_ITEM HARFBUZZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/hb-gobject-structs.cc" )
list ( REMOVE_ITEM HARFBUZZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/hb-graphite2.cc" )
list ( REMOVE_ITEM HARFBUZZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/hb-icu.cc" )
list ( REMOVE_ITEM HARFBUZZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/hb-uniscribe.cc" )


file ( GLOB_RECURSE HARFBUZZ_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h*" )

list ( REMOVE_ITEM HARFBUZZ_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/hb-coretext.h" )

add_library( harfbuzz STATIC ${HARFBUZZ_SOURCES} ${HARFBUZZ_HEADERS} )

target_include_directories ( harfbuzz PUBLIC "$<BUILD_INTERFACE:${HARFBUZZ_INCLUDES}>")
target_compile_definitions( harfbuzz PUBLIC "HAVE_FREETYPE" "HAVE_OT" "HAVE_UCDN")

if ( BUILD_HTML OR BUILD_LINUX OR BUILD_ANDROID )
    target_compile_definitions( harfbuzz PUBLIC "HB_NO_MT" ) #no multithreading in emscripten and none built in on linux
endif ( BUILD_HTML OR BUILD_LINUX OR BUILD_ANDROID )

target_link_libraries( harfbuzz freetype )
set_target_properties( harfbuzz PROPERTIES FOLDER Third-Party )
install(TARGETS harfbuzz EXPORT libmoai ARCHIVE DESTINATION lib)
