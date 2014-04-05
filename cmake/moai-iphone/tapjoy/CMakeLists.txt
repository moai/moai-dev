cmake_minimum_required ( VERSION 2.8.5 )
project ( tapjoy-ios )

set ( TAPJOY_ROOT "${MOAI_ROOT}/3rdparty/tapjoyiOS-8.1.9/TapjoyConnect")
set ( TAPJOY_COMP "${TAPJOY_ROOT}/Components")

set ( TAPJOY_INCLUDES 
  "${TAPJOY_ROOT}/"
  "${TAPJOY_COMP}/"
  "${TAPJOY_COMP}/TJCCore/"
  "${TAPJOY_COMP}/TJCCore/Utilities/"
  "${TAPJOY_COMP}/TJCCore/Views/"
  "${TAPJOY_COMP}/TJCCore/WebFetcher/"
  "${TAPJOY_COMP}/TJCAds/"
  "${TAPJOY_COMP}/TJCExtensions/"
  "${TAPJOY_COMP}/TJCFeaturedApp/"
  "${TAPJOY_COMP}/TJCOffers/"
  "${TAPJOY_COMP}/TJCUserAccount/"
  "${TAPJOY_COMP}/TJCVideoAds/"
  "${TAPJOY_COMP}/TJCVirtualGoods/"
  "${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_FileDownloading/"
  "${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_LocalDatabase/"
  "${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_Resources/"
  "${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_Views/"
  "${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_ZipArchive/"
  "${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_ZipArchive/minizip/"
)

set (TAPJOY_INCLUDES
  "${TAPJOY_INCLUDES}"
  PARENT_SCOPE
)

include_directories ( 
  ${TAPJOY_INCLUDES} 
)

file ( GLOB_RECURSE TAPJOY_SRC "${TAPJOY_ROOT}/*.m" "${TAPJOY_ROOT}/*.c")

add_library ( tapjoy-ios STATIC 
  ${TAPJOY_COMP}/TJCExtensions/OpenUDID.m
  ${TAPJOY_SRC} 
)

set_source_files_properties( ${TAPJOY_COMP}/TJCExtensions/OpenUDID.m PROPERTIES COMPILE_FLAGS -fobjc-arc )
set_target_properties( tapjoy-ios PROPERTIES FOLDER Third-Party )
