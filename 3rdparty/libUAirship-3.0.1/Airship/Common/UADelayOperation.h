
#import <Foundation/Foundation.h>

/**
 * An NSOperation that sleeps for a specified number of seconds before completing.
 *
 * This class is useful for scheduling delayed work or retry logic in an NSOperationQueue.
 */
@interface UADelayOperation : NSBlockOperation

/**
 * UADelayOperation class factory method.
 * @param seconds The number of seconds to sleep.
 */
+ (id)operationWithDelayInSeconds:(NSInteger)seconds;

/**
 * The amount of the the delay in seconds.
 */
@property(nonatomic, assign, readonly) NSInteger seconds;

@end
