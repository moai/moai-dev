cmake_minimum_required ( VERSION 2.8.5 )

project ( freetype )

if ( MOAI_FREETYPE )

    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_FREETYPE=1" CACHE INTERNAL "doc string")
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_FREETYPE=1" CACHE INTERNAL "doc string")

    # add_definitions("-DFT2_BUILD_LIBRARY -D__MOAI_LINUX_BUILD")

    set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/freetype-2.4.4/src/" )

    set ( FREETYPE_INCLUDES "${MOAI_ROOT}/3rdparty/freetype-2.4.4/include" CACHE INTERNAL "doc string" )

    set ( FREETYPE_SOURCES 

        ${CMAKE_CURRENT_SOURCE_DIR}/autofit/autofit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/basepic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftadvanc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftbase.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftbbox.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftbdf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftbitmap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftcalc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftcid.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftdbgmem.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftdebug.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftfstype.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftgasp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftgloadr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftglyph.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftgxval.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftinit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftlcdfil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftmm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftotval.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftoutln.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftpatent.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftpfr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftpic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftrfork.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftsnames.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftstream.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftstroke.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftsynth.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/fttrigon.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/fttype1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftutil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftwinfnt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftxf86.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftobjs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/ftsystem.c
        ${CMAKE_CURRENT_SOURCE_DIR}/bdf/bdf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/bdf/bdfdrivr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/bdf/bdflib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cache/ftcache.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cache/ftcbasic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cache/ftccache.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cache/ftccmap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cache/ftcglyph.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cache/ftcimage.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cache/ftcmanag.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cache/ftcmru.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cache/ftcsbits.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cff/cff.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cff/cffcmap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cff/cffdrivr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cff/cffgload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cff/cffload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cff/cffobjs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cff/cffparse.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cff/cffpic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cid/cidgload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cid/cidload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cid/cidobjs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cid/cidparse.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cid/cidriver.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cid/type1cid.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvalid.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvbsln.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvcommn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvfeat.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvjust.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvkern.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvlcar.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmod.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmort.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmort0.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmort1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmort2.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmort4.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmort5.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmorx.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmorx0.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmorx1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmorx2.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmorx4.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvmorx5.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvopbd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvprop.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gxvalid/gxvtrak.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gzip/adler32.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gzip/ftgzip.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gzip/infblock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gzip/infcodes.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gzip/inflate.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gzip/inftrees.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gzip/infutil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gzip/zutil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lzw/ftlzw.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lzw/ftzopen.c
        ${CMAKE_CURRENT_SOURCE_DIR}/otvalid/otvalid.c
        ${CMAKE_CURRENT_SOURCE_DIR}/otvalid/otvbase.c
        ${CMAKE_CURRENT_SOURCE_DIR}/otvalid/otvcommn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/otvalid/otvgdef.c
        ${CMAKE_CURRENT_SOURCE_DIR}/otvalid/otvgpos.c
        ${CMAKE_CURRENT_SOURCE_DIR}/otvalid/otvgsub.c
        ${CMAKE_CURRENT_SOURCE_DIR}/otvalid/otvjstf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/otvalid/otvmath.c
        ${CMAKE_CURRENT_SOURCE_DIR}/otvalid/otvmod.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pcf/pcf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pcf/pcfdrivr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pcf/pcfread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pcf/pcfutil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pfr/pfr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pfr/pfrcmap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pfr/pfrdrivr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pfr/pfrgload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pfr/pfrload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pfr/pfrobjs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pfr/pfrsbit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psaux/afmparse.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psaux/psaux.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psaux/psauxmod.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psaux/psconv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psaux/psobjs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psaux/t1cmap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psaux/t1decode.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pshinter/pshalgo.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pshinter/pshglob.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pshinter/pshinter.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pshinter/pshmod.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pshinter/pshpic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pshinter/pshrec.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psnames/psmodule.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psnames/psnames.c
        ${CMAKE_CURRENT_SOURCE_DIR}/psnames/pspic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/raster/raster.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/sfdriver.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/sfnt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/sfntpic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/sfobjs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/ttbdf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/ttcmap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/ttkern.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/ttload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/ttmtx.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/ttpost.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/ttsbit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sfnt/ttsbit0.c
        ${CMAKE_CURRENT_SOURCE_DIR}/smooth/smooth.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/apinames.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/ftrandom/ftrandom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/test_afm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/test_bbox.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/test_trig.c
        ${CMAKE_CURRENT_SOURCE_DIR}/truetype/truetype.c
        ${CMAKE_CURRENT_SOURCE_DIR}/truetype/ttdriver.c
        ${CMAKE_CURRENT_SOURCE_DIR}/truetype/ttgload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/truetype/ttgxvar.c
        ${CMAKE_CURRENT_SOURCE_DIR}/truetype/ttinterp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/truetype/ttobjs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/truetype/ttpic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/truetype/ttpload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type1/t1afm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type1/t1driver.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type1/t1gload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type1/t1load.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type1/t1objs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type1/t1parse.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type1/type1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type42/t42drivr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type42/t42objs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type42/t42parse.c
        ${CMAKE_CURRENT_SOURCE_DIR}/type42/type42.c
        ${CMAKE_CURRENT_SOURCE_DIR}/winfonts/winfnt.c
        
    CACHE INTERNAL "doc string" )

else ( MOAI_FREETYPE )

    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_FREETYPE=0" CACHE INTERNAL "doc string")
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_FREETYPE=0" CACHE INTERNAL "doc string")

endif ( MOAI_FREETYPE )