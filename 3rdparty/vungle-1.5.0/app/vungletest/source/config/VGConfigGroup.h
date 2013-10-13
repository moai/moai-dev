// VGConfigGroup.h -- program settings group
// by allen brunson  april 14 2009


/******************************************************************************/
/*                                                                            */
/***  class defines                                                         ***/
/*                                                                            */
/******************************************************************************/

@class VGConfigItem;
@class VGConfigViewController;


/******************************************************************************/
/*                                                                            */
/***  VGConfigGroup class                                                   ***/
/*                                                                            */
/******************************************************************************/

@interface VGConfigGroup: NSObject
{
    @private
    
    VGConfigViewController*  parent;
    NSString*                label;
    NSMutableArray*          itemList;  // VGConfigItem objects
}

// properties

@property(nonatomic, assign) VGConfigViewController*  parent;
@property(nonatomic, retain) NSString*                label;
@property(nonatomic, retain) NSMutableArray*          itemList;

// class methods

+(VGConfigGroup*)configGroup;
+(void)listSetup:(NSArray*)list parent:(VGConfigViewController*)controller;

// construction and destruction

-(void)dealloc;
-(id)init;

// public methods

-(NSInteger)checkItemWithValue:(NSInteger)value;

-(void)itemAdd:(VGConfigItem*)item;
-(VGConfigItem*)itemAt:(NSInteger)index;
-(NSInteger)itemCount;

-(NSString*)labelForValue:(NSInteger)value;
-(void)listSetup:(VGConfigViewController*)viewController;

@end


/******************************************************************************/
/*                                                                            */
/***  VGConfigGroup class                                                   ***/
/*                                                                            */
/******************************************************************************

overview
--------

all data for one config group, as displayed inside a UITableView

*/
