ADCOLONY PREMIUM MOBILE VIDEO ADVERTISING NETWORK
--------------------------------------------------
Modified: 9/9/2011
SDK Version: 1.9.7


AdColony 1.9.7 SDK Package Contents:
---------------------------------------
1. AdColonyLibrary (iOS)
	a. AdColonyPublic.h
	b. libAdColony.a
2. Documentation
	a. AdColonySDKQuickStartGuide.pdf
	b. W-9 Form.pdf
3. Sample Apps
	a. AdColonyAdvancedApp
	b. AdColonyBasicApp
	c. AdColonyCurrencyApp

	
Getting Started with AdColony:
----------------------------------
First time and returning users should review the AdColony SDK Quick Start Guide, 
as the AdColony API has been changed (Please refer to the change log for details). 
This file is available as a PDF in the "Documentation" folder and contains everything 
you need to start integrating AdColony into your app quickly.   


1.9.7 Summary Change Log:
----------------------------------
Details can also be found in the AdColonyQuickStartGuide.pdf under "Changes"

-AdColonyAdministratorDelegate protocol has been renamed AdColonyDelegate for clarity. 
A macro has been added to that defines the old name as the new name for backwards compatibility.
-AdColonyAdministratorPublic class has been renamed AdColony for clarity. 
A macro has been added to that defines the old name as the new name for backwards compatibility.
-initAdministratorWithDelegate: method has been renamed initAdColonyWithDelegate: for clarity. 
A macro has been added to that defines the old name as the new name for backwards compatibility.
-Two methods,  zoneStatusForZone: and zoneStatusForSlot: have been added so that one can determine the availability of video ads with a finer resolution. 
Please see AdColonyPublic.h for descriptions of the return values.
-Two methods,  didVideoFinishLoadingForZone: and didVideoFinishLoadingForSlot: have been deprecated, but remain fully functional. 
For future proofing, replace these methods with appropriate calls to the methods from Change 3.



1.9.6 Summary Change Log:
----------------------------------
- AdColonyVideoAdPresenterPublic is a new class for the presentation of multiple video 
ad zones
- adColonyMobclixInitializedExternally method has been removed
- noVideoFill method has been replaced by two methods called noVideoFillForAllZones: and 
noVideoFillInZone:
- adColonyVideoAdsReady, adColonyNoVideoAdsReady methods have been replaced by the 
adColonyVideoAdsReadyInZone: , adColonyVideoAdsNotReadyInZone: methods
- adColonyVirtualCurrencyAwarded and adColonyVirtualCurrencyNotAwarded methods have been 
replaced by the adColonyVirtualCurrencyAwardedByZone:currencyName:currencyAmount: and 
adColonyVirtualCurrencyNotAwardedByZone:currencyName:currencyAmount: methods
- adColonyBannerAdServed , adColonyBannerAdNotServed methods have been removed
- In AdColonyAdministratorPublic: all methods except initAdministratorWithDelegate: and 
turnAllAdsOff have been deprecated
- didVideoFinishedLoading method works as if there is only one video ad zone, but returns 
NO if multiple video ad zones are present
- serveVideoAdForDelegate: method will only play a video ad if there is only one video zone 
in the app and that zone does not have virtual currency set as an option
- virtualCurrencyAwardAvailable method works the same way if the app has only one video ad 
zone that has been configured for virtual currency, and return NO in the presence of multiple 
video ad zones
- serveVideoAdForVirtualCurrencyWithDelegate:andPostPopup, displayVirtualCurrencyPrePopupWithDelegate:andPostPopup: 
methods work the same way if the app has only one video ad zone that has been configured for virtual currency
- getVirtualCurrencyName , getVirtualCurrencyRewardAmount methods work the same way if 
the app has only one video ad zone that has been configured for virtual currency
- AdColonyStableBannerAdViewPublic entire class has been deprecated


Example Applications:
----------------------------------
Included are three sample apps to use as examples and for help on AdColony integration. 
The basic app allows users to launch an ad from either of two zones. The advanced app 
demonstrates advanced topics such as immediate ad launch (prestitial) and includes a 
basic debug logger.  The currency app demonstrates how to implement videos-for-virtual 
currency (V4VC) to enable users to watch videos in return for in-app virtual currency 
rewards (includes an example currency tracker).


Legal Requirements:
----------------------------------
You must accept the terms and conditions on the AdColony website by registering in order
to legally use the AdColony SDK. U.S. based companies will need to complete the W-9 form 
and send it to us before payments can be issued.


Contact Us:
----------------------------------
For more information, please visit AdColony.com. For questions or assistance, please 
email us at support@adcolony.com.

