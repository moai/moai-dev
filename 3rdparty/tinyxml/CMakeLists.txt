add_library(tinyxml STATIC
    tinyxml.cpp
    tinyxmlparser.cpp
    xmltest.cpp
    tinyxmlerror.cpp
    tinystr.cpp
    )

set_target_properties(tinyxml PROPERTIES COMPILE_FLAGS "-DTIXML_USE_STL")
