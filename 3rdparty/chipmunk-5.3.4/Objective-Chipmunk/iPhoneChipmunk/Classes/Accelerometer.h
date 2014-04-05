#import <UIKit/UIKit.h>

#import "ObjectiveChipmunk.h"

@interface Accelerometer : NSObject <UIAccelerometerDelegate>

+ (void)installWithInterval:(NSTimeInterval)interval andAlpha:(float)alpha;
+ (cpVect)getAcceleration;

@end
