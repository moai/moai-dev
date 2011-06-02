include_directories(lib)

include(CheckFunctionExists)
check_function_exists(memmove HAVE_MEMMOVE)
if(HAVE_MEMMOVE EQUAL 1)
    set(FLAGS "${FLAGS} -DHAVE_MEMMOVE")
endif()

add_library(expat STATIC
    lib/amigaconfig.h
    lib/ascii.h
    lib/asciitab.h
    lib/expat_external.h
    lib/expat.h
    lib/iasciitab.h
    lib/internal.h
    lib/latin1tab.h
    lib/macconfig.h
    lib/nametab.h
    lib/utf8tab.h
    lib/winconfig.h
    lib/xmlparse.c
    lib/xmlrole.c
    lib/xmlrole.h
    lib/xmltok.c
    lib/xmltok.h
    lib/xmltok_impl.c
    lib/xmltok_impl.h
    lib/xmltok_ns.c
    )

set_target_properties(expat PROPERTIES COMPILE_FLAGS "${FLAGS}")
