After looking at the unit testing frameworks out there and not finding one that (appeared) to perfectly align with our needs for Moai SDK, we decided to write our own.

The initial implementation can run tests written in C++ or Lua and is designed to integrate with our continuous build server (Jenkins). Right now it is only being used on the Windows build of Moai SDK, but will eventually be supported on all platforms.

This article is an intro to our unit testing process for contributors who would like to help us improve our coverage of Moai SDK. Of course you can also adapt the test runner for your own projects.

Overview - Staging and Testing

The testing framework is implemented as a specially instrumented Moai host linked against the moaiext-test library. This host has access to MOAITestMgr, a cleverly undocumented (as of this writing) singleton that represents the environment for a single test or suite of tests meant to run together.

The test runner is written in Lua. You can find it under moai-dev/test. It’s purpose is to iterate over individual tests, run them and gather their output.

Moai SDK unit tests are designed to run in a two-step process. The first step is called ‘staging’ and the second ‘testing.’ The purpose of the staging step is to produce any device-specific artifacts required to perform the test. For example, a test meant to detect regressions in Moai’s text layout system might set up a text box and do a screen capture when run in staging mode. When run in testing mode, the test would do the same setup but finish with a pixel-compare against the screen capture generated during staging.

For several reasons we want to generate our staging artifacts on device and not store them in our repository. First, there may be subtle differences in things like OpenGL rendering or floating point capabilities from device to device. Second, Git is not set up to efficiently handle large chunks of binary data, which is precisely what the staging phase is intended to produce.

Of course, not all tests will need to implement a staging phase. The staging phase is mainly intended for graphics-based regression tests.

How it Works

All Lua tests are held in moai-dev/test/source. Each test has its own folder with a unique name.

All C++ tests are included directly in the moaiext-test library. Each test inherits from the base class MOAITest and is registered with MOAITestMgr in the AKUTestInit () method found in AKU-test.cpp.

Every test runs in its own process and starts with its working directory set to a unique folder. The contents of the folder are copied from the previous stage. For example, when run in testing mode the contents of the directory are copied from the result of staging which was, in turn, copied from the test source (if a Lua test).

The test runner itself consists of two Lua files: main-staging.lua and main-tests.lua.

When main-staging.lua is run it populates the folders staging-cpp and staging-lua with a unique sub-folder for each C++ and Lua test, respectively. All folders (including the parent folders) are created if they don’t already exist.

For Lua tests, the contents of each test folder under ‘source’ are copied into a corresponding staging folder.

Once the staging folders are set up, each test is run in staging mode with the staging folder as its working directory.

The staging process will only be run if the staging folder for a given test does not already exist. So to clean the staging artifacts for a given test, simply find and delete its folder.

When main-testing.lua is run, it iterates over the contents of staging-cpp and staging-lua, copying each folder to a corresponding folder under (you guessed it) testing-cpp and testing-lua. Unlike the staging process, the testing process will automatically delete each testing folder prior to copying the contents of the staging folder, this cleaning any previous test results.

If there is no staging folder for the test then the test will not be run.

In addition to copying and running the tests, both processes aggregate the output of the tests into log files called staging-results and testing-results. Right now these are in an ad hoc test log format but will be replaced by JUnit output in a future release.

Keyword Filters

Running tests can be time consuming and we don’t always want to run every test. When tests are created they may be assigned filters consisting of keywords, which will be used in near-future release of the test runner.

As of this writing, valid keywords for use in filters are baked directly into MOAITestMgr. This makes it expedient to access them in either Lua or C++. In a future version of the testing framework, keywords may instead be specified in a separate file.

How to Add a Lua Test

Add a folder with a unique name for the test under moai-dev/test/source. At minimum, the folder must contain a main.lua. You may also add any other files or artifacts required to run the test; these will all be copied into the test’s staging folder and finally to its testing folder.

Somewhere in main.lua you must register your staging and testing functions by calling MOAITestMgr.setStagingFunc () and MOAITestMgr.setTestFunc (). In preparation of filter support, you should also set the filter for your test by calling MOAITestMgr.setFilter () with some combination of keywords (available as constants in MOAITestMgr and passed in via variable argument list).

Here’s a basic example (that just writes out some comments then passes):

 

function stage ()
	print ( 'staging' )
end

function test ()
	MOAITestMgr.beginTest ( 'test1' )
	MOAITestMgr.comment ( 'this is a comment' )
	MOAITestMgr.comment ( 'things are going well' )
	MOAITestMgr.endTest ( true )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.SAMPLE )
 

When the test runs, any output to stdout will be directed to the console. MOAITestMgr.beginTest (),MOAITestMgr.comment () and MOAITestMgr.endTest () will write output to the test results file.

Note that each test outputs to a local results file which is then collected and appended to the main results file only after the test process has finished running.

How to Add a C++ Test

Add your test to the moaiext-test project. You may write your test entirely in the header or in a source/header pair.

Your test should be a class that inherits from MOAITest. Use the TEST_NAME () macro to set the name for your test. This will be the name the test runner uses to create folders for your test.

To provide implementations for staging and testing override the Staging () and Test () virtual functions. To prepare for keyword filter support, declare your filter in the Staging () function implementation using MOAITestMgr::SetFilter (). This method takes either a single filter constant or a variable list of constants terminated with a 0.

As with Lua tests, anything written to stdout will appear in the console. Use MOAITestMgr::BeginTest (),MOAITestMgr::Comment (), and MOAITestMgr::EndTest () to write to the test results file.

Finally, register your test with MOAITestMgr in AKU-test.cpp’s AKUTestInit () using the REGISTER_MOAI_TEST () macro.

Here’s a basic C++ test:

 

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEST_SAMPLE_H
#define	MOAITEST_SAMPLE_H

#include <moaicore/moaicore.h>
#include <moaiext-test/MOAITest.h>
#include <moaiext-test/MOAITestKeywords.h>
#include <moaiext-test/MOAITestMgr.h>

//================================================================//
// MOAITest_sample
//================================================================//
class MOAITest_sample :
	public MOAITest {
public:

	TEST_NAME ( "sample" )

	//----------------------------------------------------------------//
	void Staging ( MOAITestMgr& testMgr ) {
		
		testMgr.SetFilter ( MOAI_TEST_UTIL, MOAI_TEST_SAMPLE, 0 );
		
		testMgr.BeginTest ( "sample" );
		testMgr.Comment ( "comment" );
		testMgr.EndTest ( true );
	}
	
	//----------------------------------------------------------------//
	void Test ( MOAITestMgr& testMgr ) {
		
		testMgr.BeginTest ( "sample" );
		testMgr.Comment ( "comment" );
		testMgr.EndTest ( true );
	}
};

#endif

 

Working Directory

Remember that the test gets a unique folder for both the staging phase and the testing phase and that when the test is run in either phase its working directory is that folder. The test always is run in a unique process which reduces the likelihood of side effects interfering with future tests. That said, when performing tests that manipulate the file system, take care not to navigate outside of the test’s working folder as doing do may interfere with the test runner or the behavior of other tests.

When to Run Staging

Staging artifacts may be manually deleted and regenerated by a build administrator whenever changes are committed that should legitimately change their output. For example, if the default filtering mode for textures is changed then the staging folders for any tests that perform screen captures of texture objects should be regenerated.

All staging artifacts should be deleted and regenerated whenever a Moai SDK release is promoted, only after verifying that there are no regressions against existing tests. Once the lack of regressions has been verified, the release is assumed to be ‘good’ and it is safe to re-generate staging.

Testing Framework TODO

As soon as possible we need to update the test results log writer in MOAITestMgr to emit JUnit so the test runner can be integrated with Jenkins.

Right now there is only a ‘source’ directory for Lua tests. In our next revision to the framework we will add an equivalent mechanism for C++ tests. There will be a source-lua and a source-cpp. This will allow test writers to commit artifacts required for C++ tests.

As previously mentioned, keyword filter declaration is supported and MOAITestMgr maybe used to check filters. The test runner scripts still need to be modified to accept filter keywords as command-line arguments.

Moai SDK has recently been updated to include support for capturing the OpenGL screen buffer to a MOAIImage which may be saved out as a .png file. What remains to be implemented is a suite of pixel-comparison APIs for MOAIImage. When these are finished it will be easy to write graphical regression tests and to output images highlighting graphical differences.

We’re going to need to adapt the current framework to eventually run on all supported platforms. Linux and OSX should be straightforward. Supporting the staging/testing paradigm on device will be trickier and may require integration with native testing frameworks, particularly if we want to keep our model of each test running in its own process.

Time to Help Out

Our formal code coverage at the moment can be improved. The process to date has been to test new SDKs against our own code then to release them and find out what breaks.

As much as we hate shipping regressions and bugs, test-driven development has yet to take hold as a daily practice. The remedy for this is more resources which will happen in time. Until then, we need to find a development process that works. That’s where you come in.

If you’re part of the community, writing unit tests is a great way to contribute and also to explore Moai SDK’s dustier corners.

In the short term I’d like to focus on breadth of coverage over depth. Just exercising the basic functionality of each framework class would be hugely valuable. We can drill into obscurity and edge cases later.

If you’d like to volunteer, let Dana know. To whatever extent coordination is required to keep people from stepping on each other’s toes, he can provide it. You can contact him at dana@ziplinegames.com or on the forums. Alternatively, if someone in the community wants to nominate themselves the Moai SDK testing czar and help manage a larger group effort we would welcome that as well.

In any event, stay tuned. Moai SDK is gaining momentum and we have some amazing contributions we’re going to be announcing soon. This project has definitely been an example of “building the plane as we’re flying it” but we couldn’t be happier with the quality of the community and games being developed right now.

 