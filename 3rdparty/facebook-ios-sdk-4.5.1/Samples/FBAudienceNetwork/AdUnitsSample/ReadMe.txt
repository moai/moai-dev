AdUnitsSample

In this sample we demonstrate how to add a banner ad unit (FBAdView) and an interstitial unit (FBInterstitialAd) to an app. We also
show how to wire up delegates to get notified when the ad status changes over time or when the user interacts with the ad.

When running the sample on the simulator, test ads are served by default. On a device however, real ads will be served unless you specify otherwise. If you test your app on a device, please make sure to call the following API to enable test ads: [FBAdSettings addTestDevice:@"<your device hash printed out on console>"]. 

How to use the sample?

Install the Facebook SDK for iOS.
1/ Launch the AdUnitsSample project using Xcode from the <Facebook SDK>/samples/AdUnitsSample directory.
2/ Open the file ViewController.m and replace "YOUR_PLACEMENT_ID" with the Placement ID you created through developer.facebook.com. 
3/ Run the sample on the simulator or on a device.
