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

#ifndef _PLAIN_LOGGER_H
#define _PLAIN_LOGGER_H

#include <SDL/SDL_stdinc.h>
#include "../runner/logger.h"


/*!
 * Prints out information about starting the test run.
 *
 * \param parameterCount How many parameters were given
 * \param runnerParameters What parameters were given to the runner
 * \param runSeed Fuzzer seed of the harness
 * \param eventTime When the execution started
 * \param data LoggerData structure which contains data for the logger
 *
 */
void PlainRunStarted(int parameterCount, char *runnerParameters[], char *runSeed,
					 time_t eventTime, LoggerData *data);

/*!
 * Prints out information about ending the test run.
 *
 * \param testCount How many tests were executed in total
 * \param suiteCount How many suite were executed in total
 * \param testPassCount How many tests passed in total
 * \param testSkippedCount How many tests were skipped in total
 * \param testFailCount How many tests failed in total
 * \param endTime When the execution ended
 * \param totalRuntime How long the execution took
 */
void PlainRunEnded(int testCount, int suiteCount, int testPassCount, int testFailCount,
				   int testSkippedCount, time_t endTime, double totalRuntime);

/*!
 * Prints the data about the test suite that'll be executed next
 *
 * \param suiteName Name of the test suite
 * \param eventTime When the execution starts
 */
void PlainSuiteStarted(const char *suiteName, time_t eventTime);

/*!
 * Prints information about the test suite that was just executed
 *
 * \param testsPassed how many tests passed from this suite
 * \param testsFailed how many tests failed from this suite
 * \param testsSkipped how many tests were skipped (not implemented)
 * \param endTime When the suite execution ended
 * \param totalRuntime How long did the suite's execution take
 */
void PlainSuiteEnded(int testsPassed, int testsFailed, int testsSkipped,
           time_t endTime, double totalRuntime);

/*!
 * Prints the data about the test test that'll be executed next
 *
 * \param testName Name of the test that'll be executed
 * \param suiteName Name of the suite of the test
 * \param testDescription Description of the test
 * \param execKey Execution key for fuzzing
 * \param startTime When the test started to execute
 */
void PlainTestStarted(const char *testName, const char *suiteName,
                      const char *testDescription, Uint64 execKey, time_t startTime);

/*!
 * Prints information about the test test that was just executed
 *
 * \param testName Name of the executed test
 * \param suiteName Name of the suite of the test
 * \param testResult Did the test fail (!= 0) or pass (== 0)
 * \param endTime When the test execution ended
 * \param totalRuntime Total runtime of the executed test
 */
void PlainTestEnded(const char *testName, const char *suiteName,
          int testResult, time_t endTime, double totalRuntime);


/*!
 * Prints information about plain assert
 *
 * \param assertName Name of the assert
 * \param assertResult Did assert fail (== 0) or success (!= 0)
 * \param assertMessage Message of the assert
 * \param eventTime When the assert happened
 */
void PlainAssert(const char *assertName, int assertResult, const char *assertMessage,
				time_t eventTime);

/*!
 * Prints information about assert that has actual and expected values
 *
 * \param assertName Name of the assert
 * \param assertResult Did assert fail (== 0) or success (!= 0)
 * \param assertMessage Message of the assert
 * \param actualValue Actual value of assert
 * \param expected Excepted value of assert
 * \param eventTime When the assert happened
 */
void PlainAssertWithValues(const char *assertName, int assertResult, const char *assertMessage,
		int actualValue, int expected, time_t eventTime);

/*!
 * Prints summary of all assertions of certain tests
 *
 * \param numAsserts Total assert count for the executed test
 * \param numAssertsFailed Count of failed asserts in the test
 * \param numAssertsPass Count of passed asserts in the test
 * \param eventTime Timestamp of the summary
 */
void PlainAssertSummary(int numAsserts, int numAssertsFailed, int numAssertsPass, time_t eventTime);

/*!
 * Prints given message
 *
 * \param eventTime Timestamp for log message
 * \param fmt Message to be logged
 */
void PlainLog(time_t eventTime, char *fmt, ...);

#endif
