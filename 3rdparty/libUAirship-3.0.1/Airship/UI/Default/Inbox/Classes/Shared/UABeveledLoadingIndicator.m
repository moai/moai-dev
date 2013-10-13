
#import "UABeveledLoadingIndicator.h"
#include <QuartzCore/QuartzCore.h>

@interface UABeveledLoadingIndicator()
@property(nonatomic, strong) UIActivityIndicatorView *activity;
@end

@implementation UABeveledLoadingIndicator

+ (UABeveledLoadingIndicator *)indicator {
    return [[UABeveledLoadingIndicator alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
}

- (void)setup {
    self.backgroundColor = [UIColor blackColor];
    self.alpha = 0.7;
    self.layer.cornerRadius = 10.0;
    self.hidden = YES;
    
    self.activity = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    self.activity.hidesWhenStopped = YES;

    [self addSubview:self.activity];
    
    self.activity.center = CGPointMake( self.frame.size.width/2, self.frame.size.height/2);
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self setup];
    }
    return self;
}

- (void)awakeFromNib {
    [self setup];
}

- (void)show {
    self.hidden = NO;
    [self.activity startAnimating];
}

- (void)hide {
    self.hidden = YES;
    [self.activity stopAnimating];
}


@end
