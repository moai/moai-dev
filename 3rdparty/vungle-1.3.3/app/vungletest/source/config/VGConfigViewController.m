// VGConfigViewController.m -- preferences view
// by allen brunson  january 31 2012

#import "VGConfigDataSource.h"
#import "VGConfigGroup.h"
#import "VGConfigItem.h"
#import "VGConfigStyle.h"
#import "VGConfigViewController.h"

#pragma mark konstants

static const CGFloat kHeaderHeight      = 35.0;
static const CGFloat kHeaderLeftOffset  = 20.0;
static const CGFloat kHeaderRightOffset = 10.0;

@implementation VGConfigViewController

@synthesize dataSource;

+(VGConfigViewController*)controller
{
    return [[[self alloc] initWithStyle:UITableViewStyleGrouped] autorelease];
}

-(void)dealloc
{
    self.dataSource = nil;
    
    [super dealloc];
}

-(id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (!self) return nil;
    
    self.dataSource = [VGConfigDataSource dataSource];
    
    return self;
}

-(void)loadGroupList
{
    self.dataSource.groupList = [self loadGroups];
    [VGConfigGroup listSetup:self.dataSource.groupList parent:self];
}

-(NSArray*)loadGroups
{
    // this method must be implemented by derived classes
    // return an NSArray of VGConfigGroup objects

    return [NSArray array];
}

// force the table view to reload

-(void)reloadData
{
    // bug workaround: if you try to reload while editing, the view will crash
    // if (VGIsKindOf(VGFirstResponder(), [UIView class])) return;
    
    [self loadGroupList];
    [self.tableView reloadData];
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)orient
{
    return TRUE;
}

-(void)tableView:(UITableView*)tableView
 didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
    [self.dataSource tableView:tableView didSelectRowAtIndexPath:indexPath];
}

-(CGFloat)tableView:(UITableView*)tableView
 heightForHeaderInSection:(NSInteger)section
{
    return kHeaderHeight;
}

-(UIView*)tableView:(UITableView*)tableView
 viewForHeaderInSection:(NSInteger)section
{
    UILabel*   labl = nil;
    CGRect     rect = CGRectZero;
    NSString*  text = nil;
    UIView*    view = nil;

    // this view is needed only for black color styles
    if (!VGColorStyleBlack()) return nil;

    // no view needed if there is no text
    text = [self titleForSection:section];
    if (!VGStringHasLength(text)) return nil;

    // set base view size
    rect.size.width  = tableView.bounds.size.width;
    rect.size.height = kHeaderHeight;

    // create base view
    view = [[[UIView alloc] initWithFrame:rect] autorelease];
    view.backgroundColor = tableView.backgroundColor;

    // set label size
    rect.size.width -= kHeaderLeftOffset + kHeaderRightOffset;
    rect.origin.x   += kHeaderLeftOffset;

    // create label
    labl = [[[UILabel alloc] initWithFrame:rect] autorelease];
    labl.backgroundColor = tableView.backgroundColor;
    labl.textColor = VGConfigHeaderTextColor();
    labl.shadowColor = VGConfigHeaderTextColorShadow();
    labl.shadowOffset = CGSizeMake(0.0, 1.0);
    labl.font = VGConfigHeaderTextFont();
    labl.text = text;

    // return header view
    [view addSubview:labl];
    return view;
}

-(NSIndexPath*)tableView:(UITableView*)tableView
 willSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
    return [self.dataSource tableView:tableView
     willSelectRowAtIndexPath:indexPath];
}

-(NSString*)titleForSection:(NSInteger)section
{
    return [self.dataSource tableView:self.tableView
     titleForHeaderInSection:section];
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    // create group list
    if (!self.dataSource.hasGroupList)
    {
        [self loadGroupList];
    }

    // connect the outlets
    self.tableView.dataSource = self.dataSource;
    self.tableView.delegate   = self;

    // custom colors
    if (VGColorStyleBlack())
    {
        self.tableView.backgroundColor = VGConfigBackgroundColor();
    }    
}

-(void)viewWillAppear:(BOOL)animated
{
    [self.tableView reloadData];
    [super viewWillAppear:animated];
}

@end
