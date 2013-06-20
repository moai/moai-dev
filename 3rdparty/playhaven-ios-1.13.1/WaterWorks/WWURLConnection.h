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

 WWURLConnection.h
 WaterWorks

 Created by Jesus Fernandez on 1/31/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>

//  WWURLConnection is a drop in replacement for NSURLConnection that subsitutes
//  network requests with arbitrary NSData or data from a file
@interface WWURLConnection : NSURLConnection

//  NSURLConnection replacement convenience method
+ (WWURLConnection *)connectionWithRequest:(NSURLRequest *)request delegate:(id)delegate;

//  Sets the response data for a given URL. See WWURLMatching to see how URL
//  matching works.
+ (void)setResponse:(NSData *)response forURL:(NSURL *)url;

//  Sets responses for multiple URLs from a file. See dev.wwfixtures to see how
//  that works.
+ (void)setResponsesFromFileNamed:(NSString *)fileName;

//  Clears all configured responses
+ (void)clearAllResponses;

//  NSURLConnection replacement delegate
@property (nonatomic, assign) id delegate;

//  NSURLConnection replacement request
@property (nonatomic, retain) NSURLRequest *request;
@end
