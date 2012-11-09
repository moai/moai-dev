# Chartboost for iOS

*Version 3.1.1*

The Chartboost iOS SDK is the cornerstone of the Chartboost network. It provides the functionality for showing ads and  more apps pages, and tracking analytics and in-app purchase revenue.


### Usage

Integrating Chartboost takes two easy steps:

 1. Drop the Chartboost folder into your Xcode project.
    
    Ensure you are linking against the following frameworks: `QuartzCore`, `SystemConfiguration`, `StoreKit`, `CoreGraphics`, and `GameKit`.

 2. Instanciate with the Chartboost SDK in your `application:didFinishLaunchingWithOptions:` method, like this:
    
        #import "Chartboost.h"
        
        - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
            
            Chartboost *cb = [Chartboost sharedChartboost];
            cb.appId = /* your app id goes here */
            cb.appSignature = /* your app signature goes here */
            [cb startSession];
            [cb showInterstitial];
            
            return YES;
        }



### Dive deeper

For more common use cases, visit our [online documentation](url).

Check out our header files `Chartboost.h`, and `CBAnalytics.h` for the full API specification.

If you encounter any issues, do not hesitate to contact our kick-ass support team at [support@chartboost.com](mailto:support@chartboost.com).
