/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 NSStringUtil.h
 playhaven-sdk-ios

 Created by Kurtiss Hare on 2/12/10.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>

/**
 * @internal
 *
 * @brief String utility class
 **/
@interface PHStringUtil : NSObject

/**
 * SEE: implementation for why you should not use this.
 **/
+ (NSString *)stringWithQueryQuirky:(NSDictionary *)params;

/**
 * Generates a unique uuid
 *
 * @return
 *   A unique uuid
 **/
+ (NSString *)uuid;

/**
 * Generates a URL query string using a dictionary of parameters
 *
 * @param params
 *   The params dictionary
 *
 * @return
 *   A URL query string
 **/
+ (NSString *)stringWithQuery:(NSDictionary *)params;

+ (NSString *)stringByHtmlEscapingString:(NSString *)input;
+ (NSString *)stringByUrlEncodingString:(NSString *)input;
+ (NSString *)stringByUrlDecodingString:(NSString *)input;

/**
 * Generates a dictionary from a URL query parameter string
 *
 * @param input
 *   A URL query parameter string
 *
 * @return
 *   A dictionary from a URL query parameter string
 **/
+ (NSDictionary *)dictionaryWithQueryString:(NSString *)input;

/**
 * @name String digest and encoding functions
 **/
/*@{*/
/**
 * String digest and encoding functions, mostly for signatures
 **/
+ (NSData *)dataDigestForString:(NSString *)input;
+ (NSString *)base64EncodedStringForData:(NSData *)data;
+ (NSString *)hexEncodedStringForData:(NSData *)data;
+ (NSString *)hexDigestForString:(NSString *)input;
+ (NSString *)b64DigestForString:(NSString *)input;
/*@}*/
@end
