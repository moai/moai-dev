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

#import <Foundation/Foundation.h>

#import "SBJson3Tokeniser.h"
#import "SBJson3StreamParser.h"

@interface SBJson3StreamParserState : NSObject
+ (id)sharedInstance;
- (BOOL)parser:(SBJson3StreamParser*)parser shouldAcceptToken:(sbjon3_token_t)token;
- (SBJson3StreamParserStatus)parserShouldReturn:(SBJson3StreamParser*)parser;
- (void)parser:(SBJson3StreamParser*)parser shouldTransitionTo:(sbjon3_token_t)tok;
- (BOOL)needKey;

- (NSString*)name;

@end

@interface SBJson3StreamParserStateStart : SBJson3StreamParserState
@end

@interface SBJson3StreamParserStateComplete : SBJson3StreamParserState
@end

@interface SBJson3StreamParserStateError : SBJson3StreamParserState
@end


@interface SBJson3StreamParserStateObjectStart : SBJson3StreamParserState
@end

@interface SBJson3StreamParserStateObjectGotKey : SBJson3StreamParserState
@end

@interface SBJson3StreamParserStateObjectSeparator : SBJson3StreamParserState
@end

@interface SBJson3StreamParserStateObjectGotValue : SBJson3StreamParserState
@end

@interface SBJson3StreamParserStateObjectNeedKey : SBJson3StreamParserState
@end

@interface SBJson3StreamParserStateArrayStart : SBJson3StreamParserState
@end

@interface SBJson3StreamParserStateArrayGotValue : SBJson3StreamParserState
@end

@interface SBJson3StreamParserStateArrayNeedValue : SBJson3StreamParserState
@end
