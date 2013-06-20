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

 WWURLMatching.m
 WaterWorks

 Created by Jesus Fernandez on 1/31/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "WWURLMatching.h"

@implementation WWURLMatching
+ (NSInteger)matchingLevelForURL:(NSURL *)url1 withURL:(NSURL *)url2
{
    NSInteger matchingLevel = 0;

    // If the scheme, host, and path match, add 1 to the matching level.
    // Otherwise, url1 does not match url2 (level=0).
    NSString *urlPath1 = [NSString stringWithFormat:@"%@://%@%@", [url1 scheme], [url1 host], [url1 path]];
    NSString *urlPath2 = [NSString stringWithFormat:@"%@://%@%@", [url2 scheme], [url2 host], [url2 path]];

    if ([urlPath1 isEqualToString:urlPath2]) {
        matchingLevel++;
    } else {
        return 0;
    }

    NSArray *queryTokens1 = [[url1 query] componentsSeparatedByString:@"&"];
    NSArray *queryTokens2 = [[url2 query] componentsSeparatedByString:@"&"];

    // Are all tokens present in queryTokens2 in queryTokens1? If not, level=0.
    for (NSString *token in queryTokens2) {
        if (![queryTokens1 containsObject:token]) {
            return 0;
        }
    }

    // Second, add one point for each matching parameter in url1 that is found in url2
    for (NSString *token in queryTokens1) {
        if ([queryTokens2 containsObject:token]) {
            matchingLevel++;
        }
    }

    return matchingLevel;
}
@end
