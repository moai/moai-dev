# Installing the framework

## Migrating from an old version

1. Remove the `#import <vunglepub/vunglepub.h>` from your pre-compiled headers if you have it (or any other include referencing vungle).
2. Remove any extra headers/library search paths referencing vungle.
3. You might want to leave the `Other Linker Flags` (`-all_load` and `-ObjC`).
4. Make sure that in your linking phase, you do not have any reference to libvungle.a
5. Remove libvungle.a and any vungle-related resources from your project (there are 3 pngs that vungle uses).
6. At this point, you should have removed vungle from your app. Follow the instructions to install vungle below.

## Installing vungle from scratch

1. Copy the `.embeddedframework` directory to some place accessible by your app (next to your Xcode project might be a good idea).
2. Drag the recently copied directory to your Xcode project. This will add the resources and the static framework.
3. Add `#import <vunglepub/vunglepub.h>` to the files that need to access vungle (probably your App Delegate and any view/game controller that will show the ads).
4. You should be done.

## Known Issues

### Landscape-only apps

Some of our ads will try to open the ProductView Sheet to show an in-app store. This increases the conversion, but it requires your app to be able to support portrait orientation.

In order to fix this, allow the app to support Portrait and Landscape (in the `Info.plist`). You can then limit your view controllers to only support portrait by returning the proper value in the `supportedInterfaceOrientations` method.

## API

The full API documentation can be located at:

https://v.vungle.com/dev/ios
