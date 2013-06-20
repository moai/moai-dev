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

 NSObject+QueryComponents.m
 playhaven-sdk-ios
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>

/**
 * @file
 * @internal
 * @brief Adapted from http://stackoverflow.com/questions/3997976/parse-nsurl-query-property.
 * Code by BadPirate (http://blog.logichigh.com)
 **/
//  TODO: Determine if this can be merged into PHStringUtil
@interface NSString (QueryComponents)
- (NSString *)stringByDecodingURLFormat;
- (NSString *)stringByEncodingURLFormat;
- (NSMutableDictionary *)dictionaryFromQueryComponents;
@end

@interface NSURL (QueryComponents)
- (NSMutableDictionary *)queryComponents;
@end

@interface NSDictionary (QueryComponents)
- (NSString *)stringFromQueryComponents;
@end
