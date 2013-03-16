// VGConfigDataSource.h -- program settings tableview data source
// by allen brunson  april 14 2009


/******************************************************************************/
/*                                                                            */
/***  class defines                                                         ***/
/*                                                                            */
/******************************************************************************/

@class VGConfigGroup;
@class VGConfigItem;


/******************************************************************************/
/*                                                                            */
/***  VGConfigDataSource class                                              ***/
/*                                                                            */
/******************************************************************************/

@interface VGConfigDataSource: NSObject <UITableViewDataSource>
{
    @private
    
    NSArray*  groupList;  // VGConfigGroup objects
}

@property(nonatomic, retain) NSArray*  groupList;

// class methods

+(VGConfigDataSource*)dataSource;

// construction and destruction

-(void)dealloc;
-(id)init;

// public methods

-(VGConfigGroup*)groupAtIndex:(NSInteger)index;
-(BOOL)hasGroupList;
-(VGConfigItem*)itemAtIndexPath:(NSIndexPath*)indexPath;

// UITableViewDataSource methods

-(NSInteger)numberOfSectionsInTableView:(UITableView*)tableView;

-(UITableViewCell*)tableView:(UITableView*)tableView
 cellForRowAtIndexPath:(NSIndexPath*)indexPath;

-(NSInteger)tableView:(UITableView*)tableView
 numberOfRowsInSection:(NSInteger)section;
 
-(NSString*)tableView:(UITableView*)tableView
 titleForHeaderInSection:(NSInteger)section;
 
// UITableViewDelegate methods

-(void)tableView:(UITableView*)tableView 
 didSelectRowAtIndexPath:(NSIndexPath*)indexPath;
 
-(NSIndexPath*)tableView:(UITableView*)tableView 
 willSelectRowAtIndexPath:(NSIndexPath*)indexPath;

@end


/******************************************************************************/
/*                                                                            */
/***  VGConfigDataSource class                                              ***/
/*                                                                            */
/******************************************************************************

overview
--------

provides data to a table view that is being used for program settings


using this class
----------------

to make use of this class, you must also have a UITableViewController that is
set to the style UITableViewStyleGrouped. this class must be set as the table
view's data source, and that class must be set as the table view's delegate.
this is necessary because UITableViewController in iphone os 2.2 and later
contains logic to make text fields jump out of the way when the virtual
keyboard appears. writing that code yourself is a massive, ugly undertaking
that you don't want to get into.

this class acts on a couple of delegate methods, which should be passed
through from the view controller.

*/
