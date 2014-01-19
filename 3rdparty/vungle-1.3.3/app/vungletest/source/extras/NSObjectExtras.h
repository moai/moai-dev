// NSObjectExtras.h -- NSObject extras
// by allen brunson  july 9 2010


/******************************************************************************/
/*                                                                            */
/***  NSObject inline functions                                             ***/
/*                                                                            */
/******************************************************************************/

NS_INLINE BOOL VGConformsTo(id object, Protocol* protocol)
{
    return object && protocol && [object conformsToProtocol:protocol];
}

NS_INLINE BOOL VGIsKindOf(id object, Class classType)
{
    return object && classType && [object isKindOfClass:classType];
}

NS_INLINE id VGObjectOfType(id object, Class classType)
{
    if (VGIsKindOf(object, classType))
    {
        return object;
    }
    else
    {
        return nil;
    }
}

NS_INLINE BOOL VGRespondsTo(id target, SEL selector)
{
    return target && selector && [target respondsToSelector:selector];
}

NS_INLINE BOOL VGPerformIfAble(id target, SEL selector)
{
    const BOOL rval = VGRespondsTo(target, selector);
    if (rval) [target performSelector:selector];
    return rval;
}

NS_INLINE BOOL VGPerformIfAbleWithObject(id target, SEL selector, id object)
{
    const BOOL rval = VGRespondsTo(target, selector);
    if (rval) [target performSelector:selector withObject:object];
    return rval;
}


/******************************************************************************/
/*                                                                            */
/***  simple duplication functions                                          ***/
/*                                                                            */
/******************************************************************************/

NS_INLINE BOOL VGDupeBool(BOOL boolean)
{
    return boolean;
}

NS_INLINE NSInteger VGDupeInteger(NSInteger integer)
{
    return integer;
}

NS_INLINE id VGDupeObject(id object)
{
    if (!object) return nil;
    return [[object copy] autorelease];
}


/******************************************************************************/
/*                                                                            */
/***  NSObjectExtras category                                               ***/
/*                                                                            */
/******************************************************************************/

@interface NSObject (NSObjectExtras)

-(NSString*)className;

-(void)performSelector:(SEL)proc afterDelay:(NSTimeInterval)delay;
-(void)performSelectorOnMainThread:(SEL)proc;
-(void)performSelectorOnMainThread:(SEL)proc withObject:(id)nobj;

@end


/******************************************************************************/
/*                                                                            */
/***  NSObjectExtras module                                                 ***/
/*                                                                            */
/******************************************************************************

overview
--------

few extra methods for NSObject

*/
