add_library(aku STATIC
    AKU.cpp
    AKUGlut.cpp
    AKUGlut.h
    AKU.h
    pch.cpp
    pch.h
    )

target_link_libraries(aku
    moaicore
    uslscore
    ${GLUT_LIBRARIES}
    )
