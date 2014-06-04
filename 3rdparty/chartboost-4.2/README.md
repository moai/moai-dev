# Chartboost for iOS

*Version 4.2*

The Chartboost iOS SDK is the cornerstone of the Chartboost network. It
provides the functionality for showing interstitials, More-Apps pages, and
tracking in-app purchase revenue.


### Usage

Integrating Chartboost takes two easy steps:

 1. Drop the Chartboost folder into your Xcode project.
    
    Ensure you are linking against the following frameworks: `QuartzCore`,
    `SystemConfiguration`, `StoreKit`, and `CoreGraphics`.  Weak-link
    `AdSupport.framework` by selecting "Optional" next to it in build phases.

 2. Instantiate the Chartboost SDK in your `applicationDidBecomeActive` method, like this:
    
        #import "Chartboost.h"
        
        - (void)applicationDidBecomeActive:(UIApplication *)application        
            
            Chartboost *cb = [Chartboost sharedChartboost];
            cb.appId = @"YOUR_CHARTBOOST_APP_ID";
            cb.appSignature = @"YOUR_CHARTBOOST_APP_SIGNATURE";
            
            [cb startSession];
            [cb showInterstitial];
            
        }


### Dive deeper

For more common use cases, visit our [online documentation](https://help.chartboost.com/documentation/ios).

Check out our header file `Chartboost.h` for the full API
specification.

If you encounter any issues, do not hesitate to contact our happy support team
at [support@chartboost.com](mailto:support@chartboost.com).
