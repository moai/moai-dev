# VunglePub SDK for iOS

This is a code library that allows you to enhance your iOS applications with video ads from Vungle.

When your program starts, the VunglePub SDK will begin downloading a video ad in the background and save it to disk. When you're ready to display an ad, you can call an SDK method that will tell you if an ad is ready to be played or not. If there is an ad that's ready to go, then you can call an SDK method that will present the ad modally over one of your existing view controllers. When the user has finished interacting with the ad, control will return to your app's view controller, and you can resume normal user interaction.

The SDK will only present videos when they have been fully downloaded to disk. That means that there is no possibility that the ad will pause partway through due to a poor network connection. Ads will only be displayed when the user's device has a working internet connection, which is needed for displaying the ad's payload contents. After an appropriate amount of time has elapsed, cached videos will be erased, freeing up disk space.

## Internal Libraries

Internally, the Vungle Pub SDK makes use of the following libraries:

* VGSBJson 3.0 [site](https://github.com/stig/json-framework)
* MiniZip 1.1 [site](http://www.winimage.com/zLibDll)
* SSZipArchive 0.1.2 [site](https://github.com/samsoffes/ssziparchive)
* OpenUDID 1.0 [site](https://github.com/ylechelle/OpenUDID)

All publicly-visible symbol names have been changed to start with VG, vg, vg_, or similar. We did this so that if you use one of these libraries in your app, your copy will not conflict with the Vungle Pub SDK's internal copy.

## Runtime Requirements

Apps built with the VunglePub library, including the VunglePub test app, will run on Apple's hardware devices running iOS 3.2 or later, and also in the iOS device simulator. All iPads, iPhones, and iPod Touch devices are supported, provided that they are running iOS 3.2 or later. In the future, we may extend that back as far as iOS 3.1.

If a device is running iOS 3.x, then video ads will be displayed with the iOS class MPMoviePlayerController. it works, but has some limitations. The biggest one is that its volume cannot be independently controlled, so video ads will not display a mute button.

If a device is running iOS 4.3 or later, then video ads are displayed with the iOS class AVPlayer. It is much more full-featured, and allows us to display a mute button.

## Build-time Requirements

Building an iOS app that includes the VunglePub SDK works in Xcode 3.x and Xcode 4.x. All recent-ish versions of Xcode should work fine. You must build your app with the iOS SDK version 4.x or 5.x.

VunglePub SDK and its associated libraries do not use Automatic Reference Counting (ARC). You can integrate the code into either ARC or non-ARC apps.

# Adding the VunglePub SDK to your project

To integrate the VunglePub SDK into your iOS application, you should contact Vungle to get a publisher ID string that will be used to identify your app. If you do not yet have a Vungle publisher ID, you can use this temporary app ID for testing purposes: "vungleTest" This is the publisher ID used by the Vungle test app.

### 1. Linking the vungle pub library and adding graphical assets

The VunglePub SDK is provided to you as a single binary, libvunglepub.a, and associated header files. This is a "fat" library that contains executable code for arm6 and arm7 hardware devices and the iOS simulator. It must be linked to your app in your Xcode project.

The VunglePub SDK has one directory of graphical assets that must be added to your project, so that it can properly display its buttons.

### 2. Linking required frameworks

Your application must be linked against the following frameworks:

* AudioToolbox.framework
* AVFoundation.framework
* CFNetwork.framework
* CoreGraphics.framework
* CoreLocation.framework _(location is used only if you choose to provide it)_
* CoreMedia.framework _(weak-link recommended)_
* Foundation.framework
* libz.dylib
* MediaPlayer.framework
* QuartzCore.framework
* SystemConfiguration.framework
* UIKit.framework

If you plan to support iOS 3.x, CoreMedia.framework should be weak-linked, because it is available only on iOS 4.0 and later.

### 3. Modify "Other Linker Flags"

In your Xcode project settings, find the field called "Other Linker Flags." Add the following text, if it's not already there:

  -ObjC -all_load

This must be added for all configurations: Debug, Release, AdHoc, and any others you might have.

This step is needed due to a quirk in the Objective-C language that makes it difficult or impossible to know which parts of a static library should be linked with your program. By adding these flags, ALL code found in static libraries will be linked in.

This step is needed for any Xcode project that links with Objective-C static libraries. Without it, your app will likely crash at runtime.

### 4. Add the main header file

Finally, the VunglePub SDK main header file must be added to your app. It is suggested that you add this line to your prefix header:

```Obj-C
    #import <vunglepub/vunglepub.h>
```

... and add the VunglePub SDK's parent directory to your project's header search paths.

If you perform all these steps and your project builds and links without any warnings or errors, then you have succeeded. If you have problems, look to the Vungle test app for guidance. It is an example app which correctly uses the VunglePub SDK to display video ass.


VunglePub SDK initialization and shutdown
-----------------------------------------

The VunglePub SDK must be initialized before use. A good place to do this is in your application delegate's startup method named:

```Obj-C
    -(BOOL)application:(UIApplication*)application
     didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
```

### SDK initialization

Here is an example method that properly starts the VunglePub SDK:

```Obj-C
    -(void)vungleStart
    {
        VGUserData*  data  = [VGUserData defaultUserData];
        NSString*    appID = @"Your App ID Here";

        // set up config data
        data.age             = 36;
        data.gender          = VGGenderFemale;
        data.adOrientation   = VGAdOrientationLandscape;
        data.locationEnabled = TRUE;

        // start vungle publisher library
        [VGVunglePub startWithPubAppID:appID userData:data];
    }
```

The members of the VGUserData object are described in greater detail in a later section of this document.

### SDK shutdown

When your app is closing, the VunglePub SDK should be given an opportunity to shut itself down properly. A good place to do this is in your application delegate's applicationWillTerminate: method. Here's an example:

```Obj-C
    -(void)applicationWillTerminate:(UIApplication*)application
    {
        [VGVunglePub stop];
    }
```

When your app is run on iOS 4.0 or later, the app delegate's termination method is usually not called. So this particular exit route is mostly for devices running iOS versions earlier than 4.0. For iOS 4.0 and later, VunglePub uses an alternate method to ensure that it is shut down appropriately, which does not require any intervention on your part.


Setup configuration options
---------------------------

When the VunglePub SDK is initialized, you must provide it with some data about your app. The primary piece of information is your Vungle publisher ID, a string which uniquely identifies your app. This string is provided by Vungle. All other configuration information is supplied via the VGUserData object, which you must have filled out before you attempt to start VunglePub.

### VGUserData initialization

To begin, get a new VGUserData filled out with default data, like this:

```Obj-C
    VGUserData*  data = [VGUserData defaultData];
```

It's acceptable to pass this data object directly to the VunglePub library without making any modifications. But if you like, you can set any or all of the following fields:

**data.age and data.gender**: The age and gender of the person using your app, if that information is known. If supplied, Vungle will be able to provide you with suitably targeted videos.

**data.adOrientation**: Should be set to VGAdOrientationPortrait or VGAdOrientationLandscape, which indicates that you'd prefer videos in either portrait or landscape orientations. This is only a hint. We can't guarantee that all videos will be supplied in the given orientation.

**data.locationEnabled**: If set to TRUE, then your user's latitude/longitude coordinates will be sent with every ad request. If enabled, Vungle will be able to send you videos that are geographically targeted.


Displaying a video ad
---------------------

When your app has reached a suitable stopping point, you can ask the VunglePub SDK whether it has an ad ready to display by calling this method:

```Obj-C
    +[VGVunglePub adIsAvailable]
```

If it returns TRUE, then at least one ad has been completely downloaded to disk and is ready for display. FALSE means that there are no videos to display at this time. Note that this method will only return TRUE when the iOS device is connected to the internet.

If an ad is ready to display, you can call this method:

```Obj-C
    +[VGVunglePub playModalAd:(UIViewController*)controller
      animated:(BOOL)animated]
```

**'controller'** is a pointer to one of your app's view controllers that will be the parent of the video ad. If 'animated' is TRUE, then the ad view controller will slide up from the bottom of the screen. When the user has finished interacting with your ad, the Vungle view controller will disappear and you can resume normal operation.


Vungle SDK delegate
-------------------

The app can be informed of interesting events by setting a delegate object. Call this method:

```Obj-C
    +[VGVunglePub setDelegate:(VGVungleDelegate)delegate]
```
  
Your delegate can implement some or all of these methods:

```Obj-C
  -(void)vungleMoviePlayed:(VGPlayData*)playData;
  -(void)vungleStatusUpdate:(VGStatusData*)statusData;
  -(void)vungleViewDidDisappear:(UIViewController*)viewController;
  -(void)vungleViewWillAppear:(UIViewController*)viewController;
```

The first method is called when a video ad has finished playing. You can inspect the member variables of the VGPlayData object to determine how long the ad was and how much of the ad was viewed by the user. This method might be called more than once per ad, if the user decides to watch the ad a second time.

The second method is called with status updates. You can examine the provided VGStatusData object for information on how many videos are cached to disk, how many are available to be played, and so on. Keep in mind that, even if Vungle has cached ads, it will not display them unless the device is connected to the internet.

The last two methods are called before Vungle's ad view controller appears and after it disappears. 'viewController' will be a pointer to Vungle's ad view controller. You should not make any assumptions about this object, other than it is a standard UIViewController.


Disk cache configuration
------------------------

You can control how much disk space the Vungle SDK will use for cached ads. Use these methods:

```Obj-C
    +[VGVunglePub cacheSizeGet]
    +[VGVunglePub cacheSizeSet:(NSInteger)megabytes]
```

The default is 20 megabytes.

Note that Vungle cached ads and all other files it uses are stored in an iOS cache directory. That means the files will not be backed up or synced, and they may be purged by the device if it is running low on disk space.


Vungle logging
--------------

By default, the Vungle SDK doesn't print anything to stdout. But if you call this method:

```Obj-C
    +[VGVunglePub logToStdout:TRUE]
```

... then it will print various bits of status information to the terminal. If you want to get really involved, you can call this method:

```Obj-C
    +[VGVunglePub logTarget:(id)target action:(SEL)action]
```

which sets a target/action pair that will be called for every line of terminal output that the VunglePub SDK produces. The action method should have a signature similar to this:

```Obj-C
    -(void)vungleLogLine:(NSString*)logLine
```

The Vungle test app takes advantage of this feature to display Vungle terminal output in a UITextView.


Video playback volume
---------------------

The Vungle SDK will not alter the device's volume level. Videos will be played at the volume set by the user with the hardware volume buttons.

If the device playing the video is running iOS 4.3 or later, the ad will have a mute button that can be pressed to suppress audio output.

# Troubleshooting

If you run into problems, please contact support@vungle.com
