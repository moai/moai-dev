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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>

#include "../../include/SDL_test.h"

#include "../runner/logger.h"

#include "xml.h"
#include "logger_helpers.h"
#include "xml_logger.h"

/*! Static strings for XML elements */
const char *documentRoot = "testlog";
const char *parametersElementName = "parameters";
const char *parameterElementName = "parameter";
const char *startTimeElementName = "startTime";
const char *seedElementName = "seed";
const char *execKeyElementName = "executionKey";
const char *numSuitesElementName = "numSuites";
const char *numTestElementName = "numTests";
const char *numPassedTestsElementName = "numPassedTests";
const char *numFailedTestsElementName = "numFailedTests";
const char *numSkippedTestsElementName = "numSkippedTests";
const char *endTimeElementName = "endTime";
const char *totalRuntimeElementName = "totalRuntime";
const char *suiteElementName = "suite";
const char *testsPassedElementName = "testsPassed";
const char *testsFailedElementName = "testsFailed";
const char *testsSkippedElementName = "testsSkipped";
const char *testElementName = "test";
const char *nameElementName = "name";
const char *descriptionElementName = "description";
const char *resultElementName = "result";
const char *resultDescriptionElementName = "resultDescription";
const char *assertElementName = "assert";
const char *messageElementName = "message";
const char *timeElementName = "time";
const char *assertSummaryElementName = "assertSummary";
const char *assertCountElementName = "assertCount";
const char *assertsPassedElementName = "assertsPassed";
const char *assertsFailedElementName = "assertsFailed";
const char *logElementName = "log";


/*! Current indentationt level */
static int indentLevel;

/*! Logging level of the logger */
static Level level = LOGGER_TERSE;

//! Constants for XMLOuputters EOL parameter
#define YES 1
#define NO 0

/*! Controls printing the indentation in relation to line breaks */
static int prevEOL = YES;

//! Handle to log file
static FILE *logFile = NULL;

/*
 * Prints out the given xml element etc.
 *
 * \todo Make the destination of the output changeable (defaults to stdout)
 *
 * \param currentIndentLevel the indent level of the message
 * \param EOL will it print end of line character or not
 * \param the XML element itself
 *
 */
void
XMLOutputter(const int currentIndentLevel,
			 int EOL, const char *message)
{
	if(ValidateString(message)) {
		int indent = 0;
		for( ; indent < currentIndentLevel && prevEOL; ++indent) {
			fprintf(logFile, "  "); // \todo make configurable?
		}

		prevEOL = EOL;

		if(EOL) {
			fprintf(logFile, "%s\n", message);
		} else {
			fprintf(logFile, "%s", message);
		}

		fflush(logFile);
	} else {
		fprintf(logFile, "Error: Tried to output invalid string!");
	}

	SDL_free((char *)message);
}

void
XMLRunStarted(int parameterCount, char *runnerParameters[], char *runSeed,
			 time_t eventTime, LoggerData *data)
{
	// Set up the logging destination
	if(data->stdoutEnabled) {
		logFile = stdout;
	} else {
		logFile = fopen(data->filename, "w");
		if(logFile == NULL) {
			fprintf(stderr, "Log file %s couldn't opened\n", data->filename);
			exit(3);
		}
	}

	// Set up the style sheet
	char *xslStylesheet = (char *)data->custom;
	level = data->level;
	//printf("Debug: %d == %d\n", level, data->level);

	char *output = XMLOpenDocument(documentRoot, xslStylesheet);
	XMLOutputter(indentLevel++, YES, output);

	// log harness parameters
	output = XMLOpenElement(parametersElementName);
	XMLOutputter(indentLevel++, YES, output);

	int counter = 0;
	for(counter = 0; counter < parameterCount; counter++) {
		char *parameter = runnerParameters[counter];

		output = XMLOpenElement(parameterElementName);
		XMLOutputter(indentLevel++, NO, output);

		output = XMLAddContent(parameter);
		XMLOutputter(indentLevel, NO, output);

		output = XMLCloseElement(parameterElementName);
		XMLOutputter(--indentLevel, YES, output);
	}

	output = XMLCloseElement(parametersElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log seed
	output = XMLOpenElement(seedElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(runSeed);
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(seedElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log start time
	output = XMLOpenElement(startTimeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(TimestampToString(eventTime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(startTimeElementName);
	XMLOutputter(--indentLevel, YES, output);
}

void
XMLRunEnded(int testCount, int suiteCount, int testPassCount, int testFailCount,
			int testSkippedCount, time_t endTime, double totalRuntime)
{
	// log suite count
	char *output = XMLOpenElement(numSuitesElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(suiteCount));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(numSuitesElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log test count
	output = XMLOpenElement(numTestElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(testCount));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(numTestElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log passed test count
	output = XMLOpenElement(numPassedTestsElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(testPassCount));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(numPassedTestsElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log failed test count
	output = XMLOpenElement(numFailedTestsElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(testFailCount));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(numFailedTestsElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log skipped test count
	output = XMLOpenElement(numSkippedTestsElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(testSkippedCount));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(numSkippedTestsElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log end tite
	output = XMLOpenElement(endTimeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(TimestampToString(endTime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(endTimeElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log total runtime
	output = XMLOpenElement(totalRuntimeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(DoubleToString(totalRuntime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(totalRuntimeElementName);
	XMLOutputter(--indentLevel, YES, output);

	output = XMLCloseDocument(documentRoot);
	XMLOutputter(--indentLevel, YES, output);

	// close the log file
	fclose(logFile);
}

void
XMLSuiteStarted(const char *suiteName, time_t eventTime)
{
	// log suite name
	char *output = XMLOpenElement(suiteElementName);
	XMLOutputter(indentLevel++, YES, output);

	output = XMLOpenElement(nameElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(suiteName);
	XMLOutputter(indentLevel, NO, output);

	// log test name
	output = XMLCloseElement(nameElementName);
	XMLOutputter(--indentLevel, YES, output);

	output = XMLOpenElement(startTimeElementName);
	XMLOutputter(indentLevel++, NO, output);

	// log beginning time
	output = XMLAddContent(TimestampToString(eventTime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(startTimeElementName);
	XMLOutputter(--indentLevel, YES, output);
}

void
XMLSuiteEnded(int testsPassed, int testsFailed, int testsSkipped,
           time_t endTime, double totalRuntime)
{
	// log tests passed
	char *output = XMLOpenElement(testsPassedElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(testsPassed));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(testsPassedElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log tests failed
	output = XMLOpenElement(testsFailedElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(testsFailed));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(testsFailedElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log tests skipped
	output = XMLOpenElement(testsSkippedElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(testsSkipped));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(testsSkippedElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log tests skipped
	output = XMLOpenElement(endTimeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(TimestampToString(endTime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(endTimeElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log total runtime
	output = XMLOpenElement(totalRuntimeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(DoubleToString(totalRuntime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(totalRuntimeElementName);
	XMLOutputter(--indentLevel, YES, output);


	output = XMLCloseElement(suiteElementName);
	XMLOutputter(--indentLevel, YES, output);
}

void
XMLTestStarted(const char *testName, const char *suiteName,
			  const char *testDescription, Uint64 execKey, time_t startTime)
{
	char * output = XMLOpenElement(testElementName);
	XMLOutputter(indentLevel++, YES, output);

	// log test name
	output = XMLOpenElement(nameElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(testName);
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(nameElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log test description
	output = XMLOpenElement(descriptionElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(testDescription);
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(descriptionElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log execution key
	output = XMLOpenElement(execKeyElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToHexString(execKey));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(execKeyElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log start time
	output = XMLOpenElement(startTimeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(TimestampToString(startTime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(startTimeElementName);
	XMLOutputter(--indentLevel, YES, output);
}

void
XMLTestEnded(const char *testName, const char *suiteName,
          int testResult, time_t endTime, double totalRuntime)
{
	// Log test result
	char *output = XMLOpenElement(resultElementName);
	XMLOutputter(indentLevel++, NO, output);

	switch(testResult) {
		case TEST_RESULT_PASS:
			output = XMLAddContent("passed");
			break;
		case TEST_RESULT_FAILURE:
			output = XMLAddContent("failed");
			break;
		case TEST_RESULT_NO_ASSERT:
			output = XMLAddContent("failed");
			break;
		case TEST_RESULT_SKIPPED:
			output = XMLAddContent("skipped");
			break;
		case TEST_RESULT_KILLED:
			output = XMLAddContent("failed");
			break;
		case TEST_RESULT_SETUP_FAILURE:
			output = XMLAddContent("failed");
			break;
	}
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(resultElementName);
	XMLOutputter(--indentLevel, YES, output);

	// Log description of test result. Why the test failed,
	// if there's some specific reason
	output = XMLOpenElement(resultDescriptionElementName);
	XMLOutputter(indentLevel++, NO, output);

	switch(testResult) {
		case TEST_RESULT_PASS:
		case TEST_RESULT_FAILURE:
		case TEST_RESULT_SKIPPED:
			output = XMLAddContent("");
			break;
		case TEST_RESULT_NO_ASSERT:
			output = XMLAddContent("No assert");
			break;
		case TEST_RESULT_KILLED:
			output = XMLAddContent("Timeout exceeded");
			break;
		case TEST_RESULT_SETUP_FAILURE:
			output = XMLAddContent("Setup failure, couldn't be executed");
			break;
	}
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(resultDescriptionElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log total runtime
	output = XMLOpenElement(endTimeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(TimestampToString(endTime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(endTimeElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log total runtime
	output = XMLOpenElement(totalRuntimeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(DoubleToString(totalRuntime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(totalRuntimeElementName);
	XMLOutputter(--indentLevel, YES, output);

	output = XMLCloseElement(testElementName);
	XMLOutputter(--indentLevel, YES, output);
}

void
XMLAssert(const char *assertName, int assertResult, const char *assertMessage,
		  time_t eventTime)
{
	// Log passed asserts only on VERBOSE level
	if(level <= LOGGER_TERSE && assertResult == ASSERT_PASS) {
		return ;
	}

	char *output = XMLOpenElement(assertElementName);
	XMLOutputter(indentLevel++, YES, output);

	// log assert name
	output = XMLOpenElement(nameElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(assertName);
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(nameElementName);
	XMLOutputter(--indentLevel, YES, output);


	// log assert result
	output = XMLOpenElement(resultElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent((assertResult) ? "pass" : "failure");
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(resultElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log assert message
	output = XMLOpenElement(messageElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(assertMessage);
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(messageElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log event time
	output = XMLOpenElement(timeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(TimestampToString(eventTime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(timeElementName);
	XMLOutputter(--indentLevel, YES, output);

	output = XMLCloseElement(assertElementName);
	XMLOutputter(--indentLevel, YES, output);
}

void
XMLAssertWithValues(const char *assertName, int assertResult, const char *assertMessage,
		int actualValue, int excpected, time_t eventTime)
{
	// Log passed asserts only on VERBOSE level
	if(level <= LOGGER_TERSE && assertResult == ASSERT_PASS) {
		return ;
	}

	char *output = XMLOpenElement(assertElementName);
	XMLOutputter(indentLevel++, YES, output);

	// log assert name
	output = XMLOpenElement(nameElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(assertName);
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(nameElementName);
	XMLOutputter(--indentLevel, YES, output);


	// log assert result
	output = XMLOpenElement(resultElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent((assertResult) ? "pass" : "failure");
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(resultElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log assert message
	output = XMLOpenElement(messageElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(assertMessage);
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(messageElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log event time
	output = XMLOpenElement(timeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(TimestampToString(eventTime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(timeElementName);
	XMLOutputter(--indentLevel, YES, output);

	output = XMLCloseElement(assertElementName);
	XMLOutputter(--indentLevel, YES, output);
}

void
XMLAssertSummary(int numAsserts, int numAssertsFailed,
				 int numAssertsPass, time_t eventTime)
{
	char *output = XMLOpenElement(assertSummaryElementName);
	XMLOutputter(indentLevel++, YES, output);

	output = XMLOpenElement(assertCountElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(numAsserts));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(assertCountElementName);
	XMLOutputter(--indentLevel, YES, output);

	output = XMLOpenElement(assertsPassedElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(numAssertsPass));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(assertsPassedElementName);
	XMLOutputter(--indentLevel, YES, output);

	output = XMLOpenElement(assertsFailedElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(IntToString(numAsserts));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(assertsFailedElementName);
	XMLOutputter(--indentLevel, YES, output);

	output = XMLCloseElement(assertSummaryElementName);
	XMLOutputter(--indentLevel, YES, output);
}

void
XMLLog(time_t eventTime, char *fmt, ...)
{
	// create the log message
	va_list args;
	char logMessage[1024];
	memset(logMessage, 0, sizeof(logMessage));

	va_start( args, fmt );
	SDL_vsnprintf( logMessage, sizeof(logMessage), fmt, args );
	va_end( args );

	char *output = XMLOpenElement(logElementName);
	XMLOutputter(indentLevel++, YES, output);

	// log message
	output = XMLOpenElement(messageElementName);
	XMLOutputter(indentLevel++, NO, output);

	// fix this here!
	output = XMLAddContent(logMessage);
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(messageElementName);
	XMLOutputter(--indentLevel, YES, output);

	// log eventTime
	output = XMLOpenElement(timeElementName);
	XMLOutputter(indentLevel++, NO, output);

	output = XMLAddContent(TimestampToString(eventTime));
	XMLOutputter(indentLevel, NO, output);

	output = XMLCloseElement(timeElementName);
	XMLOutputter(--indentLevel, YES, output);

	output = XMLCloseElement(logElementName);
	XMLOutputter(--indentLevel, YES, output);
}
