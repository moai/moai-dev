//
//  AA_DownloadTracker.h
//  libBurstlyDownloadTracker
//

#import <Foundation/Foundation.h>


@interface BurstlyDownloadTrackerImpl : NSObject {}

+ (BurstlyDownloadTrackerImpl*)current;
- (void)track;

@end
