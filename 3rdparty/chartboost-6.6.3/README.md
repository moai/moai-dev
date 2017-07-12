# Chartboost for iOS

*Version 6.6.3*

The Chartboost iOS SDK is the cornerstone of the Chartboost network. It
provides the functionality for showing interstitials, More-Apps pages, and
tracking in-app purchase revenue.


### Usage
1. Before you begin:
 - [Have you signed up for a Chartboost account?](https://www.chartboost.com/signup/)
 - [Did you add an app to your dashboard?](https://answers.chartboost.com/hc/en-us/articles/200797729)
 - [Did you download the latest SDK?](https://answers.chartboost.com/hc/en-us/articles/201220095#top)
 - [Do you have an active publishing campaign?](https://answers.chartboost.com/hc/en-us/sections/201082359)
 - Apps must be compiled with iOS base SDK v7.0 or higher. iOS 6.0 Suport has been dropped as of Chartboost version 6.5.0
 - As of June 1, 2016, Apple requires that all submitted apps support IPv6.
 - The Chartboost SDK runs only on devices with iOS version 7.0 or higher, iPhone 4 or later, iPod Touch 5th generation or later, and iPad 2nd generation or later.
 - `startWithAppId:appSignature:delegate:` must always be called during hard and soft bootups, no matter what other actions your app is taking.

2. Drop Chartboost.framework into your Xcode project.

PRO TIP: Checkmark the Copy items if needed option. This creates a local copy of the framework for your project, which keeps your project organized.

3. Link the `StoreKit`, `Foundation`, `CoreGraphic`s, and `UIKit` frameworks.

4. Add the import header `#import <Chartboost/Chartboost.h>` to your AppDelegate.m file.

5. Initialize Chartboost in your `didFinishLaunchingWithOptions` method.
```
(BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Initialize the Chartboost library
    [Chartboost startWithAppId:@"YOUR_CHARTBOOST_APP_ID"
                  appSignature:@"YOUR_CHARTBOOST_APP_SIGNATURE"
                      delegate:self];
    return YES;
}
```
    **Please note:**

    - `startWithAppId:appSignature:delegate:` must always be called during hard and soft bootups, no matter what other actions your app is taking.
    - You might receive a warning when passing an object as a delegate into the `startWithAppID:appSignature:delegate:` method. You can usually ignore this warning, but you can also resolve it by ensuring that the object you're passing conforms to the ChartboostDelegate protocol.

6. Add your app ID and app signature.
    - Replace `YOUR_CHARTBOOST_APP_ID` and `YOUR_CHARTBOOST_APP_SIGNATURE` with your app ID and app signature.
    - [Where can I find my app ID and app signature?](https://answers.chartboost.com/hc/en-us/articles/201465075)

7. To show a static or video interstitial ad:
    - `[Chartboost showInterstitial:CBLocationHomeScreen];` shows the interstitial at location HomeScreen. See Chartboost.h for available location options.
    - For customized control over how ads behave in your game, the Chartboost SDK also offers more features such as caching, delegate methods, and named locations.
    - [Learn more about Chartboost video ads.] (https://answers.chartboost.com/hc/en-us/articles/201220275)
    - **Important:** Chartboost calls should always be made from a main – not background – thread, or these calls may time out!


8.  To show a rewarded video ad:
    - `[Chartboost showRewardedVideo:CBLocationMainMenu];` shows the rewarded video pre-roll message and video ad at location MainMenu. See Chartboost.h for available location options.
    - For customized control over how ads behave in your game, the Chartboost SDK also offers more features such as caching, delegate methods, and named locations.
    - [Learn more about Chartboost video ads.](https://answers.chartboost.com/hc/en-us/articles/201220275)
    - **Important:** Chartboost calls should always be made from a main – not background – thread, or these calls may time out!

9. Test your integration.
    - Build and run your project from Xcode on a device or Simulator.
    - [If you have an active publishing campaign and have integrated "show interstitial" or "show rewarded video" calls, you should see live ads.] (https://answers.chartboost.com/hc/en-us/articles/204930539)
    - [If you don't have any publishing campaigns and you've still integrated these calls, you can use Test Mode to see if test ads show up.](https://answers.chartboost.com/hc/en-us/articles/200780549)
    - [Why can't I see ads in my game?](https://answers.chartboost.com/hc/en-us/articles/201121969)

10. Check the SDK icon in the Chartboost dashboard.
    - Go to your app's App Settings > Basic Settings in your [dashboard](https://dashboard.chartboost.com/).
    - When our servers successfully receive a bootup call from our SDK using your app ID, the SDK icon underneath your app’s icon will turn from gray to green.


### Dive deeper

For more common use cases, visit our [online documentation](https://help.chartboost.com/documentation/ios).

Check out our header file `Chartboost.h` for the full API specification.

If you encounter any issues, do not hesitate to contact our happy support team at [support@chartboost.com](mailto:support@chartboost.com).
