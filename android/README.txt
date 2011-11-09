----------------------------------------------------------------
-- ALL PLATFORMS
----------------------------------------------------------------

	-To create an Android host, execute "make-host.sh" and pass it the package for your application. For example: bash make-host.sh -p com.getmoai.samples
	-You will need to have the libmoai build working before "make-host.sh" will work. Check libmoai/README.txt for details.
	-Once "make-host.sh" has executed successfully, it will create a "untitled-host" directory. This directory contains everything you need to run your Moai project on Android.
	-The Android host uses a template project to dynamically generate a host for you when you run it. To edit this template, examine and edit the contents of untitled-host/host-source.

----------------------------------------------------------------
-- WINDOWS
----------------------------------------------------------------

	-To run the Android command line build (which this is based upon), you will need Apache Ant installed a configured. This includes adding Ant's bin folder to your path, installing a JDK, and creating JAVA_HOME and ANT_HOME environment variables.