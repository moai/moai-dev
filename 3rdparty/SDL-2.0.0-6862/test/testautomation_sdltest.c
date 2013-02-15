/**
 * SDL_test test suite
 */

#include <stdio.h>
#include <limits.h>
#include <float.h>

#include "SDL.h"
#include "SDL_test.h"

/* Test case functions */

/**
 * @brief Calls to SDLTest_GetFuzzerInvocationCount()
 */
int
sdltest_getFuzzerInvocationCount(void *arg)
{
  Uint8 result;
  int fuzzerCount1, fuzzerCount2;
 
  fuzzerCount1 = SDLTest_GetFuzzerInvocationCount();
  SDLTest_AssertPass("Call to SDLTest_GetFuzzerInvocationCount()");
  SDLTest_AssertCheck(fuzzerCount1 >= 0, "Verify returned value, expected: >=0, got: %d", fuzzerCount1);
     
  result = SDLTest_RandomUint8();
  SDLTest_AssertPass("Call to SDLTest_RandomUint8(), returned %d", result);

  fuzzerCount2 = SDLTest_GetFuzzerInvocationCount();
  SDLTest_AssertPass("Call to SDLTest_GetFuzzerInvocationCount()");
  SDLTest_AssertCheck(fuzzerCount2 > fuzzerCount1, "Verify returned value, expected: >%d, got: %d", fuzzerCount1, fuzzerCount2);
       
  return TEST_COMPLETED;
}


/**
 * @brief Calls to random number generators
 */
int
sdltest_randomNumber(void *arg)
{
  Sint64 result;
  Uint64 uresult;
  double dresult;
  Uint64 umax;
  Sint64 min, max;
     
  result = (Sint64)SDLTest_RandomUint8();
  umax = (1 << 8) - 1;
  SDLTest_AssertPass("Call to SDLTest_RandomUint8");
  SDLTest_AssertCheck(result >= 0 && result <= umax, "Verify result value, expected: [0,%llu], got: %lld", umax, result);

  result = (Sint64)SDLTest_RandomSint8();
  min = 1 - (1 << 7);
  max =     (1 << 7) - 1;
  SDLTest_AssertPass("Call to SDLTest_RandomSint8");
  SDLTest_AssertCheck(result >= min && result <= max, "Verify result value, expected: [%lld,%lld], got: %lld", min, max, result);
  
  result = (Sint64)SDLTest_RandomUint16();
  umax = (1 << 16) - 1;
  SDLTest_AssertPass("Call to SDLTest_RandomUint16");
  SDLTest_AssertCheck(result >= 0 && result <= umax, "Verify result value, expected: [0,%llu], got: %lld", umax, result);

  result = (Sint64)SDLTest_RandomSint16();
  min = 1 - (1 << 15);
  max =     (1 << 15) - 1;
  SDLTest_AssertPass("Call to SDLTest_RandomSint16");
  SDLTest_AssertCheck(result >= min && result <= max, "Verify result value, expected: [%lld,%lld], got: %lld", min, max, result);

  result = (Sint64)SDLTest_RandomUint32();
  umax = ((Uint64)1 << 32) - 1;
  SDLTest_AssertPass("Call to SDLTest_RandomUint32");
  SDLTest_AssertCheck(result >= 0 && result <= umax, "Verify result value, expected: [0,%llu], got: %lld", umax, result);

  result = (Sint64)SDLTest_RandomSint32();
  min = 1 - ((Sint64)1 << 31);
  max =     ((Sint64)1 << 31) - 1;
  SDLTest_AssertPass("Call to SDLTest_RandomSint32");
  SDLTest_AssertCheck(result >= min && result <= max, "Verify result value, expected: [%lld,%lld], got: %lld", min, max, result);

  result = (Sint64)SDLTest_RandomUint32();
  umax = ((Uint64)1 << 32) - 1;
  SDLTest_AssertPass("Call to SDLTest_RandomUint32");
  SDLTest_AssertCheck(result >= 0 && result <= umax, "Verify result value, expected: [0,%llu], got: %lld", umax, result);

  result = (Sint64)SDLTest_RandomSint32();
  min = 1 - ((Sint64)1 << 31);
  max =     ((Sint64)1 << 31) - 1;
  SDLTest_AssertPass("Call to SDLTest_RandomSint32");
  SDLTest_AssertCheck(result >= min && result <= max, "Verify result value, expected: [%lld,%lld], got: %lld", min, max, result);

  uresult = SDLTest_RandomUint64();
  SDLTest_AssertPass("Call to SDLTest_RandomUint64");

  result = SDLTest_RandomSint64();
  SDLTest_AssertPass("Call to SDLTest_RandomSint64");

  dresult = (double)SDLTest_RandomUnitFloat();
  SDLTest_AssertPass("Call to SDLTest_RandomUnitFloat");
  SDLTest_AssertCheck(dresult >= 0.0 && dresult < 1.0, "Verify result value, expected: [0.0,1.0[, got: %e", dresult);
    
  dresult = (double)SDLTest_RandomFloat();
  SDLTest_AssertPass("Call to SDLTest_RandomFloat");
  SDLTest_AssertCheck(dresult >= (double)(-FLT_MAX) && dresult <= (double)FLT_MAX, "Verify result value, expected: [%e,%e], got: %e", (double)(-FLT_MAX), (double)FLT_MAX, dresult);

  dresult = (double)SDLTest_RandomUnitDouble();
  SDLTest_AssertPass("Call to SDLTest_RandomUnitDouble");
  SDLTest_AssertCheck(dresult >= 0.0 && dresult < 1.0, "Verify result value, expected: [0.0,1.0[, got: %e", dresult);

  dresult = SDLTest_RandomDouble();
  SDLTest_AssertPass("Call to SDLTest_RandomDouble");
    
  return TEST_COMPLETED;
}

/* ================= Test References ================== */

/* SDL_test test cases */
static const SDLTest_TestCaseReference sdltestTest1 =
		{ (SDLTest_TestCaseFp)sdltest_getFuzzerInvocationCount, "sdltest_getFuzzerInvocationCount", "Call to sdltest_GetFuzzerInvocationCount", TEST_ENABLED };

static const SDLTest_TestCaseReference sdltestTest2 =
		{ (SDLTest_TestCaseFp)sdltest_randomNumber, "sdltest_randomNumber", "Calls to random number generators", TEST_ENABLED };

/* Sequence of SDL_test test cases */
static const SDLTest_TestCaseReference *sdltestTests[] =  {
	&sdltestTest1, &sdltestTest2, NULL
};

/* SDL_test test suite (global) */
SDLTest_TestSuiteReference sdltestTestSuite = {
	"SDLtest",
	NULL,
	sdltestTests,
	NULL
};
