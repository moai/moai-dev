// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCViewCommons.h"
#import "TJCLog.h"
#import "TJCOffersWebView.h"
#import "SynthesizeSingleton.h"

@implementation TJCViewCommons

TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCViewCommons)

- (id)init
{
	self = [super init];
	
	if (self)
	{
		viewsIntegrationType_ = viewsIntegrationTypePlain;
		primaryColorCode_ = TJC_DEFAULT_COLOR;
		
		if (userDefinedColor_ == nil)
		{
			userDefinedColor_ = [TJC_HEXCOLOR(TJC_DEFAULT_COLOR) retain];
		}
	}
	return self;
}


//Hanlde the current selected animation style 
+ (void)animateTJCView:(UIView*)viewRef withTJCTransition:(TJCTransitionEnum)transEffect withDelay:(float)delay 
{
	// Used for certain transitions that have a delay.
	BOOL shouldCommitAnimation = YES;
	
	if(transEffect == TJCTransitionNoEffect)
		return; // No effect requierd so return 

	CGRect animRect = viewRef.frame;
	
	switch (transEffect) 
	{
		case TJCTransitionBottomToTop:
		{
			//Bottom to Top Animation
			animRect.origin.y = animRect.origin.y + animRect.size.height;
			viewRef.frame = animRect;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			animRect.origin.y = animRect.origin.y - animRect.size.height;
			viewRef.frame = animRect;
		}
			break;
			
		case TJCTransitionNormalToBottom:
		{
			//Bottom to Top Animation
			animRect.origin.y = animRect.origin.y;
			viewRef.frame = animRect;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			animRect.origin.y = animRect.origin.y + animRect.size.height;
			viewRef.frame = animRect;
		}
			break;
			
		case TJCTransitionNormalToTop:
		{
			//Bottom to Top Animation
			animRect.origin.y = animRect.origin.y;
			viewRef.frame = animRect;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			animRect.origin.y = animRect.origin.y - animRect.size.height;
			viewRef.frame = animRect;
		}
			break;
			
		case TJCTransitionTopToBottom:
		{
			//top to bottom
			animRect.origin.y = animRect.origin.y - animRect.size.height;
			viewRef.frame = animRect;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			animRect.origin.y = animRect.origin.y + animRect.size.height;
			viewRef.frame = animRect;
		}
			break;
			
		case TJCTransitionLeftToRight:
		{
			animRect.origin.x = animRect.origin.x - animRect.size.width;
			viewRef.frame = animRect;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			animRect.origin.x = animRect.origin.x + animRect.size.width;
			viewRef.frame = animRect;
		}
			break;
			
		case TJCTransitionNormalToRight:
		{
			animRect.origin.x = animRect.origin.x;
			viewRef.frame = animRect;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			animRect.origin.x = animRect.origin.x + animRect.size.width;
			viewRef.frame = animRect;
		}
			break;
			
		case TJCTransitionRightToLeft:
		{
			//top to bottom
			animRect.origin.x = animRect.origin.x + animRect.size.width;
			viewRef.frame = animRect;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			animRect.origin.x = animRect.origin.x - animRect.size.width;
			viewRef.frame = animRect;
		}
			break;
			
		case TJCTransitionNormalToLeft:
		{
			animRect.origin.x = animRect.origin.x;
			viewRef.frame = animRect;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			animRect.origin.x = animRect.origin.x - animRect.size.width;
			viewRef.frame = animRect;
		}
			break;
			
		case TJCTransitionFadeEffect:
		{
			//Fade Effect
			viewRef.alpha = 0;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			viewRef.alpha = 1;
		}
			break;
			
		case TJCTransitionFadeEffectReverse:
		{
			viewRef.alpha = 1;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			viewRef.alpha = 0;
		}
			break;
			
		case TJCTransitionExpand:
		{
			[viewRef setAlpha:0];
			
			// Fixes an issue where this animation was being skipped on devices.
			NSMutableDictionary *tmpDict = [[[NSMutableDictionary alloc] init] autorelease];
			[tmpDict setObject:viewRef forKey:@"viewref"];
			[tmpDict setObject:[NSNumber numberWithFloat:delay] forKey:@"delay"];
			[NSTimer scheduledTimerWithTimeInterval:0.05f 
														target:[TJCViewCommons sharedTJCViewCommons] 
													 selector:@selector(runExpandTransition:) 
													 userInfo:tmpDict
													  repeats:NO];

			shouldCommitAnimation = NO;
		}
			break;
			
		case TJCTransitionShrink:
		{
			CGAffineTransform transBegin = CGAffineTransformScale(viewRef.transform, 1, 1);
			CGAffineTransform transEnd = CGAffineTransformScale(viewRef.transform, 0.01f, 0.01f);
			viewRef.transform = transBegin;
			[viewRef setAlpha:1];
			
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			[viewRef setAlpha:0];
			viewRef.transform = transEnd;
		}
			break;
			
		case TJCTransitionFlip:
		{
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			[UIView setAnimationTransition:UIViewAnimationTransitionFlipFromRight 
										  forView:[viewRef superview]
											 cache:YES];
		}
			break;
			
		case TJCTransitionFlipReverse:
		{
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			[UIView setAnimationTransition:UIViewAnimationTransitionFlipFromLeft
										  forView:[viewRef superview]
											 cache:YES];
			// Ensures the after the flip, we see the original view.
			[viewRef removeFromSuperview];
		}
			break;
			
		case TJCTransitionPageCurl:
		{
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			[UIView setAnimationTransition:UIViewAnimationTransitionCurlDown
										  forView:[viewRef superview]
											 cache:YES];
		}
			break;
			
		case TJCTransitionPageCurlReverse:
		{
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			[UIView setAnimationTransition:UIViewAnimationTransitionCurlUp
										  forView:[viewRef superview]
											 cache:YES];
			// Ensures that the view is removed when the page is curled up and removed.
			[viewRef removeFromSuperview];
		}
			break;
			
		default:
		{
			animRect.origin.y = animRect.origin.y + animRect.size.height;
			viewRef.frame = animRect;
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:delay];
			animRect.origin.y = animRect.origin.y - animRect.size.height;
			viewRef.frame = animRect;
		}
			break;
	}
	
	if (shouldCommitAnimation)
	{
		[UIView commitAnimations];
	}
	
	// Remove the view after the animation finishes.
	[[TJCViewCommons sharedTJCViewCommons] performSelector:@selector(removeTempView:) withObject:viewRef afterDelay:delay];
}


- (void)runExpandTransition:(NSTimer*)timer
{
	NSMutableDictionary *dict = [timer userInfo];
	UIView *viewRef = [dict objectForKey:@"viewref"];
	float delay = [[dict objectForKey:@"delay"] floatValue];
	
	CGAffineTransform transBegin = CGAffineTransformScale(viewRef.transform, 0.01f, 0.01f);
	CGAffineTransform transEnd = CGAffineTransformScale(viewRef.transform, 1, 1);
	viewRef.transform = transBegin;
	[viewRef setAlpha:0];
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:delay];
	[viewRef setAlpha:1];
	viewRef.transform = transEnd;
	
	[UIView commitAnimations];
}


- (void)removeTempView:(id)object
{
	UIView *viewRef = object;
	for (UIView *tmpView in [viewRef subviews])
	{
		if ([tmpView tag] == TJC_GENERIC_TAG_ID)
		{
			[tmpView removeFromSuperview];
		}
	}
}


- (float)getTransitionDelay
{
	return transitionDelay_;
}


- (void)setTransitionEffect:(TJCTransitionEnum) transitionEffect
{
	currentTransitionEffect_ = transitionEffect;
	if(transitionEffect == TJCTransitionNoEffect)
	{
		reverseTransitionEffect_ = TJCTransitionNoEffect;
		transitionDelay_ = 0;
	}
	else
	{
		transitionDelay_ = TJC_DEFAULT_TRANSITION_DELAY;
		if(transitionEffect == TJCTransitionBottomToTop)
		{
			reverseTransitionEffect_ = TJCTransitionNormalToBottom;
		}
		else if(transitionEffect == TJCTransitionTopToBottom)
		{
			reverseTransitionEffect_ = TJCTransitionNormalToTop;
		}
		else if(transitionEffect == TJCTransitionRightToLeft)
		{
			reverseTransitionEffect_ = TJCTransitionNormalToRight;
		}
		else if(transitionEffect == TJCTransitionLeftToRight)
		{
			reverseTransitionEffect_ = TJCTransitionNormalToLeft;
		}
		else if(transitionEffect == TJCTransitionFadeEffect)
		{
			reverseTransitionEffect_ = TJCTransitionFadeEffectReverse;
		}
		else if (transitionEffect == TJCTransitionExpand)
		{
			reverseTransitionEffect_ = TJCTransitionShrink;
		}
		else if (transitionEffect == TJCTransitionShrink)
		{
			reverseTransitionEffect_ = TJCTransitionExpand;
		}
		else if (transitionEffect == TJCTransitionFlip)
		{
			reverseTransitionEffect_ = TJCTransitionFlipReverse;
		}
		else if (transitionEffect == TJCTransitionPageCurl)
		{
			reverseTransitionEffect_ = TJCTransitionPageCurlReverse;
		}
		else
		{
			reverseTransitionEffect_ = TJCTransitionNormalToBottom;
		}
	}
}


- (TJCTransitionEnum)getCurrentTransitionEffect
{
	return currentTransitionEffect_;
}


- (TJCTransitionEnum)getReverseTransitionEffect
{
	return reverseTransitionEffect_;
}


- (int)getUserDefinedColorCode
{
	if(primaryColorCode_ == -1)
		primaryColorCode_ = TJC_DEFAULT_COLOR; //0x0E2c4c
	
	return primaryColorCode_;
}


- (UIColor*)getUserDefinedColor
{
	if(userDefinedColor_ == nil) //set user default color
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"PrimaryColor Tag not properly configured, so picking default color"];
		userDefinedColor_ = [TJC_HEXCOLOR(TJC_DEFAULT_COLOR) retain];	
	}
	
	return userDefinedColor_;
}


- (void)setUserdefinedColorWithIntValue:(int)colorValue
{
	primaryColorCode_ = colorValue;
	
	int red = (primaryColorCode_ & 0x00FF0000) >> 16;
	int green = (primaryColorCode_ & 0x0000FF00) >> 8;
	int blue = (primaryColorCode_ & 0x000000FF);
	
	[userDefinedColor_ release]; //release if already assigned a value by end user
	userDefinedColor_ = [[UIColor colorWithRed:(red/255.0f) green:(green/255.0f) blue:(blue/255.0f) alpha:1] retain];
}


- (void)setUserDefinedColorWithRed:(int)red WithGreen:(int)green WithBlue:(int)blue
{
	if(red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255)
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Invalid value of color Red/Blue/Green picking library's default"];
		primaryColorCode_ = TJC_DEFAULT_COLOR;
	}
	else
	{
		primaryColorCode_ = 256*256*red+256*green+blue;
	}	
	[userDefinedColor_ release]; //release if already assigned a value by end user
	
	userDefinedColor_ = [[UIColor colorWithRed:(red/255.0f) green:(green/255.0f) blue:(blue/255.0f) alpha:1] retain];
}


- (void)dealloc
{
	[userDefinedColor_ release];
	[super dealloc];
}

@end



@implementation TapjoyConnect (TJCViewCommons)

+ (void)setTransitionEffect:(TJCTransitionEnum) transitionEffect
{
	[[TJCViewCommons sharedTJCViewCommons] setTransitionEffect:transitionEffect];
}


+ (void)setUserdefinedColorWithIntValue:(int) colorValue
{
	[[TJCViewCommons sharedTJCViewCommons] setUserdefinedColorWithIntValue:colorValue];
}


+ (void)setUserDefinedColorWithRed:(int)red WithGreen:(int)green WithBlue:(int)blue
{
	[[TJCViewCommons sharedTJCViewCommons] setUserDefinedColorWithRed:red WithGreen:green WithBlue:blue];	
}


+ (void)updateViewsWithOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	[[TJCCallsWrapper sharedTJCCallsWrapper] updateViewsWithOrientation:interfaceOrientation];
}


+ (void)animateTJCView:(UIView*)viewRef withTJCTransition:(TJCTransitionEnum)transEffect withDelay:(float)delay 
{
	[TJCViewCommons animateTJCView:viewRef withTJCTransition:transEffect withDelay:delay];
}


+ (float)getTransitionDelay
{
	return [[TJCViewCommons sharedTJCViewCommons] getTransitionDelay];
}


+ (TJCTransitionEnum)getCurrentTransitionEffect
{
	return [[TJCViewCommons sharedTJCViewCommons] getCurrentTransitionEffect];
}


+ (TJCTransitionEnum)getReverseTransitionEffect
{
	return [[TJCViewCommons sharedTJCViewCommons] getReverseTransitionEffect];
}

@end