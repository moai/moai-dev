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

 PHConnectionManager.m
 playhaven-sdk-ios

 Created by Lilli Szafranski on 1/31/13.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHConnectionManager.h"
#import "PHConstants.h"

#ifdef PH_USE_NETWORK_FIXTURES
#import "WWURLConnection.h"
#endif



@interface PHConnectionBundle : NSObject
@property (retain)   NSURLRequest  *request;
@property (retain)   NSURLResponse *response;
@property (retain)   NSMutableData *data;
@property (readonly) id             context;
@property (readonly) id<PHConnectionManagerDelegate> delegate;
@end

@implementation PHConnectionBundle
@synthesize request  = _request;
@synthesize response = _response;
@synthesize data     = _data;
@synthesize context  = _context;
@synthesize delegate = _delegate;

- (id)initWithRequest:(NSURLRequest *)request forDelegate:(id <PHConnectionManagerDelegate>)delegate withContext:(id)context
{
    if ((self = [super init]))
    {
        _request  = [request retain];
        _context  = [context retain];

        _response = nil;
        _data     = nil;

        _delegate = [delegate retain];
    }

    return self;
}

+ (id)connectionBundleWithRequest:(NSURLRequest *)request forDelegate:(id <PHConnectionManagerDelegate>)delegate withContext:(id)context
{
    return [[[PHConnectionBundle alloc] initWithRequest:request forDelegate:delegate withContext:context] autorelease];
}

- (void)dealloc
{
    [_request release];
    [_response release];
    [_data release];
    [_delegate release];
    [_context release];

    [super dealloc];
}
@end

@interface PHConnectionManager ()
@property CFMutableDictionaryRef connections;
@end

@implementation PHConnectionManager
@synthesize connections      = _connections;

static PHConnectionManager *singleton = nil;

- (id)init
{
    if ((self = [super init]))
    {
        _connections = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                 &kCFTypeDictionaryKeyCallBacks,
                                                 &kCFTypeDictionaryValueCallBacks);
    }

    return self;
}

+ (id)sharedInstance
{
    if (singleton == nil) {
        singleton = [((PHConnectionManager *)[super allocWithZone:NULL]) init];
    }

    return singleton;
}

+ (id)allocWithZone:(NSZone *)zone
{
    return [[self sharedInstance] retain];
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)retain
{
    return self;
}

- (NSUInteger)retainCount
{
    return NSUIntegerMax;
}

- (oneway void)release { }

- (id)autorelease
{
    return self;
}

// Here just cuz...
- (void)dealloc
{
    CFRelease(_connections);

    [super dealloc];
}

+ (BOOL)createConnectionFromRequest:(NSURLRequest *)request forDelegate:(id <PHConnectionManagerDelegate>)delegate withContext:(id)context
{
    PHConnectionManager *connectionManager = [PHConnectionManager sharedInstance];

    if (![NSURLConnection canHandleRequest:request])
        return NO;

    NSURLConnection *connection;

#ifdef PH_USE_NETWORK_FIXTURES // TODO: Will there be an issue with this starting before the connection bundle is created and saved? Nvm, this doesn't start it immediately... for shame...
    connection = [WWURLConnection connectionWithRequest:request delegate:self];
#else
    connection = [[[NSURLConnection alloc] initWithRequest:request
                                                  delegate:connectionManager
                                          startImmediately:NO] autorelease];
#endif

    if (!connection)
        return NO;

    PHConnectionBundle *connectionBundle = [PHConnectionBundle connectionBundleWithRequest:request
                                                                               forDelegate:delegate
                                                                               withContext:context];

    CFDictionaryAddValue(connectionManager.connections,
                         connection,
                         connectionBundle);

    [connection start];

    return YES;
}

+ (void)stopConnectionsForDelegate:(id<PHConnectionManagerDelegate>)delegate
{
    //DLog(@"");

    PHConnectionManager *connectionManager = [PHConnectionManager sharedInstance];
    PHConnectionBundle  *connectionBundle  = nil;

    for (NSURLConnection *connection in [(NSMutableDictionary *)connectionManager.connections allKeys])
    {
        connectionBundle = [(PHConnectionBundle *) CFDictionaryGetValue(connectionManager.connections, connection) retain];

        // TODO: This code should only be called if the connection is still live, to avoid releasing the connection bundle too soon; Then remove the retain/release stuff
        if ([connectionBundle delegate] == delegate)
        {
            [connection cancel];

            if ([delegate respondsToSelector:@selector(connectionWasStoppedForRequest:context:)])
                [delegate connectionWasStoppedForRequest:[connectionBundle request] context:[connectionBundle context]];

            CFDictionaryRemoveValue(connectionManager.connections, connection);
        }
        [connectionBundle release];
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    //DLog(@"");

    if ([response isKindOfClass:[NSHTTPURLResponse class]]) {
        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
        PH_LOG(@"Request recieved HTTP response: %d", [httpResponse statusCode]);
    }

    PHConnectionBundle *connectionBundle = (PHConnectionBundle *)CFDictionaryGetValue(self.connections, connection);

    [connectionBundle setData:[[[NSMutableData alloc] init] autorelease]];

    connectionBundle.response = response;
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    [[(PHConnectionBundle *)CFDictionaryGetValue(self.connections, connection) data] appendData:data];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    PHConnectionBundle *connectionBundle = [(PHConnectionBundle *) CFDictionaryGetValue(self.connections, connection) retain];

    NSURLRequest  *request  = [connectionBundle request];
    NSURLResponse *response = [connectionBundle response];
    NSData        *data     = [connectionBundle data];
    id             context  = [connectionBundle context];
    id<PHConnectionManagerDelegate> delegate = [connectionBundle delegate];

    if ([delegate respondsToSelector:@selector(connectionDidFinishLoadingWithRequest:response:data:context:)])
        [delegate connectionDidFinishLoadingWithRequest:request response:response data:data context:context];

    CFDictionaryRemoveValue(self.connections, connection);
    [connectionBundle release];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    //DLog(@"");
    PHConnectionBundle *connectionBundle = [(PHConnectionBundle *) CFDictionaryGetValue(self.connections, connection) retain];

    NSURLRequest *request  = [connectionBundle request];
    id            context  = [connectionBundle context];
    id<PHConnectionManagerDelegate> delegate = [connectionBundle delegate];

    if ([delegate respondsToSelector:@selector(connectionDidFailWithError:request:context:)])
        [delegate connectionDidFailWithError:error request:request context:context];

    CFDictionaryRemoveValue(self.connections, connection);
    [connectionBundle release];
}

- (NSURLRequest *)connection:(NSURLConnection *)connection willSendRequest:(NSURLRequest *)request
                                                          redirectResponse:(NSURLResponse *)redirectResponse
{
    // TODO: Figure out if we should save this response or not...
    PHConnectionBundle *connectionBundle = (PHConnectionBundle *)CFDictionaryGetValue(self.connections, connection);
    connectionBundle.response = redirectResponse;

    return request;
}

- (NSCachedURLResponse *)connection:(NSURLConnection *)connection willCacheResponse:(NSCachedURLResponse *)cachedResponse
{
    return cachedResponse;
}

- (void)connection:(NSURLConnection *)connection didCancelAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge  { /*DLog(@"");*/ }
- (void)connection:(NSURLConnection *)connection didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge { /*DLog(@"");*/ }
//- (NSCachedURLResponse *)connection:(NSURLConnection *)connection willCacheResponse:(NSCachedURLResponse *)cachedResponse    { DLog(@""); return cachedResponse; }
- (void)connection:(NSURLConnection *)connection didSendBodyData:(NSInteger)bytesWritten
                                               totalBytesWritten:(NSInteger)totalBytesWritten
                                       totalBytesExpectedToWrite:(NSInteger)totalBytesExpectedToWrite
{ /*DLog(@"bytesWritten: %d, totalBytesWritten: %d, totalBytesExpected: %d", bytesWritten, totalBytesWritten, totalBytesExpectedToWrite);*/ }
@end
