include(CheckIncludeFiles)
check_include_files(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H EQUAL 1)
    set(FLAGS "${FLAGS} -DHAVE_STDINT_H")
endif()

add_library(sqlite3 STATIC sqlite3.c)

set_target_properties(sqlite3 PROPERTIES COMPILE_FLAGS "${FLAGS}")
