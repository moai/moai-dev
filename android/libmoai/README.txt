----------------------------------------------------------------
-- WINDOWS
----------------------------------------------------------------

	Requires Android NDK and Cygwin installed (http://www.cygwin.com) 

	1. Make sure you have Android NDK downloaded and on your computer.
	2. Make sure that ndk-build is on your path.
	3. Make sure that your Cygwin bin folder is on the path. Make sure it's ahead of any other Bash folders you have (like Git Bash).
	4. Make sure you have make, ar, ranlib, and sed installed in your Cygwin.
	5. Set the permissions for the Android NDK folder to include "Everyone" with full control permissions. We do this because otherwise libstdc++.a won't have the proper permissions and you'll get an error while building.
	6. run build.bat.

----------------------------------------------------------------
-- OSX
----------------------------------------------------------------

	Requires Android NDK

	1. Make sure you have Android NDK downloaded and on your computer.
	2. Make sure that ndk-build is on your path.
	3. run build.sh.