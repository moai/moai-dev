#include <stdio.h>

#include <SDL/SDL.h>

#include "../../include/SDL_test.h"



/*!
 * Note: Add test for keyboard here
 *
 */

/* Test cases */
static const TestCaseReference test1 =
		(TestCaseReference){ "keyboard_test", "description", TEST_DISABLED, 0, 0 };

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
keyboard_test(void *arg)
{
	AssertPass("");
}
