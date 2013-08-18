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

 Adapted from http://stackoverflow.com/questions/3997976/parse-nsurl-query-property
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "NSObject+QueryComponents.h"

@implementation NSString (QueryComponents)
- (NSString *)stringByDecodingURLFormat
{
    NSString *result = [self stringByReplacingOccurrencesOfString:@"+" withString:@" "];
    result = [result stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    return result;
}

// TODO: This doesn't escape all characters!
- (NSString *)stringByEncodingURLFormat
{
    NSString *result = [self stringByReplacingOccurrencesOfString:@" " withString:@"+"];
    result = [result stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    return result;
}

- (NSMutableDictionary *)dictionaryFromQueryComponents
{
    NSMutableDictionary *queryComponents = [NSMutableDictionary dictionary];
    for (NSString *keyValuePairString in [self componentsSeparatedByString:@"&"])
    {
        NSArray *keyValuePairArray = [keyValuePairString componentsSeparatedByString:@"="];

        if ([keyValuePairArray count] < 2) continue; // Verify that there is at least one key, and at least one value.  Ignore extra = signs

        NSString *key    = [[keyValuePairArray objectAtIndex:0] stringByDecodingURLFormat];
        NSString *value  = [[keyValuePairArray objectAtIndex:1] stringByDecodingURLFormat];
        id        result = [queryComponents objectForKey:key]; // URL spec says that multiple values are allowed per key

        if (!result) { // First object
            [queryComponents setObject:value forKey:key];
        } else if (![result isKindOfClass:[NSMutableArray class]]) {
            NSMutableArray *results = [NSMutableArray arrayWithObjects:result, value, nil];
            [queryComponents setObject:results forKey:key];
        } else {
            NSMutableArray *results = (NSMutableArray *)result;
            [results addObject:value];
        }
    }
    return queryComponents;
}
@end

@implementation NSURL (QueryComponents)
- (NSMutableDictionary *)queryComponents
{
    return [[self query] dictionaryFromQueryComponents];
}
@end

@implementation NSDictionary (QueryComponents)
- (NSString *)stringFromQueryComponents
{
    NSString *result = nil;
    for (NSString *key in [self allKeys])
    {
        key = [key stringByEncodingURLFormat];
        NSArray *allValues = [self objectForKey:key];

        if ([allValues isKindOfClass:[NSArray class]]) {
            for (NSString *value in allValues)
            {
                value = [[value description] stringByEncodingURLFormat];

                if (!result)
                    result = [NSString stringWithFormat:@"%@=%@",key,value];
                else
                    result = [result stringByAppendingFormat:@"&%@=%@",key,value];
            }
        } else {
            NSString *value = [[allValues description] stringByEncodingURLFormat];

            if (!result)
                result = [NSString stringWithFormat:@"%@=%@",key,value];
            else
                result = [result stringByAppendingFormat:@"&%@=%@",key,value];
        }
    }

    return result;
}
@end
