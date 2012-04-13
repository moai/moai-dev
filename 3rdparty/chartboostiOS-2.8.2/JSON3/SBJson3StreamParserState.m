/*
 Copyright (c) 2010, Stig Brautaset.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

   Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   Neither the name of the the author nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "SBJson3StreamParserState.h"
#import "SBJson3StreamParser.h"

#define SINGLETON \
+ (id)sharedInstance { \
    static id state; \
    if (!state) state = [[self alloc] init]; \
    return state; \
}

@implementation SBJson3StreamParserState

+ (id)sharedInstance { return nil; }

- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
	return NO;
}

- (SBJson3StreamParserStatus)parserShouldReturn:(SBJson3StreamParser*)parser {
	return SBJson3StreamParserWaitingForData;
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {}

- (BOOL)needKey {
	return NO;
}

- (NSString*)name {
	return @"<aaiie!>";
}

@end

#pragma mark -

@implementation SBJson3StreamParserStateStart

SINGLETON

- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
	return token == sbjon3_token_array_start || token == sbjon3_token_object_start;
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {

	SBJson3StreamParserState *state = nil;
	switch (tok) {
		case sbjon3_token_array_start:
			state = [SBJson3StreamParserStateArrayStart sharedInstance];
			break;

		case sbjon3_token_object_start:
			state = [SBJson3StreamParserStateObjectStart sharedInstance];
			break;

		case sbjon3_token_array_end:
		case sbjon3_token_object_end:
			if (parser.supportMultipleDocuments)
				state = parser.state;
			else
				state = [SBJson3StreamParserStateComplete sharedInstance];
			break;

		case sbjon3_token_eof:
			return;

		default:
			state = [SBJson3StreamParserStateError sharedInstance];
			break;
	}


	parser.state = state;
}

- (NSString*)name { return @"before outer-most array or object"; }

@end

#pragma mark -

@implementation SBJson3StreamParserStateComplete

SINGLETON

- (NSString*)name { return @"after outer-most array or object"; }

- (SBJson3StreamParserStatus)parserShouldReturn:(SBJson3StreamParser*)parser {
	return SBJson3StreamParserComplete;
}

@end

#pragma mark -

@implementation SBJson3StreamParserStateError

SINGLETON

- (NSString*)name { return @"in error"; }

- (SBJson3StreamParserStatus)parserShouldReturn:(SBJson3StreamParser*)parser {
	return SBJson3StreamParserError;
}

@end

#pragma mark -

@implementation SBJson3StreamParserStateObjectStart

SINGLETON

- (NSString*)name { return @"at beginning of object"; }

- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
	switch (token) {
		case sbjon3_token_object_end:
		case sbjon3_token_string:
			return YES;
			break;
		default:
			return NO;
			break;
	}
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {
	parser.state = [SBJson3StreamParserStateObjectGotKey sharedInstance];
}

- (BOOL)needKey {
	return YES;
}

@end

#pragma mark -

@implementation SBJson3StreamParserStateObjectGotKey

SINGLETON

- (NSString*)name { return @"after object key"; }

- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
	return token == sbjon3_token_keyval_separator;
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {
	parser.state = [SBJson3StreamParserStateObjectSeparator sharedInstance];
}

@end

#pragma mark -

@implementation SBJson3StreamParserStateObjectSeparator

SINGLETON

- (NSString*)name { return @"as object value"; }

- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
	switch (token) {
		case sbjon3_token_object_start:
		case sbjon3_token_array_start:
		case sbjon3_token_true:
		case sbjon3_token_false:
		case sbjon3_token_null:
		case sbjon3_token_number:
		case sbjon3_token_string:
			return YES;
			break;

		default:
			return NO;
			break;
	}
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {
	parser.state = [SBJson3StreamParserStateObjectGotValue sharedInstance];
}

@end

#pragma mark -

@implementation SBJson3StreamParserStateObjectGotValue

SINGLETON

- (NSString*)name { return @"after object value"; }

- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
	switch (token) {
		case sbjon3_token_object_end:
		case sbjon3_token_separator:
			return YES;
			break;
		default:
			return NO;
			break;
	}
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {
	parser.state = [SBJson3StreamParserStateObjectNeedKey sharedInstance];
}


@end

#pragma mark -

@implementation SBJson3StreamParserStateObjectNeedKey

SINGLETON

- (NSString*)name { return @"in place of object key"; }

- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
    return sbjon3_token_string == token;
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {
	parser.state = [SBJson3StreamParserStateObjectGotKey sharedInstance];
}

- (BOOL)needKey {
	return YES;
}

@end

#pragma mark -

@implementation SBJson3StreamParserStateArrayStart

SINGLETON

- (NSString*)name { return @"at array start"; }

- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
	switch (token) {
		case sbjon3_token_object_end:
		case sbjon3_token_keyval_separator:
		case sbjon3_token_separator:
			return NO;
			break;

		default:
			return YES;
			break;
	}
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {
	parser.state = [SBJson3StreamParserStateArrayGotValue sharedInstance];
}

@end

#pragma mark -

@implementation SBJson3StreamParserStateArrayGotValue

SINGLETON

- (NSString*)name { return @"after array value"; }


- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
	return token == sbjon3_token_array_end || token == sbjon3_token_separator;
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {
	if (tok == sbjon3_token_separator)
		parser.state = [SBJson3StreamParserStateArrayNeedValue sharedInstance];
}

@end

#pragma mark -

@implementation SBJson3StreamParserStateArrayNeedValue

SINGLETON

- (NSString*)name { return @"as array value"; }


- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token {
	switch (token) {
		case sbjon3_token_array_end:
		case sbjon3_token_keyval_separator:
		case sbjon3_token_object_end:
		case sbjon3_token_separator:
			return NO;
			break;

		default:
			return YES;
			break;
	}
}

- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok {
	parser.state = [SBJson3StreamParserStateArrayGotValue sharedInstance];
}

@end

