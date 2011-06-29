----------------------------------------------------------------
-- WINDOWS
----------------------------------------------------------------

Requires Android NDK and Cygwin fully installed (http://www.cygwin.com) 

1. Make sure you have Android NDK downloaded and on your computer.
2. Make sure that ndk-build is on your path.
3. Make sure that your Cygwin bin folder is on the path. Make sure it's ahead of any other Bash folders you have (like Git Bash).
4. Set the permissions for the Android NDK folder to include "Everyone" with full control permissions. We do this because otherwise libstdc++.a won't have the proper permissions and you'll get an error while building.
5. run build.bat.

----------------------------------------------------------------
-- MAC
----------------------------------------------------------------

Requires Android NDK

1. Make sure that ndk-build is on your path. You can accomplish this by adding the path to your environment.plist file.
2. Run Eclipse from the exec shortcut in the eclipse folder. For some reason, adding the ndk path to the PATH breaks eclipse's launcher. It can launch fine from the command line though. We will figure out a better solution for this soon.
3. run build.sh.