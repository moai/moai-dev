// NSStringExtras.m -- NSString extras
// by allen brunson  january 9 2005

#import "NSStringExtras.h"

static NSCharacterSet* csetFromChar(unichar ch)
{
    NSString* text = [NSString stringWithCharacters:&ch length:1];
    return [NSCharacterSet characterSetWithCharactersInString:text];
}

@implementation NSString (NSStringExtras)

// find first occurrence of char 'ch' and return its range, with a length
// of 1. if the char can't be found, return a range with location NSNotFound,
// length 0

-(NSRange)findFirstChar:(unichar)ch
{
    return [self rangeOfCharacterFromSet:csetFromChar(ch)];
}

+(NSArray*)lines:(NSString*)string
{
    if (VGStringIsEmpty(string))
    {
        return [NSArray arrayWithObject:@""];
    }
    else
    {
        return [string componentsSeparatedByString:@"\n"];
    }    
}

+(NSString*)stringWithBool:(BOOL)boolValue
{
    return [NSString stringWithFormat:@"%d", boolValue];
}

+(NSString*)stringWithData:(NSData*)data
{
    const void*             dptr = data.bytes;
    const NSUInteger        dsiz = data.length;
    const NSStringEncoding  nenc = NSUTF8StringEncoding;
    NSString*               text = nil;
    
    if ((dptr != NULL) && (dsiz >= 1))
    {
        text = [NSString alloc];
        text = [text initWithBytes:dptr length:dsiz encoding:nenc];
        [text autorelease];
        
        return text;
    }
    else
    {
        return @"";
    }
}

+(NSString*)stringWithFloat:(float)floatValue
{
    return [NSString stringWithFormat:@"%f", floatValue];
}

+(NSString*)stringWithInteger:(NSInteger)integer
{
    return [NSString stringWithFormat:@"%d", integer];
}

// remove leading and trailing whitespace from a string. if the string
// doesn't have any, it will be returned unaltered

+(NSString*)trimWhitespace:(NSString*)string
{
    NSCharacterSet*  cset = nil;
    NSRange          rang;
    
    if (!VGStringHasLength(string))
    {
        return @"";
    }    
    
    cset = [NSCharacterSet whitespaceAndNewlineCharacterSet];
    rang = [string rangeOfCharacterFromSet:cset];    
    
    if (rang.location == NSNotFound)
    {
        return [NSString stringWithString:string];
    }
    else
    {    
        return [string stringByTrimmingCharactersInSet:cset];
    }    
}

@end

@implementation NSMutableString (NSMutableStringExtras)

// remove the oldest line in the string

-(void)removeOldestLine
{
    const NSRange  rang = [self findFirstChar:'\n'];
    
    if (rang.location == NSNotFound)
    {
        [self setString:@""];
    }
    else
    { 
        [self deleteCharactersInRange:NSMakeRange(0, rang.location + 1)];
    }    
}

@end
