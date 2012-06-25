For both:

1. Staging is the process of computing the expected outputs by running a known working version and saving the output. The idea is that you run staging once per machine and then never again unless you change the test source.
2. Testing is the process of running all of your tests and comparing the output with what was staged. If something is different from when it was staged, then you should take a look at both and determine which output was correct, the staged output or the new output.
3. Execute run-staging.bat to stage all of your source, results are saved in staging-results.
	Note: Files will only be staged if they are not already in the staging-lua/staging-cpp directories, so to re-stage you have to delete them there.
4. Execute run-tests.bat to test, results are saved in testing-results.


================================================================

How to write lua tests:

1. Create main.lua in source/[test_name], include all assets here as well.
2. In main.lua:
	a. Create stage () and test () functions, include this code below them:
		MOAITestMgr.setStagingFunc ( stage )
		MOAITestMgr.setTestFunc ( test )
		MOAITestMgr.setFilter ( [filter] ) -- one of: MOAITestMgr.UTIL, MOAITestMgr.MATH, MOAITestMgr.SAMPLE
	b. Write the stage () function.
	c. Write the test () function, use the following functions to begin and end a test:
		MOAITestMgr.beginTest ( 'Test Name' )
		MOAITestMgr.comment ( 'Write a comment that will be saved in the output file here' )
		MOAITestMgr.endTest ( [bool pass/fail] )


================================================================

How to write C++ tests:

1. Create [test_name].h and add it to the project
2. In the file, create a class MOAI[test_name] and inherit MOAITest.
3. In the public members, name the test with: TEST_NAME ( "Name" )
4. Create function: void Staging ( MOAITestMgr& testMgr )
	a. Set filter with: testMgr.SetFilter ( [filters], 0 ) // one or more of: MOAI_TEST_UTIL, MOAI_TEST_MATH, MOAI_TEST_SAMPLE
	b. Stage anything that is needed by:
		testMgr.BeginTest ( "Test Name" );
		testMgr.Comment ( "Write a comment that will be saved in the output file here" );
		testMgr.EndTest ( [bool pass/fail] );
5. Create function: void Test ( MOAITestMgr& testMgr )
	a. Create tests just like staging them above in 4b.