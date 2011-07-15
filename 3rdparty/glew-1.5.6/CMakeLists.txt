include_directories(
    include
    )

add_library(glew STATIC
    src/glew.c
    src/glewinfo.c
    src/visualinfo.c
    )
