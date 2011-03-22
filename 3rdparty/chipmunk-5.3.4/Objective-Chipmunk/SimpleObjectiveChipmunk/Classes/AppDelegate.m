#import "AppDelegate.h"
#import "ViewController.h"

#import <AudioToolbox/AudioToolbox.h>
#import <OpenAL/alc.h>

@implementation AppDelegate

@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {   
  // Some quick sound code for this demonstration.
	ALCdevice *device = alcOpenDevice(NULL);
	ALCcontext *context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	
	AudioSessionInitialize(NULL, NULL, NULL, NULL);
	UInt32 sessionCategory = kAudioSessionCategory_SoloAmbientSound;
	AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
	AudioSessionSetActive(TRUE);
	
	[window addSubview:viewController.view];
	[window makeKeyAndVisible];

	return YES;
}

- (void)dealloc {
	[viewController release];
	[window release];
	[super dealloc];
}

@end
