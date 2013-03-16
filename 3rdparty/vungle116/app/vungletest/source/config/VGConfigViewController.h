// VGConfigViewController.h -- preferences view
// by allen brunson  january 31 2012


/******************************************************************************/
/*                                                                            */
/***  class defines                                                         ***/
/*                                                                            */
/******************************************************************************/

@class VGConfigDataSource;


/******************************************************************************/
/*                                                                            */
/***  VGConfigViewController class                                          ***/
/*                                                                            */
/******************************************************************************/

@interface VGConfigViewController: UITableViewController
{
    @private
    
    VGConfigDataSource*  dataSource;
}

// properties

@property(nonatomic, retain) VGConfigDataSource*  dataSource;

// class methods

+(VGConfigViewController*)controller;

// construction and destruction

-(void)dealloc;
-(id)initWithStyle:(UITableViewStyle)style;

// public methods

-(void)loadGroupList;
-(NSArray*)loadGroups;

-(void)reloadData;
-(NSString*)titleForSection:(NSInteger)section;

// UIViewController methods

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)orient;
-(void)viewDidLoad;
-(void)viewWillAppear:(BOOL)animated;

// UITableViewDelegate methods

-(void)tableView:(UITableView*)tableView
 didSelectRowAtIndexPath:(NSIndexPath*)indexPath;

-(CGFloat)tableView:(UITableView*)tableView
 heightForHeaderInSection:(NSInteger)section;

-(UIView*)tableView:(UITableView*)tableView
 viewForHeaderInSection:(NSInteger)section;

-(NSIndexPath*)tableView:(UITableView*)tableView
 willSelectRowAtIndexPath:(NSIndexPath*)indexPath;

@end


/******************************************************************************/
/*                                                                            */
/***  VGConfigViewController class                                          ***/
/*                                                                            */
/******************************************************************************

overview
--------

view controller for a settings view. this class must be a descendent of
UITableViewController and it must be set as the table view's delegate. if you
don't do this, then you won't get UITableViewController's automatic virtual
keyboard handling, which would suck.

*/
