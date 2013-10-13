cmake_minimum_required ( VERSION 2.8.5 )
project ( tapjoy-ios )

set ( TAPJOY_ROOT "../../../3rdparty/tapjoyiOS-8.1.9/TapjoyConnect")
set ( TAPJOY_COMP "${TAPJOY_ROOT}/Components")

set ( TAPJOY_INCLUDES 
  ${TAPJOY_ROOT}
  ${TAPJOY_COMP}/TJCAds
  ${TAPJOY_COMP}/TJCCore
  ${TAPJOY_COMP}/TJCCore/Utilities
  ${TAPJOY_COMP}/TJCCore/Views
  ${TAPJOY_COMP}/TJCCore/WebFetcher
  ${TAPJOY_COMP}/TJCExtensions
  ${TAPJOY_COMP}/TJCFeaturedApp
  ${TAPJOY_COMP}/TJCOffers
  ${TAPJOY_COMP}/TJCUserAccount
  ${TAPJOY_COMP}/TJCVideoAds
  ${TAPJOY_COMP}/TJCVirtualGoods
  ${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_FileDownloading
  ${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_LocalDatabase
  ${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_Resources
  ${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_Views
  ${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_ZipArchive
  ${TAPJOY_COMP}/TJCVirtualGoods/TJCVG_ZipArchive/minizip
)


include_directories ( 
  ${TAPJOY_INCLUDES} 
)

file ( GLOB_RECURSE TAPJOY_SRC "${TAPJOY_ROOT}/*.m" )


add_library ( tapjoy-ios STATIC 
                   ${TAPJOY_SRC} 
            )

