
#import <UIKit/UIKit.h>

/**
 * An abstraction around a nicer looking
 * loading indicator that embeds a UIActivityIndicatorView
 * in a translucent black beveled rect.
 */
@interface UABeveledLoadingIndicator : UIView

/**
 * Construct an autoreleased UABeveledLoadingIndicator with
 * a container rect size of 100 x 100.
 */
+ (UABeveledLoadingIndicator *)indicator;

/**
 * Show and animate the indicator
 */
- (void)show;

/**
 * Hide the indicator.
 */
- (void)hide;

@end
