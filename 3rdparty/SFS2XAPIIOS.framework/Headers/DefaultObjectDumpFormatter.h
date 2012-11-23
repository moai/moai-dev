//
//  DefaultObjectDumpFormatter.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"

EXTERN NSString * const DefaultObjectDumpFormatter_TOKEN_INDENT_OPEN;
EXTERN NSString * const DefaultObjectDumpFormatter_TOKEN_INDENT_CLOSE;
EXTERN NSString * const DefaultObjectDumpFormatter_TOKEN_DIVIDER;

EXTERN NSString * const DefaultObjectDumpFormatter_NEW_LINE;
EXTERN NSString * const DefaultObjectDumpFormatter_TAB;
EXTERN NSString * const DefaultObjectDumpFormatter_DOT;

EXTERN NSInteger const DefaultObjectDumpFormatter_HEX_BYTES_PER_LINE;


@interface DefaultObjectDumpFormatter : NSObject {
}

+(NSString *)prettyPrintDump:(NSString *)dump;

+(NSString *)hexDump:(NSData *)data bytesPerLine:(NSInteger)bytesPerLine;

@end
