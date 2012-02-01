//
//  CrittercismRootViewController.h
//  CrittercismIPhone3
//
//  Created by Robert Kwok on 10/4/10.
//  Copyright Crittercism 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Crittercism.h"

@interface CrittercismRootViewController : UIViewController {
	IBOutlet UITableView *feedbackTable;
	IBOutlet UISearchBar *searchBar;
	IBOutlet UITextField *searchTF;
	IBOutlet UIButton *ideaToggle;
	IBOutlet UIButton *bugToggle;
	IBOutlet UIButton *loveToggle;	
	IBOutlet UIButton *questionToggle;
	IBOutlet UIImageView *greenCategorizeBox;
	IBOutlet UIImageView *searchBackground;
	IBOutlet UILabel *categorizeAsLabel;
	IBOutlet UIActivityIndicatorView *loadingIndicator;
	IBOutlet UIButton *cancelButton;
	IBOutlet UIButton *showVotesButton;
	
	BOOL searching;
	BOOL isNavigationHidden;
	BOOL isRefreshing;
	NSString *appIDStr;
	NSString *keyStr;
	NSString *secretStr;
	NSMutableData *responseData;
	
	NSMutableArray *copyListOfItems;
}


@property(nonatomic, retain) NSString *appIDStr;
@property(nonatomic, retain) NSString *keyStr;
@property(nonatomic, retain) NSString *secretStr;

@property (nonatomic, retain) IBOutlet UIActivityIndicatorView *loadingIndicator;
@property (nonatomic, retain) IBOutlet UILabel *categorizeAsLabel;
@property (nonatomic, retain) IBOutlet UIImageView *searchBackground;
@property (nonatomic, retain) IBOutlet UIImageView *greenCategorizeBox;
@property (nonatomic, retain) IBOutlet UIButton *ideaToggle;
@property (nonatomic, retain) IBOutlet UIButton *bugToggle;
@property (nonatomic, retain) IBOutlet UIButton *loveToggle;
@property (nonatomic, retain) IBOutlet UIButton *questionToggle;
@property (nonatomic, retain) IBOutlet UITextField *searchTF;
@property (nonatomic, retain) IBOutlet UIButton *cancelButton;
@property (nonatomic, retain) IBOutlet UIButton *showVotesButton;

@property (nonatomic, retain) UITableView *feedbackTable;
@property (nonatomic, retain) UISearchBar *searchBar;

-(id) initWithAppID:(NSString *)_app_id andKey:(NSString *)_keyStr andSecret:(NSString *)_secretStr;

-(void) showAbout;
-(void) closeButtonHit;
-(void) showVotesLeft;
-(void) hideVotes;
-(void) hideVoteBubble;
-(IBAction) searchTFEntered:(id) sender;
-(IBAction) searchTFExited:(id) sender;
-(IBAction) bugHit:(id) sender;
-(IBAction) ideaHit:(id) sender;
-(IBAction) loveHit:(id) sender;
-(IBAction) questionHit:(id) sender;
-(IBAction) thumbsUpHit:(id) sender;
-(IBAction) thumbsDownHit:(id) sender;
-(IBAction) searchChanged:(id) sender;
-(IBAction) cancelHit:(id) sender;

-(IBAction) earnVoteHit:(id) sender;

-(NSString *) getSelectedCategory;
-(void) searchTableView;
-(void) vote:(int)row andThumbsUp:(int)isUp;
-(void) hideNavigationBar;
-(void) unhideNavigationBar;
-(void) unhideNavigationBar:(BOOL)isAnimated;
-(void) refreshTable:(BOOL)isRefresh;
-(void) keyboardWillShow:(NSNotification *) note;


@end
