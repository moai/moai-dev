
#import <Foundation/Foundation.h>

typedef void (^UAInboxMessageIDBlock)(NSString *messageID);

/**
 * Rich Push helper methods.
 */
@interface UAInboxUtils : NSObject

/**
 * Retrieves a rich push message ID from a notification dictionary, executing
 * the supplied block with the ID as an argument if it is found.
 * @param notification The notification dictionary.
 * @param actionBlock The UAInboxMessageIDBlock to be executed.
 */
+ (void)getRichPushMessageIDFromNotification:(NSDictionary *)notification withAction:(UAInboxMessageIDBlock)actionBlock;

@end
