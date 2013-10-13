/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.

 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC ``AS IS'' AND ANY EXPRESS OR
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

#import "UAGlobal.h"

@class UAInboxMessage;

#define OLD_DB_NAME @"UAInbox.db"

#define CORE_DATA_STORE_NAME @"Inbox-%@.sqlite"
#define CORE_DATA_DIRECTORY_NAME @"UAInbox"

/**
 * The UAInboxDBManager singleton provides access to the Rich Push Inbox data.
 */
@interface UAInboxDBManager : NSObject {
}

SINGLETON_INTERFACE(UAInboxDBManager);

/**
 * Gets the current users messages, sorted descending 
 * by the messageSent time.
 *
 * @return NSArray of UAInboxMessages
 */
- (NSArray *)getMessages;

/**
 * Adds a message inbox.
 * @param dictionary A dictionary with keys and values conforming to the
 * Urban Airship JSON API for retrieving inbox messages.
 * @return A message, populated with data from the message dictionary.
 */
- (UAInboxMessage *)addMessageFromDictionary:(NSDictionary *)dictionary;


/**
 * Updates an existing message in the inbox.
 *
 * @param dictionary A dictionary with keys and values conforming to the
 * Urban Airship JSON API for retrieving inbox messages.
 *
 * @return YES if the message was updated, NO otherwise.
 */
- (BOOL)updateMessageWithDictionary:(NSDictionary *)dictionary;

/**
 * Deletes a list of messages from the database
 * @param messages NSArray of UAInboxMessages to be deleted 
 */
- (void)deleteMessages:(NSArray *)messages;

/**
 * Deletes a set of messages from the database
 * @param messageIDs NSArray of the messages IDs to be deleted
 */
- (void)deleteMessagesWithIDs:(NSSet *)messageIDs;

/**
 * Retuns the set of the current message ids in the inbox
 * @return An NSSet of message ids
 */
- (NSSet *)messageIDs;
/**
 * Saves any changes to the database
 */
- (void)saveContext;

@end
