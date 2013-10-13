//
//  CRFilter.h
//  Crittercism iOS Library
//
//  Created by Sean Hermany on 7/16/13.
//  Copyright (c) 2013 Crittercism, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CRFilter : NSObject

@property (readonly, assign) BOOL onlyScrubQuery;

// Convenience method to create a blacklisting filter
+ (CRFilter *)filterWithString:(NSString *)matchToken;

// Convenience method to create a filter which only scrubs the query portion
// of URLs
+ (CRFilter *)queryOnlyFilterWithString:(NSString *)matchToken;

// Initialize a filter that blacklists all URLs that match the specified token
- (id)initWithString:(NSString *)matchToken;

// Initialize a filter that can either blacklist the URL entirely from
// instrumentation, or optionally only scrub off only the query parameters,
// i.e. given:
//
// https://store.acmecorp.com/purchase?user=wcoyote&ccnum=4444555566667777
//
// the scrubbed url sent to Crittercism will be:
//
// https://store.acmecorp.com/purchase
//
// (designated initializer)
- (id)initWithString:(NSString *)matchToken
           queryOnly:(BOOL)onlyScrubQuery;

// Does specified URL match this filter?
- (BOOL)matches:(NSString *)url;

// Scrub supplied URL
// Returns nil if this is a blacklist filter
- (NSString *)applyToURL:(NSString *)url;

@end
