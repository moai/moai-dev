/**
 * Original code: automated SDL audio test written by Edgar Simo "bobbens"
 * New/updated tests: aschiffler at ferzkopp dot net
 */

#include <stdio.h>

#include <SDL/SDL.h>

#include "../../include/SDL_test.h"

/* Test cases */
static const TestCaseReference test1 =
		(TestCaseReference){ "audio_enumerateAndNameAudioDevices", "Enumerate and name available audio devices (output and capture)", TEST_ENABLED, TEST_REQUIRES_AUDIO, 0};

static const TestCaseReference test2 =
		(TestCaseReference){ "audio_enumerateAndNameAudioDevicesNegativeTests", "Netative tests around enumeration and naming of audio devices.", TEST_ENABLED, TEST_REQUIRES_AUDIO, 0};

static const TestCaseReference test3 =
		(TestCaseReference){ "audio_printAudioDrivers", "Checks available audio driver names.", TEST_ENABLED, TEST_REQUIRES_AUDIO, 0};

static const TestCaseReference test4 =
		(TestCaseReference){ "audio_printCurrentAudioDriver", "Checks current audio driver name with initialized audio.", TEST_ENABLED, TEST_REQUIRES_AUDIO, 0};

/* Test suite */
extern const TestCaseReference *testSuite[] =  {
	&test1, &test2, &test3, &test4, NULL
};


TestCaseReference **QueryTestSuite() {
	return (TestCaseReference **)testSuite;
}

/* Fixture */

void
SetUp(void *arg)
{
	/* Start SDL. */
	int ret = SDL_Init( SDL_INIT_AUDIO );
	AssertTrue(ret==0, "SDL_Init(SDL_INIT_AUDIO): %s", SDL_GetError());
}

void
TearDown(void *arg)
{
	/* Quit SDL. */
	SDL_Quit();
}

/* Test case functions */

/**
 * \brief Enumerate and name available audio devices (output and capture).
 * 
 * \sa http://wiki.libsdl.org/moin.cgi/SDL_GetNumAudioDevices
 * \sa http://wiki.libsdl.org/moin.cgi/SDL_GetAudioDeviceName
 */
int audio_enumerateAndNameAudioDevices()
{
   int ret;
   int t, tt;
   int i, n, nn;
   const char *name, *nameAgain;

   /* Iterate over types: t=0 output device, t=1 input/capture device */
   for (t=0; t<2; t++) {
   
      /* Get number of devices. */
      n = SDL_GetNumAudioDevices(t);
      AssertTrue(n>=0, 
         "Number of %s devices < 0, reported as %i: %s", 
         (t) ? "output" : "capture",
         n, 
         SDL_GetError());

      /* Variation of non-zero type */
      if (t==1) {
         tt = t + RandomIntegerInRange(1,10);
         nn = SDL_GetNumAudioDevices(tt);
         AssertTrue(n==nn, "SDL_GetNumAudioDevices(%i) : expected same number of audio devices %i, got %i", tt, n, nn);
         nn = SDL_GetNumAudioDevices(-tt);
         AssertTrue(n==nn, "SDL_GetNumAudioDevices(%i) : expected same number of audio devices %i, got %i", -tt, n, nn);
      } 
   
      /* List devices. */
      if (n>0) {
         for (i=0; i<n; i++) {
            name = SDL_GetAudioDeviceName(i, t);
            AssertTrue(name != NULL, "SDL_GetAudioDeviceName(%i, %i): returned NULL name", i, t);
            AssertTrue(strlen(name)>0, "SDL_GetAudioDeviceName(%i, %i): returned empty name string", i, t);
            if (t==1) {
               /* Also try non-zero type */
               nameAgain = SDL_GetAudioDeviceName(i, tt);
               AssertTrue(nameAgain != NULL, "SDL_GetAudioDeviceName(%i, %i): returned NULL name", i, tt);
               AssertTrue(strlen(nameAgain)>0, "SDL_GetAudioDeviceName(%i, %i): returned empty name string", i, tt);
               AssertTrue(strcmp(name, nameAgain)==0, 
                  "SDL_GetAudioDeviceName(%i, %i): returned unexpected name string %s, expected %s", 
                  i, tt, nameAgain, name);
            }
         }
      }
   }
}

/**
 * \brief Negative tests around enumeration and naming of audio devices.
 * 
 * \sa http://wiki.libsdl.org/moin.cgi/SDL_GetNumAudioDevices
 * \sa http://wiki.libsdl.org/moin.cgi/SDL_GetAudioDeviceName
 */
int audio_enumerateAndNameAudioDevicesNegativeTests()
{
   int ret;
   int t;
   int i, j, no, nc;
   const char *name;
      
   /* Get number of devices. */
   no = SDL_GetNumAudioDevices(0);
   nc = SDL_GetNumAudioDevices(1);
   
   /* Invalid device index when getting name */
   for (t=0; t<2; t++) {
      /* Negative device index */
      i = -1;
      name = SDL_GetAudioDeviceName(i, t);
      AssertTrue(name == NULL, "SDL_GetAudioDeviceName(%i, %i): returned a name, should return NULL", i, t);
      
      /* Device index past range */
      for (j=0; j<3; j++) {
         i = (t) ? nc+j : no+j;
         name = SDL_GetAudioDeviceName(i, t);
         AssertTrue(name == NULL, "SDL_GetAudioDeviceName(%i, %i): returned a name, should return NULL", i, t);
      }
      
      /* Capture index past capture range but within output range */
      if ((no>0) && (no>nc) && (t==1)) {
         i = no-1;
         name = SDL_GetAudioDeviceName(i, t);
         AssertTrue(name == NULL, "SDL_GetAudioDeviceName(%i, %i): returned a name, should return NULL", i, t);
      }
   }
}

/**
 * @brief Checks available audio driver names.
 */
int audio_printAudioDrivers()
{
   int i, n;
   const char *name;

   /* Get number of drivers */
   n = SDL_GetNumAudioDrivers();
   AssertTrue(n>=0, "Number of audio drivers >= 0");
   
   /* List drivers. */
   if (n>0)
   {
      for (i=0; i<n; i++) {
         name = SDL_GetAudioDriver(i);
         AssertTrue(name != NULL, "name != NULL");
         AssertTrue(strlen(name)>0, "name empty");
      }
   }
}

/**
 * @brief Checks current audio driver name with initialized audio.
 */
int audio_printCurrentAudioDriver()
{
   int ret;
   const char *name;

   /* Check current audio driver */
   name = SDL_GetCurrentAudioDriver();
   AssertTrue(name != NULL, "name != NULL");
   AssertTrue(strlen(name)>0, "name empty");
}
