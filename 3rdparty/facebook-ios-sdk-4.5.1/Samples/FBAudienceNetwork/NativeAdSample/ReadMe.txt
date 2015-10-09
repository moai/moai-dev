NativeAdSample

In this sample we demonstrate how to use the Native Ad API (FBNativeAd) from the Facebook Ads SDK for iOS. FBNativeAd gives the flexibility to the to allow developer to render the ad in a more native format that blends into their app. We also show how to wire up the delegate to get notified when the ad status changes over time or when the user interacts with the ad.

When running the sample on the simulator, test ads are served by default. On a device however, real ads will be served unless you specify otherwise. If you test your app on a device, please make sure to call the following API to enable test ads: [FBAdSettings addTestDevice:@"<your device hash printed out on console>"]. 

How to use the sample?

1/ Launch the NativeAdSample project using Xcode from the <Facebook SDK>/samples/NativeAdSample directory.
2/ Open the file ViewController.m and replace "YOUR_PLACEMENT_ID" with the Placement ID you created through developer.facebook.com. 
3/ Run the sample on the simulator or on a device.
