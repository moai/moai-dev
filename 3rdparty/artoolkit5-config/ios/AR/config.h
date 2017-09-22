/*
 *  config.h
 *  ARToolKit5
 *
 *  This file is part of ARToolKit.
 *
 *  ARToolKit is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ARToolKit is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with ARToolKit.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As a special exception, the copyright holders of this library give you
 *  permission to link this library with independent modules to produce an
 *  executable, regardless of the license terms of these independent modules, and to
 *  copy and distribute the resulting executable under terms of your choice,
 *  provided that you also meet, for each linked independent module, the terms and
 *  conditions of the license of that module. An independent module is a module
 *  which is neither derived from nor based on this library. If you modify this
 *  library, you may extend this exception to your version of the library, but you
 *  are not obligated to do so. If you do not wish to do so, delete this exception
 *  statement from your version.
 *
 *  Copyright 2015-2016 Daqri, LLC.
 *  Copyright 2002-2015 ARToolworks, Inc.
 *
 *  Author(s): Hirokazu Kato, Philip Lamb
 *
 */
/*!
	@header config.h
	@abstract ARToolKit core configuration.
	@version 5.3.2
	@discussion
	@copyright 2015-2016 Daqri, LLC.
 */


#ifndef AR_CONFIG0_H
#define AR_CONFIG0_H

//
// As of version 2.72 and 4.1, ARToolKit supports an OpenGL-like
// versioning system, with both header versions (for the version
// of the ARToolKit SDK installed) and runtime version reporting
// via arGetVersion().
//

// The MAJOR version number defines non-backwards compatible
// changes in the ARToolKit API. Range: [0-99].
#define AR_HEADER_VERSION_MAJOR		5

// The MINOR version number defines additions to the ARToolKit
// API, or (occsasionally) other significant backwards-compatible
// changes in runtime functionality. Range: [0-99].
#define AR_HEADER_VERSION_MINOR		3

// The TINY version number defines bug-fixes to existing
// functionality. Range: [0-99].
#define AR_HEADER_VERSION_TINY		2

// The BUILD version number will always be zero in releases,
// but may be non-zero in internal builds or in version-control
// repository-sourced code. Range: [0-99].
#define AR_HEADER_VERSION_BUILD		0

// The string representation below must match the major, minor
// and tiny release numbers.
#define AR_HEADER_VERSION_STRING	"5.3.2"

// The macros below are convenience macros to enable use
// of certain ARToolKit header functionality by the release
// version in which it appeared.
// Each time the major version number is incremented, all
// existing macros must be removed, and just one new one
// added for the new major version.
// Each time the minor version number is incremented, a new
// AR_HAVE_HEADER_VERSION_ macro definition must be added.
// Tiny version numbers (being bug-fix releases, by definition)
// are NOT included in the AR_HAVE_HEADER_VERSION_ system.
#define AR_HAVE_HEADER_VERSION_5
#define AR_HAVE_HEADER_VERSION_5_1
#define AR_HAVE_HEADER_VERSION_5_2
#define AR_HAVE_HEADER_VERSION_5_3

//
// End version definitions.
//

/*
#undef   AR_BIG_ENDIAN
#undef   AR_LITTLE_ENDIAN
*/

/*!
    @typedef AR_PIXEL_FORMAT
	@abstract ARToolKit pixel-format specifiers.
	@discussion
		ARToolKit functions can accept pixel data in a variety of formats.
		This enumerations provides a set of constants you can use to request
		data in a particular pixel format from an ARToolKit function that
		returns data to you, or to specify that data you are providing to an
		ARToolKit function is in a particular pixel format.
	@constant AR_PIXEL_FORMAT_RGB
		Each pixel is represented by 24 bits. Eight bits per each Red, Green,
		and Blue component. This is the native 24 bit format for the Mac platform.
	@constant AR_PIXEL_FORMAT_BGR
		Each pixel is represented by 24 bits. Eight bits per each Blue, Red, and
		Green component. This is the native 24 bit format for the Win32 platform.
	@constant AR_PIXEL_FORMAT_RGBA
		Each pixel is represented by 32 bits. Eight bits per each Red, Green,
		Blue, and Alpha component.
	@constant AR_PIXEL_FORMAT_BGRA
		Each pixel is represented by 32 bits. Eight bits per each Blue, Green,
		Red, and Alpha component. This is the native 32 bit format for the Win32
		and Mac Intel platforms.
	@constant AR_PIXEL_FORMAT_ABGR
		Each pixel is represented by 32 bits. Eight bits per each Alpha, Blue,
		Green, and Red component. This is the native 32 bit format for the SGI
		platform.
	@constant AR_PIXEL_FORMAT_MONO
		Each pixel is represented by 8 bits of luminance information.
	@constant AR_PIXEL_FORMAT_ARGB
		Each pixel is represented by 32 bits. Eight bits per each Alpha, Red,
		Green, and Blue component. This is the native 32 bit format for the Mac
		PowerPC platform.
	@constant AR_PIXEL_FORMAT_2vuy
		8-bit 4:2:2 Component Y'CbCr format. Each 16 bit pixel is represented
		by an unsigned eight bit luminance component and two unsigned eight bit
		chroma components. Each pair of pixels shares a common set of chroma
		values. The components are ordered in memory; Cb, Y0, Cr, Y1. The
		luminance components have a range of [16, 235], while the chroma value
		has a range of [16, 240]. This is consistent with the CCIR601 spec.
		This format is fairly prevalent on both Mac and Win32 platforms.
		'2vuy' is the Apple QuickTime four-character code for this pixel format.
		The equivalent Microsoft fourCC is 'UYVY'.
	@constant AR_PIXEL_FORMAT_yuvs
		8-bit 4:2:2 Component Y'CbCr format. Identical to the AR_PIXEL_FORMAT_2vuy except
		each 16 bit word has been byte swapped. This results in a component
		ordering of; Y0, Cb, Y1, Cr.
		This is most prevalent yuv 4:2:2 format on both Mac and Win32 platforms.
		'yuvs' is the Apple QuickTime four-character code for this pixel format.
		The equivalent Microsoft fourCC is 'YUY2'.
    @constant AR_PIXEL_FORMAT_RGB_565
        A packed-pixel format. Each 16 bit pixel consists of 5 bits of red color
        information in bits 15-11, 6 bits of green color information in bits 10-5,
        and 5 bits of blue color information in bits 4-0. Byte ordering is big-endian.
    @constant AR_PIXEL_FORMAT_RGBA_5551
        A packed-pixel format. Each 16 bit pixel consists of 5 bits of red color
        information in bits 15-11, 5 bits of green color information in bits 10-6,
        5 bits of blue color information in bits 5-1, and a single alpha bit in bit 0.
        Byte ordering is big-endian.
    @constant AR_PIXEL_FORMAT_RGBA_4444
        A packed-pixel format. Each 16 bit pixel consists of 4 bits of red color
        information in bits 15-12, 6 bits of green color information in bits 11-8,
        4 bits of blue color information in bits 7-4, and 4 bits of alpha information
        in bits 3-0. Byte ordering is big-endian.
    @constant AR_PIXEL_FORMAT_420v
         8-bit 4:2:0 Component Y'CbCr format. Each 2x2 pixel block is represented
         by 4 unsigned eight bit luminance values and two unsigned eight bit
         chroma values. The chroma plane and luma plane are separated in memory. The
         luminance components have a range of [16, 235], while the chroma value
         has a range of [16, 240]. This is consistent with the CCIR601 spec.
         '420v' is the Apple Core Video four-character code for this pixel format.
    @constant AR_PIXEL_FORMAT_420f
         8-bit 4:2:0 Component Y'CbCr format. Each 2x2 pixel block is represented
         by 4 unsigned eight bit luminance components and two unsigned eight bit
         chroma components. The chroma plane and luma plane are separated in memory. The
         luminance components have a range of [0, 255], while the chroma value
         has a range of [1, 255].
         '420f' is the Apple Core Video four-character code for this pixel format.
         The equivalent Microsoft fourCC is 'NV12'.
    @constant AR_PIXEL_FORMAT_NV21
        8-bit 4:2:0 Component Y'CbCr format. Each 2x2 pixel block is represented
        by 4 unsigned eight bit luminance components and two unsigned eight bit
        chroma components. The chroma plane and luma plane are separated in memory. The
        luminance components have a range of [0, 255], while the chroma value
        has a range of [1, 255].
 */
typedef enum {
    AR_PIXEL_FORMAT_INVALID = -1,
	AR_PIXEL_FORMAT_RGB = 0,
	AR_PIXEL_FORMAT_BGR,
    AR_PIXEL_FORMAT_RGBA,
    AR_PIXEL_FORMAT_BGRA,
    AR_PIXEL_FORMAT_ABGR,
    AR_PIXEL_FORMAT_MONO,
    AR_PIXEL_FORMAT_ARGB,
    AR_PIXEL_FORMAT_2vuy,
    AR_PIXEL_FORMAT_yuvs,
    AR_PIXEL_FORMAT_RGB_565,
    AR_PIXEL_FORMAT_RGBA_5551,
    AR_PIXEL_FORMAT_RGBA_4444,
    AR_PIXEL_FORMAT_420v,
    AR_PIXEL_FORMAT_420f,
    AR_PIXEL_FORMAT_NV21
} AR_PIXEL_FORMAT;
#define       AR_PIXEL_FORMAT_UYVY      AR_PIXEL_FORMAT_2vuy
#define       AR_PIXEL_FORMAT_YUY2      AR_PIXEL_FORMAT_yuvs

// Note if making changes to the above table:
// If the number of pixel formats supported changes, AR_PIXEL_FORMAT_MAX must too.
// The functions arVideoUtilGetPixelSize(), arVideoUtilGetPixelFormatName()
// and arUtilGetPixelSize() arUtilGetPixelFormatName() must also be edited.
#define       AR_PIXEL_FORMAT_MAX       AR_PIXEL_FORMAT_NV21

enum {
    AR_LOG_LEVEL_DEBUG = 0,
    AR_LOG_LEVEL_INFO,
    AR_LOG_LEVEL_WARN,
    AR_LOG_LEVEL_ERROR,
    AR_LOG_LEVEL_REL_INFO
};
#define AR_LOG_LEVEL_DEFAULT AR_LOG_LEVEL_INFO

/*------------------------------------------------------------*/
/*                                                            */
/*  For Linux                                                 */
/*    AR_INPUT_V4L:     use of standard Video4Linux Library   */
/*    AR_INPUT_V4L2:    use of standard Video4Linux2 Library  */
/*    AR_INPUT_DV:      use of DV Camera                      */
/*    AR_INPUT_1394CAM: use of 1394 Digital Camara            */
/*    AR_INPUT_GSTREAMER: use of GStreamer library            */
/*                                                            */
/*------------------------------------------------------------*/
#ifdef EMSCRIPTEN
#  define __linux
#endif

#ifdef __linux

#define AR_CALLBACK

// Determine architecture endianess using gcc's macro, or assume little-endian by default.
#  if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || defined(__BIG_ENDIAN__)
#    define  AR_BIG_ENDIAN  // Most Significant Byte has greatest address in memory (ppc).
#    undef   AR_LITTLE_ENDIAN
#  elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || defined (__LITTLE_ENDIAN__)
#    undef   AR_BIG_ENDIAN   // Least significant Byte has greatest address in memory (x86).
#    define  AR_LITTLE_ENDIAN
#  else
#    define  AR_LITTLE_ENDIAN
#  endif

// Input modules. This is edited by the configure script.
#undef  AR_INPUT_V4L
#undef  AR_INPUT_V4L2
#undef  AR_INPUT_DV
#undef  AR_INPUT_1394CAM
#undef  AR_INPUT_GSTREAMER
#undef  AR_INPUT_IMAGE
#define AR_INPUT_DUMMY

// Default input module. This is edited by the configure script.
#undef  AR_DEFAULT_INPUT_V4L
#undef  AR_DEFAULT_INPUT_V4L2
#undef  AR_DEFAULT_INPUT_DV
#undef  AR_DEFAULT_INPUT_1394CAM
#undef  AR_DEFAULT_INPUT_GSTREAMER
#undef  AR_DEFAULT_INPUT_IMAGE
#undef  AR_DEFAULT_INPUT_DUMMY

// Other Linux-only configuration.
#define HAVE_LIBJPEG 1

#endif // __linux

// Default pixel formats.

#ifdef AR_INPUT_V4L
/* #define  AR_INPUT_V4L_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGR  */
/* #define  AR_INPUT_V4L_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGRA */
#define AR_INPUT_V4L_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGR
#endif

#ifdef AR_INPUT_V4L2
/* #define  AR_INPUT_V4L2_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGR  */
/* #define  AR_INPUT_V4L2_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGRA */
#define AR_INPUT_V4L2_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGR
#endif

#ifdef  AR_INPUT_DV
/* #define  AR_INPUT_DV_DEFAULT_PIXEL_FORMAT    AR_PIXEL_FORMAT_RGB  */
/* #define  AR_INPUT_DV_DEFAULT_PIXEL_FORMAT    AR_PIXEL_FORMAT_BGRA */
#define AR_INPUT_DV_DEFAULT_PIXEL_FORMAT    AR_PIXEL_FORMAT_RGB
#endif

#ifdef AR_INPUT_1394CAM
#undef AR_INPUT_1394CAM_USE_LIBDC1394_V2
/* #define  AR_INPUT_1394CAM_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_MONO */
/* #define  AR_INPUT_1394CAM_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_RGB  */
#define   AR_INPUT_1394CAM_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_MONO
#undef   AR_INPUT_1394CAM_USE_DRAGONFLY
#undef   AR_INPUT_1394CAM_USE_DF_EXPRESS
#undef   AR_INPUT_1394CAM_USE_FLEA
#undef   AR_INPUT_1394CAM_USE_FLEA_XGA
#undef   AR_INPUT_1394CAM_USE_DFK21AF04
#endif

#ifdef AR_INPUT_GSTREAMER
#define AR_INPUT_GSTREAMER_PIXEL_FORMAT   AR_PIXEL_FORMAT_RGB 
#endif



/*------------------------------------------------------------*/
/*  For SGI                                                   */
/*------------------------------------------------------------*/
#ifdef __sgi
#define AR_CALLBACK
#define  AR_BIG_ENDIAN
#undef   AR_LITTLE_ENDIAN
#define  AR_INPUT_SGI
#define AR_INPUT_DUMMY
#define  AR_DEFAULT_INPUT_SGI */
#undef  AR_DEFAULT_INPUT_DUMMY
#endif // __sgi

#ifdef AR_INPUT_SGI
#define  AR_INPUT_SGI_PIXEL_FORMAT   AR_PIXEL_FORMAT_ABGR
#endif

/*------------------------------------------------------------*/
/*  For Windows                                               */
/*------------------------------------------------------------*/
#ifdef _WIN32

// Include Windows API.
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif
#include <sdkddkver.h> // Minimum supported version. See http://msdn.microsoft.com/en-us/library/windows/desktop/aa383745.aspx
#include <windows.h>

#define AR_CALLBACK __stdcall
#define strdup _strdup

// Define _WINRT for support Windows Runtime platforms.
#if defined(WINAPI_FAMILY)
#  if (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP) // Windows Phone 8.1 and later.
#    if (_WIN32_WINNT >= 0x0603) // (_WIN32_WINNT_WINBLUE)
#      define _WINRT
#      define LIBARVIDEO_STATIC
#      define ARDOUBLE_IS_FLOAT
#    else
#      error ARToolKit for Windows Phone requires Windows Phone 8.1 or later. Please compile with Visual Studio 2013 or later with Windows Phone 8.1 SDK installed and with _WIN32_WINNT=0x0603 in your project compiler settings (setting /D_WIN32_WINNT=0x0603).
#    endif
#  elif (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP) // Windows Store 8.1 and later.
#    if (_WIN32_WINNT >= 0x0603) // (_WIN32_WINNT_WINBLUE)
#      define _WINRT
#      define LIBARVIDEO_STATIC
#      define ARDOUBLE_IS_FLOAT
#    else
#      error ARToolKit for Windows Store requires Windows 8.1 or later. Please compile with Visual Studio 2013 or later with Windows 8.1 SDK installed and with _WIN32_WINNT=0x0603 in your project compiler settings (setting /D_WIN32_WINNT=0x0603).
#    endif
#  endif
#endif

// Endianness.
// Windows on x86, x86-64 and ARM all run little-endian.
#undef   AR_BIG_ENDIAN
#define  AR_LITTLE_ENDIAN

// Input modules. This is edited by the configure script.
#define AR_INPUT_DUMMY
#undef  AR_INPUT_IMAGE
#undef  AR_INPUT_WINDOWS_DIRECTSHOW
#if !defined(_WIN64) || _MSC_VER >= 1800 // DSVideoLib 64-bit only on release for Visual Studio 2013 and later.
#undef  AR_INPUT_WINDOWS_DSVIDEOLIB
#endif
#ifndef _WIN64 // QuickTime is not available for 64-bit.
#undef  AR_INPUT_QUICKTIME
#endif
#undef  AR_INPUT_WINDOWS_DRAGONFLY
#undef  AR_INPUT_WINDOWS_MEDIA_FOUNDATION
#undef  AR_INPUT_WINDOWS_MEDIA_CAPTURE

// Default input module. This is edited by the configure script.
#undef  AR_DEFAULT_INPUT_DUMMY
#undef  AR_DEFAULT_INPUT_IMAGE
#undef  AR_DEFAULT_INPUT_WINDOWS_DIRECTSHOW
#if !defined(_WIN64) || _MSC_VER >= 1800 // DSVideoLib 64-bit only on release for Visual Studio 2013 and later.
#undef  AR_DEFAULT_INPUT_WINDOWS_DSVIDEOLIB
#endif
#ifndef _WIN64 // QuickTime is not available for 64-bit.
#undef  AR_DEFAULT_INPUT_QUICKTIME
#endif
#undef  AR_DEFAULT_INPUT_WINDOWS_DRAGONFLY
#undef  AR_DEFAULT_INPUT_WINDOWS_MEDIA_FOUNDATION
#undef  AR_DEFAULT_INPUT_WINDOWS_MEDIA_CAPTURE

// Other Windows-only configuration.
#define HAVE_LIBJPEG 1

#endif // _WIN32

// Default pixel formats.

#ifdef AR_INPUT_WINDOWS_DIRECTSHOW
#define  AR_INPUT_WINDOWS_DIRECTSHOW_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGR
#endif

#ifdef AR_INPUT_WINDOWS_DSVIDEOLIB
#define  AR_INPUT_WINDOWS_DSVIDEOLIB_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGRA
#endif

#ifdef AR_INPUT_WINDOWS_DRAGONFLY
#define  AR_INPUT_WINDOWS_DRAGONFLY_PIXEL_FORMAT    AR_PIXEL_FORMAT_BGR
#endif

#ifdef AR_INPUT_WINDOWS_MEDIA_FOUNDATION
#define  AR_INPUT_WINDOWS_MEDIA_FOUNDATION_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGRA
#endif

#ifdef AR_INPUT_WINDOWS_MEDIA_CAPTURE
#define  AR_INPUT_WINDOWS_MEDIA_CAPTURE_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGRA
#endif

/*------------------------------------------------------------*/
/*  For Android                                               */
/*    Note that Android NDK also defines __linux              */
/*------------------------------------------------------------*/

#if defined ANDROID
#define AR_CALLBACK
#define ARDOUBLE_IS_FLOAT
#undef  AR_INPUT_DUMMY
#define AR_INPUT_ANDROID
#undef  AR_INPUT_IMAGE
#undef  AR_DEFAULT_INPUT_DUMMY
#define AR_DEFAULT_INPUT_ANDROID
#undef  AR_DEFAULT_INPUT_IMAGE
#define HAVE_LIBJPEG 1
#define USE_OPENGL_ES 1
#endif // _ANDROID

// Default pixel formats.

#ifdef AR_INPUT_ANDROID
#define  AR_INPUT_ANDROID_PIXEL_FORMAT   AR_PIXEL_FORMAT_NV21
#endif

/*------------------------------------------------------------*/
/*  For Mac OS X                                              */
/*------------------------------------------------------------*/
#if __APPLE__

#  include <TargetConditionals.h>
#  include <AvailabilityMacros.h>

#  define AR_CALLBACK

// Endianness.
#  if TARGET_RT_BIG_ENDIAN
#    define  AR_BIG_ENDIAN  // Most Significant Byte has greatest address in memory (ppc).
#    undef   AR_LITTLE_ENDIAN
#  elif TARGET_RT_LITTLE_ENDIAN
#    undef   AR_BIG_ENDIAN
#    define  AR_LITTLE_ENDIAN
#  else
#    error
#  endif

#if TARGET_IPHONE_SIMULATOR

#error This release does not support the simulator. Please target an iOS device.
#define ARDOUBLE_IS_FLOAT
#define AR_INPUT_DUMMY
#define AR_DEFAULT_INPUT_DUMMY

#elif TARGET_OS_IPHONE

#define ARDOUBLE_IS_FLOAT
#undef  AR_INPUT_DUMMY
#define AR_INPUT_IPHONE
#undef  AR_INPUT_IMAGE
#undef  AR_DEFAULT_INPUT_DUMMY
#define AR_DEFAULT_INPUT_IPHONE
#undef  AR_DEFAULT_INPUT_IMAGE
#define HAVE_LIBJPEG 1
#define USE_OPENGL_ES 1
#ifdef __LP64__
#  define HAVE_ARM64_NEON 1
#else
#  define HAVE_ARM_NEON 1
#endif

#elif TARGET_OS_MAC

#if !defined(__LP64__) // QuickTime not available on 64-bit system.
#define AR_INPUT_QUICKTIME
#define AR_DEFAULT_INPUT_QUICKTIME
#endif
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4 || QTKIT_VERSION_MAX_ALLOWED >= QTKIT_VERSION_7_2 // QTKit not available until OS X 10.4.
#define AR_INPUT_QUICKTIME7
#undef  AR_DEFAULT_INPUT_QUICKTIME
#define AR_DEFAULT_INPUT_QUICKTIME7
#endif
#define AR_INPUT_DUMMY
#define AR_INPUT_IMAGE
#undef  AR_DEFAULT_INPUT_DUMMY
#undef  AR_DEFAULT_INPUT_IMAGE
#define HAVE_LIBJPEG 1

#endif

#endif // __APPLE__

// Default pixel formats.

#ifdef  AR_INPUT_QUICKTIME7
#define  AR_INPUT_QUICKTIME7_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_ARGB
#endif

#ifdef  AR_INPUT_IPHONE
#define  AR_INPUT_IPHONE_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_BGRA
#endif

/*------------------------------------------------------------*/
/*  Emscripten input                                          */
/*------------------------------------------------------------*/

#ifdef EMSCRIPTEN
#define AR_DEFAULT_INPUT_EMSCRIPTEN
#define AR_INPUT_EMSCRIPTEN_PIXEL_FORMAT   AR_PIXEL_FORMAT_RGBA
#endif

/*------------------------------------------------------------*/
/*  Multi-platform inputs                                     */
/*------------------------------------------------------------*/

#ifdef AR_INPUT_DUMMY
#define AR_INPUT_DUMMY_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_RGB
#endif

#ifdef  AR_INPUT_QUICKTIME
#define  AR_INPUT_QUICKTIME_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_ARGB
#endif

#ifdef AR_INPUT_IMAGE
#define AR_INPUT_IMAGE_DEFAULT_PIXEL_FORMAT   AR_PIXEL_FORMAT_RGB
#endif

//
// Setup AR_DEFAULT_PIXEL_FORMAT.
//
 
#if defined(AR_DEFAULT_INPUT_DUMMY)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_DUMMY_DEFAULT_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_V4L)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_V4L_DEFAULT_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_V4L2)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_V4L2_DEFAULT_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_DV)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_DV_DEFAULT_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_1394CAM)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_1394CAM_DEFAULT_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_SGI)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_SGI_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_WINDOWS_DIRECTSHOW)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_WINDOWS_DIRECTSHOW_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_WINDOWS_DRAGONFLY)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_WINDOWS_DRAGONFLY_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_QUICKTIME)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_QUICKTIME_DEFAULT_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_WINDOWS_DSVIDEOLIB)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_WINDOWS_DSVIDEOLIB_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_GSTREAMER)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_GSTREAMER_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_IPHONE)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_IPHONE_DEFAULT_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_QUICKTIME7)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_QUICKTIME7_DEFAULT_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_ANDROID)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_ANDROID_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_IMAGE)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_IMAGE_DEFAULT_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_WINDOWS_MEDIA_FOUNDATION)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_WINDOWS_MEDIA_FOUNDATION_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_WINDOWS_MEDIA_CAPTURE)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_WINDOWS_MEDIA_CAPTURE_PIXEL_FORMAT
#elif defined(AR_DEFAULT_INPUT_EMSCRIPTEN)
#  define AR_DEFAULT_PIXEL_FORMAT   AR_INPUT_EMSCRIPTEN_PIXEL_FORMAT
#else
#  error
#endif

//
// If trying to minimize memory footprint, disable a few things.
//

#if AR_ENABLE_MINIMIZE_MEMORY_FOOTPRINT
#define AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE 1
#define AR_DISABLE_NON_CORE_FNS 1
#define AR_DISABLE_LABELING_DEBUG_MODE 1
#endif

#endif // !AR_CONFIG0_H
