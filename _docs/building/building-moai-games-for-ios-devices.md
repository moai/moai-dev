---
title: "Building Moai Games For iOS Devices"
---

Running on iOS with Xcode
-------------------------

Building and deploying to an iOS device requires use of Xcode and the latest iOS SDK, which can be found at [Apple's developer website](http://developer.apple.com). Once you have Xcode installed, open the Moai Xcode sample project located at 'hosts/xcode-ios/moai.xcodeproject'. The sample project is set up to link to the Moai static libraries in *bin/ios*.

Before building, it's worth taking a closer look at the Xcode project structure.

![]({{ site.baseurl }}/img/docs/Ios1.png)

There is a small set of code files under the 'Classes' folder at the top of the project. These are the files that make up the Moai host. There is also a folder called 'Other Sources'. This folder contains the 'main' for the iOS app and the precompiled header.

The 'Resources' folder contains the app icons required by iOS, and a few other standard resource files. It also contains a subfolder 'build' which contains a file called 'moai-target' and a folder reference called 'lua'. By default, these two files should be shown in red because they don't exist until the project is built. The function of this folder is explained in greater detail below.

Finally, the 'Frameworks' folder contains references to the frameworks and static libraries required by the targets and the 'Products' folder contains references to the products built by the targets.

The project group structure outlined above is not specific to Moai; it is the standard layout recommended by Apple for iOS application projects.

During the iOS build process, a package script runs which reads the contents of the 'moai-target' file in the 'Resources/build' project folder in order to locate the project's Lua resources. The script copies the entire content of this target folder into the Lua reference folder (also found in the 'Resources/build' project folder) and marks the contents as read-only (to indicate they should not be edited). The standard iOS bundle creation continues and the Lua resources are included in the package under a folder called 'lua'. The AppDelegate file then executes a file called 'main.lua' in the 'lua' folder of your package. You can change this behavior by editing the AppDelegate file if you wish. This setup allows you to quickly try out any of the samples provided in the SDK. Simply point it at a new folder and the MoaiSample host will play the contents of that folder.

![]({{ site.baseurl }}/img/docs/Ios2.png)

There are a few additional features built into this packaging script. First, you can include any number of references, one per line, and the package script will grab the contents of each and combine them in your Lua reference folder. This is useful if you want to place general files somewhere besides your project folder.

Another feature of the packaging script is the ability to define folder dependencies within the folders themselves. For example, the 'samples/iphone/app-apsalar' sample depends on the Apsalar Lua module included elsewhere within the SDK. This folder contains a file called 'moai-target-ext'. When a target folder is packaged, the script checks for the existence of this file. If it's there, the package script is called recursively on the folders listed in this extension file. This allows you to point your main 'moai-target' file at the 'app-apsalar' folder and have its dependency copied automatically.

Building for the device simulator is simple: using Xcode 3, select 'Simulator' and 'Debug' from the build configuration dropdown. Now select 'Build and Debug' from the 'Build' menu. You should see some build output in the 'Build Results' view of Xcode. A few seconds after the build is finished, the iOS simulator should appear, complete with the now-familiar spinning cat head.

![]({{ site.baseurl }}/img/docs/Ios3.png)

To build and deploy on an iOS device, just select 'Device' from the build configuration dropdown instead of 'Simulator'.

Note that if these instructions do not match the behavior and appearance of your Xcode 3 setup, you may not be using Xcode in its 'all in one' mode. To set up the all in one mode, go to 'General/Preferences...' and select the 'General' tab. From the 'Layout' drop down select 'All-In-One'. When you reopen the Moai solution, Xcode should now be in all in one mode. Note also that you cannot edit this preference if you have any open projects in Xcode, so close your projects beforehand.

Troubleshooting
---------------

There are a few common errors that may crop up during this process. If you are told that Xcode does not have permission to open package.sh (or any other shell file), you will have to grant this position manually through a terminal window. Use the Finder to locate the script on your computer, then navigate to the containing folder via the terminal and use the command "chmod +x

<script>
" where

<script>
represents the name of the script in question. Then rebuild and this error should go away.

If Xcode can't find a paticular library (such as libz.1.2.3.dylib - it may error out with "exit code 1"), right-click on Frameworks in the project tree and choose "add files to moai". You may need to search for the exact location of this file in your Finder, then navigate to it via Xcode and plug it into place.

If you get errors about missing things like "GKLeaderboard" and "GKLocalPlayer", that means Xcode is missing Gamekit.Framework. Use the same search and link process described above.

Finally, remember than Apple has a series of processes developers must step through to build for iOS devices. If your sample project works in the simulator but fails on the iPhone or iPad, you may have missed a step in the certificate signature or device provisioning process. More information is available in the [members area](https://developer.apple.com/membercenter/index.action) of Apple's developer website.

Configuring a New Xcode Project
-------------------------------

To get started building your own Moai projects for iOS, the best approach is to copy the Moai samples project and adapt it to your needs.

Create a new folder to contain your own project and resources. Copy the Moai Xcode project and its subfolders into the new folder.

You may wish to remove the 'moai-target' file and its accompanying scripts, and package your Lua resources in another way. If this is the case, simply remove the files 'moai-target', 'mt.default', 'package.sh', and the folder 'lua' from your project folder in Finder. You'll also want to delete the 'Resources/build' folder in your Xcode project tree. To disable the build script, expand the 'Targets' area in your project tree, and then expand the 'MoaiSample' target. Select and delete the build step called 'Run Script' to remove the call to package.sh.

![]({{ site.baseurl }}/img/docs/Ios4.png)

Open the 'Frameworks' group and look to see if any of the Moai libraries are highlighted in red. To fix the broken links, right click on the target icon and select 'Get Info'. On the 'General' tab you will see the references to external frameworks. All of the broken links should be visible under 'Linked Libraries'. Select these and remove them. Now re-add them by clicking the plus icon at the bottom left of the window.

Now that the missing libraries have been added, you will also need to fix the broken path to the Aku headers. Close the target's info window. Right click on the project root, choose 'Get Info' and select the 'Build' tab. Make sure that 'All Configurations' and 'All Settings' are selected. Scroll down in this window until you locate the 'Search Paths' section, where you will find the 'Header Search Paths' setting. The original relative path to the Moai 'include' folder is there. You will need to change this to a new path (relative or absolute) to the Moai 'include' folder.

If you've removed the packaging script, you'll need to add the Lua files for your new project. Return to Finder and add a project subfolder called 'lua'.

The recommended method for adding your Lua files to the project is by using a folder reference. To do this, right click on the 'Resources' group in your project and choose 'Add/Existing Files...' Use the file selector dialog that appears to select the 'lua' folder you just created within the project folder in Finder. Choose 'Add', select the 'Create Folder References for any added folders' radio button and then click the 'Add' button at the bottom of the dialog. If you did this step correctly, you should see a folder reference appear in your project. Unlike regular project groups, the new folder reference is blue.

A folder reference is convenient because any files you add under the referenced folder in Finder will be automatically included in your project and sent to the app bundle each time you build.
