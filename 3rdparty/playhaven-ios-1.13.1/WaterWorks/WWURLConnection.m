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

 WWURLConnection.m
 WaterWorks

 Created by Jesus Fernandez on 1/31/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "WWURLConnection.h"
#import "WWURLMatching.h"

NSString *readLineAsNSString(FILE *file);

NSString *readLineAsNSString(FILE *file)
{
    char buffer[4096];

    // Tune this capacity to your liking -- larger buffer sizes will be faster, but
    // use more memory
    NSMutableString *result = [NSMutableString stringWithCapacity:256];

    // Read up to 4095 non-newline characters
    int charsRead; // TODO: How can this method possibly work?  charsRead is never initialized, and doesn't appear to be getting changed anywhere. Does it go on forever?
    do
    {
        if (fgets(buffer, 4095, file) != NULL) {
            // Remove newline from the end of the buffer
            NSString *line = [NSString stringWithFormat:@"%s", buffer];
            line = [line stringByTrimmingCharactersInSet:[NSCharacterSet newlineCharacterSet]];

            [result appendString:line];
        }
        else
            break;
    } while (charsRead == 4095);

    return result;
}

@protocol WWURLResponse <NSObject>
- (NSData *)data;
@end

@interface WWURLMemoryResponse : NSObject<WWURLResponse>
@property (nonatomic, retain) NSData *data;
@end

@implementation WWURLMemoryResponse
@synthesize data = _data;

- (void)dealloc
{
    [_data release], _data = nil;
    [super dealloc];
}
@end

@interface WWURLFileResponse : NSObject<WWURLResponse>
@property (nonatomic, copy) NSString *filePath;
@end

@implementation WWURLFileResponse
@synthesize filePath = _filePath;

- (NSData *)data
{
    return [NSData dataWithContentsOfFile:self.filePath];
}

- (void)dealloc
{
    [_filePath release], _filePath = nil;
    [super dealloc];
}
@end

@interface WWURLConnection (Private)
+ (NSMutableDictionary *)allResponses;
+ (NSData *)getResponseForURL:(NSURL *)url;
- (void)startInBackground;
+ (NSData *)bestResponseForURL:(NSURL *)url;

@end

@implementation WWURLConnection
@synthesize delegate = _delegate;
@synthesize request  = _request;

#pragma mark -
#pragma mark NSURLConnection substitution
+ (WWURLConnection *)connectionWithRequest:(NSURLRequest *)request delegate:(id)delegate
{
    WWURLConnection *result = [[[WWURLConnection alloc] init] autorelease];
    result.request  = request;
    result.delegate = delegate;

    return result;
}

#pragma mark -
#pragma mark Content redirection
+ (NSMutableDictionary *)allResponses
{
    static NSMutableDictionary *allResponses = nil;
    if (allResponses == nil) {
        allResponses = [[NSMutableDictionary alloc] init];
    }

    return allResponses;
}

+ (void)setResponse:(NSData *)response forURL:(NSURL *)url
{
    if (!!response) {
        WWURLMemoryResponse *responseObject = [WWURLMemoryResponse new];
        responseObject.data = response;

        [[self allResponses] setObject:responseObject forKey:url];
        [responseObject release];
    } else  {
        [[self allResponses] removeObjectForKey:url];
    }
}

+ (NSData *)getResponseForURL:(NSURL *)url
{
    id<WWURLResponse> responseObject = (id<WWURLResponse>)[[self allResponses] objectForKey:url];
    return [responseObject data];
}

+ (void)setResponsesFromFileNamed:(NSString *)fileName
{
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    const char *routesPath = [[resourcePath stringByAppendingPathComponent:fileName] UTF8String];
    FILE *file;
    if ((file = fopen(routesPath, "r")) != NULL) {
        while (!feof(file)) {
            // Read the next line ignoring comments
            NSString *line = readLineAsNSString(file);
            if ([line length] > 1 && ![[line substringToIndex:1] isEqualToString:@"#"]) {
                // Are there at least two tokens?
                NSArray *components = [line componentsSeparatedByString:@" "];
                if ([components count] >= 2) {
                    // Is the first component a valid URL?
                    NSURL *url = [NSURL URLWithString:[components objectAtIndex:0]];
                    if (!!url) {
                        // Is the second component a valid file?
                        NSString *responsePath = [resourcePath stringByAppendingPathComponent:[components objectAtIndex:1]];
                        if ([[NSFileManager defaultManager] fileExistsAtPath:responsePath]) {
                            WWURLFileResponse *responseObject = [WWURLFileResponse new];
                            responseObject.filePath = responsePath;
                            [[self allResponses] setObject:responseObject forKey:url];
                            [responseObject release];
                        }
                    }
                }
            }
        }
        fclose(file);
    }
}

+ (NSData *)bestResponseForURL:(NSURL *)url
{
    NSData *bestResponse = nil;
    NSInteger bestMatchingLevel = 0;

    // Iterate over the dictionary of responses to find a better matching level
    NSEnumerator *keyEnumerator = [[WWURLConnection allResponses] keyEnumerator];
    NSURL *matchURL;
    while ((matchURL = [keyEnumerator nextObject])) {
        NSInteger nextMatchingLevel = [WWURLMatching matchingLevelForURL:url withURL:matchURL];
        if (nextMatchingLevel > bestMatchingLevel) {
            bestResponse = [WWURLConnection getResponseForURL:matchURL],
            bestMatchingLevel = nextMatchingLevel;
        }
    }

    return bestResponse;
}

+ (void)clearAllResponses
{
    [[self allResponses] removeAllObjects];
}

- (void)dealloc
{
    [[self class] cancelPreviousPerformRequestsWithTarget:self selector:@selector(startInBackground) object:nil];
    _delegate = nil;
    [_request release], _request = nil;

    //[super dealloc]; // TODO: Why is super commented out?
}

- (void)start
{
    [self performSelector:@selector(startInBackground) withObject:nil afterDelay:0.0];
}

- (void)cancel
{

}

- (void)startInBackground
{
    NSData *responseData = [WWURLConnection bestResponseForURL:self.request.URL];
    if ([self.delegate respondsToSelector:@selector(connection:didReceiveResponse:)]) {
        NSInteger statusCode = (!!responseData) ? 200: 404;

        NSHTTPURLResponse *response = nil;
        if ([[NSHTTPURLResponse class] instancesRespondToSelector:@selector(initWithURL:statusCode:HTTPVersion:headerFields:)]) {
            response = [[NSHTTPURLResponse alloc] initWithURL:self.request.URL statusCode:statusCode HTTPVersion:nil headerFields:nil];
        }

        [self.delegate connection:nil didReceiveResponse:response];
        [response release];
    }

    if ([self.delegate respondsToSelector:@selector(connection:didReceiveData:)]) {
        [self.delegate connection:nil didReceiveData:responseData];
    }

    if ([self.delegate respondsToSelector:@selector(connectionDidFinishLoading:)]) {
        [self.delegate connectionDidFinishLoading:nil];
    }
}
@end
