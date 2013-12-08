// VGConfigGroup.m -- program settings group
// by allen brunson  april 14 2009

#import "VGConfigGroup.h"
#import "VGConfigItem.h"
#import "VGConfigViewController.h"

@implementation VGConfigGroup

@synthesize parent, label, itemList;

-(NSInteger)checkItemWithValue:(NSInteger)value
{
    NSInteger  totl = 0;

    for (VGConfigItem* item in self.itemList)
    {
        VG_ASSERT(VGIsKindOf(item, [VGConfigItem class]));
        
        item.cellType = UITableViewCellAccessoryNone;
        item.group    = self;

        if (item.valueInteger == value)
        {
            totl++;
            item.cellType = UITableViewCellAccessoryCheckmark;
        }
    }
    
    VG_ASSERT(totl == 1);
    return totl;
}

-(void)dealloc
{
    self.parent   = nil;
    self.label    = nil;
    self.itemList = nil;
    
    [super dealloc];
}

+(VGConfigGroup*)configGroup
{
    return [[[self alloc] init] autorelease];
}

-(id)init
{
    self = [super init];
    if (!self) return nil;

    self.itemList = [NSMutableArray array];
    
    return self;
}

-(void)itemAdd:(VGConfigItem*)item
{
    VG_ASSERT(VGIsKindOf(item, [VGConfigItem class]));
    [self.itemList addObject:item];
}

-(VGConfigItem*)itemAt:(NSInteger)indexValue
{
    return [self.itemList objectAtIndex:indexValue];
}

-(NSInteger)itemCount
{
    return self.itemList.count;
}

-(NSString*)labelForValue:(NSInteger)value
{
    for (VGConfigItem* item in self.itemList)
    {
        VG_ASSERT(VGIsKindOf(item, [VGConfigItem class]));
        if (item.valueInteger == value) return item.label;
    }
    
    return @"unknown";
}

// initialization for a VGConfigGroup and its children

-(void)listSetup:(VGConfigViewController*)viewController;
{
    VG_ASSERT(VGIsKindOf(viewController, [VGConfigViewController class]));
    
    self.parent = viewController;
    
    for (VGConfigItem* item in self.itemList)
    {
        VG_ASSERT(VGIsKindOf(item, [VGConfigItem class]));
        [item listSetup:viewController];
    }
}

// initialization for a list of VGConfigGroup objects

+(void)listSetup:(NSArray*)list parent:(VGConfigViewController*)viewController
{
    for (VGConfigGroup* vgcg in list)
    {
        VG_ASSERT(VGIsKindOf(vgcg, [VGConfigGroup class]));
        [vgcg listSetup:viewController];
    }
}

@end
