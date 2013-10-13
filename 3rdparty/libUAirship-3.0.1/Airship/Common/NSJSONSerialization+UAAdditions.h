/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.

 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 EVENT SHALL URBAN AIRSHIP INC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <Foundation/Foundation.h>

/**
 * The NSJSONSerialization convenience methods.
 */
@interface NSJSONSerialization (UAAdditions)

/**
 * Converts a Foundation object to a JSON formatted NSString
 * @param jsonObject Foundation object to convert 
 * @return NSString formatted as JSON, or nil if an error occurs
 */
+ (NSString *)stringWithObject:(id)jsonObject;

/**
 * Converts a Foundation object to a JSON formatted NSString
 * @param jsonObject Foundation object to convert
 * @param opt NSJSONWritingOptions options
 * @return NSString formatted as JSON, or nil if an error occurs
 */
+ (NSString *)stringWithObject:(id)jsonObject options:(NSJSONWritingOptions)opt;

/**
 * Create a Foundation object from JSON string
 * @param jsonString the JSON NSString to convert
 * @return A Foundation object, or nil if an error occurs.
 */
+ (id)objectWithString:(NSString *)jsonString;

@end
