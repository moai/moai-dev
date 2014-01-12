// MainView.m -- main view for demo app
// by allen brunson  january 9 2012

#import "AppDelegate.h"
#import "MainView.h"

static const CGFloat kTerminalBorder = 2.0;

static CGFloat buttonGap(void)
{
    if (VGDeviceIsPad()) return 32.0;
    return 16.0;
}

static CGFloat buttonHeight(void)
{
    if (VGDeviceIsPad()) return 70.0;
    return 55.0;
}

static CGFloat buttonWidthPlay(void)
{
    if (VGDeviceIsPad()) return 350.0;
    return 180.0;
}

static CGFloat buttonWidthPrefs(void)
{
    if (VGDeviceIsPad()) return 180.0;
    return 100.0;
}

static UIFont* terminalFont(void)
{
    CGFloat  size = 0.0;
    
    if (VGDeviceIsPad())
    {
        size = 12.0;
    }
    else
    {
        size = 9.0;
    }
    
    return [UIFont fontWithName:@"Verdana" size:size];
}

@implementation MainView

@synthesize playButton, prefsButton;
@synthesize vungleStatus, vungleTerminal;

-(void)dealloc
{
    self.playButton     = nil;
    self.prefsButton    = nil;
    self.vungleStatus   = nil;
    self.vungleTerminal = nil;
    
    [super dealloc];
}

-(CGFloat)heightStatus
{
    UIFont* font = terminalFont();
    return font.fontLineHeight + 2.0;
}

-(CGFloat)heightUserArea
{
    const CGFloat  high = buttonHeight();
    
    if (VGDeviceIsPad())
    {
        return high + 60.0;
    }
    else
    {
        return high + 16.0;
    }
}

-(id)initWithFrame:(CGRect)rect
{
    self = [super initWithFrame:rect];
    if (!self) return nil;
    
    self.autoresizingMask = VGAutoresizingWidthHeight;
    self.backgroundColor  = [UIColor grayColor];
    
    self.playButton       = [self makeButton:@"Play"];
    self.prefsButton      = [self makeButton:@"Prefs"];
    self.vungleStatus     = [self makeVungleStatus];
    self.vungleTerminal   = [self makeVungleTerminal];
    
    [self addSubview:self.playButton];
    [self addSubview:self.prefsButton];
    [self addSubview:self.vungleStatus];
    [self addSubview:self.vungleTerminal];
    
    // force first status update
    [self vungleStatusUpdate:[VGVunglePub currentStatusData]];
    
    return self;
}

-(void)layoutSubviews
{
    [super layoutSubviews];
    
    self.playButton.frameUpdate     = self.rectPlayButton;
    self.prefsButton.frameUpdate    = self.rectPrefsButton;
    self.vungleStatus.frameUpdate   = self.rectVungleStatus;
    self.vungleTerminal.frameUpdate = self.rectVungleTerminal;
}

-(UIButton*)makeButton:(NSString*)title
{
    UIButton*  bttn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    
    bttn.titleLabel.font = [UIFont systemFontOfSize:24.0];
    
    [bttn setTitle:title forState:UIControlStateNormal];
    
    return bttn;
}

-(UILabel*)makeVungleStatus
{
    UILabel*  view = [[[UILabel alloc] initWithFrame:CGRectZero] autorelease];
    
    view.font            = terminalFont();
    view.backgroundColor = [UIColor colorWithWhite:0.75 alpha:1.0];
    view.textColor       = [UIColor whiteColor];
    
    return view;
}

-(UITextView*)makeVungleTerminal
{
    UIEdgeInsets  edge = UIEdgeInsetsZero;
    UIFont*       font = terminalFont();
    const CGRect  rect = self.bounds;
    UITextView*   view = [[[UITextView alloc] initWithFrame:rect] autorelease];
    
    edge.left   = -4.0;
    edge.top    = -4.0;
    edge.right  =  0.0;
    edge.bottom = -font.fontLineHeight;
    
    view.autoresizingMask = VGAutoresizingWidthHeight;
    view.contentInset     = edge;
    view.editable         = FALSE;
    view.font             = font;
    
    view.backgroundColor  = [UIColor lightGrayColor];
    view.textColor        = [UIColor whiteColor];
    
    view.alwaysBounceHorizontal         = FALSE;
    view.alwaysBounceVertical           = TRUE;
    view.bounces                        = TRUE;
    view.delaysContentTouches           = TRUE;
    view.directionalLockEnabled         = TRUE;
    view.scrollsToTop                   = TRUE;
    view.showsHorizontalScrollIndicator = FALSE;
    view.showsVerticalScrollIndicator   = TRUE;
    
    return view;
}

-(CGRect)rectButtons
{
    const CGRect  bnds = self.rectUserArea;
    CGRect        rect = CGRectZero;

    rect.size.width  = buttonWidthPlay() + buttonGap() + buttonWidthPrefs();
    rect.size.height = buttonHeight();
    
    return VGRectCenter(bnds, rect);
}

-(CGRect)rectPlayButton
{
    const CGRect  bnds = self.rectButtons;
    CGRect        rect = bnds;
    
    rect.size.width  = buttonWidthPlay();
    rect.size.height = buttonHeight();
    
    return VGRectLeft(bnds, rect);
}

-(CGRect)rectPrefsButton
{
    const CGRect  bnds = self.rectButtons;
    CGRect        rect = bnds;
    
    rect.size.width  = buttonWidthPrefs();
    rect.size.height = buttonHeight();
    
    return VGRectRight(bnds, rect);
}

// large rect at the bottom for the status line and terminal

-(CGRect)rectTerminalArea
{
    const CGRect  bnds = self.bounds;
    CGRect        rect = bnds;
    
    rect.size.height = bnds.size.height - self.heightUserArea;
    
    return VGRectBottom(bnds, rect);
}

// small rect at the top for user controls

-(CGRect)rectUserArea
{
    const CGRect  bnds = self.bounds;
    CGRect        rect = bnds;
    
    rect.size.height = self.heightUserArea;
    
    return VGRectTop(bnds, rect);
}

-(CGRect)rectVungleStatus
{
    const CGRect  bnds = self.rectTerminalArea;
    CGRect        rect = CGRectZero;
    
    rect.size.height = self.heightStatus;
    rect.size.width  = bnds.size.width - (kTerminalBorder * 2.0);
    
    rect = VGRectCenter(bnds, rect);
    rect = VGRectTop(bnds, rect);
    
    return rect;
}

-(CGRect)rectVungleTerminal
{
    const CGRect  bnds = self.rectTerminalArea;
    CGRect        rect = bnds;
    
    rect.size.height -= self.heightStatus;
    
    rect = VGRectBottom(bnds, rect);
    rect = VGRectZoom(rect, -kTerminalBorder);
    
    return rect;
}

-(void)terminalClear
{
    self.vungleTerminal.text = @"";
}

+(MainView*)view
{
    const CGRect  rect = [[UIScreen mainScreen] bounds];
    MainView*     view = [[[self alloc] initWithFrame:rect] autorelease];
    
    return view;
}

// this method is called with each new line to be displayed in the terminal

-(void)vungleLogLine:(NSString*)logLine
{
    NSMutableString*  text = [NSMutableString string];
    
    // set up text buffer with newly-added line
    [text appendString:self.vungleTerminal.text];
    [text appendString:logLine];
    [text appendString:@"\n"];
    
    // cap max text size at 16k characters
    while (text.length > (16 * 1024))
    {
        [text removeOldestLine];
    }
    
    // set new text and scroll to the bottom
    [self.vungleTerminal setText:text];
    [self.vungleTerminal scrollToBottom];
}

-(void)vungleStatusUpdate:(VGStatusData*)statusData
{
    self.vungleStatus.text =
     [NSString stringWithFormat:@"  version: %@  %@",
      [VGVunglePub versionString], statusData.description];
}

@end
