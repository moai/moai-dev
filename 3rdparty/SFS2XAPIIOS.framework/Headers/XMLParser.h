//
//  UDPController.m
//  SFS2X
//
//  Created by Wayne Helman on 12-01-20.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface XMLParser : NSObject <NSXMLParserDelegate>
{
    NSError **_error;
    NSMutableArray *_nodeStack;
    NSMutableString *_nodeText;
}

/*
 Parses XML data from an NSData source
 
    NSDictionary *data = [XMLParser parseXMLData:xmlData error:&parseError];
 
 */
+ (NSDictionary *)parseXMLData:(NSData *)data error:(NSError **)error;

/*
 Parses XML data from a String source
 
    NSDictionary *data = [XMLParser parseXMLString:xmlString error:&parseError];
 
 */
+ (NSDictionary *)parseXMLString:(NSString *)string error:(NSError **)error;

@end
