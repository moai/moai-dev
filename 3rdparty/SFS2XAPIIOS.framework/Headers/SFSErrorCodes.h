//
//  SFSErrorCodes.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

/** Provides a means of translation between Server error codes and their relative error messages.
 
 Error messages are provided by defualt in the English language but they can be localized and substituted in any other language.
 The error messages contain special placeholders that are processed at runtime and substituted with runtime data. They are in the form of a number enclosed in curly brackets such as: {0}, {1} etc... 
 Please be sure to maintain these placeholders while translating the messages. 
 */
@interface SFSErrorCodes : NSObject {

}

+(void)setErrorMessage:(NSInteger)code message:(NSString *)message;
+(NSString *)getErrorMessage:(NSInteger)code params:(NSArray *)params;

@end
