// NSStringExtras.h -- NSString extras
// by allen brunson  january 9 2005


/******************************************************************************/
/*                                                                            */
/***  inline functions                                                      ***/
/*                                                                            */
/******************************************************************************/

NS_INLINE BOOL VGIsString(NSString* string)
{
    if (!string) return FALSE;
    
    if ([string isKindOfClass:[NSString class]])        return TRUE;
    if ([string isKindOfClass:[NSMutableString class]]) return TRUE;
    
    return FALSE;
}

NS_INLINE NSString* VGSanitizeString(NSString* string)
{
    if (VGIsString(string))
    {
        return string;
    }
    else
    {
        return @"";
    }
}

NS_INLINE BOOL VGStringHasLength(NSString* string)
{
    return VGIsString(string) && (string.length >= 1);
}

NS_INLINE BOOL VGStringIsEmpty(NSString* string)
{
    return VGStringHasLength(string) == FALSE;
}


/******************************************************************************/
/*                                                                            */
/***  NSStringExtras category                                               ***/
/*                                                                            */
/******************************************************************************/

@interface NSString (NSStringExtras)

// class methods

+(NSArray*)lines:(NSString*)string;

+(NSString*)stringWithBool:(BOOL)boolValue;
+(NSString*)stringWithData:(NSData*)data;
+(NSString*)stringWithFloat:(float)floatValue;
+(NSString*)stringWithInteger:(NSInteger)integer;

+(NSString*)trimWhitespace:(NSString*)string;

// instance methods

-(NSRange)findFirstChar:(unichar)ch;

@end


/******************************************************************************/
/*                                                                            */
/***  macros                                                                ***/
/*                                                                            */
/******************************************************************************/

// shortcut for trim function

NS_INLINE NSString* VGTrim(NSString* text)
{
    return [NSString trimWhitespace:text];
}


/******************************************************************************/
/*                                                                            */
/***  NSMutableStringExtras category                                        ***/
/*                                                                            */
/******************************************************************************/

@interface NSMutableString (NSMutableStringExtras)

-(void)removeOldestLine;

@end


/******************************************************************************/
/*                                                                            */
/***  NSStringExtras module                                                 ***/
/*                                                                            */
/******************************************************************************

overview
--------

few extra methods for NSString

*/
