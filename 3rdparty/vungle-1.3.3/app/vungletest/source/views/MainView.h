// MainView.h -- main view for demo app
// by allen brunson  january 9 2012


/******************************************************************************/
/*                                                                            */
/***  MainView class                                                        ***/
/*                                                                            */
/******************************************************************************/

@interface MainView: UIView
{
    @private
    
    UIButton*    playButton;
    UIButton*    prefsButton;
    UILabel*     vungleStatus;
    UITextView*  vungleTerminal;
}

// properties

@property(nonatomic, retain) UIButton*    playButton;
@property(nonatomic, retain) UIButton*    prefsButton;
@property(nonatomic, retain) UILabel*     vungleStatus;
@property(nonatomic, retain) UITextView*  vungleTerminal;

// class methods

+(MainView*)view;

// construction and destruction

-(void)dealloc;
-(id)initWithFrame:(CGRect)frame;

// instance methods

-(void)terminalClear;

-(void)vungleLogLine:(NSString*)logLine;
-(void)vungleStatusUpdate:(VGStatusData*)statusData;

// @private

-(CGFloat)heightStatus;
-(CGFloat)heightUserArea;

-(UIButton*)makeButton:(NSString*)title;
-(UILabel*)makeVungleStatus;
-(UITextView*)makeVungleTerminal;

-(CGRect)rectButtons;
-(CGRect)rectPlayButton;
-(CGRect)rectPrefsButton;
-(CGRect)rectTerminalArea;
-(CGRect)rectUserArea;
-(CGRect)rectVungleStatus;
-(CGRect)rectVungleTerminal;

// UIView methods

-(void)layoutSubviews;

@end


/******************************************************************************/
/*                                                                            */
/***  MainView                                                              ***/
/*                                                                            */
/******************************************************************************

overview
--------

main view for vungle sdk demo app

*/
