#pragma once


#define SLEDGE_HOST_USE_LUAEXT

// ARCHITECTURE
// -----------------------------------------------------------------------
// VC
#if defined(_WIN64)
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif

// GCC
#if defined(__GNUC__)
#if defined(__x86_64__)
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// LIBRARIES -------------------------------------------------------------
// C standard library
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#ifdef _DEBUG
#include <cassert>
#endif

// STL
#include <string>
#include <vector>
#include <list>

#include <sys/types.h>
#include <sys/stat.h> 
#include <errno.h>

// TinyXML
#include <tinyxml.h>

// MOAI
#if defined(_WIN32) || defined(_WIN64)
#include <kashmir/uuid.h>
#endif
#include <uslscore/pch.h>
#include <uslscore/USAccessors.h>
#include <uslscore/USBox.h>
#include <uslscore/USColor.h>
#include <uslscore/USLeanArray.h>
#include <uslscore/USStream.h>
#include <uslscore/USStreamReader.h>
#include <uslscore/USStreamWriter.h>
#include <uslscore/STLList.h>
#include <uslscore/STLSet.h>
#include <uslscore/STLMap.h>
#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

// Folder-watcher stuff
#if defined(_WIN32) || defined(_WIN64)
#include "FolderWatcher-win.h"
#elif defined(__APPLE__)
#include <FolderWatcher-mac.h>
#endif

// libSDL
#include <SDL.h>
#define SDL_main main

// OpenGL
#if defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#endif

// OS headers
#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#endif

// MACROS ----------------------------------------------------------------
#ifndef UNUSED
#define UNUSED(p) (( void )p)
#endif