// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCCoreFetcher.h"

@implementation TJCCoreFetcher

@synthesize hasFetched = hasFetched_;
@synthesize baseURL = baseURL_;
@synthesize alternateURL = alternateURL_;
@synthesize bindings = bindings_;
@synthesize error = error_;
@synthesize data = data_;
@synthesize POSTdata = POSTdata_;
@synthesize responseCode = responseCode_;
@synthesize requestTimeout = requestTimeout_;
@synthesize connection = connection_;
@synthesize invocation = invocation_;
@synthesize requestMethod = requestMethod_;
@synthesize postParameters = postParameters_;
@synthesize retryCount = retryCount_;
@synthesize requestTag = requestTag_;



- (id)init
{
	if (!(self = [super init])) 
	{
		return nil;
	}
	
	retryCount_ = 0;
	requestMethod_ = [[NSString alloc] initWithString:@"GET"];
	
	return self;
}


- (void)fetchAsynchronouslyWithCompletionInvocation:(NSInvocation*)myInvocation
{
	if (invocation_)
	{
		[invocation_ release];
		invocation_ = nil;
	}
	
	invocation_ = [myInvocation retain];
	[self initiateConnection];
}


- (void)initiateConnection
{
	NSURL *myURL;      
	NSMutableURLRequest *urlRequest;
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"Final URL = %@ ",[self requestURL]];
	
	myURL = [[NSURL alloc] initWithString:[self requestURL]];  // allocing url 2nd
	
	urlRequest = [NSMutableURLRequest requestWithURL: myURL
													 cachePolicy: NSURLRequestReloadIgnoringLocalAndRemoteCacheData
												timeoutInterval: requestTimeout_];
	
	[urlRequest setHTTPMethod:requestMethod_];
	
	if (POSTdata_)
	{
		[urlRequest setHTTPBody:POSTdata_];
	}
	
	[myURL release];
	
	// Init the data to handle the response coming back from the server.
	if (data_) 
	{
		[data_ release];
	}
	data_ = [[NSMutableData alloc] init];
	
	connection_ = [[NSURLConnection alloc] initWithRequest: urlRequest delegate: self];
}


- (NSCachedURLResponse*)connection:(NSURLConnection*)connection willCacheResponse:(NSCachedURLResponse*)cachedResponse 
{
	// Returning nil will ensure that no cached response will be stored for the connection.
	// This is in case the cache is being used by something else.
	return nil;
}


- (NSString*)createQueryStringFromDict:(NSDictionary*)paramDict
{
	if (!paramDict)
	{
		return nil;
	}
	
	if ([paramDict count] == 0)
	{
		return nil;
	}
	
	NSMutableArray *parts = [NSMutableArray array];
	
	for (id key in [paramDict allKeys])
	{
		id value = [paramDict objectForKey: key];
		
		// Encode string to a legal URL string.
		NSString *encodedString = (NSString*)CFURLCreateStringByAddingPercentEscapes(NULL,
																											  (CFStringRef)value,
																											  NULL,
																											  (CFStringRef)@"!*'();:@&=+$,/?%#[]",
																											  kCFStringEncodingUTF8);
		
		NSString *part = [NSString stringWithFormat: @"%@=%@", key, encodedString];
		
		[encodedString release];
		
		[parts addObject: part];
	}
	return [parts componentsJoinedByString: @"&"];
}


- (NSString*)requestURL
{
	NSString *result = nil;
	NSString *requestURL;
	
	if (retryCount_ == 0)
	{
		requestURL = baseURL_;
	}
	else
	{
		requestURL = alternateURL_;
	}
	
	NSAssert3([requestURL length], @"%s: %d; %s; Precondition violated - [baseURL length] == 0",  __FILE__, __LINE__, __PRETTY_FUNCTION__);
	
	if (!bindings_) 
	{
		result = requestURL;
	} 
	else 
	{
		//result = [NSString stringWithFormat: @"%@?%@", requestURL, [self urlEncodedBindings]];
		result = [NSString stringWithFormat: @"%@?%@", requestURL, [self createQueryStringFromDict:bindings_]];
	}
	
	return result;
}


- (NSString*)urlEncodedBindings
{
	NSMutableString * result = [NSMutableString string];
	NSEnumerator * enumerator = [[bindings_ allKeys] objectEnumerator];
	NSString * bindingKey = nil;
	
	while ((bindingKey = [enumerator nextObject]))
	{
		id bindingValue = [bindings_ objectForKey: bindingKey];
		NSString * bindingString = nil;
		
		// confirm that we have a string
		if (![bindingValue isKindOfClass: [NSString class]])
		{
			if ([bindingValue respondsToSelector: @selector(stringValue)]) 
			{
				bindingValue = [bindingValue stringValue];
			} 
			else
			{
				bindingValue = [bindingValue description];
			}
		}
		
		// if we got here, we should have a string
		if (![bindingValue length]) 
		{
			bindingString = [NSString stringWithFormat: @"&%@=%@", bindingKey, bindingValue];
			[result appendString: bindingString];
			continue;
		}
		
		if ([result length]) 
		{
			// not the first binding - so prepend an &
			bindingString = [NSString stringWithFormat: @"&%@=%@", bindingKey, bindingValue];
		} 
		else 
		{
			// first binding
			bindingString = [NSString stringWithFormat: @"%@=%@", bindingKey, bindingValue];
		}
		
		if (bindingString) 
		{
			[result appendString: bindingString];
		}
		else
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"Error in URLEncodedBindings"];
		}
	}
	return result;
}


- (void)retry
{
	retryCount_++;
	if (alternateURL_ && invocation_)
	{
		[self fetchAsynchronouslyWithCompletionInvocation:invocation_];
	}
}


- (BOOL)hasError
{
	if (error_) 
	{
		return YES;
	}
	
	return NO;
}


- (void)dealloc
{	
	[baseURL_ release];
	[alternateURL_ release];
	[bindings_ release];
	[error_ release];
	[data_ release];
	[connection_ release];
	[invocation_ release];
	[requestMethod_ release];
	[postParameters_ release];
	[super dealloc];
}





#pragma mark delegate methods for asynchronous requests

- (void)connection:(NSURLConnection *) myConnection didReceiveResponse:(NSURLResponse *) myResponse
{
	responseCode_ = [(NSHTTPURLResponse*)myResponse statusCode];
}


- (void)connection:(NSURLConnection *) myConnection didReceiveData:(NSData *) myData
{
	[data_ appendData: myData];
}


- (void)connection:(NSURLConnection *) myConnection didFailWithError:(NSError *) myError
{
	NSLog(@"baseURL:%@", baseURL_);
	
	if (retryCount_ < 1 && alternateURL_)
	{
		retryCount_++;
		
		//and what happens to the previous connection object ...must release it here
		[connection_ release];
		connection_ = nil;
		
		[self initiateConnection];
		return;
	}
	
	hasFetched_ = YES;
	if (error_)
	{
		[error_ release];
		error_ = nil;
	}
		 
	error_ = [myError retain];
	
	if (invocation_ != nil)
	{
		[invocation_ invoke];
		
		[invocation_ release];
		invocation_ = nil;
	}
}

- (void)connectionDidFinishLoading:(NSURLConnection *) myConnection
{
	[connection_ release];
	connection_ = nil;
	
	hasFetched_ = YES;

	if (invocation_ != nil)
	{
		[invocation_ invoke];
		
		[invocation_ release];
		invocation_ = nil;
	}
}

@end
