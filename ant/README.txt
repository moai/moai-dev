-------------------------------------------------------------------------------
-- ALL PLATFORMS
-------------------------------------------------------------------------------

  - To create an Android host, execute make-host.sh and pass it the package name
    for your application. For example: bash make-host.sh -p com.getmoai.samples
	If libmoai.so has not yet been built, it will be compiled first, which will
	take some time.
  - If successful, make-host.sh will create a directory called "untitled-host".
    This directory contains an Android host template in a directory called
    "host-source" that can be edited as needed and configured via settings files.
  - Use the run-host.sh script in the generated directory to create the Android
    host once it has been configured. This will create a directory called "build"
    which contains a full Android project that can be imported into Eclipse and
    then launches the application onto the default Android emulator or device.

-------------------------------------------------------------------------------
-- WINDOWS
-------------------------------------------------------------------------------

  - Ensure that Apache Ant is properly installed and configured. This includes
    installing a JDK, setting JAVA_HOME and ANT_HOME environment variables and
    adding the Ant bin/ directory to your PATH.

