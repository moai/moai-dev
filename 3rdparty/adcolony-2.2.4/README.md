AdColony iOS SDK
==================================
Modified: 2013/11/18  
SDK Version: 2.2.4  

Contains:
----------------------------------
* AdColony.framework (iOS)
* Sample Apps
  * AdColonyAdvanced
  * AdColonyBasic
  * AdColonyV4VC
* W-9 Form.pdf

Getting Started with AdColony:
----------------------------------
First time and returning users should review the [quick start guide](https://github.com/AdColony/AdColony-iOS-SDK/wiki) because the API and usage of our iOS SDK have changed with this release.

2.2.4 Change Log:
----------------------------------
* Added support for the 64-bit ARM architecture on new Apple devices
* The AdColony iOS SDK disables itself on iOS 4.3 (iOS 5.0+ is fully supported); the minimum Xcode Deployment Target remains iOS 4.3
* Bug fixes

2.2 Change Log:
----------------------------------
* AdColony 2.2 has been fully tested against the most recent iOS 7 betas and gold master seed
* AdColony is now packaged as a framework and its API is not backwards compatible with AdColony 2.0 integrations
* AdColony relies on additional frameworks and libraries; see the [quick start guide](https://github.com/AdColony/AdColony-iOS-SDK/wiki) for details. 
* The AdColony class has had methods removed and renamed for consistency
* The AdColonyDelegate protocol has had methods removed and renamed; its use is no longer mandatory
* The AdColonyTakeoverAdDelegate protocol has been renamed to AdColonyAdDelegate; it has had methods removed and renamed
* Improved detail and transparency of information regarding ad availability
* Various user experience improvements during ad display
* Increased developer control over network usage; improved efficiency and reliability
* Added console log messages to indicate when the SDK is out of date
* Bug fixes

2.0.1.33 Change Log:
----------------------------------
* Removed all usage of Apple's UDID in accordance with Apple policy

2.0 Change Log:
----------------------------------
* Support for Xcode 4.5, iOS 6.0, iPhone 5, and new "Limit Ad Tracking" setting
* Removed support for armv6 architecture devices
* Requires Automatic Reference Counting (ARC) for AdColony library (or whole project)
* Numerous bug fixes, stability improvements and performance gains
* Built-in support for multiple video views per V4VC reward
* Can collect per-user metadata that unlocks higher-value ads
* New sample applications
* Simplified interface for apps that need to cancel an ad in progress
* Simplified interface for apps that need custom user IDs for server-side V4VC transactions
* Improved log messages for easier debugging


Sample Applications:
----------------------------------
Included are three sample apps to use as examples and for help on AdColony integration. The basic app allows users to launch an ad, demonstrating simple usage of AdColony. The currency app demonstrates how to implement videos-for-virtual currency (V4VC) to enable users to watch videos in return for in-app virtual currency rewards (with currency balances stored client-side). The advanced app demonstrates advanced topics such as multiple zones and playing ads in apps with audio and music. 


Legal Requirements:
----------------------------------
You must accept the terms and conditions on the AdColony website by registering in order to legally use the AdColony SDK. U.S. based companies will need to complete the W-9 form and send it to us, as well as complete the section on payment information on clients.adcolony.com, before publisher payments can be issued.


Contact Us:
----------------------------------
For more information, please visit AdColony.com. For questions or assistance, please email us at support@adcolony.com.

