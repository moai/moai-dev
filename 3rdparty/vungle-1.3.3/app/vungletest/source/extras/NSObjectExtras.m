// NSObjectExtras.m -- NSObject extras
// by allen brunson  july 9 2010

#import "NSObjectExtras.h"

@implementation NSObject (NSObjectExtras)

-(NSString*)className
{
    return NSStringFromClass([self class]);
}

-(void)performSelector:(SEL)proc afterDelay:(NSTimeInterval)delay
{
    [self performSelector:proc withObject:nil afterDelay:delay];
}

-(void)performSelectorOnMainThread:(SEL)proc
{
    [self performSelectorOnMainThread:proc withObject:nil waitUntilDone:FALSE];
}

-(void)performSelectorOnMainThread:(SEL)proc withObject:(id)nobj
{
    [self performSelectorOnMainThread:proc withObject:nobj waitUntilDone:FALSE];
}

@end
