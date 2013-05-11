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

#ifndef _SDL_TEST_H
#define _SDL_TEST_H

#include "../src/runner/logger.h"

#include "../src/libSDLtest/common/common.h"
#include "../src/libSDLtest/common/images.h"
#include "../src/libSDLtest/fuzzer/fuzzer.h"

#define TEST_ENABLED  1
#define TEST_DISABLED 0

//! Definitions of assert results
#define ASSERT_PASS 1
#define ASSERT_FAILURE 0

//! Definition of all the possible test results
#define TEST_RESULT_PASS			0
#define TEST_RESULT_FAILURE			1
#define TEST_RESULT_NO_ASSERT		2
#define TEST_RESULT_SKIPPED			3
#define TEST_RESULT_KILLED			4
#define TEST_RESULT_SETUP_FAILURE	5

//! Definitions for test requirements
#define TEST_REQUIRES_AUDIO 1
//! Defines that the a test requires STDIO
#define TEST_REQUIRES_STDIO 1

/*!
 * Holds information about a test case
 */
typedef struct TestCaseReference {
	/*!< "Func2Stress" */
	char *name;
	/*!< "This test beats the crap out of func2()" */
	char *description;
	/*!< Set to TEST_ENABLED or TEST_DISABLED */
	int enabled;
	/*!< Set to TEST_REQUIRES_OPENGL, TEST_REQUIRES_AUDIO, ... */
	long requirements;
	/*<! Timeout value in seconds. If exceeded runner will kill the test. 0 means infinite time */
	long timeout;
} TestCaseReference;

/*!
 *  Initialized the test environment such as asserts. Must be called at
 *  the beginning of every test case, before doing anything else.
 *
 *  \param execKey Execution key for the test
 *  \param inproc Whether tests are run in-process or not
 */
void _InitTestEnvironment(Uint64 execKey, SDL_bool inproc);

/*!
 *  Deinitializes the test environment and
 *  returns the result of the test (pass or failure)
 *
 * \return 0 if test succeeded, otherwise 1
 */
int _QuitTestEnvironment();

/*!
 * Can be used to query the number of failed asserts
 * \return Returns the failed assert count.
 */
int _CountFailedAsserts();

/*!
 *  Assert function. Tests if the expected value equals the actual value, then
 *  the test assert succeeds, otherwise it fails and warns about it.
 *
 * \param expected Value user expects to have
 * \param actual The actual value of tested variable
 * \param message Message that will be printed
 */
void AssertEquals(int expected, int actual, char *message, ...);

/*!
 *  Assert function. Tests if the given condition is true. True in
 *  this case means non-zero value. If the condition is true, the
 *  assert passes, otherwise it fails.
 *
 * \param condition Condition which will be evaluated
 * \param message Message that will be printed
 */
void AssertTrue(int condition, char *message, ...);

/*!
 *  Assert function which will always fail
 *
 * \param message Message that will be printed
 */
void AssertFail(char *message, ...);

/*!
 *  Assert function which will always pass
 *
 * \param message Message that will be printed
 */
void AssertPass(char *message, ...);

#endif
