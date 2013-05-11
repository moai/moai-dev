/**
 * Automated SDL_RWops test.
 *
 * Original code written by Edgar Simo "bobbens"
 * Ported by Markus Kauppila (markus.kauppila@gmail.com)
 *
 * Released under Public Domain.
 */

#include <stdio.h>

#include <SDL/SDL.h>

#include "../../include/SDL_test.h"

const char* RWOPS_READ = "rwops_read";
const char* RWOPS_WRITE = "rwops_write";

static const char hello_world[] = "Hello World!";
static const char const_mem[] = "Hello World!";


/* Test cases */
static const TestCaseReference test1 =
		(TestCaseReference){ "rwops_testParam", " Negative test for SDL_RWFromFile parameters", TEST_ENABLED, 0, 60 };

static const TestCaseReference test2 =
		(TestCaseReference){ "rwops_testMem", "Tests opening from memory", TEST_ENABLED, 0, 60 };

static const TestCaseReference test3 =
		(TestCaseReference){ "rwops_testConstMem", "Tests opening from (const) memory", TEST_ENABLED, 0, 60 };

static const TestCaseReference test4 =
		(TestCaseReference){ "rwops_testFileRead", "Tests reading from a file", TEST_ENABLED, 0, 60 };

static const TestCaseReference test5 =
		(TestCaseReference){ "rwops_testFileWrite", "Test writing to a file", TEST_ENABLED, 0, 60 };

static const TestCaseReference test6 =
		(TestCaseReference){ "rwops_testFPRead", "Test reading from stdio", TEST_ENABLED, TEST_REQUIRES_STDIO, 60 };

static const TestCaseReference test7 =
		(TestCaseReference){ "rwops_testFPWrite", "Test writing to stdio", TEST_ENABLED, TEST_REQUIRES_STDIO, 60 };



/* Test suite */
extern const TestCaseReference *testSuite[] =  {
	&test1, &test2, &test3, &test4, &test5, &test6, &test7, NULL
};

TestCaseReference **QueryTestSuite() {
	return (TestCaseReference **)testSuite;
}


// Fixture
void
SetUp(void *arg)
{
	FILE *handle = fopen(RWOPS_READ, "w");
	AssertTrue(handle != NULL, "Creating file '%s' failed", RWOPS_READ);

	fwrite(hello_world, 1, SDL_strlen(hello_world), handle);
	fclose(handle);
}

void
TearDown(void *arg)
{
	// Remove the created files
	remove(RWOPS_READ);
	remove(RWOPS_WRITE);
}

/**
 * @brief Makes sure parameters work properly. Helper function
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_RWseek
 * http://wiki.libsdl.org/moin.cgi/SDL_RWread
 */
int _testGeneric( SDL_RWops *rw, int write )
{
   char buf[sizeof(hello_world)];
   int i;

   /* Set to start. */
   i = SDL_RWseek( rw, 0, RW_SEEK_SET );
   AssertEquals(i, 0, "Seeking with SDL_RWseek (RW_SEEK_SET)");

   /* Test write. */
   i = SDL_RWwrite( rw, hello_world, sizeof(hello_world)-1, 1);

   if (write) {
	   AssertEquals(i, 1, "Writing with SDL_RWwrite (failed to write)");
   }
   else {
		AssertTrue(i <= 0, "Writing with SDL_RWwrite (wrote when shouldn't have): %d <= 0", i);
   }

   /* Test seek. */
   i = SDL_RWseek( rw, 6, RW_SEEK_SET );

   AssertEquals(i, 6, "Seeking with SDL_RWseek (RW_SEEK_SET)");

   /* Test seek. */
   i = SDL_RWseek( rw, 0, RW_SEEK_SET );
   AssertEquals(i, 0, "Seeking with SDL_RWseek (RW_SEEK_SET)");

   /* Test read. */
   i = SDL_RWread( rw, buf, 1, sizeof(hello_world)-1 );
   AssertEquals(i, sizeof(hello_world)-1, "Reading with SDL_RWread");
   AssertEquals(SDL_memcmp( buf, hello_world, sizeof(hello_world)-1 ), 0, "Memory read does not match memory written");

   /* More seek tests. */
   i = SDL_RWseek( rw, -4, RW_SEEK_CUR );
   AssertEquals(i, sizeof(hello_world)-5, "Seeking with SDL_RWseek (RW_SEEK_CUR))");

   i = SDL_RWseek( rw, -1, RW_SEEK_END );
   AssertEquals(i, sizeof(hello_world)-2, "Seeking with SDL_RWseek (RW_SEEK_END)");
}

/*!
 * Negative test for SDL_RWFromFile parameters
 *
 * \sa http://wiki.libsdl.org/moin.cgi/SDL_RWFromFile
 *
 */
void rwops_testParam (void)
{
   SDL_RWops *rwops;

   /* These should all fail. */
   rwops = SDL_RWFromFile(NULL, NULL);
   AssertTrue(rwops == NULL, "SDL_RWFromFile(NULL, NULL) worked");

   rwops = SDL_RWFromFile(NULL, "ab+");
   AssertTrue(rwops == NULL, "SDL_RWFromFile(NULL, NULL) worked");

   rwops = SDL_RWFromFile(NULL, "sldfkjsldkfj");
   AssertTrue(rwops == NULL, "SDL_RWFromFile(NULL, \"sldfkjsldkfj\") worked");

   rwops = SDL_RWFromFile("something", "");
   AssertTrue(rwops == NULL, "SDL_RWFromFile(\"something\", \"\") worked");

   rwops = SDL_RWFromFile("something", NULL);
   AssertTrue(rwops == NULL, "SDL_RWFromFile(\"something\", NULL) worked");
}


/**
 * @brief Tests opening from memory.
 *
 * \sa http://wiki.libsdl.org/moin.cgi/SDL_RWFromMem
 */
void rwops_testMem (void)
{
   char mem[sizeof(hello_world)];
   SDL_RWops *rw;

   /* Open. */
   rw = SDL_RWFromMem( mem, sizeof(hello_world)-1 );
   AssertTrue(rw != NULL, "Opening memory with SDL_RWFromMem");

   /* Run generic tests. */
   _testGeneric( rw, 1 );

   /* Close. */
   SDL_FreeRW( rw );
}


/**
 * @brief Tests opening from memory.
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_RWFromConstMem
 */
void rwops_testConstMem (void)
{
   SDL_RWops *rw;

   /* Open. */
   rw = SDL_RWFromConstMem( const_mem, sizeof(const_mem)-1 );
   AssertTrue(rw != NULL, "Opening memory with SDL_RWFromConstMem");

   /* Run generic tests. */
   _testGeneric( rw, 0 );

   /* Close. */
   SDL_FreeRW( rw );
}


/**
 * @brief Tests reading from memory.
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_RWFromFile
 * http://wiki.libsdl.org/moin.cgi/SDL_FreeRW
 */
void
rwops_testFileRead(void)
{
   SDL_RWops *rw;

   /* Read test. */
   rw = SDL_RWFromFile(RWOPS_READ, "r");
   AssertTrue(rw != NULL, "Opening memory with SDL_RWFromFile RWOPS_READ");

   _testGeneric( rw, 0 );

   SDL_FreeRW( rw );
}

/**
 * @brief Tests writing from memory.
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_RWFromFile
 * http://wiki.libsdl.org/moin.cgi/SDL_FreeRW
 */
void
rwops_testFileWrite(void)
{
   SDL_RWops *rw;

   /* Write test. */
   rw = SDL_RWFromFile(RWOPS_WRITE, "w+");
   AssertTrue(rw != NULL, "Opening memory with SDL_RWFromFile RWOPS_WRITE");

   _testGeneric( rw, 1 );

   SDL_FreeRW( rw );
}


/**
 * @brief Tests reading from stdio
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_RWFromFP
 * http://wiki.libsdl.org/moin.cgi/SDL_FreeRW
 *
 */
void
rwops_testFPRead(void)
{
   FILE *fp;
   SDL_RWops *rw;

   /* Run read tests. */
   fp = fopen(RWOPS_READ, "r");
   AssertTrue(fp != NULL, "Failed to open file %s,", RWOPS_READ);

   rw = SDL_RWFromFP( fp, 1 );
   AssertTrue(rw != NULL, "Opening memory with SDL_RWFromFP");

   // TODO bail out if NULL
   _testGeneric( rw, 0 );

   SDL_FreeRW( rw );
   fclose(fp);
}

void
rwops_testFPWrite(void)
{
   FILE *fp;
   SDL_RWops *rw;

   /* Run write tests. */
   fp = fopen(RWOPS_WRITE, "w+");
   AssertTrue(fp != NULL, "Failed to open file %s", RWOPS_WRITE);

   rw = SDL_RWFromFP( fp, 1 );
   AssertTrue( rw != NULL, "Opening memory with SDL_RWFromFP");

   _testGeneric( rw, 1 );

   SDL_FreeRW( rw );
   fclose(fp);
}
