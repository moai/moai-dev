#include <stdio.h>

#include <SDL/SDL.h>

#include "../../include/SDL_test.h"



/*!
 * Note: Add test for video here
 *
 */

/* Test cases */
static const TestCaseReference test1 =
		(TestCaseReference){ "video_test", "video stuff", TEST_DISABLED, 0, 0 };

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
video_test(void *arg)
{
	AssertPass("");
}
