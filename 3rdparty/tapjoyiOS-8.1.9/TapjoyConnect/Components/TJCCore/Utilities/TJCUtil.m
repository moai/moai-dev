// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCUtil.h"
#import "TJCConstants.h"
#import "TJCLog.h"

#define SHOW_REWARD_PROMPT_KEY @"TJCShouldUserSeePrompt"
#define SHOW_PROMPT_AFTER_KEY @"TJCShowPromptAfter"
#define NO_SELECTED_COUNT_KEY @"TJCNoSelectedCount"


@implementation TJCUtil


+ (BOOL)isPad
{
#ifdef UI_USER_INTERFACE_IDIOM
	return (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
#else
	return NO;
#endif
}


+(BOOL) caseInsenstiveCompare:(NSString*) str1 AndString2:(NSString*)str2 
{
	if (str1 && str2 && [[str1 lowercaseString] isEqualToString:[str2 lowercaseString]]) 
	{
		return true;
	}
	return false;
}


+ (NSData *) dataWithBase64EncodedString:(NSString *) string; 
{
	//	NSData *dataObj = [NSData allocWithZone:nil];
	NSData *dataObj = [self initWithBase64EncodedString:string];
	return dataObj;
}

+ (id) initWithBase64EncodedString:(NSString *) string; 
{
	NSMutableData *mutableData = nil;
	
	if( string ) 
	{
		unsigned long ixtext = 0;
		unsigned long lentext = 0;
		unsigned char ch = 0;
		unsigned char inbuf[4] = {0};
        unsigned char outbuf[3] = {0};
		short i = 0, ixinbuf = 0;
		BOOL flignore = NO;
		BOOL flendtext = NO;
		NSData *base64Data = nil;
		const unsigned char *base64Bytes = nil;
		
		// Convert the string to ASCII data.
		base64Data = [string dataUsingEncoding:NSASCIIStringEncoding];
		base64Bytes = [base64Data bytes];
		mutableData = [NSMutableData dataWithCapacity:[base64Data length]];
		lentext = [base64Data length];
		
		while( YES )
		{
			if ( ixtext >= lentext ) 
                break;
			ch = base64Bytes[ixtext++];
			flignore = NO;
			
			if ( ( ch >= 'A' ) && ( ch <= 'Z' ) ) 
                ch = ch - 'A';
			else if ( ( ch >= 'a' ) && ( ch <= 'z' ) ) 
                ch = ch - 'a' + 26;
			else if ( ( ch >= '0' ) && ( ch <= '9' ) )
                ch = ch - '0' + 52;
			else if ( ch == '+' )
                ch = 62;
			else if ( ch == '=' )
                flendtext = YES;
			else if ( ch == '/' )
                ch = 63;
			else
                flignore = YES;
			
			if ( !flignore ) 
			{
				short ctcharsinbuf = 3;
				BOOL flbreak = NO;
				
				if( flendtext ) 
				{
					if ( ! ixinbuf ) 
                        break;
					if ( ( ixinbuf == 1 ) || ( ixinbuf == 2 ) ) 
                        ctcharsinbuf = 1;
					else 
                        ctcharsinbuf = 2;
					ixinbuf = 3;
					flbreak = YES;
				}
				
				inbuf [ixinbuf++] = ch;
				
				if( ixinbuf == 4 ) 
				{
					ixinbuf = 0;
					outbuf [0] = ( inbuf[0] << 2 ) | ( ( inbuf[1] & 0x30 ) >> 4 );
					outbuf [1] = ( ( inbuf[1] & 0x0F ) << 4 ) | ( ( inbuf[2] & 0x3C ) >> 2 );
					outbuf [2] = ( ( inbuf[2] & 0x03 ) << 6 ) | ( inbuf[3] & 0x3F );
					
					for( i = 0; i < ctcharsinbuf; i++ )
                    {
						[mutableData appendBytes:&outbuf[i] length:1];
                    }
				}
				
				if( flbreak )  
                    break;
			}
		}
	}
	
	//	self = [self initWithData:mutableData];
	return [[[NSData allocWithZone:nil] initWithData:mutableData] autorelease];
}


@end
