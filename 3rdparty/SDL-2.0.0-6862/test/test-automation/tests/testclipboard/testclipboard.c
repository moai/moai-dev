#include <stdio.h>

#include <SDL/SDL.h>

#include "../../include/SDL_test.h"


/*!
 * Note: Add test for clipboard here
 *
 */

/* Test cases */
static const TestCaseReference test1 =
		(TestCaseReference){ "clipboard_testHasClipboardText", "Check call to SDL_HasClipboardText", TEST_ENABLED, 0, 0 };

static const TestCaseReference test2 =
		(TestCaseReference){ "clipboard_testGetClipboardText", "Check call to SDL_GetClipboardText", TEST_ENABLED, 0, 0 };

static const TestCaseReference test3 =
		(TestCaseReference){ "clipboard_testSetClipboardText", "Check call to SDL_SetClipboardText", TEST_ENABLED, 0, 0 };

static const TestCaseReference test4 =
		(TestCaseReference){ "clipboard_testClipboardTextFunctions", "End-to-end test of SDL_xyzClipboardText functions", TEST_ENABLED, 0, 0 };

/* Test suite */
extern const TestCaseReference *testSuite[] =  {
	&test1, &test2, &test3, &test4, NULL
};

TestCaseReference **QueryTestSuite() {
	return (TestCaseReference **)testSuite;
}

void
SetUp(void *arg)
{
  /* Start SDL video */
  int ret = SDL_InitSubSystem( SDL_INIT_VIDEO );
  AssertTrue(ret==0, "SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
}
                        
void
TearDown(void *arg)
{
  /* Quit SDL video */
 SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

/**
 * \brief Check call to SDL_HasClipboardText
 *
 * \sa 
 * http://wiki.libsdl.org/moin.cgi/SDL_HasClipboardText
 */
int
clipboard_testHasClipboardText(void *arg)
{
	SDL_bool result;
	result = SDL_HasClipboardText();
	AssertPass("Call to SDL_HasClipboardText succeeded");
}

/**
 * \brief Check call to SDL_GetClipboardText
 *
 * \sa 
 * http://wiki.libsdl.org/moin.cgi/SDL_GetClipboardText
 */
int
clipboard_testGetClipboardText(void *arg)
{
	char *result;
	result = SDL_GetClipboardText();
	AssertPass("Call to SDL_GetClipboardText succeeded");
}

/**
 * \brief Check call to SDL_SetClipboardText
 * \sa 
 * http://wiki.libsdl.org/moin.cgi/SDL_SetClipboardText
 */
int
clipboard_testSetClipboardText(void *arg)
{
	char *textRef = RandomAsciiString();
	char *text = strdup(textRef);
	int result;
	result = SDL_SetClipboardText((const char *)text);	
	AssertTrue(
		result == 0, 
		"Call to SDL_SetClipboardText failed with error %i: %s",
		result, SDL_GetError());
	AssertTrue(
		strcmp(textRef, text) == 0, 
		"SDL_SetClipboardText modified input string: expected %s, got %s",
		textRef, text);
		
        /* Cleanup */
        if (textRef) free(textRef);
        if (text) free(text);
}


/**
 * \brief End-to-end test of SDL_xyzClipboardText functions
 * \sa 
 * http://wiki.libsdl.org/moin.cgi/SDL_HasClipboardText
 * http://wiki.libsdl.org/moin.cgi/SDL_GetClipboardText
 * http://wiki.libsdl.org/moin.cgi/SDL_SetClipboardText
 */
int
clipboard_testClipboardTextFunctions(void *arg)
{
	char *textRef = RandomAsciiString();
	char *text = strdup(textRef);
	SDL_bool boolResult;
	int intResult;
	char *charResult;
	
	/* Clear clipboard text state */
	boolResult = SDL_HasClipboardText();
	if (boolResult == SDL_TRUE) {
             intResult = SDL_SetClipboardText((const char *)NULL);
	     AssertTrue(
		intResult == 0, 
		"Call to SDL_SetClipboardText("") failed with error %i: %s",
		intResult, SDL_GetError());
	     charResult = SDL_GetClipboardText();
	     boolResult = SDL_HasClipboardText();
	     AssertTrue(
                boolResult == SDL_FALSE,
	        "SDL_HasClipboardText returned TRUE, expected FALSE");        
        }
        
        /* Empty clipboard  */        
	charResult = SDL_GetClipboardText();
	AssertTrue(
		charResult != NULL,
		"SDL_GetClipboardText returned NULL");		
	AssertTrue(
		strlen(charResult) == 0,
		"SDL_GetClipboardText returned string with length >0: got length %i",
		strlen(charResult));
        intResult = SDL_SetClipboardText((const char *)text);	
	AssertTrue(
		intResult == 0, 
		"Call to SDL_SetClipboardText failed with error %i: %s",
		intResult, SDL_GetError());
	AssertTrue(
		strcmp(textRef, text) == 0, 
		"SDL_SetClipboardText modified input string: expected %s, got %s",
		textRef, text);
	boolResult = SDL_HasClipboardText();
	AssertTrue(
             boolResult == SDL_TRUE,
	     "SDL_HasClipboardText returned FALSE, expected TRUE");        
	charResult = SDL_GetClipboardText();
	AssertTrue(
		strcmp(textRef, charResult) == 0, 
		"SDL_GetClipboardText did not return correst string: expected %s, got %s",
		textRef, charResult);

        /* Cleanup */
        if (textRef) free(textRef);
        if (text) free(text);
        if (charResult) free(charResult);
}
