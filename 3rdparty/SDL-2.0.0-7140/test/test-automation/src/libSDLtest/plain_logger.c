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

#ifndef _PLAIN_LOGGER
#define _PLAIN_LOGGER

#include "stdio.h"

#include "../../include/SDL_test.h"

#include "../runner/logger.h"

#include "logger_helpers.h"
#include "plain_logger.h"

/*! Current indentationt level */
static int indentLevel;

/*! Logging level of the logger */
static Level level = LOGGER_TERSE;

//! Handle to log file
static FILE *logFile;

/*!
 * Prints out the output of the logger
 *
 * \param currentIndentLevel The currently used indentation level
 * \param message The message to be printed out
 */
int
Output(const int currentIndentLevel, const char *message, ...)
{
	if(logFile == NULL) {
		fprintf(stderr, "logfile is NULL\n");
		exit(3);
	}

	int indent = 0;
	for( ; indent < currentIndentLevel; ++indent) {
		fprintf(logFile, "  "); // \todo make configurable?
	}

	char buffer[1024];
	memset(buffer, 0, 1024);

	va_list list;
	va_start(list, message);

	SDL_vsnprintf(buffer, 1024, message, list);

	va_end(list);
	fprintf(logFile, "%s\n", buffer);
	fflush(logFile);
}


void
PlainRunStarted(int parameterCount, char *runnerParameters[], char *runSeed,
			    time_t eventTime, LoggerData *data)
{
	if(data == NULL) {
		fprintf(stderr, "Logger data is NULL\n");
		exit(3);
	}

	// Set up the logging destination
	if(data->stdoutEnabled == 1) {
		logFile = stdout;
	} else {
		logFile = fopen(data->filename, "w");
		if(logFile == NULL) {
			fprintf(stderr, "Log file %s couldn't opened\n", data->filename);
			exit(3);
		}
	}


	level = data->level;

	Output(indentLevel, "Test run started at %s", TimestampToString(eventTime));
	Output(indentLevel, "Fuzzer seed is: %s", runSeed);
	Output(indentLevel, "Runner parameters: ");

	int counter = 0;
	for(counter = 0; counter < parameterCount; counter++) {
		char *parameter = runnerParameters[counter];
		Output(indentLevel, "\t%s", parameter);
	}

	Output(indentLevel, "");
}

void
PlainRunEnded(int testCount, int suiteCount, int testPassCount, int testFailCount,
			  int testSkippedCount, time_t endTime, double totalRuntime)
{
	Output(indentLevel, "Test run ended at %s", TimestampToString(endTime));

	Output(indentLevel, "Ran %d tests in %0.5f seconds from %d suites.",
			testCount, totalRuntime, suiteCount);

	Output(indentLevel, "%d tests passed", testPassCount);
	Output(indentLevel, "%d tests failed", testFailCount);
	Output(indentLevel, "%d tests skipped", testSkippedCount);

	fclose(logFile);
}

void
PlainSuiteStarted(const char *suiteName, time_t eventTime)
{
	Output(indentLevel++, "Executing tests from %s", suiteName);
}

void
PlainSuiteEnded(int testsPassed, int testsFailed, int testsSkipped,
           time_t endTime, double totalRuntime)
{
	Output(--indentLevel, "Suite executed. %d passed, %d failed and %d skipped. Total runtime %0.5f seconds",
			testsPassed, testsFailed, testsSkipped, totalRuntime);
	Output(indentLevel, "");
}

void
PlainTestStarted(const char *testName, const char *suiteName,
				const char *testDescription, Uint64 execKey, time_t startTime)
{
	Output(indentLevel, "Executing test: %s (in %s, exec key: %llX)", testName, suiteName, execKey);
	Output(indentLevel++, "Test description: %s", testDescription);
}

void
PlainTestEnded(const char *testName, const char *suiteName,
          int testResult, time_t endTime, double totalRuntime)
{
	switch(testResult) {
		case TEST_RESULT_PASS:
			Output(--indentLevel, "%s: ok", testName);
			break;
		case TEST_RESULT_FAILURE:
			Output(--indentLevel, "%s: failed", testName);
			break;
		case TEST_RESULT_NO_ASSERT:
			Output(--indentLevel, "%s: failed -> no assert", testName);
			break;
		case TEST_RESULT_SKIPPED:
			Output(--indentLevel, "%s: skipped", testName);
			break;
		case TEST_RESULT_KILLED:
			Output(--indentLevel, "%s: killed, exceeded timeout", testName);
			break;
		case TEST_RESULT_SETUP_FAILURE:
			Output(--indentLevel, "%s: killed, setup failure", testName);
			break;
	}
}

void
PlainAssert(const char *assertName, int assertResult, const char *assertMessage,
		time_t eventTime)
{
	// Log passed asserts only on VERBOSE level
	if(level <= LOGGER_TERSE && assertResult == ASSERT_PASS) {
		return ;
	}

	const char *result = (assertResult) ? "passed" : "failed";
	Output(indentLevel, "%s: %s - %s", assertName, result, assertMessage);
}

void
PlainAssertWithValues(const char *assertName, int assertResult, const char *assertMessage,
		int actualValue, int expectedValue, time_t eventTime)
{
	// Log passed asserts only on VERBOSE level
	if(level <= LOGGER_TERSE && assertResult == ASSERT_PASS) {
		return ;
	}

	const char *result = (assertResult) ? "passed" : "failed";
	Output(indentLevel, "%s: %s (expected %d, actualValue %d) - %s",
			assertName, result, expectedValue, actualValue, assertMessage);
}

void
PlainAssertSummary(int numAsserts, int numAssertsFailed, int numAssertsPass, time_t eventTime)
{
	Output(indentLevel, "Assert summary: %d failed, %d passed (total: %d)",
			numAssertsFailed, numAssertsPass, numAsserts);
}

void
PlainLog(time_t eventTime, char *fmt, ...)
{
	// create the log message
	va_list args;
	char logMessage[1024];
	memset(logMessage, 0, sizeof(logMessage));

	va_start( args, fmt );
	SDL_vsnprintf( logMessage, sizeof(logMessage), fmt, args );
	va_end( args );

	Output(indentLevel, "%s", logMessage);
}

#endif
