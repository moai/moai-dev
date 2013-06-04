cmake_minimum_required ( VERSION 2.8.5 )

project ( freetype )

if ( MOAI_FREETYPE )

    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_FREETYPE=1" CACHE INTERNAL "doc string")
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_FREETYPE=1" CACHE INTERNAL "doc string")

    # add_definitions("-DFT2_BUILD_LIBRARY -D__MOAI_LINUX_BUILD")

    set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/freetype-2.4.4/src/" )

    set ( FREETYPE_INCLUDES "../../3rdparty/freetype-2.4.4/include" CACHE INTERNAL "doc string" )
	set ( FT_DIR "../../3rdparty/freetype-2.4.4" )

    set ( FREETYPE_SOURCES 
#source
     ${FT_DIR}/src/autofit/autofit.c
    ${FT_DIR}/src/bdf/bdf.c
    ${FT_DIR}/src/cff/cff.c
    ${FT_DIR}/src/base/ftbase.c
    ${FT_DIR}/src/base/ftbitmap.c
    ${FT_DIR}/src/cache/ftcache.c
    ${FT_DIR}/src/base/ftdebug.c
    ${FT_DIR}/src/base/ftfstype.c
    ${FT_DIR}/src/base/ftgasp.c
    ${FT_DIR}/src/base/ftglyph.c
    ${FT_DIR}/src/gzip/ftgzip.c
    ${FT_DIR}/src/base/ftinit.c
    ${FT_DIR}/src/lzw/ftlzw.c
    ${FT_DIR}/src/base/ftstroke.c
    ${FT_DIR}/src/base/ftsystem.c
    ${FT_DIR}/src/smooth/smooth.c
#modules
  ${FT_DIR}/src/base/ftbbox.c
    ${FT_DIR}/src/base/ftgxval.c
    ${FT_DIR}/src/base/ftlcdfil.c
    ${FT_DIR}/src/base/ftmm.c
    ${FT_DIR}/src/base/ftotval.c
    ${FT_DIR}/src/base/ftpatent.c
    ${FT_DIR}/src/base/ftpfr.c
    ${FT_DIR}/src/base/ftsynth.c
    ${FT_DIR}/src/base/fttype1.c
    ${FT_DIR}/src/base/ftwinfnt.c
    ${FT_DIR}/src/base/ftxf86.c
    ${FT_DIR}/src/pcf/pcf.c
    ${FT_DIR}/src/pfr/pfr.c
    ${FT_DIR}/src/psaux/psaux.c
    ${FT_DIR}/src/pshinter/pshinter.c
    ${FT_DIR}/src/psnames/psmodule.c
    ${FT_DIR}/src/raster/raster.c
    ${FT_DIR}/src/sfnt/sfnt.c
    ${FT_DIR}/src/truetype/truetype.c
    ${FT_DIR}/src/type1/type1.c
    ${FT_DIR}/src/cid/type1cid.c
    ${FT_DIR}/src/type42/type42.c
    ${FT_DIR}/src/winfonts/winfnt.c
        
    CACHE INTERNAL "doc string" )

else ( MOAI_FREETYPE )

    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_FREETYPE=0" CACHE INTERNAL "doc string")
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_FREETYPE=0" CACHE INTERNAL "doc string")

endif ( MOAI_FREETYPE )
