---
title: "Setup landscape in Moai Games For iOS Devices"
---

Step 1
------

Select the class:

MoaiVC.mm

Verify the following function exists with the correct landscape orientations:

```objc
//----------------------------------------------------------------//
- ( BOOL ) shouldAutorotateToInterfaceOrientation :
  ( UIInterfaceOrientation )interfaceOrientation {

    if ( interfaceOrientation == UIInterfaceOrientationLandscapeRight 
      || interfaceOrientation == UIInterfaceOrientationLandscapeLeft ) {
        return true;
    } else {
        return false;
    }
}
```

Step 2
------

In MoaiAppDelegate.mm change the line

```
mMoaiView = [[ MoaiView alloc ] initWithFrame:[ UIScreen mainScreen ].bounds ];
```
to:

```objc
CGRect viewBounds;
viewBounds.origin.x = [ UIScreen mainScreen ].bounds.origin.x;
viewBounds.origin.y = [ UIScreen mainScreen ].bounds.origin.y;
viewBounds.size.width = [ UIScreen mainScreen ].bounds.size.height;
viewBounds.size.height = [ UIScreen mainScreen ].bounds.size.width;
       
mMoaiView = [[ MoaiView alloc ] initWithFrame:viewBounds ];
```

Step 3
------

Lastly, select your targets and go to the Summary tab. There are sections for iPhone/iPod deployment and iPad deployment. In both of these, find the Supported Device Orientations section and choose your preferred landscape, or both if you want iOS to rotate the view for you. Make sure you have both orientations called out to return true from Step 1 if you want iOS to auto rotate.

You should now be good to go with landscape!
