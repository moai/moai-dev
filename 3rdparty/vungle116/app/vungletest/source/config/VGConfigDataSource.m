// VGConfigDataSource.m -- program settings tableview data source
// by allen brunson  april 14 2009

#import "VGConfigDataSource.h"
#import "VGConfigGroup.h"
#import "VGConfigItem.h"

@implementation VGConfigDataSource

@synthesize groupList;

+(VGConfigDataSource*)dataSource
{
    return [[[self alloc] init] autorelease];
}

-(void)dealloc
{
    self.groupList = nil;
    
    [super dealloc];
}

-(VGConfigGroup*)groupAtIndex:(NSInteger)indexValue
{
    VGConfigGroup* vgcg = [self.groupList objectAtIndex:indexValue];
    VG_ASSERT(VGIsKindOf(vgcg, [VGConfigGroup class]));
    return vgcg;
}

-(BOOL)hasGroupList
{
    return VGArrayHasObjects(self.groupList);
}

-(id)init
{
    self = [super init];
    if (!self) return nil;

    self.groupList = [NSArray array];
    
    return self;
}

-(VGConfigItem*)itemAtIndexPath:(NSIndexPath*)indexPath
{
    VGConfigGroup*  vgcg = [self groupAtIndex:indexPath.section];
    VGConfigItem*   item = [vgcg itemAt:indexPath.row];

    VG_ASSERT(VGIsKindOf(item, [VGConfigItem class]));
    return item;
}

-(NSInteger)numberOfSectionsInTableView:(UITableView*)tableView
{
    return self.groupList.count;
}

-(UITableViewCell*)tableView:(UITableView*)tableView
 cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
    VGConfigItem*     item = [self itemAtIndexPath:indexPath];
    UITableViewCell*  cell = [item cellForTableView:tableView];

    return cell;
}

-(void)tableView:(UITableView*)tableView
 didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
    UITableViewCell*  cell = [tableView cellForRowAtIndexPath:indexPath];
    VGConfigItem*     item = [self itemAtIndexPath:indexPath];

    [tableView deselectRowAtIndexPath:indexPath animated:FALSE];
    [item selectionExecute:cell];

    if (item.reload)
    {
        [tableView performSelector:@selector(reloadData)
         withObject:nil afterDelay:(VGConfigButtonAnimateDelay + 0.01)];
    }
}

-(NSInteger)tableView:(UITableView*)tableView
 numberOfRowsInSection:(NSInteger)section
{
    VGConfigGroup* vgcg = [self groupAtIndex:section];
    return vgcg.itemCount;
}

-(NSString*)tableView:(UITableView*)tableView
 titleForHeaderInSection:(NSInteger)section
{
    VGConfigGroup* vgcg = [self groupAtIndex:section];
    return vgcg.label;
}

-(NSIndexPath*)tableView:(UITableView*)tableView
 willSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
    VGConfigItem*  item = [self itemAtIndexPath:indexPath];

    if (!item.selectionAllowed)
    {
        indexPath = nil;
    }

    return indexPath;
}

@end
