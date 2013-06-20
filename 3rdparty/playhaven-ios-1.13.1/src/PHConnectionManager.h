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

 PHConnectionManager.h
 playhaven-sdk-ios

 Created by Lilli Szafranski on 1/31/13.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>


@protocol PHConnectionManagerDelegate <NSObject>
@optional
- (void)connectionDidFinishLoadingWithRequest:(NSURLRequest *)request
                                     response:(NSURLResponse *)response
                                         data:(NSData *)data
                                      context:(id)context;
- (void)connectionDidFailWithError:(NSError *)error request:(NSURLRequest *)request context:(id)context;
- (void)connectionWasStoppedForRequest:(NSURLRequest *)request context:(id)context;
@end

@interface PHConnectionManager : NSObject
+ (BOOL)createConnectionFromRequest:(NSURLRequest *)request
                        forDelegate:(id<PHConnectionManagerDelegate>)delegate
                        withContext:(id)context;

+ (void)stopConnectionsForDelegate:(id<PHConnectionManagerDelegate>)delegate;
@end

