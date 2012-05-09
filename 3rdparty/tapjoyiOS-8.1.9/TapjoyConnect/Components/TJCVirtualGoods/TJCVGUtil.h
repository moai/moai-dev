// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <SystemConfiguration/SCNetworkReachability.h>
#import <QuartzCore/QuartzCore.h> // for CALayer

@interface TJCVGUtil : NSObject 
{
	
}


#pragma mark GENERAL_PRAGMA	

+ (BOOL)connectedToNetwork;

+ (BOOL)createDirAtPath:(NSString *)dirPath;

+ (NSFileHandle*)createWriteableFileAtPath:(NSString *)filePath;

// reflect the image make the height as passed
+ (UIImage*)reflectedImage:(UIImageView *)fromImage withHeight:(NSUInteger)height;

+ (UIImage*)makeRoundCornerImage : (UIImage*) img : (int) cornerWidth : (int) cornerHeight;

+ (BOOL)isLegitToReleaseImage;

+ (UIImage*)scaleToSize:(CGSize)size withImage:(UIImage *)myImage;

+ (UIImage*)genericGradientButtonImageWithHeight:(int)height color1:(int)color1 color2:(int)color2 color3:(int)color3;

+ (UIImage*)genericGradientButtonImageWithHeight:(int)height width:(int)width color1:(int)color1 color2:(int)color2 color3:(int)color3;

+ (UIImage*)gradientCellBackgroundImageWithWidth:(int)width height:(int)height white1:(int)white1 white2:(int)white2;

+ (UIImage*)primaryThemeColorGradientButtonImage:(int)height :(int)width;

+ (UIColor*)getUserDefinedColorWithWhiteValue:(int)whiteVal;

+ (int)addWhiteWithColorInt:(int)colorInt withWhiteInt:(int)white;

+ (CGImageRef)CreateGradientImageWithWidth:(int)pixelsWide withHeight:(int)pixelsHigh;

+ (CGContextRef)MyCreateBitmapContextWithWidth:(int)pixelsWide withHeight:(int)pixelsHigh;

/** Unzip file to directory */
+ (BOOL)unZipFile:(NSString *)filePath toDir:(NSString *)destPath;





@end
