
#import <Foundation/Foundation.h>

@class UAInboxMessage;

/**
 * The UAInboxMessageListDelegate protocol declares methods that
 * are implemented by delegates of a UAInboxMessageList API calls.
 *
 * Delegates are notified when messages are modified, loaded or
 * fail to load or update.
 *
 */
@protocol UAInboxMessageListDelegate <NSObject>

@optional

/**
 * Tells the delegate that a request for inbox messages succeeded.
 */
- (void)messageListLoadSucceeded;
/**
 * Tells the delegate that a request for inbox messages failed.
 */
- (void)messageListLoadFailed;

/**
 * Tells the delegate that a message has been marked as read.
 *
 * @param message The message marked as read
 */
- (void)singleMessageMarkAsReadFinished:(UAInboxMessage *)message;

/**
 * Tells the delegate that a mark-as-read request failed.
 *
 * @param message The message that failed to update
 */
- (void)singleMessageMarkAsReadFailed:(UAInboxMessage *)message;

/**
 * Tells the delegate that a batch of messages has been marked as read.
 */
- (void)batchMarkAsReadFinished;

/**
 * Tells the delegate that a batch mark-as-read request failed.
 */
- (void)batchMarkAsReadFailed;

/**
 * Tells the delegate that a batch of messages has been deleted.
 */
- (void)batchDeleteFinished;

/**
 * Tells the delegate that a batch update of messages has failed.
 */
- (void)batchDeleteFailed;


@end
