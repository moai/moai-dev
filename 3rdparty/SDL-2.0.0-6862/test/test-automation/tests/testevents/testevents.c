#include <stdio.h>

#include <SDL/SDL.h>

#include "../../include/SDL_test.h"



/*!
 * Note: Add test for events here
 *
 */

/* Test cases */
static const TestCaseReference test1 =
		(TestCaseReference){ "events_test", "description", TEST_DISABLED, 0, 0 };

/* Test suite */
extern const TestCaseReference *testSuite[] =  {
	&test1, NULL
};

TestCaseReference **QueryTestSuite() {
	return (TestCaseReference **)testSuite;
}

/**
 * @brief Document test case here
 */
int
events_test(void *arg)
{
	AssertPass("");
}
