// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJDebugTextView.h"

@interface TJDebugTextView ()

@property (assign) CGRect originalFrame;

@end

@implementation TJDebugTextView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];

    if (self)
	{

    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
	self = [super initWithCoder:aDecoder];
	
	if (self)
	{
		_originalFrame = self.bounds;
	}
	
	return self;
}

- (void)log:(NSString *)format, ...
{
    // Make string from the format and vars provided
    va_list args;
    va_start(args, format);
    NSString *s = [[NSString alloc] initWithFormat:format arguments:args];
    va_end(args);
	
	// Append string onto our debug log
    [self setText:[NSString stringWithFormat: @"%@\n%@", [self text], s]];

	CGRect frame = self.frame;
	frame.size.height = 200;
	self.frame = frame;
}




@end
