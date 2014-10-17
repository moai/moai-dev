// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJUtils.h"
#import <QuartzCore/QuartzCore.h>

@implementation TJUtils

// Rounded corners for views. http://stackoverflow.com/questions/4847163/round-two-corners-in-uiview
+ (UIView*)applyRoundedMaskToView:(UIView*)view
						radius_tl:(CGFloat)radius_tl
						radius_tr:(CGFloat)radius_tr
						radius_bl:(CGFloat)radius_bl
						radius_br:(CGFloat)radius_br
{
	CGRect rect = view.bounds;
	
    CGContextRef context;
    CGColorSpaceRef colorSpace;
	
    colorSpace = CGColorSpaceCreateDeviceRGB();
	
    // create a bitmap graphics context the size of the image
    context = CGBitmapContextCreate( NULL, rect.size.width, rect.size.height, 8, 0, colorSpace, kCGImageAlphaPremultipliedLast );
	
    // free the rgb colorspace
    CGColorSpaceRelease(colorSpace);
	
    if ( context == NULL ) {
        return NULL;
    }
	
    // cerate mask
	
    CGFloat minx = CGRectGetMinX( rect ), midx = CGRectGetMidX( rect ), maxx = CGRectGetMaxX( rect );
    CGFloat miny = CGRectGetMinY( rect ), midy = CGRectGetMidY( rect ), maxy = CGRectGetMaxY( rect );
	
    CGContextBeginPath( context );
    CGContextSetGrayFillColor( context, 1.0, 0.0 );
    CGContextAddRect( context, rect );
    CGContextClosePath( context );
    CGContextDrawPath( context, kCGPathFill );
	
    CGContextSetGrayFillColor( context, 1.0, 1.0 );
    CGContextBeginPath( context );
    CGContextMoveToPoint( context, minx, midy );
    CGContextAddArcToPoint( context, minx, miny, midx, miny, radius_bl );
    CGContextAddArcToPoint( context, maxx, miny, maxx, midy, radius_br );
    CGContextAddArcToPoint( context, maxx, maxy, midx, maxy, radius_tr );
    CGContextAddArcToPoint( context, minx, maxy, minx, midy, radius_tl );
    CGContextClosePath( context );
    CGContextDrawPath( context, kCGPathFill );
	
    // Create CGImageRef of the main view bitmap content, and then
    // release that bitmap context
    CGImageRef bitmapContext = CGBitmapContextCreateImage( context );
    CGContextRelease( context );
	
    // convert the finished resized image to a UIImage
    UIImage *theImage = [UIImage imageWithCGImage:bitmapContext];
    // image is retained by the property setting above, so we can
    // release the original
    CGImageRelease(bitmapContext);
	
	
    // Create a new layer that will work as a mask
    CALayer *layerMask = [CALayer layer];
    layerMask.frame = view.bounds;
    // Put the mask image as content of the layer
    layerMask.contents = (id)theImage.CGImage;
    // set the mask layer as mask of the view layer
    view.layer.mask = layerMask;
	
	return view;
}

+ (UIView*)applyTopRoundedCorners:(UIView*)view
{
	return [TJUtils applyRoundedMaskToView:view radius_tl:TJCornerRadius radius_tr:TJCornerRadius radius_bl:0 radius_br:0];
}

+ (UIView*)applyBottomRoundedCorners:(UIView*)view
{
	return [TJUtils applyRoundedMaskToView:view radius_tl:0 radius_tr:0 radius_bl:TJCornerRadius radius_br:TJCornerRadius];
}

+ (UIView*)applyRoundedCorners:(UIView*)view
{
	return [TJUtils applyRoundedMaskToView:view radius_tl:TJCornerRadius radius_tr:TJCornerRadius radius_bl:TJCornerRadius radius_br:TJCornerRadius];
}

+ (BOOL)isValidAppID:(NSString *)appID
{
	NSString *IDRegexString = @"^[a-zA-Z0-9]{8}-[a-zA-Z0-9]{4}-[a-zA-Z0-9]{4}-[a-zA-Z0-9]{4}-[a-zA-Z0-9]{12}$";
	NSError *error = nil;
	NSRegularExpression *IDRegex = [NSRegularExpression regularExpressionWithPattern:IDRegexString
																			 options:0
																			   error:&error];
	
	NSRange appIDRange = [IDRegex rangeOfFirstMatchInString:appID
													options:0
													  range:NSMakeRange(0, [appID length])];

	if (appIDRange.location == NSNotFound)
	{
		return NO;
	}
	
	return YES;
}

+ (BOOL)isValidCurrencyID:(NSString *)currencyID
{
	return [TJUtils isValidAppID:currencyID];
}

+ (BOOL)isValidSecretKey:(NSString *)secretKey
{
	NSString *keyRegexString = @"^[a-zA-Z0-9]{20}$";
	NSError *error = nil;
	NSRegularExpression *keyRegex = [NSRegularExpression regularExpressionWithPattern:keyRegexString
																			  options:0
																				error:&error];
	
	NSRange keyIDRange = [keyRegex rangeOfFirstMatchInString:secretKey
													 options:0
													   range:NSMakeRange(0, [secretKey length])];
	if (keyIDRange.location == NSNotFound)
	{
		return NO;
	}
	
	return YES;
}

@end
