/*
  Copyright (C) 2011 Markus Kauppila <markus.kauppila@gmail.com>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <stdio.h> /* printf/fprintf */
#include <stdarg.h> /* va_list */
#include <time.h>

#include <SDL/SDL_stdinc.h>

#include "fuzzer/fuzzer.h"

#include "../runner/logger.h"

#include "../../include/SDL_test.h"


/*! \brief return value of test case. Non-zero value means that the test failed */
int _testReturnValue;

/*! \brief counts the failed asserts */
int _testAssertsFailed;

/*! \brief counts the passed asserts */
int _testAssertsPassed;

/*! \brief is the execution done in-process? */
SDL_bool canBailOut;

void
_InitTestEnvironment(Uint64 execKey, SDL_bool inproc)
{
	InitFuzzer(execKey);

	canBailOut = inproc == 0;

	_testReturnValue = TEST_RESULT_PASS;
	_testAssertsFailed = 0;
	_testAssertsPassed = 0;
}

int
_QuitTestEnvironment()
{
	AssertSummary(_testAssertsFailed + _testAssertsPassed,
                  _testAssertsFailed, _testAssertsPassed, time(0));

	if(_testAssertsFailed == 0 && _testAssertsPassed == 0) {
		_testReturnValue = TEST_RESULT_NO_ASSERT;
	}

	if(GetInvocationCount() > 0) {
		Log(time(0), "Fuzzer invocation count: %d", GetInvocationCount());
	}

	DeinitFuzzer();

	return _testReturnValue;
}

int
_CountFailedAsserts() {
	return _testAssertsFailed;
}

/*!
 * Bail out from test case. For example, the function is used to bail out
 * from a test case after a failed assert.
 */
void
_BailOut()
{
    if(!canBailOut)
    	return ;

	AssertSummary(_testAssertsFailed + _testAssertsPassed,
                  _testAssertsFailed, _testAssertsPassed, time(0));

	if(GetInvocationCount() > 0) {
		Log(time(0), "Fuzzer invocation count: %d", GetInvocationCount());
	}

	DeinitFuzzer();

	exit(TEST_RESULT_FAILURE); // bail out from the test
}

void
AssertEquals(int expected, int actual, char *message, ...)
{
   va_list args;
   char buf[256];

   va_start( args, message );
   memset(buf, 0, sizeof(buf));
   SDL_vsnprintf( buf, sizeof(buf), message, args );
   va_end( args );

   if(expected != actual) {
      AssertWithValues("AssertEquals", 0, buf, actual, expected, time(0));

      _testReturnValue = TEST_RESULT_FAILURE;
      _testAssertsFailed++;

      _BailOut();
      } else {
	   AssertWithValues("AssertEquals", 1, buf,
    		  actual, expected, time(0));

      _testAssertsPassed++;
   }
}


void
AssertTrue(int condition, char *message, ...)
{
   va_list args;
   char buf[256];
   va_start( args, message );
   SDL_vsnprintf( buf, sizeof(buf), message, args );
   va_end( args );

   if (!condition) {
      Assert("AssertTrue", 0, buf, time(0));

      _testReturnValue = TEST_RESULT_FAILURE;
      _testAssertsFailed++;

      _BailOut();
   } else {
		Assert("AssertTrue", 1, buf, time(0));

		_testAssertsPassed++;
   }
}

void
AssertPass(char *message, ...)
{
   va_list args;
   char buf[256];
   
   va_start( args, message );
   SDL_vsnprintf( buf, sizeof(buf), message, args );
   va_end( args );

   Assert("AssertPass", 1, buf, time(0));

   _testAssertsPassed++;
}


void
AssertFail(char *message, ...)
{
   va_list args;
   char buf[256];
   
   va_start( args, message );
   SDL_vsnprintf( buf, sizeof(buf), message, args );
   va_end( args );

   Assert("AssertFail", 0, buf, time(0));

   _testReturnValue = TEST_RESULT_FAILURE;
   _testAssertsFailed++;

   _BailOut();
}

