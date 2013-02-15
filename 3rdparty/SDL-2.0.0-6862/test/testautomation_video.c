/**
 * Video test suite
 */

#include <stdio.h>

#include "SDL.h"
#include "SDL_test.h"

/* Test case functions */

/**
 * @brief Enable and disable screensaver while checking state
 */
int
video_enableDisableScreensaver(void *arg)
{
	SDL_bool initialResult;
	SDL_bool result;

	/* Get current state and proceed according to current state */
	initialResult = SDL_IsScreenSaverEnabled();
	SDLTest_AssertPass("Call to SDL_IsScreenSaverEnabled()");	
	if (initialResult == SDL_TRUE) {
	
	  /* Currently enabled: disable first, then enable again */
	  
	  /* Disable screensaver and check */	
	  SDL_DisableScreenSaver();
	  SDLTest_AssertPass("Call to SDL_DisableScreenSaver()");	
	  result = SDL_IsScreenSaverEnabled();
	  SDLTest_AssertPass("Call to SDL_IsScreenSaverEnabled()");
	  SDLTest_AssertCheck(result == SDL_FALSE, "Verify result from SDL_IsScreenSaverEnabled, expected: %i, got: %i", SDL_FALSE, result);
	
	  /* Enable screensaver and check */	
	  SDL_EnableScreenSaver();
	  SDLTest_AssertPass("Call to SDL_EnableScreenSaver()");
	  result = SDL_IsScreenSaverEnabled();
	  SDLTest_AssertPass("Call to SDL_IsScreenSaverEnabled()");
	  SDLTest_AssertCheck(result == SDL_TRUE, "Verify result from SDL_IsScreenSaverEnabled, expected: %i, got: %i", SDL_TRUE, result);

	} else {

	  /* Currently disabled: enable first, then disable again */
	  
	  /* Enable screensaver and check */	
	  SDL_EnableScreenSaver();
	  SDLTest_AssertPass("Call to SDL_EnableScreenSaver()");
	  result = SDL_IsScreenSaverEnabled();
	  SDLTest_AssertPass("Call to SDL_IsScreenSaverEnabled()");
	  SDLTest_AssertCheck(result == SDL_TRUE, "Verify result from SDL_IsScreenSaverEnabled, expected: %i, got: %i", SDL_TRUE, result);

	  /* Disable screensaver and check */	
	  SDL_DisableScreenSaver();
	  SDLTest_AssertPass("Call to SDL_DisableScreenSaver()");	
	  result = SDL_IsScreenSaverEnabled();
	  SDLTest_AssertPass("Call to SDL_IsScreenSaverEnabled()");
	  SDLTest_AssertCheck(result == SDL_FALSE, "Verify result from SDL_IsScreenSaverEnabled, expected: %i, got: %i", SDL_FALSE, result);	
	}	
	
	return TEST_COMPLETED;
}

/* ================= Test References ================== */

/* Video test cases */
static const SDLTest_TestCaseReference videoTest1 =
		{ (SDLTest_TestCaseFp)video_enableDisableScreensaver, "video_enableDisableScreensaver",  "Enable and disable screenaver while checking state", TEST_ENABLED };

/* Sequence of Video test cases */
static const SDLTest_TestCaseReference *videoTests[] =  {
	&videoTest1, NULL
};

/* Video test suite (global) */
SDLTest_TestSuiteReference videoTestSuite = {
	"Video",
	NULL,
	videoTests,
	NULL
};
