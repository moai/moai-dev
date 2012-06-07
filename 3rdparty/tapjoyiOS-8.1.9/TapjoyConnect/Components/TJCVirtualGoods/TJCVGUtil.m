// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TapjoyConnect.h"
#include <netinet/in.h> 
#import "TJCLog.h"
#import "TJCVGUtil.h"
#import "TJCConstants.h"
#import "TJCUtil.h"
#import "TJCZipArchive.h"

#define SHOW_REWARD_PROMPT_KEY @"TJCShouldUserSeePrompt"
#define SHOW_PROMPT_AFTER_KEY @"TJCShowPromptAfter"
#define NO_SELECTED_COUNT_KEY @"TJCNoSelectedCount"


@implementation TJCVGUtil


+ (BOOL)connectedToNetwork
{
	// Create zero addy
	struct sockaddr_in zeroAddress;
	bzero(&zeroAddress, sizeof(zeroAddress));
	zeroAddress.sin_len = sizeof(zeroAddress);
	zeroAddress.sin_family = AF_INET;
	
	// Recover reachability flags
	SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
	SCNetworkReachabilityFlags flags;
	
	BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
	CFRelease(defaultRouteReachability);
	
	if (!didRetrieveFlags)
	{
		printf("Error. Could not recover network reachability flags\n");
		return 0;
	}
	
	BOOL isReachable = flags & kSCNetworkFlagsReachable;
	BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
	return (isReachable && !needsConnection) ? YES : NO;
}


+ (BOOL)createDirAtPath:(NSString*)dirPath
{
	// Path already exists
	BOOL isDir = NO;
	if ( [[NSFileManager defaultManager] fileExistsAtPath:dirPath isDirectory:&isDir] ) 
	{
		if (NO == isDir) 
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"ERROR: Not a directory. Path is: %@", dirPath];
			return NO;
		}
	}
	[TJCLog logWithLevel:LOG_DEBUG format:@"My Full Path is = %@",dirPath];
	
	if (YES == [[NSFileManager defaultManager] createDirectoryAtPath:dirPath withIntermediateDirectories:YES attributes:nil error:nil]) 
	{
		return YES;
	}
	return NO;
}


/** 
 * Create writeable file and return NSFileHandle
 */
+ (NSFileHandle*) createWriteableFileAtPath:(NSString*)filePath
{
	// if file already exists, delete it
	if ([[NSFileManager defaultManager] fileExistsAtPath:filePath]) 
	{
		if(NO == [[NSFileManager defaultManager] removeItemAtPath:filePath error:nil]) 
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"Error: Deleting existing file: %@", filePath];
		}
	}
	
	// Create new file.
	if(NO == [[NSFileManager defaultManager] createFileAtPath:filePath contents:nil attributes:nil]) 
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"Error creating file: %@", filePath];
		return nil;
	}
	
	NSFileHandle *fh = [NSFileHandle fileHandleForWritingAtPath:filePath];
	if (fh == nil) 
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"Error: Opening file handle."];
	}
	return fh;
}


+ (UIColor*)getUserDefinedColorWithWhiteValue:(int)whiteVal
{
	int primaryColorCode = [[TJCViewCommons sharedTJCViewCommons] getUserDefinedColorCode];
	// 2nd part of the gradient.
	float lighterRed = ((float)((primaryColorCode & 0x00FF0000) >> 16) + whiteVal) / 255;
	float lighterGreen = ((float)((primaryColorCode & 0x0000FF00) >> 8) + whiteVal) / 255;
	float lighterBlue = ((float)(primaryColorCode & 0x000000FF) + whiteVal) / 255;
	
	// Ensure that the values don't exceed 1.0f
	lighterRed = lighterRed > 1.0f ? 1.0f : lighterRed;
	lighterGreen = lighterGreen > 1.0f ? 1.0f : lighterGreen;
	lighterBlue = lighterBlue > 1.0f ? 1.0f : lighterBlue;
	
	UIColor *lighterColor = [UIColor colorWithRed:lighterRed
														 green:lighterGreen
														  blue:lighterBlue
														 alpha:1.0f];
	
	return lighterColor;
}


+ (float)red:(int)colorInt
{
	return (float)((colorInt & 0x00FF0000) >> 16);
}


+ (float)green:(int)colorInt
{
	return (float)((colorInt & 0x0000FF00) >> 8);
}


+ (float)blue:(int)colorInt
{
	return (float)((colorInt & 0x000000FF));
}


+ (int)addWhiteWithColorInt:(int)colorInt withWhiteInt:(int)white;
{
	int r = [TJCVGUtil red:colorInt] + white;
	int g = [TJCVGUtil green:colorInt] + white;
	int b = [TJCVGUtil blue:colorInt] + white;
	if(r < 0) r = 0;  if(r>0xFF)r=0xFF;
	if(g < 0) g = 0;  if(g>0xFF)g=0xFF;
	if(b < 0) b = 0;  if(b>0xFF)b=0xFF;
	
	return (r << 16) + (g << 8) + b;	
}


+ (UIImage*)genericGradientButtonImageWithHeight:(int)height color1:(int)color1 color2:(int)color2 color3:(int)color3
{
	int width = 20;
	int leftCapWidth = 10;
	
	CGImageRef theCGImage = NULL;
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	
	// create the bitmap context
	CGContextRef gradientBitmapContext = CGBitmapContextCreate(NULL, width, height, 8,width*4, colorSpace, kCGImageAlphaPremultipliedLast);
	
	// our bitmap context doesn't support alpha the gradient requires it)
	CGFloat colors[12] = 
	{
		[TJCVGUtil red:color1]/255.0f, [TJCVGUtil green:color1]/255.0f, [TJCVGUtil blue:color1]/255.0f, 1.0f ,
		[TJCVGUtil red:color2]/255.0f, [TJCVGUtil green:color2]/255.0f, [TJCVGUtil blue:color2]/255.0f, 1.0f ,
		[TJCVGUtil red:color3]/255.0f, [TJCVGUtil green:color3]/255.0f, [TJCVGUtil blue:color3]/255.0f, 1.0f 
	};
	
	CGFloat locations[3] = { 0.0f, 0.5f, 1.0f };
	
	// create the CGGradient and then release the color space
	CGGradientRef gradient = CGGradientCreateWithColorComponents(colorSpace, colors, locations, 3);
	CGColorSpaceRelease(colorSpace);
	
	// create the start and end points for the gradient vector (straight down)
	CGPoint gradientStartPoint = CGPointZero;
	CGPoint gradientEndPoint = CGPointMake(0, height);
	
	// draw the gradient into the gray bitmap context
	CGContextDrawLinearGradient(gradientBitmapContext, gradient, gradientStartPoint,
										 gradientEndPoint, 0);
	
	// convert the context into a CGImageRef and release the context
	theCGImage = CGBitmapContextCreateImage(gradientBitmapContext);
	CGContextRelease(gradientBitmapContext);
	CGGradientRelease(gradient);
	// return the imageref containing the gradient
	
	UIImage *img = [TJCVGUtil makeRoundCornerImage:[UIImage imageWithCGImage:theCGImage] :7 :7];
	UIImage *roundedImage = [img stretchableImageWithLeftCapWidth:leftCapWidth topCapHeight:height/10];
	//[img release];
	
	//Had to check this for the resolution of "pointer being freed was not allocated" issue;
	//if([TJCVGUtil isLegitToReleaseImage])
	{
		CGImageRelease(theCGImage);
	}
	
	return  roundedImage;
}


+ (UIImage*)genericGradientButtonImageWithHeight:(int)height width:(int)width color1:(int)color1 color2:(int)color2 color3:(int)color3
{
	//int width = myWidth;
	//int leftCapWidth = 10;
	
	CGImageRef theCGImage = NULL;
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	
	// create the bitmap context
	CGContextRef gradientBitmapContext = CGBitmapContextCreate(NULL, width, height, 8,width*4, colorSpace, kCGImageAlphaPremultipliedLast);
	
	// our bitmap context doesn't support alpha the gradient requires it)
	CGFloat colors[12] =
	{
		[TJCVGUtil red:color1]/255.0f, [TJCVGUtil green:color1]/255.0f, [TJCVGUtil blue:color1]/255.0f, 1.0f ,
		[TJCVGUtil red:color2]/255.0f, [TJCVGUtil green:color2]/255.0f, [TJCVGUtil blue:color2]/255.0f, 1.0f ,
		[TJCVGUtil red:color3]/255.0f, [TJCVGUtil green:color3]/255.0f, [TJCVGUtil blue:color3]/255.0f, 1.0f 
	};
	
	CGFloat locations[3] = { 0.0f, 0.5f, 1.0f };
	
	// create the CGGradient and then release the color space
	CGGradientRef gradient = CGGradientCreateWithColorComponents(colorSpace, colors, locations, 3);
	CGColorSpaceRelease(colorSpace);
	
	// create the start and end points for the gradient vector (straight down)
	CGPoint gradientStartPoint = CGPointZero;
	CGPoint gradientEndPoint = CGPointMake(0, height);
	
	// draw the gradient into the gray bitmap context
	CGContextDrawLinearGradient(gradientBitmapContext, gradient, gradientStartPoint,
										 gradientEndPoint, 0);
	
	// convert the context into a CGImageRef and release the context
	theCGImage = CGBitmapContextCreateImage(gradientBitmapContext);
	CGContextRelease(gradientBitmapContext);
	CGGradientRelease(gradient);
	// return the imageref containing the gradient
	
	UIImage *img = [TJCVGUtil makeRoundCornerImage:[UIImage imageWithCGImage:theCGImage] :7 :7];
	UIImage *roundedImage =img;// [img stretchableImageWithLeftCapWidth:leftCapWidth topCapHeight:height/10];
	//[img release];
	//Had to check this for the resolution of "pointer being freed was not allocated" issue;
	//if([TJCVGUtil isLegitToReleaseImage])
	{
		CGImageRelease(theCGImage);
	}
	
	return  roundedImage;
}


+ (UIImage*)primaryThemeColorGradientButtonImage:(int)height :(int)width
{	
	int color = [[TJCViewCommons sharedTJCViewCommons] getUserDefinedColorCode];
	int white1 = 110;
	int white2 = 0;
	int white3 = 0;
	
	return [self genericGradientButtonImageWithHeight:height
															  width:width
															 color1:[TJCVGUtil addWhiteWithColorInt:color withWhiteInt:white3]
															 color2:[TJCVGUtil addWhiteWithColorInt:color withWhiteInt:white2]
															 color3:[TJCVGUtil addWhiteWithColorInt:color withWhiteInt:white1]];
}


+ (UIImage*)gradientCellBackgroundImageWithWidth:(int)width height:(int)height white1:(int)white1 white2:(int)white2
{
	CGImageRef theCGImage = NULL;
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB(); //CGColorSpaceCreateDeviceGray();
	
	// create the bitmap context
	CGContextRef gradientBitmapContext = CGBitmapContextCreate(NULL, width, height, 8,width*4, colorSpace, kCGImageAlphaPremultipliedLast);
	
	int startValue = 200;
	
	//221 - 192
	//int red=192,green=192,blue=192;
	int red=startValue,green=startValue,blue=startValue;
	
	white1+=29;
	
	// define the start and end grayscale values (with the alpha, even though
	// our bitmap context doesn't support alpha the gradient requires it)
	CGFloat colors[8] = {
		(float)(red+white2) / 255.0f, (float)(green+white2) / 255.0f, (float)(blue+white2) / 255.0f, 1.0f,
		(float)(red+white1) / 255.0f, (float)(green+white1) / 255.0f, (float)(blue+white1) / 255.0f, 1.0f 
	};
	
	CGFloat locations[2] = { 0.0f, 1.0f };
	
	// create the CGGradient and then release the gray color space
	CGGradientRef grayScaleGradient = CGGradientCreateWithColorComponents(colorSpace, colors, locations, 2);
	CGColorSpaceRelease(colorSpace);
	
	// create the start and end points for the gradient vector (straight down)
	CGPoint gradientStartPoint = CGPointZero;
	CGPoint gradientEndPoint = CGPointMake(0, height);
	
	// draw the gradient into the gray bitmap context
	CGContextDrawLinearGradient(gradientBitmapContext, grayScaleGradient, gradientStartPoint,
										 gradientEndPoint, 0);//kCGGradientDrawsAfterEndLocation);
	// convert the context into a CGImageRef and release the context
	theCGImage = CGBitmapContextCreateImage(gradientBitmapContext);
	CGContextRelease(gradientBitmapContext);
	
	// return the imageref containing the gradient
	UIImage *img = [UIImage imageWithCGImage:theCGImage];
	//Had to check this for the resolution of "pointer being freed was not allocated" issue;
	//if([TJCVGUtil isLegitToReleaseImage])
	{
		CGImageRelease(theCGImage);
	}
	CGGradientRelease(grayScaleGradient);
	return img;
}


+ (CGImageRef)CreateGradientImageWithWidth:(int)pixelsWide withHeight:(int)pixelsHigh
{
	CGImageRef theCGImage = NULL;
	
	// gradient is always black-white and the mask must be in the gray colorspace
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();
	
	// create the bitmap context
	CGContextRef gradientBitmapContext = CGBitmapContextCreate(nil, pixelsWide, pixelsHigh,
																				  8, 0, colorSpace, kCGImageAlphaNone);
	
	// define the start and end grayscale values (with the alpha, even though
	// our bitmap context doesn't support alpha the gradient requires it)
	CGFloat colors[] = {0.0f, 1.0f, 1.0f, 1.0f};
	
	// create the CGGradient and then release the gray color space
	CGGradientRef grayScaleGradient = CGGradientCreateWithColorComponents(colorSpace, colors, NULL, 2);
	CGColorSpaceRelease(colorSpace);
	
	// create the start and end points for the gradient vector (straight down)
	CGPoint gradientStartPoint = CGPointZero;
	CGPoint gradientEndPoint = CGPointMake(0, pixelsHigh);
	
	// draw the gradient into the gray bitmap context
	CGContextDrawLinearGradient(gradientBitmapContext, grayScaleGradient, gradientStartPoint,
										 gradientEndPoint, kCGGradientDrawsAfterEndLocation);
	
	// convert the context into a CGImageRef and release the context
	theCGImage = CGBitmapContextCreateImage(gradientBitmapContext);
	CGContextRelease(gradientBitmapContext);
	
	CGGradientRelease(grayScaleGradient);
	
	// return the imageref containing the gradient
	return theCGImage;
}


+ (CGContextRef)MyCreateBitmapContextWithWidth:(int)pixelsWide withHeight:(int)pixelsHigh
{
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	
	// create the bitmap context
	CGContextRef bitmapContext = CGBitmapContextCreate (nil, pixelsWide, pixelsHigh, 8,
																		 0, colorSpace,
																		 // this will give us an optimal BGRA format for the device:
																		 (kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst));
	CGColorSpaceRelease(colorSpace);
	
	return bitmapContext;
}


+ (UIImage*)reflectedImage:(UIImageView*)fromImage withHeight:(NSUInteger)height
{
	// create a bitmap graphics context the size of the image
	CGContextRef mainViewContentContext = [TJCVGUtil MyCreateBitmapContextWithWidth:fromImage.bounds.size.width withHeight:height];
	
	// offset the context -
	// This is necessary because, by default, the layer created by a view for caching its content is flipped.
	// But when you actually access the layer content and have it rendered it is inverted.  Since we're only creating
	// a context the size of our reflection view (a fraction of the size of the main view) we have to translate the
	// context the delta in size, and render it.
	//
	CGFloat translateVertical= fromImage.bounds.size.height - height;
	CGContextTranslateCTM(mainViewContentContext, 0, -translateVertical);
	
	// render the layer into the bitmap context
	CALayer *layer = fromImage.layer;
	[layer renderInContext:mainViewContentContext];
	
	// create CGImageRef of the main view bitmap content, and then release that bitmap context
	CGImageRef mainViewContentBitmapContext = CGBitmapContextCreateImage(mainViewContentContext);
	CGContextRelease(mainViewContentContext);
	
	// create a 2 bit CGImage containing a gradient that will be used for masking the 
	// main view content to create the 'fade' of the reflection.  The CGImageCreateWithMask
	// function will stretch the bitmap image as required, so we can create a 1 pixel wide gradient
	CGImageRef gradientMaskImage = [TJCVGUtil CreateGradientImageWithWidth:1 withHeight:height];
	
	// create an image by masking the bitmap of the mainView content with the gradient view
	// then release the  pre-masked content bitmap and the gradient bitmap
	CGImageRef reflectionImage = CGImageCreateWithMask(mainViewContentBitmapContext, gradientMaskImage);
	
	//Had to check this for the resolution of "pointer being freed was not allocated" issue;
	if([TJCVGUtil isLegitToReleaseImage])
	{
		CGImageRelease(mainViewContentBitmapContext);
		CGImageRelease(gradientMaskImage);
	}
	
	// convert the finished reflection image to a UIImage 
	UIImage *theImage = [UIImage imageWithCGImage:reflectionImage];
	
	// image is retained by the property setting above, so we can release the original
	//Had to check this for the resolution of "pointer being freed was not allocated" issue;
	if([TJCVGUtil isLegitToReleaseImage])
	{
		CGImageRelease(reflectionImage);
	}
	
	return theImage;
}


+ (UIImage*)scaleToSize:(CGSize)size withImage:(UIImage*)myImage
{
	@try 
	{
		UIGraphicsBeginImageContext(size);
		
		CGContextRef context = UIGraphicsGetCurrentContext();
		CGContextTranslateCTM(context, 0.0f, size.height);
		CGContextScaleCTM(context, 1.0f, -1.0f);
		
		CGContextDrawImage(context, CGRectMake(0.0f, 0.0f, size.width, size.height), myImage.CGImage);
		
		UIImage* scaledImage = UIGraphicsGetImageFromCurrentImageContext();
		
		UIGraphicsEndImageContext();
		
		return scaledImage;
	}
	@catch (NSException * e)
	{
		return nil;
	}
	return nil;
}


static void addRoundedRectToPath(CGContextRef context, CGRect rect, float ovalWidth, float ovalHeight)
{
	float fw, fh;
	if (ovalWidth == 0 || ovalHeight == 0) 
	{
		CGContextAddRect(context, rect);
		return;
	}
	
	CGContextSaveGState(context);
	CGContextTranslateCTM (context, CGRectGetMinX(rect), CGRectGetMinY(rect));
	CGContextScaleCTM (context, ovalWidth, ovalHeight);
	fw = CGRectGetWidth (rect) / ovalWidth;
	fh = CGRectGetHeight (rect) / ovalHeight;
	CGContextMoveToPoint(context, fw, fh/2);
	CGContextAddArcToPoint(context, fw, fh, fw/2, fh, 1);
	CGContextAddArcToPoint(context, 0, fh, 0, fh/2, 1);
	CGContextAddArcToPoint(context, 0, 0, fw/2, 0, 1);
	CGContextAddArcToPoint(context, fw, 0, fw, fh/2, 1);
	CGContextClosePath(context);
	CGContextRestoreGState(context);
}


+ (UIImage*)makeRoundCornerImage :(UIImage*)img :(int)cornerWidth :(int)cornerHeight
{
	UIImage * newImage = nil;
	
	if( nil != img)
	{
		//NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
		int w = img.size.width;
		int h = img.size.height;
		
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		CGContextRef context = CGBitmapContextCreate(NULL, w, h, 8, 4 * w, colorSpace, kCGImageAlphaPremultipliedFirst);
		
		CGContextBeginPath(context);
		CGRect rect = CGRectMake(0, 0, img.size.width, img.size.height);
		addRoundedRectToPath(context, rect, cornerWidth, cornerHeight);
		CGContextClosePath(context);
		CGContextClip(context);
		
		CGContextDrawImage(context, CGRectMake(0, 0, w, h), img.CGImage);
		
		CGImageRef imageMasked = CGBitmapContextCreateImage(context);
		CGContextRelease(context);
		CGColorSpaceRelease(colorSpace);
		//[img release];
		
		newImage = [UIImage imageWithCGImage:imageMasked];
		
		//CFRetain(CGImageGetDataProvider(imageMasked));
		//Had to check this for the resolution of "pointer being freed was not allocated" issue;
		//if([TJCVGUtil isLegitToReleaseImage])
		{
			CGImageRelease(imageMasked);
		}
		
		//[pool release];
	}
	
	return newImage;
}


+ (BOOL)isLegitToReleaseImage
{
	//Apple bug based on xcode for ipad on simulator 3.0 
	NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
	NSString *currSysModel = [[UIDevice currentDevice] model];
	
	if(!(currSysVer && 
		  [currSysVer compare:@"3.0" options:NSNumericSearch] == NSOrderedSame && currSysModel &&[currSysModel isEqualToString:@"iPhone Simulator"]))
	{
		return YES;
	}
	else
	{
		[TJCLog logWithLevel:LOG_DEBUG 
						  format:@"malloc: *** error for object 0x00000: pointer being freed was not allocated *** set a breakpoint in malloc_error_break."];
	}
	return NO;
}


/**
 * Unzip file to directory.
 */
+ (BOOL)unZipFile:(NSString *)filePath toDir:(NSString *)destPath
{
	BOOL retVal = NO;
	TJCZipArchive* za = [[TJCZipArchive alloc] init];
	
	if( [za UnzipOpenFile:filePath] ) 
	{
		retVal = [za UnzipFileTo:destPath overWrite:YES];
		if (NO == retVal) 
		{
			//TODO: use TJCLogging.
			[TJCLog logWithLevel:LOG_DEBUG format:@"Error unzipping."];
		}
		[TJCLog logWithLevel:LOG_DEBUG format:@"Zipped successfully."];        
		[za UnzipCloseFile];
	}
	[za release];
	return retVal;
}


@end
