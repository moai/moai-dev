//
//  CRFilter.h
//  Crittercism iOS Library
//
//  Created by Sean Hermany on 7/16/13.
//  Copyright (c) 2013 Crittercism, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

// Filters can be supplied to Crittercism to adjust the behavior of how URLs are
// captured and reported on for performance monitoring purposes.
//
// By default, all URLs captured by Crittercism will be reported on when
// performance monitoring is enabled.
//
// The query portion of URLs will be stripped by default. That is, a request
// for the URL:
//
// https://store.acmecorp.com/purchase?user=wcoyote&ccnum=4444555566667777
//
// will be reported as:
//
// https://store.acmecorp.com/purchase
//
// This behavior can be altered with two types of filters:
//
// * Blacklist filters
//
// Blacklist filters cause Crittercism to discard data pertaining to any
// matching URLs.
//
// * Query preserving filters
//
// If you wish to report the full requested URL for certain endpoints, including
// the query portion, you can create and supply a query preserving filter to the
// library.
//
// Examples:
//
// Filters can either be supplied when Crittercism is enabled, as an array of
// CRFilter objects, or added dynamically at any time.
//
// The preferred method for creating filters is with the supplied convenience
// constructors, although the normal initializers, as well as the filtering
// function +[CRFillter applyFilter:ToURL] are left public so that developers
// can test the filtering behavior.
//
// * Creating a blacklist filter:
//
// [CRFilter filterWithString:@"purchase.php"]
//
// * Creating a query preserving filter:
//
// [CRFilter queryPreservingFilterWithString:@"lookupMovie"]
//
// * Initializing Crittercism with the above filters:
//
// [Crittercism enableWithAppID:@"YOURAPPID"
//                andURLFilters:@[[CRFilter filterWithString:@"purchase.php"],
//                                [CRFilter queryPreservingFilterWithString:@"lookupMovie"]]];
//
// Notes:
//
// * Filters match URLs via CASE SENSITIVE substring matching
// * Blacklist filters always take precedence. If a captured URL matches both a
//   blacklist filter, and a query preserving filter, it will NOT be reported
//   on.
// * Older versions of the agent did not scrub query parameters by default, and
//   so a "queryOnlyFilter" existed. To avoid compile time errors for existing
//   Crittercism users, this filter type can still be created, but it will be
//   ignored by the library. This filter type is deprecated, and will be removed
//   in a future version of the agent.


// Support NS_ENUM macro if building against iOS SDK < 6.0

#ifndef NS_ENUM

#if (__cplusplus && __cplusplus >= 201103L && (__has_extension(cxx_strong_enums) || __has_feature(objc_fixed_enum))) || (!__cplusplus && __has_feature(objc_fixed_enum))
#define NS_ENUM(_type, _name) enum _name : _type _name; enum _name : _type
#if (__cplusplus)
#define NS_OPTIONS(_type, _name) _type _name; enum : _type
#else
#define NS_OPTIONS(_type, _name) enum _name : _type _name; enum _name : _type
#endif
#else
#define NS_ENUM(_type, _name) _type _name; enum
#define NS_OPTIONS(_type, _name) _type _name; enum
#endif

#endif

typedef NS_ENUM(NSUInteger, CRFilterType) {
  CRFilterTypeScrubQuery, // Default
  CRFilterTypeBlacklist,
  CRFilterTypePreserveQuery
};

@interface CRFilter : NSObject

@property (readonly, assign) CRFilterType filterType;

#pragma mark - Class Methods

// Convenience method to create a blacklisting filter
+ (CRFilter *)filterWithString:(NSString *)matchToken;

// Convenience method to create a filter which preserves the query portion of
// URLs (as opposed to the default of scrubbing the query)
+ (CRFilter *)queryPreservingFilterWithString:(NSString *)matchToken;

// Filter a URL, specifying which type of filter to use.
// Returns nil when a blacklist filter is specified.
+ (NSString *)applyFilter:(CRFilterType)filterType ToURL:(NSString *)url;

#pragma mark - Instance Methods

// Initialize a filter that blacklists all URLs that match the specified token
- (id)initWithString:(NSString *)matchToken;

// (designated initializer)
- (id)initWithString:(NSString *)matchToken
       andFilterType:(CRFilterType)filterType;

// Does specified URL match this filter?
- (BOOL)doesMatch:(NSString *)url;

#pragma mark - Deprecated

// Convenience method to create a filter which only scrubs the query portion
// of URLs
//
// Note - This is now a no-op, as query strings will be scrubbed by default.
+ (CRFilter *)queryOnlyFilterWithString:(NSString *)matchToken
__attribute__((deprecated));

@end
