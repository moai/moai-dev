// MainViewController.h -- main view controller for test app
// by allen brunson  january 9 2012

#import "MainView.h"


/******************************************************************************/
/*                                                                            */
/***  MainViewController class                                              ***/
/*                                                                            */
/******************************************************************************/

@interface MainViewController: UIViewController <VGVunglePubDelegate>
{
    @private
    
    MainView*  mainView;
}

// properties

@property(nonatomic,retain) MainView*  mainView;

// class methods

+(MainViewController*)controller;

// construction and destruction

-(void)dealloc;
-(id)initWithNibName:(NSString*)nibName bundle:(NSBundle*)nibBundle;

// public

-(void)terminalClear;

// private

-(void)buttonPlayAd:(id)sender;
-(void)buttonPrefs:(id)sender;

// UIViewController methods

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)orient;
-(void)viewDidLoad;
-(void)viewDidUnload;

// VGVunglePubDelegate methods

-(void)vungleLogLine:(NSString*)logLine;  // not really a delegate method
-(void)vungleMoviePlayed:(VGPlayData*)playData;
-(void)vungleStatusUpdate:(VGStatusData*)statusData;
-(void)vungleViewDidDisappear:(UIViewController*)viewController;
-(void)vungleViewWillAppear:(UIViewController*)viewController;
-(void)vungleAppStoreViewDidDisappear;

@end


/******************************************************************************/
/*                                                                            */
/***  MainViewController class                                              ***/
/*                                                                            */
/******************************************************************************

overview
--------

view controller that displays the minimal user interface for the test program

*/
