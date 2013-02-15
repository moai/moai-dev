/**
 * Original code: automated SDL platform test written by Edgar Simo "bobbens"
 * Extended and updated by aschiffler at ferzkopp dot net
 */

#include <stdio.h>

#include <SDL/SDL.h>

#include "../../include/SDL_test.h"

/* Test cases */
static const TestCaseReference test1 =
		(TestCaseReference){ "platform_testTypes", "Tests predefined types", TEST_ENABLED, 0, 0 };

static const TestCaseReference test2 =
		(TestCaseReference){ "platform_testEndianessAndSwap", "Tests endianess and swap functions", TEST_ENABLED, 0, 0 };

static const TestCaseReference test3 =
		(TestCaseReference){ "platform_testGetFunctions", "Tests various SDL_GetXYZ functions", TEST_ENABLED, 0, 0 };

static const TestCaseReference test4 =
		(TestCaseReference){ "platform_testHasFunctions", "Tests various SDL_HasXYZ functions", TEST_ENABLED, 0, 0 };

static const TestCaseReference test5 =
		(TestCaseReference){ "platform_testGetVersion", "Tests SDL_GetVersion function", TEST_ENABLED, 0, 0 };

static const TestCaseReference test6 =
		(TestCaseReference){ "platform_testSDLVersion", "Tests SDL_VERSION macro", TEST_ENABLED, 0, 0 };

static const TestCaseReference test7 =
		(TestCaseReference){ "platform_testDefaultInit", "Tests default SDL_Init", TEST_ENABLED, 0, 0 };

static const TestCaseReference test8 =
		(TestCaseReference){ "platform_testGetSetClearError", "Tests SDL_Get/Set/ClearError", TEST_ENABLED, 0, 0 };

static const TestCaseReference test9 =
		(TestCaseReference){ "platform_testSetErrorEmptyInput", "Tests SDL_SetError with empty input", TEST_ENABLED, 0, 0 };

static const TestCaseReference test10 =
		(TestCaseReference){ "platform_testSetErrorInvalidInput", "Tests SDL_SetError with invalid input", TEST_ENABLED, 0, 0 };

static const TestCaseReference test11 =
		(TestCaseReference){ "platform_testGetPowerInfo", "Tests SDL_GetPowerInfo function", TEST_ENABLED, 0, 0 };

/* Test suite */
extern const TestCaseReference *testSuite[] =  {
	&test1, &test2, &test3, &test4, &test5, &test6, &test7, &test8, &test9, &test10, &test11, NULL
};

TestCaseReference **QueryTestSuite() {
	return (TestCaseReference **)testSuite;
}

/**
 * @brief Compare sizes of types.
 *
 * @note Watcom C flags these as Warning 201: "Unreachable code" if you just
 *  compare them directly, so we push it through a function to keep the
 *  compiler quiet.  --ryan.
 */
static int _compareSizeOfType( size_t sizeoftype, size_t hardcodetype )
{
    return sizeoftype != hardcodetype;
}

/**
 * @brief Tests type sizes.
 */
int platform_testTypes(void *arg)
{
   int ret;

   ret = _compareSizeOfType( sizeof(Uint8), 1 );
   AssertTrue( ret == 0, "sizeof(Uint8) = %lu, expected  1", sizeof(Uint8) );

   ret = _compareSizeOfType( sizeof(Uint16), 2 );
   AssertTrue( ret == 0, "sizeof(Uint16) = %lu, expected 2", sizeof(Uint16) );

   ret = _compareSizeOfType( sizeof(Uint32), 4 );
   AssertTrue( ret == 0, "sizeof(Uint32) = %lu, expected 4", sizeof(Uint32) );

   ret = _compareSizeOfType( sizeof(Uint64), 8 );
   AssertTrue( ret == 0, "sizeof(Uint64) = %lu, expected 8", sizeof(Uint64) );
}

/**
 * @brief Tests platform endianness and SDL_SwapXY functions.
 */
int platform_testEndianessAndSwap(void *arg)
{
    int real_byteorder;
    Uint16 value = 0x1234;
    Uint16 value16 = 0xCDAB;
    Uint16 swapped16 = 0xABCD;
    Uint32 value32 = 0xEFBEADDE;
    Uint32 swapped32 = 0xDEADBEEF;

    Uint64 value64, swapped64;
    value64 = 0xEFBEADDE;
    value64 <<= 32;
    value64 |= 0xCDAB3412;
    swapped64 = 0x1234ABCD;
    swapped64 <<= 32;
    swapped64 |= 0xDEADBEEF;
    
    if ((*((char *) &value) >> 4) == 0x1) {
        real_byteorder = SDL_BIG_ENDIAN;
    } else {
        real_byteorder = SDL_LIL_ENDIAN;
    }

    /* Test endianness. */    
    AssertTrue( real_byteorder == SDL_BYTEORDER,
             "Machine detected as %s endian, appears to be %s endian.",
             (SDL_BYTEORDER == SDL_LIL_ENDIAN) ? "little" : "big",
             (real_byteorder == SDL_LIL_ENDIAN) ? "little" : "big" );

    /* Test 16 swap. */
    AssertTrue( SDL_Swap16(value16) == swapped16,
             "SDL_Swap16(): 16 bit swapped: 0x%X => 0x%X",
             value16, SDL_Swap16(value16) );

    /* Test 32 swap. */
    AssertTrue( SDL_Swap32(value32) == swapped32,
             "SDL_Swap32(): 32 bit swapped: 0x%X => 0x%X",
             value32, SDL_Swap32(value32) );

    /* Test 64 swap. */
    AssertTrue( SDL_Swap64(value64) == swapped64,
#ifdef _MSC_VER
             "SDL_Swap64(): 64 bit swapped: 0x%I64X => 0x%I64X",
#else
             "SDL_Swap64(): 64 bit swapped: 0x%llX => 0x%llX",
#endif
             value64, SDL_Swap64(value64) );
}

/*!
 * \brief Tests SDL_GetXYZ() functions
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_GetPlatform
 * http://wiki.libsdl.org/moin.cgi/SDL_GetCPUCount
 * http://wiki.libsdl.org/moin.cgi/SDL_GetCPUCacheLineSize
 * http://wiki.libsdl.org/moin.cgi/SDL_GetRevision
 * http://wiki.libsdl.org/moin.cgi/SDL_GetRevisionNumber
 */
int platform_testGetFunctions (void *arg)
{
   char *platform;
   char *revision;
   int ret;
   int len;
 
   platform = (char *)SDL_GetPlatform();
   AssertPass("SDL_GetPlatform()");
   AssertTrue(platform != NULL, "SDL_GetPlatform() != NULL");
   if (platform != NULL) {
     len = strlen(platform);
     AssertTrue(len > 0, 
             "SDL_GetPlatform(): expected non-empty platform, was platform: '%s', len: %i", 
             platform, 
             len);
   }
    
   ret = SDL_GetCPUCount();
   AssertPass("SDL_GetCPUCount()");
   AssertTrue(ret > 0,
             "SDL_GetCPUCount(): expected count > 0, was: %i", 
             ret);    

   ret = SDL_GetCPUCacheLineSize();
   AssertPass("SDL_GetCPUCacheLineSize()");
   AssertTrue(ret >= 0,
             "SDL_GetCPUCacheLineSize(): expected size >= 0, was: %i", 
             ret);    

   revision = (char *)SDL_GetRevision();
   AssertPass("SDL_GetRevision()");
   AssertTrue(revision != NULL, "SDL_GetRevision() != NULL");

   ret = SDL_GetRevisionNumber();
   AssertPass("SDL_GetRevisionNumber()");
}

/*!
 * \brief Tests SDL_HasXYZ() functions
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_Has3DNow
 * http://wiki.libsdl.org/moin.cgi/SDL_HasAltiVec
 * http://wiki.libsdl.org/moin.cgi/SDL_HasMMX
 * http://wiki.libsdl.org/moin.cgi/SDL_HasRDTSC
 * http://wiki.libsdl.org/moin.cgi/SDL_HasSSE
 * http://wiki.libsdl.org/moin.cgi/SDL_HasSSE2
 * http://wiki.libsdl.org/moin.cgi/SDL_HasSSE3
 * http://wiki.libsdl.org/moin.cgi/SDL_HasSSE41
 * http://wiki.libsdl.org/moin.cgi/SDL_HasSSE42
 */
int platform_testHasFunctions (void *arg)
{
   int ret;
   
   // TODO: independently determine and compare values as well
   
   ret = SDL_HasRDTSC();
   AssertPass("SDL_HasRDTSC()");
   
   ret = SDL_HasAltiVec();
   AssertPass("SDL_HasAltiVec()");

   ret = SDL_HasMMX();
   AssertPass("SDL_HasMMX()");

   ret = SDL_Has3DNow();
   AssertPass("SDL_Has3DNow()");

   ret = SDL_HasSSE();
   AssertPass("SDL_HasSSE()");

   ret = SDL_HasSSE2();
   AssertPass("SDL_HasSSE2()");

   ret = SDL_HasSSE3();
   AssertPass("SDL_HasSSE3()");

   ret = SDL_HasSSE41();
   AssertPass("SDL_HasSSE41()");

   ret = SDL_HasSSE42();
   AssertPass("SDL_HasSSE42()");
}

/*!
 * \brief Tests SDL_GetVersion
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_GetVersion
 */
int platform_testGetVersion(void *arg)
{
   SDL_version linked;
   
   SDL_GetVersion(&linked);
   AssertTrue( linked.major >= SDL_MAJOR_VERSION,
             "SDL_GetVersion(): returned major %i (>= %i)",
             linked.major,
             SDL_MAJOR_VERSION);   
   AssertTrue( linked.minor >= SDL_MINOR_VERSION,
             "SDL_GetVersion(): returned minor %i (>= %i)",
             linked.minor,
             SDL_MINOR_VERSION);   
}

/*!
 * \brief Tests SDL_VERSION macro
 */
int platform_testSDLVersion(void *arg)
{
   SDL_version compiled;
   
   SDL_VERSION(&compiled);
   AssertTrue( compiled.major >= SDL_MAJOR_VERSION,
             "SDL_VERSION() returned major %i (>= %i)",
             compiled.major,
             SDL_MAJOR_VERSION);   
   AssertTrue( compiled.minor >= SDL_MINOR_VERSION,
             "SDL_VERSION() returned minor %i (>= %i)",
             compiled.minor,
             SDL_MINOR_VERSION);   
}

/*!
 * \brief Tests default SDL_Init
 */
int platform_testDefaultInit(void *arg)
{
   int ret;
   int subsystem;
   
   ret = SDL_Init(0);   
   AssertTrue( ret == 0, 
             "SDL_Init(0): returned %i, expected 0, error: %s",
             ret,
             SDL_GetError());
             
   subsystem = SDL_WasInit(0);
   AssertTrue( subsystem == 0, 
             "SDL_WasInit(0): returned %i, expected 0",
             ret);   
             
   SDL_Quit();
}

/*!
 * \brief Tests SDL_Get/Set/ClearError
 * \sa 
 * http://wiki.libsdl.org/moin.cgi/SDL_GetError
 * http://wiki.libsdl.org/moin.cgi/SDL_SetError
 * http://wiki.libsdl.org/moin.cgi/SDL_ClearError
 */
int platform_testGetSetClearError(void *arg)
{
   const char *testError = "Testing";
   char *lastError;
   int len;
   
   SDL_ClearError();
   AssertPass("SDL_ClearError()");
   
   lastError = (char *)SDL_GetError();   
   AssertPass("SDL_GetError()");
   AssertTrue(lastError != NULL, 
             "SDL_GetError() != NULL");
   if (lastError != NULL)
   {
     len = strlen(lastError);
     AssertTrue(len == 0, 
             "SDL_GetError(): no message expected, len: %i", len);
   }
   
   SDL_SetError("%s", testError);
   AssertPass("SDL_SetError()");
   lastError = (char *)SDL_GetError();   
   AssertTrue(lastError != NULL, 
             "SDL_GetError() != NULL");
   if (lastError != NULL)
   {
     len = strlen(lastError);
     AssertTrue(len == strlen(testError), 
             "SDL_GetError(): expected message len %i, was len: %i", 
             strlen(testError), 
             len);
     AssertTrue(strcmp(lastError, testError) == 0, 
             "SDL_GetError(): expected message %s, was message: %s", 
             testError, 
             lastError);
   }

   // Clean up                
   SDL_ClearError();
}

/*!
 * \brief Tests SDL_SetError with empty input
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_SetError
 */
int platform_testSetErrorEmptyInput(void *arg)
{
   const char *testError = "";
   char *lastError;
   int len;
   
   SDL_SetError("%s", testError);
   AssertPass("SDL_SetError()");
   lastError = (char *)SDL_GetError();   
   AssertTrue(lastError != NULL, 
             "SDL_GetError() != NULL");
   if (lastError != NULL)
   {
     len = strlen(lastError);
     AssertTrue(len == strlen(testError), 
             "SDL_GetError(): expected message len %i, was len: %i", 
             strlen(testError), 
             len);
     AssertTrue(strcmp(lastError, testError) == 0, 
             "SDL_GetError(): expected message '%s', was message: '%s'", 
             testError, 
             lastError);
   }

   // Clean up                
   SDL_ClearError();
}

/*!
 * \brief Tests SDL_SetError with invalid input
 * \sa 
 * http://wiki.libsdl.org/moin.cgi/SDL_SetError
 */
int platform_testSetErrorInvalidInput(void *arg)
{
   const char *testError = NULL;
   const char *probeError = "Testing";
   char *lastError;
   int len;

   // Reset
   SDL_ClearError();
   
   // Check for no-op
   SDL_SetError(testError);
   AssertPass("SDL_SetError()");
   lastError = (char *)SDL_GetError();   
   AssertTrue(lastError != NULL, 
             "SDL_GetError() != NULL");
   if (lastError != NULL)
   {
     len = strlen(lastError);
     AssertTrue(len == 0, 
             "SDL_GetError(): expected message len 0, was len: %i", 
             0, 
             len);
     AssertTrue(strcmp(lastError, "") == 0, 
             "SDL_GetError(): expected message '', was message: '%s'",           
             lastError);
   }
   
   // Set
   SDL_SetError(probeError);
   
   // Check for no-op
   SDL_SetError(testError);
   AssertPass("SDL_SetError()");
   lastError = (char *)SDL_GetError();   
   AssertTrue(lastError != NULL, 
             "SDL_GetError() != NULL");
   if (lastError != NULL)
   {
     len = strlen(lastError);
     AssertTrue(len == strlen(probeError), 
             "SDL_GetError(): expected message len %i, was len: %i", 
             strlen(probeError), 
             len);
     AssertTrue(strcmp(lastError, probeError) == 0, 
             "SDL_GetError(): expected message '%s', was message: '%s'",
             probeError,
             lastError);
   }

   // Clean up                
   SDL_ClearError();
}

/*!
 * \brief Tests SDL_GetPowerInfo
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_GetPowerInfo
 */
int platform_testGetPowerInfo(void *arg)
{
   SDL_PowerState state;
   SDL_PowerState stateAgain;
   int secs;
   int secsAgain;
   int pct;
   int pctAgain;
   
   state = SDL_GetPowerInfo(&secs, &pct);
   AssertPass("SDL_GetPowerInfo()");
   AssertTrue(
       state==SDL_POWERSTATE_UNKNOWN ||
       state==SDL_POWERSTATE_ON_BATTERY ||
       state==SDL_POWERSTATE_NO_BATTERY ||
       state==SDL_POWERSTATE_CHARGING ||
       state==SDL_POWERSTATE_CHARGED,
       "SDL_GetPowerInfo(): state %i is one of the expected values",
       (int)state);
       
   if (state==SDL_POWERSTATE_ON_BATTERY)
   {
      AssertTrue(
         secs >= 0,
         "SDL_GetPowerInfo(): on battery, secs >= 0, was: %i",
         secs);
      AssertTrue(
         (pct >= 0) && (pct <= 100),
         "SDL_GetPowerInfo(): on battery, pct=[0,100], was: %i",
         pct);         
   }

   if (state==SDL_POWERSTATE_UNKNOWN ||
       state==SDL_POWERSTATE_NO_BATTERY)
   {
      AssertTrue(
         secs == -1,
         "SDL_GetPowerInfo(): no battery, secs == -1, was: %i",
         secs);
      AssertTrue(
         pct == -1,
         "SDL_GetPowerInfo(): no battery, pct == -1, was: %i",
         pct);         
   }

   // Partial return value variations   
   stateAgain = SDL_GetPowerInfo(&secsAgain, NULL);
   AssertTrue(
        state==stateAgain,
        "State %i returned when only 'secs' requested",
        stateAgain);   
   AssertTrue(
        secs==secsAgain,
        "Value %i matches when only 'secs' requested",
        secsAgain);   
   stateAgain = SDL_GetPowerInfo(NULL, &pctAgain);
   AssertTrue(
        state==stateAgain,
        "State %i returned when only 'pct' requested",
        stateAgain);   
   AssertTrue(
        pct==pctAgain,
        "Value %i matches when only 'pct' requested",
        pctAgain);   
   stateAgain = SDL_GetPowerInfo(NULL, NULL);
   AssertTrue(
        state==stateAgain,
        "State %i returned when no value requested",
        stateAgain); 
}
