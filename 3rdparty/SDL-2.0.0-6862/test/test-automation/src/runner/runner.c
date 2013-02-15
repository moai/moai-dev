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

#include "SDL/SDL.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../../include/SDL_test.h"
#include "../../config.h"

#include "../libSDLtest/fuzzer/fuzzer.h"

#include "../libSDLtest/plain_logger.h"
#include "../libSDLtest/xml_logger.h"
#include "../libSDLtest/logger_helpers.h"

#include "logger.h"
#include "support.h"

//!< Function pointer to a test case function
typedef void (*TestCaseFp)(void *arg);
//!< Function pointer to a test case init function
typedef void (*InitTestInvironmentFp)(Uint64, SDL_bool);
//!< Function pointer to a test case quit function
typedef int  (*QuitTestInvironmentFp)(void);
//!< Function pointer to a test case set up function
typedef void (*TestCaseSetUpFp)(void *arg);
//!< Function pointer to a test case tear down function
typedef void  (*TestCaseTearDownFp)(void *arg);
//!< Function pointer to a function which returns the failed assert count
typedef int (*CountFailedAssertsFp)(void);


//!< Flag for executing tests in-process
static int execute_inproc = 0;

//!< Flag for only printing out the test names
static int only_print_tests = 0;

//!< Flag for executing only test with selected name
static int only_selected_test  = 0;

//!< Flag for executing only the selected test suite
static int only_selected_suite = 0;

//!< Flag for executing only tests that contain certain string in their name
static int only_tests_with_string = 0;

//!< Flag for enabling XML logging
static int xml_enabled = 0;

//! Flag for enabling user-supplied style sheet for XML test report
static int custom_xsl_enabled = 0;

//! Flag for disabling xsl-style from xml report
static int xsl_enabled = 0;

//! Flag for enabling universal timeout for tests
static int universal_timeout_enabled = 0;

//! Flag for enabling verbose logging
static int enable_verbose_logger = 0;

//! Flag for using user supplied run seed
static int userRunSeed = 0;

//! Whether or not logger should log to stdout instead of file
static int log_stdout_enabled = 1;

//! Whether or not dummy suite should be included to the test run
static int include_dummy_suite = 0;

//!< Size of the test and suite name buffers
#define NAME_BUFFER_SIZE 1024

//!< Name of the selected test
char selected_test_name[NAME_BUFFER_SIZE];

//!< Name of the selected suite
char selected_suite_name[NAME_BUFFER_SIZE];

//!< substring of test case name
char testcase_name_substring[NAME_BUFFER_SIZE];

//! Name for user-supplied XSL style sheet name
char xsl_stylesheet_name[NAME_BUFFER_SIZE];

//! User-suppled timeout value for tests
int universal_timeout = -1;

//! Default directory of the test suites
#define DEFAULT_TEST_DIRECTORY "tests/"

//! Default directory for placing log files
#define DEFAULT_LOG_DIRECTORY "logs"

//! Default directory of the test suites
#define DEFAULT_LOG_FILENAME "testrun"

//! Defines directory separator
#define DIRECTORY_SEPARATOR '/'

#define DUMMY_TEST_NAME "libtestdummy"

//! Name of the default stylesheet
const char *defaultXSLStylesheet = "style.xsl";

//! Fuzzer seed for the harness
char *runSeed = NULL;

//! Execution key that user supplied via command options
Uint64 userExecKey = 0;

//! How man time a test will be invocated
int testInvocationCount = 1;

//! Stores the basename for log files
char log_basename[NAME_BUFFER_SIZE];

//! Stores directory name for placing the logs
char log_directory[NAME_BUFFER_SIZE];

// \todo add comments
int totalTestFailureCount = 0, totalTestPassCount = 0, totalTestSkipCount = 0;
int testFailureCount = 0, testPassCount = 0, testSkipCount = 0;


/*!
 * Holds information about test suite such as it's name
 * and pointer to dynamic library. Implemented as linked list.
 */
typedef struct TestSuiteReference {
	char *name; //!< test suite name
	char *directoryPath; //!< test suites path (eg. tests/libtestsuite)
	void *library; //!< pointer to shared/dynamic library implementing the suite

	struct TestSuiteReference *next; //!< Pointer to next item in the list
} TestSuiteReference;


/*!
 * Holds information about the tests that will be executed.
 *
 * Implemented as linked list.
 */
typedef struct TestCaseItem {
	char *testName;
	char *suiteName;

	char *description;
	long requirements;
	long timeout;

	InitTestInvironmentFp initTestEnvironment;
	TestCaseSetUpFp testSetUp;
	TestCaseFp testCase;
	TestCaseTearDownFp testTearDown;
 	QuitTestInvironmentFp quitTestEnvironment;

 	CountFailedAssertsFp countFailedAsserts;

	struct TestCaseItem *next;
} TestCase;


/*! Some function prototypes. Add the rest of functions and move to runner.h */
TestCaseFp LoadTestCaseFunction(void *suite, char *testName);
InitTestInvironmentFp LoadInitTestInvironmentFunction(void *suite);
QuitTestInvironmentFp LoadQuitTestInvironmentFunction(void *suite);
TestCaseReference **QueryTestCaseReferences(void *library);
TestCaseSetUpFp LoadTestSetUpFunction(void *suite);
TestCaseTearDownFp LoadTestTearDownFunction(void *suite);
CountFailedAssertsFp LoadCountFailedAssertsFunction(void *suite);
void KillHungTestInChildProcess(int signum);
void UnloadTestSuites(TestSuiteReference *suites);
int FilterTestCase(TestCaseReference *testReference);
int HandleChildProcessReturnValue(int stat_lock);


/*! Pointers to selected logger implementation */
RunStartedFp RunStarted = NULL;
RunEndedFp RunEnded = NULL;
SuiteStartedFp SuiteStarted = NULL;
SuiteEndedFp SuiteEnded = NULL;
TestStartedFp TestStarted = NULL;
TestEndedFp TestEnded = NULL;
AssertFp Assert = NULL;
AssertWithValuesFp AssertWithValues = NULL;
AssertSummaryFp AssertSummary = NULL;
LogFp Log = NULL;


/*!
 * Scans the tests/ directory and returns the names
 * of the dynamic libraries implementing the test suites.
 *
 * Note: currently function assumes that test suites names
 * are in following format: libtestsuite.dylib or libtestsuite.so.
 *
 * Note: if only_selected_suite flags is non-zero, only the selected
 * test will be loaded.
 *
 * \param directoryName Name of the directory which will be scanned
 * \param extension What file extension is used with dynamic objects
 *
 * \return Pointer to TestSuiteReference which holds all the info about suites
 * 		   or NULL on failure
 */
TestSuiteReference *
ScanForTestSuites(char *directoryName, char *extension)
{
	typedef struct dirent Entry;
	TestSuiteReference *suites = NULL;
	TestSuiteReference *reference = NULL;
	Entry *entry = NULL;
	DIR *directory = NULL;

	if(directoryName == NULL || extension == NULL ||
		SDL_strlen(directoryName) == 0 || SDL_strlen(extension) == 0) {
		return NULL;
	}

	directory = opendir(directoryName);
	if(!directory) {
		fprintf(stderr, "Failed to open test suite directory: %s\n", directoryName);
		perror("Error message");
		exit(2);
	}

	while( (entry = readdir(directory)) ) {
		 // discards . and .. and hidden files starting with dot and directories etc.
		if(strlen(entry->d_name) > 2 && entry->d_name[0] != '.' && entry->d_type == DT_REG) {
			const char *delimiters = ".";
			char *name = strtok(entry->d_name, delimiters);
			char *ext = strtok(NULL, delimiters);

			if(name == NULL || ext == NULL) {
				goto error;
			}


			int ok = 1; // on default, we want to include all tests

			// filter out all other suites but the selected test suite
			if(only_selected_suite) {
				ok = SDL_strncmp(selected_suite_name, name, NAME_BUFFER_SIZE) == 0;
			}

			if(SDL_strncmp(name, DUMMY_TEST_NAME, NAME_BUFFER_SIZE) == 0) {
				if(include_dummy_suite) {
					ok =  1;
				} else {
					ok = 0;
				}
			}

			if(ok && SDL_strncmp(ext, extension, SDL_strlen(extension))  == 0) {
				// create test suite reference
				reference = (TestSuiteReference *) SDL_malloc(sizeof(TestSuiteReference));
				if(reference == NULL) {
					goto error;
				}

				memset(reference, 0, sizeof(TestSuiteReference));

				const Uint32 dirSize = SDL_strlen(directoryName);
				const Uint32 extSize = SDL_strlen(ext);
				const Uint32 nameSize = SDL_strlen(name) + 1;

				// copy the name
				reference->name = SDL_malloc(nameSize * sizeof(char));
				if(reference->name == NULL) {
					goto error;
				}

				SDL_snprintf(reference->name, nameSize, "%s", name);

				// copy the directory path
				const Uint32 dpSize = dirSize + nameSize + 1 + extSize + 1;
				reference->directoryPath = SDL_malloc(dpSize * sizeof(char));
				if(reference->directoryPath == NULL) {
					goto error;
				}

				SDL_snprintf(reference->directoryPath, dpSize, "%s%s.%s",
						directoryName, name, ext);

				reference->next = suites;
				suites = reference;
			}
		}
	}

	goto finished;

	error:
	if(reference) {
		SDL_free(reference->name);
		SDL_free(reference->directoryPath);
		SDL_free(reference);
	}

	// Unload all the suites that are loaded thus far
	UnloadTestSuites(suites);
	suites = NULL;

	finished:
	if(directory) {
		closedir(directory);
	}

	return suites;
}


/*!
 * Loads test suite which is implemented as dynamic library.
 *
 * \param suite Reference to test suite that'll be loaded
 *
 * \return Pointer to loaded test suite, or NULL if library could not be loaded
 */
void *
LoadTestSuite(const TestSuiteReference *suite)
{
	void *library = SDL_LoadObject(suite->directoryPath);
	if(library == NULL) {
		fprintf(stderr, "Loading %s failed\n", suite->name);
		fprintf(stderr, "%s\n", SDL_GetError());
	}

	return library;
}


/*!
 * Goes through all the given TestSuiteReferences
 * and loads the dynamic libraries. Updates the suites
 * parameter on-the-fly and returns it.
 *
 * \param suites Suites that will be loaded
 *
 * \return Updated TestSuiteReferences with pointer to loaded libraries
 */
TestSuiteReference *
LoadTestSuites(TestSuiteReference *suites)
{
	TestSuiteReference *reference = NULL;
	for(reference = suites; reference; reference = reference->next) {
		reference->library = LoadTestSuite(reference);
	}

	return suites;
}


/*!
 * Unloads the given TestSuiteReferences. Frees all
 * the allocated resources including the dynamic libraries.
 *
 * \param suites TestSuiteReferences for deallocation process
 */
void
UnloadTestSuites(TestSuiteReference *suites)
{
	TestSuiteReference *ref = suites;
	while(ref) {
		if(ref->name)
			SDL_free(ref->name);

		if(ref->directoryPath)
			SDL_free(ref->directoryPath);

		if(ref->library)
			SDL_UnloadObject(ref->library);

		TestSuiteReference *temp = ref->next;
		SDL_free(ref);
		ref = temp;
	}

	suites = NULL;
}


/*!
 * Goes through the previously loaded test suites and
 * loads test cases from them. Test cases are filtered
 * during the process. Function will only return the
 * test cases which aren't filtered out.
 *
 * \param suites previously loaded test suites
 *
 * \return Test cases that survived filtering process.
 */
TestCase *
LoadTestCases(TestSuiteReference *suites)
{
	TestCase *testCases = NULL;

	TestSuiteReference *suiteReference = NULL;
	for(suiteReference = suites; suiteReference; suiteReference = suiteReference->next) {
		TestCaseReference **tests = QueryTestCaseReferences(suiteReference->library);

		TestCaseReference *testReference = NULL;
		int counter = 0;
 		for(testReference = tests[counter]; testReference; testReference = tests[++counter]) {
			//void *suite = suiteReference->library;

			// Load test case functions
			InitTestInvironmentFp initTestEnvironment = LoadInitTestInvironmentFunction(suiteReference->library);
			QuitTestInvironmentFp quitTestEnvironment = LoadQuitTestInvironmentFunction(suiteReference->library);

			TestCaseSetUpFp testSetUp = LoadTestSetUpFunction(suiteReference->library);
			TestCaseTearDownFp testTearDown = LoadTestTearDownFunction(suiteReference->library);

			TestCaseFp testCase = LoadTestCaseFunction(suiteReference->library, testReference->name);

			CountFailedAssertsFp countFailedAsserts = LoadCountFailedAssertsFunction(suiteReference->library);

			// Do the filtering
			if(FilterTestCase(testReference)) {
				TestCase *item = SDL_malloc(sizeof(TestCase));
				memset(item, 0, sizeof(TestCase));

				item->initTestEnvironment = initTestEnvironment;
				item->quitTestEnvironment = quitTestEnvironment;

				item->testSetUp = testSetUp;
				item->testTearDown = testTearDown;

				item->testCase = testCase;

				item->countFailedAsserts = countFailedAsserts;

				// copy suite name
				int length = SDL_strlen(suiteReference->name) + 1;
				item->suiteName = SDL_malloc(length);
				if(item->suiteName == NULL) {
					SDL_free(item);
					return NULL;
				}
				strncpy(item->suiteName, suiteReference->name, length);

				// copy test name
				length = SDL_strlen(testReference->name) + 1;
				item->testName = SDL_malloc(length);
				if(item->testName == NULL) {
						SDL_free(item->suiteName);
						SDL_free(item);
						return NULL;
				}
				strncpy(item->testName, testReference->name, length);

				// copy test description
				length = SDL_strlen(testReference->description) + 1;
				item->description = SDL_malloc(length);
				if(item->description == NULL) {
						SDL_free(item->description);
						SDL_free(item->suiteName);
						SDL_free(item);
						return NULL;
				}
				strncpy(item->description, testReference->description, length);

				item->requirements = testReference->requirements;
				item->timeout = testReference->timeout;

				// prepend the list
				item->next = testCases;
				testCases = item;

				//printf("Added test: %s\n", testReference->name);
			}
		}
	}

	return testCases;
}


/*!
 * Unloads the given TestCases. Frees all the resources
 * allocated for test cases.
 *
 * \param testCases Test cases to be deallocated
 */
void
UnloadTestCases(TestCase *testCases)
{
	TestCase *ref = testCases;
	while(ref) {
		if(ref->testName)
			SDL_free(ref->testName);

		if(ref->suiteName)
			SDL_free(ref->suiteName);

		if(ref->description)
			SDL_free(ref->description);

		TestCase *temp = ref->next;
		SDL_free(ref);
		ref = temp;
	}

	testCases = NULL;
}


/*!
 * Filters a test case based on its properties in TestCaseReference and user
 * preference.
 *
 * \return Non-zero means test will be added to execution list, zero means opposite
 */
int
FilterTestCase(TestCaseReference *testReference)
{
	int retVal = 1;

	if(testReference->enabled == TEST_DISABLED) {
		retVal = 0;
	}

	if(only_selected_test) {
		if(SDL_strncmp(testReference->name, selected_test_name, NAME_BUFFER_SIZE) == 0) {
			retVal = 1;
		} else {
			retVal = 0;
		}
	}

	if(only_tests_with_string) {
		if(strstr(testReference->name, testcase_name_substring) != NULL) {
			retVal = 1;
		} else {
			retVal = 0;
		}
	}

	return retVal;
}


/*!
 * Loads the test case references from the given test suite.

 * \param library Previously loaded dynamic library AKA test suite
 * \return Pointer to array of TestCaseReferences or NULL if function failed
 */
TestCaseReference **
QueryTestCaseReferences(void *library)
{
	TestCaseReference **(*suite)(void);

	suite = (TestCaseReference **(*)(void)) SDL_LoadFunction(library, "QueryTestSuite");
	if(suite == NULL) {
		fprintf(stderr, "Loading QueryTestCaseReferences() failed.\n");
		fprintf(stderr, "%s\n", SDL_GetError());
	}

	TestCaseReference **tests = suite();
	if(tests == NULL) {
		fprintf(stderr, "Failed to load test references.\n");
		fprintf(stderr, "%s\n", SDL_GetError());
	}

	return tests;
}


/*!
 * Loads test case from a test suite
 *
 * \param suite a test suite
 * \param testName Name of the test that is going to be loaded
 *
 * \return Function Pointer (TestCase) to loaded test case, NULL if function failed
 */
TestCaseFp
LoadTestCaseFunction(void *suite, char *testName)
{
	TestCaseFp test = (TestCaseFp) SDL_LoadFunction(suite, testName);
	if(test == NULL) {
		fprintf(stderr, "Loading test %s failed, test == NULL\n", testName);
		fprintf(stderr, "%s\n", SDL_GetError());
	}

	return test;
}


/*!
 * Loads function that sets up a fixture for a test case. Note: if there's
 * no SetUp function present in the suite the function will return NULL.
 *
 * \param suite Used test suite
 *
 * \return Function pointer to test case's set up function
 */
TestCaseSetUpFp
LoadTestSetUpFunction(void *suite) {
	return (TestCaseSetUpFp) SDL_LoadFunction(suite, "SetUp");
}


/*!
 * Loads function that tears down a fixture for a test case. Note: if there's
 * no TearDown function present in the suite the function will return NULL.
 *
 * \param suite Used test suite
 *
 * \return Function pointer to test case's tear down function
 */
TestCaseTearDownFp
LoadTestTearDownFunction(void *suite) {
	return (TestCaseTearDownFp) SDL_LoadFunction(suite, "TearDown");
}


/*!
 * Loads function that initialises the test environment for
 * a test case in the given suite.
 *
 * \param suite Used test suite
 *
 * \return Function pointer (InitTestInvironmentFp) which points to loaded init function. NULL if function fails.
 */
InitTestInvironmentFp
LoadInitTestInvironmentFunction(void *suite) {
	InitTestInvironmentFp testEnvInit = (InitTestInvironmentFp) SDL_LoadFunction(suite, "_InitTestEnvironment");
	if(testEnvInit == NULL) {
		fprintf(stderr, "Loading _InitTestInvironment function failed, testEnvInit == NULL\n");
		fprintf(stderr, "%s\n", SDL_GetError());
	}

	return testEnvInit;
}


/*!
 * Loads function that deinitialises the test environment (and returns
 * the test case's result) created for the test case in the given suite.
 *
 * \param suite Used test suite
 *
 * \return Function pointer (QuitTestInvironmentFp) which points to loaded init function. NULL if function fails.
 */
QuitTestInvironmentFp
LoadQuitTestInvironmentFunction(void *suite) {
	QuitTestInvironmentFp testEnvQuit = (QuitTestInvironmentFp) SDL_LoadFunction(suite, "_QuitTestEnvironment");
	if(testEnvQuit == NULL) {
		fprintf(stderr, "Loading _QuitTestEnvironment function failed, testEnvQuit == NULL\n");
		fprintf(stderr, "%s\n", SDL_GetError());
	}

	return testEnvQuit;
}


/*!
 * Loads function that returns failed assert count in the current
 * test environment
 *
 * \param suite Used test suite
 *
 * \return Function pointer to _CountFailedAsserts function
 */
CountFailedAssertsFp
LoadCountFailedAssertsFunction(void *suite) {
	CountFailedAssertsFp countFailedAssert = (CountFailedAssertsFp) SDL_LoadFunction(suite, "_CountFailedAsserts");
	if(countFailedAssert == NULL) {
		fprintf(stderr, "Loading _CountFailedAsserts function failed, countFailedAssert == NULL\n");
		fprintf(stderr, "%s\n", SDL_GetError());
	}

	return countFailedAssert;
}

#define USE_SDL_TIMER_FOR_TIMEOUT

/*!
 * Set timeout for test.
 *
 * \param timeout Timeout interval in seconds!
 * \param callback Function that will be called after timeout has elapsed
 */
void
SetTestTimeout(int timeout, void (*callback)(int))
{
	if(callback == NULL) {
		fprintf(stderr, "Error: timeout callback can't be NULL");
	}

	if(timeout < 0) {
		fprintf(stderr, "Error: timeout value must be bigger than zero.");
	}

	int tm = (timeout > universal_timeout ? timeout : universal_timeout);

#ifdef USE_SDL_TIMER_FOR_TIMEOUT
	/* Init SDL timer if not initialized before */
	if(SDL_WasInit(SDL_INIT_TIMER) == 0) {
		if(SDL_InitSubSystem(SDL_INIT_TIMER)) {
			fprintf(stderr, "Error: Failed to init timer subsystem");
			fprintf(stderr, "%s\n", SDL_GetError());
		}
	}

	/* Note:
	 * SDL_Init(SDL_INIT_TIMER) should be successfully called before using this
	 */
	int timeoutInMilliseconds = tm * 1000;

	SDL_TimerID timerID = SDL_AddTimer(timeoutInMilliseconds, (SDL_TimerCallback) callback, 0x0);
	if(timerID == 0) {
		fprintf(stderr, "Error: Creation of SDL timer failed.\n");
		fprintf(stderr, "Error: %s\n", SDL_GetError());
	}
#else
	signal(SIGALRM, callback);
	alarm((unsigned int) tm);
#endif
}


/*!
 * Kills test that hungs. Test hungs when its execution
 * takes longer than timeout specified for it.
 *
 * When test will be killed SIG_ALRM will be triggered and
 * it'll call this function which kills the test process.
 *
 * Note: if runner is executed with --in-proc then hung tests
 * can't be killed
 *
 * \param signum
 */
void
KillHungTestInChildProcess(int signum)
{
	(void)signum; // keeps the compiler silent about unused variable

	exit(TEST_RESULT_KILLED);
}

/*!
 * Checks if given test case can be executed on the current platform.
 *
 * \param testCase Test to be checked
 * \returns 1 if test is runnable, otherwise 0. On error returns -1
 */
int
CheckTestRequirements(TestCase *testCase)
{
	int retVal = 1;

	if(testCase == NULL) {
		fprintf(stderr, "TestCase parameter can't be NULL");
		return -1;
	}

	if(testCase->requirements & TEST_REQUIRES_AUDIO) {
		retVal = PlatformSupportsAudio();
	}

	if(testCase->requirements & TEST_REQUIRES_STDIO) {
		retVal = PlatformSupportsStdio();
	}


	return retVal;
}


/*
 * Execute a test. Loads the test, executes it and
 * returns the tests return value to the caller.
 *
 * \param testItem Test to be executed
 * \param test result
 */
int
RunTest(TestCase *testCase, Uint64 execKey)
{
	if(!testCase) {
		return -1;
	}

	int runnable = CheckTestRequirements(testCase);
	if(runnable != 1) {
		return TEST_RESULT_SKIPPED;
	}

	if(testCase->timeout > 0 || universal_timeout > 0) {
		if(execute_inproc) {
			Log(time(0), "Test asked for timeout which is not supported.");
		}
		else {
			SetTestTimeout(testCase->timeout, KillHungTestInChildProcess);
		}
	}

	testCase->initTestEnvironment(execKey, execute_inproc);

	if(testCase->testSetUp) {
		testCase->testSetUp(0x0);
	}

	int cntFailedAsserts = testCase->countFailedAsserts();
	if(cntFailedAsserts != 0) {
		return TEST_RESULT_SETUP_FAILURE;
	}

	testCase->testCase(0x0);

	if(testCase->testTearDown) {
		testCase->testTearDown(0x0);
	}

	return testCase->quitTestEnvironment();
}


/*!
 * Sets up a test case. Decideds wheter the test will
 * be executed in-proc or out-of-proc.
 *
 * \param testItem The test case that will be executed
 * \param execKey Execution key for the test case
 *
 * \return The return value of the test. Zero means success, non-zero failure.
 */
int
ExecuteTest(TestCase *testItem, Uint64 execKey) {
	int retVal = -1;

	if(execute_inproc) {
		retVal = RunTest(testItem, execKey);
	} else {
		int childpid = fork();
		if(childpid == 0) {
			exit(RunTest(testItem, execKey));
		} else {
			int stat_lock = -1;
			wait(&stat_lock);

			retVal = HandleChildProcessReturnValue(stat_lock);
		}
	}

	if(retVal == TEST_RESULT_SKIPPED) {
		testSkipCount++;
		totalTestSkipCount++;
	}
	else if(retVal) {
		totalTestFailureCount++;
		testFailureCount++;
	}
	else {
		totalTestPassCount++;
		testPassCount++;
	}

	// return the value for logger
	return retVal;
}


/*!
 * If using out-of-proc execution of tests. This function
 * will handle the return value of the child process
 * and interprets it to the runner. Also prints warnings
 * if child was aborted by a signela.
 *
 * \param stat_lock information about the exited child process
 *
 * \return 0 if test case succeeded, 1 otherwise
 */
int
HandleChildProcessReturnValue(int stat_lock)
{
	int returnValue = -1;

	if(WIFEXITED(stat_lock)) {
		returnValue = WEXITSTATUS(stat_lock);
	} else if(WIFSIGNALED(stat_lock)) {
		int signal = WTERMSIG(stat_lock);
		// \todo add this to logger (add signal number)
		Log(time(0), "FAILURE: test was aborted due to  %d\n", signal);
		returnValue = 1;
	}

	return returnValue;
}


/*!
 * Generates a random run seed for the harness. Seed
 * can contain characters 0-9A-Z
 *
 * \param length The length of the generated seed
 *
 * \returns The generated seed
 */
char *
GenerateRunSeed(const int length)
{
	if(length <= 0) {
		fprintf(stderr, "Error: length of the harness seed can't be less than zero\n");
		return NULL;
	}

	char *seed = SDL_malloc((length + 1) * sizeof(char));
	if(seed == NULL) {
		fprintf(stderr, "Error: malloc for run seed failed\n");
		return NULL;
	}

	RND_CTX randomContext;

	utl_randomInitTime(&randomContext);

	int counter = 0;
	for( ; counter < length - 1; ++counter) {
		int number = abs(utl_random(&randomContext));
		char ch = (char) (number % (91 - 48)) + 48;

		if(ch >= 58 && ch <= 64) {
			ch = 65;
		}

		seed[counter] = ch;
	}

	seed[counter] = '\0';

	return seed;
}

/*!
 * Sets up the logger.
 *
 * \return Logger data structure (that needs be deallocated)
 */
LoggerData *
SetUpLogger(const int log_stdout_enabled, const int xml_enabled, const int xsl_enabled,
			const int custom_xsl_enabled, const char *defaultXslSheet, const time_t timestamp)
{
	LoggerData *loggerData = SDL_malloc(sizeof(LoggerData));
	if(loggerData == NULL) {
		fprintf(stderr, "Error: Logger data structure not allocated.");
		return NULL;
	}
	memset(loggerData, 0, sizeof(LoggerData));

	loggerData->level = (enable_verbose_logger ? LOGGER_VERBOSE : LOGGER_TERSE);

	if(log_stdout_enabled == 1) {
		loggerData->stdoutEnabled = 1;
		loggerData->filename = NULL;
	} else {
		loggerData->stdoutEnabled = 0;

		const char *extension = (xml_enabled ? "xml" : "log");

		// create directory (if it doesn't exist yet)
		unsigned int mode = S_IRWXU | S_IRGRP | S_ISUID;
		mkdir(log_directory, mode);

		char *timeString = TimestampToStringWithFormat(timestamp, "%Y%m%d_%H:%M:%S");


		/* Combine and create directory for log file */
		const Uint32 directoryLength = SDL_strlen(log_directory);
		const Uint32 basenameLength = SDL_strlen(log_basename);
		const Uint32 seedLength = SDL_strlen(runSeed);
		const Uint32 extensionLength = SDL_strlen(extension);

		const Uint32 timeLength = SDL_strlen(timeString);

		// couple of extras bytes for '/', '-', '.' and '\0' at the end
		const Uint32 length = directoryLength + basenameLength + seedLength
							+ extensionLength + timeLength + 5;

		if(length <= 0) {
			return NULL;
		}

		char *filename = SDL_malloc(length);
		if(filename == NULL) {
			SDL_free(loggerData);

			fprintf(stderr, "Error: Failed to allocate memory for filename of log");
			return NULL;
		}
		memset(filename, 0, length);

		SDL_snprintf(filename, length, "%s%c%s-%s-%s.%s", log_directory,
				DIRECTORY_SEPARATOR, log_basename, timeString, runSeed, extension);

		loggerData->filename = filename;
	}

	if(xml_enabled) {
		char *sheet = NULL;
		if(xsl_enabled) {
			sheet = (char *) defaultXslSheet;
		}

		if(custom_xsl_enabled) {
			sheet = xsl_stylesheet_name;
		}

		loggerData->custom = sheet;
	}

	if(xml_enabled) {
		RunStarted = XMLRunStarted;
		RunEnded = XMLRunEnded;

		SuiteStarted = XMLSuiteStarted;
		SuiteEnded = XMLSuiteEnded;

		TestStarted = XMLTestStarted;
		TestEnded = XMLTestEnded;

		Assert = XMLAssert;
		AssertWithValues = XMLAssertWithValues;
		AssertSummary = XMLAssertSummary;

		Log = XMLLog;
	} else {
		RunStarted = PlainRunStarted;
		RunEnded = PlainRunEnded;

		SuiteStarted = PlainSuiteStarted;
		SuiteEnded = PlainSuiteEnded;

		TestStarted = PlainTestStarted;
		TestEnded = PlainTestEnded;

		Assert = PlainAssert;
		AssertWithValues = PlainAssertWithValues;
		AssertSummary = PlainAssertSummary;

		Log = PlainLog;
	}

	return loggerData;
}


/*!
 * Prints usage information
 */
void
PrintUsage() {
	  printf("Usage: ./runner [--in-proc] [--show-tests] [--verbose]\n");
	  printf("                [--logfile [BASENAME]] [--logdir DIR] [--xml]\n");
	  printf("                [--xsl [STYLESHEET]] [--seed VALUE] [--iterations VALUE]\n");
	  printf("                [--exec-key KEY] [--timeout VALUE] [--test TEST]\n");
	  printf("                [--name-contains SUBSTR] [--suite SUITE] [--include-dummy]\n");
	  printf("                [--version] [--help]\n");
	  printf("Options:\n");
	  printf("     --in-proc                Executes tests in-process\n");
	  printf("     --show-tests             Prints out all the executable tests\n");
	  printf(" -v  --verbose                Enables verbose logging\n");
	  printf("     --logfile [BASENAME]     Enables logging to a file. If BASENAME is\n");
	  printf("                              specified it'll be used as  basename for\n");
	  printf("                              the log file\n");
	  printf("     --logdir DIR             Define directory for logs. Defaults to 'logs'\n");
	  printf("     --xml                    Enables XML logger\n");
	  printf("     --xsl [STYLESHEET]       Adds XSL stylesheet to the XML test reports for\n");
	  printf("                              browser viewing. Optionally uses the specified XSL\n");
	  printf("                              file or URL instead of the default one\n");
	  printf("     --seed VALUE             Specify fuzzing seed for the harness\n");
	  printf("     --iterations VALUE       Specify how many times a test will be executed\n");
	  printf("     --exec-key KEY           Run test(s) with specific execution key\n");
	  printf(" -tm --timeout VALUE          Specify common timeout value for all tests\n");
	  printf("                              Timeout is given in seconds and it'll override\n");
	  printf("                              test specific timeout value only if the given\n");
	  printf("                              value is greater than the test specific value\n");
	  printf("                              note: doesn't work with --in-proc option.\n");
	  printf(" -t  --test TEST              Executes only tests with given name\n");
	  printf(" -ts --name-contains SUBSTR   Executes only tests that have given\n");
	  printf("                              substring in test name\n");
	  printf(" -s  --suite SUITE            Executes only the given test suite\n");
	  printf("     --include-dummy          Includes dummy test suite in the test run\n");

	  printf("     --version                Print version information\n");
	  printf(" -h  --help                   Print this help\n");
}


/*!
 * Parse command line arguments
 *
 * \param argc Count of command line arguments
 * \param argv Array of commond lines arguments
 */
void
ParseOptions(int argc, char *argv[])
{
   int i;

   for (i = 1; i < argc; ++i) {
      const char *arg = argv[i];
      if(SDL_strcmp(arg, "--in-proc") == 0) {
         execute_inproc = 1;
      }
      else if(SDL_strcmp(arg, "--show-tests") == 0) {
    	  only_print_tests = 1;
      }
      else if(SDL_strcmp(arg, "--xml") == 0) {
    	  xml_enabled = 1;
      }
      else if(SDL_strcmp(arg, "--verbose") == 0 || SDL_strcmp(arg, "-v") == 0) {
    	  enable_verbose_logger = 1;
      }
      else if(SDL_strcmp(arg, "--logdir") == 0) {
    	  char *dirString = NULL;

    	  if( (i + 1) < argc && argv[i+1][0] != '-')  {
    		  dirString = argv[++i];
    	  }  else {
    		  printf("runner: dir is missing\n");
    		  PrintUsage();
    		  exit(1);
    	  }

    	  memset(log_directory, 0, NAME_BUFFER_SIZE);
    	  memcpy(log_directory, dirString, SDL_strlen(dirString));

    	  log_stdout_enabled = 0;
      }
      else if(SDL_strcmp(arg, "--logfile") == 0) {
		  char *fileString = NULL;

		  if( (i + 1) < argc && argv[i+1][0] != '-')  {
			  fileString = argv[++i];
		  }  else {
			  fileString = DEFAULT_LOG_FILENAME;
		  }

   	    memset(log_basename, 0, NAME_BUFFER_SIZE);
		memcpy(log_basename, fileString, SDL_strlen(fileString));

		log_stdout_enabled = 0;
      }
      else if(SDL_strcmp(arg, "--timeout") == 0 || SDL_strcmp(arg, "-tm") == 0) {
    	  universal_timeout_enabled = 1;
    	  char *timeoutString = NULL;

    	  if( (i + 1) < argc && argv[i+1][0] != '-')  {
    		  timeoutString = argv[++i];
    	  }  else {
    		  printf("runner: timeout is missing\n");
    		  PrintUsage();
    		  exit(1);
    	  }

    	  universal_timeout = atoi(timeoutString);
      }
      else if(SDL_strcmp(arg, "--seed") == 0) {
    	  userRunSeed = 1;

    	  if( (i + 1) < argc && argv[i+1][0] != '-')  {
    		  runSeed = argv[++i];
    	  }  else {
    		  printf("runner: seed value is missing\n");
    		  PrintUsage();
    		  exit(1);
    	  }
    	  //!Ê\todo should the seed be copied to a buffer?
      }
      else if(SDL_strcmp(arg, "--iterations") == 0) {
    	  char *iterationsString = NULL;
    	  if( (i + 1) < argc && argv[i+1][0] != '-')  {
    		  iterationsString = argv[++i];
    	  }  else {
    		  printf("runner: iterations value is missing\n");
    		  PrintUsage();
    		  exit(1);
    	  }

    	  testInvocationCount = atoi(iterationsString);
    	  if(testInvocationCount < 1) {
    		  printf("Iteration value has to bigger than 0.\n");
    		  exit(1);
    	  }
      }
      else if(SDL_strcmp(arg, "--exec-key") == 0) {
    	  char *execKeyString = NULL;
    	  if( (i + 1) < argc && argv[i+1][0] != '-')  {
    		  execKeyString = argv[++i];
    	  }  else {
    		  printf("runner: execkey value is missing\n");
    		  PrintUsage();
    		  exit(1);
    	  }

    	  int ret = sscanf(execKeyString, "%llx", &userExecKey);
    	  if(ret != 1) {
    		  fprintf(stderr, "Error: Failed to parse exec-key option");
    		  exit(1);
    	  }
      }
      else if(SDL_strcmp(arg, "--test") == 0 || SDL_strcmp(arg, "-t") == 0) {
    	  only_selected_test = 1;
    	  char *testName = NULL;

    	  if( (i + 1) < argc && argv[i+1][0] != '-')  {
    		  testName = argv[++i];
    	  }  else {
    		  printf("runner: test name is missing\n");
    		  PrintUsage();
    		  exit(1);
    	  }

    	  memset(selected_test_name, 0, NAME_BUFFER_SIZE);
    	  strncpy(selected_test_name, testName, NAME_BUFFER_SIZE);
      }
      else if(SDL_strcmp(arg, "--xsl") == 0) {
    	  xsl_enabled = 1;

    	  if( (i + 1) < argc && argv[i+1][0] != '-')  {
    		  char *stylesheet = argv[++i];
    		  if(stylesheet[0] != '-') {
    	    	  custom_xsl_enabled = 1;

    	    	  memset(xsl_stylesheet_name, 0, NAME_BUFFER_SIZE);
    	    	  strncpy(xsl_stylesheet_name, stylesheet, NAME_BUFFER_SIZE);
    		  }
    	  }
      }
      else if(SDL_strcmp(arg, "--name-contains") == 0 || SDL_strcmp(arg, "-ts") == 0) {
    	  only_tests_with_string = 1;
    	  char *substring = NULL;

    	  if( (i + 1) < argc && argv[i+1][0] != '-')  {
    		  substring = argv[++i];
    	  }  else {
    		  printf("runner: substring of test name is missing\n");
    		  PrintUsage();
    		  exit(1);
    	  }

    	  memset(testcase_name_substring, 0, NAME_BUFFER_SIZE);
    	  strncpy(testcase_name_substring, strdup(substring), NAME_BUFFER_SIZE);
      }
      else if(SDL_strcmp(arg, "--suite") == 0 || SDL_strcmp(arg, "-s") == 0) {
    	  only_selected_suite = 1;

    	  char *suiteName = NULL;
    	  if( (i + 1) < argc && argv[i+1][0] != '-')  {
    		  suiteName = argv[++i];
    	  }  else {
    		  printf("runner: suite name is missing\n");
    		  PrintUsage();
    		  exit(1);
    	  }

    	  memset(selected_suite_name, 0, NAME_BUFFER_SIZE);
    	  strcpy(selected_suite_name, suiteName);
      }
      else if(SDL_strcmp(arg, "--include-dummy") == 0) {
    	  include_dummy_suite = 1;
      }
      else if(SDL_strcmp(arg, "--version") == 0) {
    	  fprintf(stdout, "SDL test harness (version %s)\n", PACKAGE_VERSION);

     	  // print: Testing against SDL version fuu (rev: bar)

    	  exit(0);
      }
      else if(SDL_strcmp(arg, "--help") == 0 || SDL_strcmp(arg, "-h") == 0) {
    	  PrintUsage();
    	  exit(0);
      }
      else {
    	  printf("runner: unknown command '%s'\n", arg);
    	  PrintUsage();
    	  exit(0);
      }
   }
}

void
InitRunner()
{
	// Inits some global buffers to their default values
	memcpy(log_basename, (void *)DEFAULT_LOG_FILENAME, SDL_strlen(DEFAULT_LOG_FILENAME));
	memcpy(log_directory, (void *)DEFAULT_LOG_DIRECTORY, SDL_strlen(DEFAULT_LOG_DIRECTORY));

	memset(selected_test_name, 0, NAME_BUFFER_SIZE);
	memset(selected_suite_name, 0, NAME_BUFFER_SIZE);
	memset(testcase_name_substring, 0, NAME_BUFFER_SIZE);
	memset(xsl_stylesheet_name, 0, NAME_BUFFER_SIZE);
}


/*!
 * Entry point for test runner
 *
 * \param argc Count of command line arguments
 * \param argv Array of commond lines arguments
 */
int
main(int argc, char *argv[])
{
	InitRunner();

	ParseOptions(argc, argv);

	int suiteCounter = 0;

#if defined(linux) || defined( __linux)
	char *extension = "so";
#else
	char *extension = "dylib";
#endif

	const Uint32 startTicks = SDL_GetTicks();

	TestSuiteReference *suites = ScanForTestSuites(DEFAULT_TEST_DIRECTORY, extension);
	if(suites == NULL) {
		fprintf(stderr, "No test suites loaded.\n");
		fprintf(stderr, "Compile you suites and install them to tests/\n");
		return 2;
	}
	suites = LoadTestSuites(suites);

	TestCase *testCases = LoadTestCases(suites);
	if(testCases == NULL) {
		fprintf(stderr, "Found zero test cases\n");
		fprintf(stderr, "Check out your command line options\n");
		return 2;
	}

	// if --show-tests option is given, only print tests and exit
	if(only_print_tests) {
		TestCase *testItem = NULL;
		char *lastSuiteName = NULL;
		for(testItem = testCases; testItem; testItem = testItem->next) {
		        if ((lastSuiteName == NULL) || (strcmp(lastSuiteName, testItem->suiteName)!=0)) {
		                lastSuiteName = testItem->suiteName;
		                printf ("%s:\n", lastSuiteName);
		        }
			printf("  %s: %s", testItem->testName, testItem->description);
			if (testItem->timeout>0) {
			        printf (" (timeout: %i sec)", testItem->timeout);
			}
			printf ("\n");
		}

		return 0;
	}

	if(userRunSeed == 0) {
		runSeed = GenerateRunSeed(16);
		if(runSeed == NULL) {
			fprintf(stderr, "Error: Generating harness seed failed\n");
			return 2;
		}
	}

	const time_t startTimestamp = time(0);

	LoggerData *loggerData = SetUpLogger(log_stdout_enabled, xml_enabled,
			xsl_enabled, custom_xsl_enabled, defaultXSLStylesheet, startTimestamp);
	if(loggerData == NULL) {
		printf("Failed to create a logger.\n");
		return 2;
	}

	if(log_stdout_enabled == 0) {
		printf("Runner is executing the tests.\n");
		if(enable_verbose_logger) {
			printf("Logging level is set to verbose.\n");
		}
		printf("Test report is created to: %s\n", loggerData->filename);
		fflush(stdout);
	}

	RunStarted(argc, argv, runSeed, startTimestamp, loggerData);

	// logger data is no longer used so free it
	SDL_free(loggerData->filename);
	SDL_free(loggerData);


	// validate the parsed command options
	// \todo add a lot more and refactor. There are many more combinations
	//       of commands that doesn't make sense together
	if(execute_inproc && universal_timeout_enabled) {
		Log(time(0), "Test timeout is not supported with in-proc execution.");
		Log(time(0), "Timeout will be disabled...");

		universal_timeout_enabled = 0;
		universal_timeout = -1;
	} /*
	if(userExecKey && testInvocationCount > 1 || userRunSeed) {
		printf("The given combination of command line options doesn't make sense\n");
		printf("--exec-key should only be used to rerun failed fuzz tests\n");
	}*/

	char *currentSuiteName = NULL;
	int suiteStartTime = SDL_GetTicks();

	int notFirstSuite = 0;
	int startNewSuite = 1;
	TestCase *testItem = NULL;
	for(testItem = testCases; testItem; testItem = testItem->next) {
		if(currentSuiteName && strncmp(currentSuiteName, testItem->suiteName, NAME_BUFFER_SIZE) != 0) {
			startNewSuite = 1;
		}

		if(startNewSuite) {
			if(notFirstSuite) {
				const double suiteRuntime = (SDL_GetTicks() - suiteStartTime) / 1000.0f;

				SuiteEnded(testPassCount, testFailureCount, testSkipCount, time(0),
							suiteRuntime);
			}

			suiteStartTime = SDL_GetTicks();

			currentSuiteName = testItem->suiteName;
			SuiteStarted(currentSuiteName, time(0));
			testFailureCount = testPassCount = testSkipCount = 0;
			suiteCounter++;

			startNewSuite = 0;
			notFirstSuite = 1;
		}

		int currentIteration = testInvocationCount;
		while(currentIteration > 0) {
			Uint64 execKey = 5;
			if(userExecKey != 0) {
				execKey = userExecKey;
			} else {
				execKey  = GenerateExecKey(runSeed, testItem->suiteName,
											  testItem->testName, currentIteration);
			}

			TestStarted(testItem->testName, testItem->suiteName,
						testItem->description, execKey, time(0));

			const Uint32 testTimeStart = SDL_GetTicks();

			int retVal = ExecuteTest(testItem, execKey);

			const double testTotalRuntime = (SDL_GetTicks() - testTimeStart) / 1000.0f;

			TestEnded(testItem->testName, testItem->suiteName, retVal, time(0), testTotalRuntime);

			currentIteration--;
		}
	}

	if(currentSuiteName) {
		SuiteEnded(testPassCount, testFailureCount, testSkipCount, time(0),
					(SDL_GetTicks() - suiteStartTime) / 1000.0f);
	}

	UnloadTestCases(testCases);
	UnloadTestSuites(suites);

	const Uint32 endTicks = SDL_GetTicks();
	const double totalRunTime = (endTicks - startTicks) / 1000.0f;

	RunEnded(totalTestPassCount + totalTestFailureCount + totalTestSkipCount, suiteCounter,
			 totalTestPassCount, totalTestFailureCount, totalTestSkipCount, time(0), totalRunTime);

	// Some SDL subsystem might be init'ed so shut them down
	SDL_Quit();

	return (totalTestFailureCount ? 1 : 0);
}
