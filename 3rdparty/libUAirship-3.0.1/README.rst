iOS Urban Airship Library
=========================

Overview
--------

Urban Airship's libUAirship is a drop-in static library that provides a simple way to
integrate Urban Airship services into your iOS applications. This entire project will
allow you to build the library files and all sample projects. If you just want to
include the library in your app, you can download the latest ``libUAirship.zip``
These zips contain two pre-compiled universal libraries: an armv7/armv7s/arm64/i386/x86_64
version (``libUAirship-x.y.z.a``), which includes 32 and 64-bit binaries and can be used with
iOS7+, and a 32-bit only package (``libUAirship-iOS5-x.y.z.a``) which can be used with iOS5+
application targets.

Resources
----------
- Urban Airship iOS Library Reference: http://docs.urbanairship.com/reference/libraries/ios/latest/
- Getting started guide: http://docs.urbanairship.com/build/ios.html
- Library Upgrade Guides: http://docs.urbanairship.com/topic_guides/ios_migration.html

Working with the Library
------------------------

Copy libUAirship Files
######################

Download and unzip the latest version of libUAirship.  If you are using one of our sample
projects, copy the ``Airship`` directory into the same directory as your project::

    cp -r Airship /SomeDirectory/ (where /SomeDirectory/YourProject/ is your project)

If you are not using a sample project, you'll need to import the source files for the User 
Interface into your project. These are located under /Airship/UI/Default

Required Libraries
##################

The core library requires your application to link against the following Frameworks (sample UIs
have additional linking requirements)::

    libUAirship-<current_version>.a
    CFNetwork.framework
    CoreGraphics.framework
    Foundation.framework
    MobileCoreServices.framework
    Security.framework
    SystemConfiguration.framework
    UIKit.framework
    libz.dylib
    libsqlite3.dylib
    CoreTelephony.framework
    CoreLocation.framework

Build Settings
##############

**Compiler**
The latest version of LLVM is the default compiler for all projects and the static library.
     
**Header search path**                          
Ensure that your build target's header search path includes the Airship directory.

Quickstart
----------

Prerequisite
############

Before getting started you must perform the steps outlined above.

In addition you'll need to include *UAirship.h* and *UAPush.h* in your source files.

The AirshipConfig File
######################

The library uses a .plist configuration file named `AirshipConfig.plist` to manage your production and development
application profiles. Example copies of this file are available in all of the sample projects. Place this file
in your project and set the following values to the ones in your application at http://go.urbanairship.com.  To 
view all the possible keys and values, visit http://docs.urbanairship.com/ios-lib/Classes/UAConfig.html

You can also edit the file as plain-text:

.. code:: xml

    <?xml version="1.0" encoding="UTF-8"?>
    <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
    <plist version="1.0">
    <dict>
      <key>inProduction</key>
      <false/>
      <key>developmentAppKey</key>
      <string>Your Development App Key</string>
      <key>developmentAppSecret</key>
      <string>Your Development App Secret</string>
      <key>productionAppKey</key>
      <string>Your Production App Key</string>
      <key>productionAppSecret</key>
      <string>Your Production App Secret</string>
    </dict>
    </plist>

If you are using development builds and testing using the Apple sandbox set `inProduction` to `false`. For
App Store and Ad-Hoc builds, set it to `true`. You may also allow the library to auto-detect the production
mode by setting `detectProvisioningMode` to `true`.

Advanced users may add scripting or preprocessing logic to this .plist file to automate the switch from
development to production keys based on the build type.

Push Integration
################

To enable push notifications, you will need to make several additions to your application delegate.
    
.. code:: obj-c

    - (BOOL)application:(UIApplication *)application 
            didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
        // Your other application code.....
    
        // This prevents the UA Library from registering with UIApplication by default. This will allow
        // you to prompt your users at a later time. This gives your app the opportunity to explain the
        // benefits of push or allows users to turn it on explicitly in a settings screen.
        //
        // If you just want everyone to immediately be prompted for push, you can
        // leave this line out.
        [UAPush setDefaultPushEnabledValue:NO];
    
        // Set log level for debugging config loading (optional)
        // It will be set to the value in the loaded config upon takeOff
        [UAirship setLogLevel:UALogLevelTrace];
    
        // Populate AirshipConfig.plist with your app's info from https://go.urbanairship.com
        // or set runtime properties here.
        UAConfig *config = [UAConfig defaultConfig];

        // You can then programatically override the plist values:
        // config.developmentAppKey = @"YourKey";
        // etc.
    
        // Call takeOff (which creates the UAirship singleton)
        // You may also simply call [UAirship takeOff] without any arguments if you want
        // to use the default config loaded from AirshipConfig.plist
        [UAirship takeOff:config];
    
        // Print out the application configuration for debugging (optional)
        UA_LDEBUG(@"Config:\n%@", [config description]);
    
        // Set the icon badge to zero on startup (optional)
        [[UAPush shared] resetBadge];
    
        // Set the notification types required for the app (optional). With the default value of push set to no,
        // UAPush will record the desired remote notification types, but not register for
        // push notifications as mentioned above. When push is enabled at a later time, the registration
        // will occur normally. This value defaults to badge, alert and sound, so it's only necessary to
        // set it if you want to add or remove types.
        [UAPush shared].notificationTypes = (UIRemoteNotificationTypeBadge |
                                             UIRemoteNotificationTypeSound |
                                             UIRemoteNotificationTypeAlert);

        return YES;
    }
    
To enable push later on in your application:

.. code:: obj-c

    // Somewhere in the app, this will enable push, setting it to NO will disable push.
    // This will trigger the proper registration or de-registration code in the library.
    [[UAPush shared] setPushEnabled:YES];

Logging
-------

Logging can be configured through either the AirshipConfig.plist file or directly in code. The
default log level for production apps is `UALogLevelError` and the default for development apps
is `UALogLevelDebug`.

In `AirshipConfig.plist`, set `LOG_LEVEL` to one of the following integer values:

.. code:: obj-c

    None = 0
    Error = 1
    Warn = 2
    Info = 3
    Debug = 4
    Trace = 5

To set the log level in code, call `setLogLevel` after `takeOff`:

.. code:: obj-c

    [UAirship setLogLevel:UALogLevelWarn];

The available log levels are:

.. code:: obj-c

    UALogLevelNone
    UALogLevelError
    UALogLevelWarn
    UALogLevelInfo
    UALogLevelDebug
    UALogLevelTrace


Building the Library
--------------------

Running Tests
#############

The unit tests in this project require OCMock. OCMock can be installed automatically
with the use of our install script, mock_setup.sh.

Building for Distribution
#########################

To build full and push-only static libraries from the command line, run the distribution script:

.. code:: bash
    
    ./Deploy/distribute.sh

This will produce static libraries (.a files) in /Airship and create the samples and Airship library distribution zip file in
Deploy/output

Contributing Code
-----------------

We accept pull requests! If you would like to submit a pull request, please fill out and submit a
Code Contribution Agreement (http://urbanairship.com/legal/contribution-agreement/).


Third Party Packages
--------------------

Core Library
############

===================  ========  ======================================================
Third party Package  License   Copyright / Creator 
===================  ========  ======================================================
Base64               BSD       Copyright 2009-2010 Matt Gallagher.
Reachability         BSD       Copyright (C) 2010 Apple Inc.
===================  ========  ======================================================

Sample Code
###########

===================  ========  =============================
Third party Package  License   Copyright / Creator
===================  ========  =============================
MTPopupWindow        MIT       Copyright 2011 Marin Todorov
===================  ========  =============================

Test Code
#########

===================  ========  ================================
Third party Package  License   Copyright / Creator
===================  ========  ================================
JRSwizzle            MIT       Copyright 2012 Jonathan Rentzsch
===================  ========  ================================
