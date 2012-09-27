/**
	@mainpage Objective-Chipmunk API reference.
	
	<h1>What is Chipmunk?</h1>
	
	First of all, <a href="http://code.google.com/p/chipmunk-physics/">Chipmunk</a> is a 2D rigid body physics library distributed under the MIT license.
	It is intended to be fast, portable, numerically stable, and easy to use.
	For this reason it’s been used in hundreds of games on every system you can name.
	This includes top quality titles such as Night Sky for the Wii and many #1 sellers on the iPhone App Store!
	I’ve put thousands of hours of work over many years to make Chipmunk what it is today.

	Objective-Chipmunk is an Objective-C wrapper for Chipmunk that makes developing physics based iPhone games a snap.
	It integrates with the Objective-C memory model and provides a number of higher level APIs that ease development and save you time.
	While we do charge for Objective-Chipmunk, it will almost certainly save you more time than the small cost to license it.
	As a bonus, you’ll be helping to ensure that we can afford to continue to work on Chipmunk improvements.
	
	You can find out more about Objective-Chipmunk on our webpage. <a href="http://howlingmoonsoftware.com/objectiveChipmunk.php">Objective-Chipmunk</a>
	
	<h1>Where do I start?</h1>
	
	Probably the best place to start is to by going over the <a href="http://files.slembcke.net/chipmunk/tutorials/SimpleObjectiveChipmunk/">Simple Objective-Chipmunk tutorial</a>.
	This tutorial is included with the base Chipmunk source distribution. So you probably already have it. Check in the Objective-Chipmunk/ directory.
	
	From there, you probably want to skim over the latest <a href="http://files.slembcke.net/chipmunk/release/ChipmunkLatest-Docs/">documentation</a>.
	While geared towards C developers, it introduces all the important building blocks. The Objective-C API simply takes things to the next logical step.
*/

#import <TargetConditionals.h>
#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
	#import <CoreGraphics/CGAffineTransform.h>
#endif

// Override the Chipmunk types for Objective-Chipmunk
#define CP_USE_CGPOINTS 1
#define CP_DATA_POINTER_TYPE id
#define CP_GROUP_TYPE id
#define CP_COLLISION_TYPE_TYPE id

#import "chipmunk/chipmunk.h"

#import "ChipmunkObject.h"
#import "ChipmunkBody.h"
#import "ChipmunkShape.h"
#import "ChipmunkConstraint.h"
#import "ChipmunkSpace.h"
#import "ChipmunkExtras.h"
