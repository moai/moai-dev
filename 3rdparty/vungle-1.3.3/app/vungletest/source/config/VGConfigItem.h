// VGConfigItem.h -- program settings item
// by allen brunson  april 14 2009


/******************************************************************************/
/*                                                                            */
/***  class defines                                                         ***/
/*                                                                            */
/******************************************************************************/

@class VGConfigGroup;
@class VGConfigViewController;

#define  VGConfigButtonAnimateDelay  0.25

typedef enum
{
    VGConfUnknown,           // don't use this one
    VGConfBoolean,           // label plus an on/off switch
    VGConfButtonChooserItem, // one button for VGConfChooser
    VGConfButtonPlain,       // plain old button
    VGConfButtonToGroup,     // button that triggers a VGConfigGroup
    VGConfChooser,           // button that triggers a list of choices
    VGConfDisplay,           // just a long string of text
    VGConfFloat,             // editable float value
    VGConfInteger,           // editable integer value
    VGConfLabel,             // display in name/value format
    VGConfText,              // name/value, right part is editable
    VGConfURL                // name/value, optimized for url input
}   VGConfigItemType;


/******************************************************************************/
/*                                                                            */
/***  VGConfigItem class                                                    ***/
/*                                                                            */
/******************************************************************************/

@interface VGConfigItem: NSObject <UITextFieldDelegate>
{
    @private

    VGConfigItemType  type;
    NSInteger         tag;
    UIColor*          color;
    NSString*         label;
    NSString*         value;
    NSString*         placeholder;
    UIControl*        control;
    UIView*           accessoryView;
    CGFloat           editWidth;

    VGConfigGroup*                group;
    UITableViewCellAccessoryType  cellType;
    BOOL                          reload;

    id   bttnTarget;
    SEL  bttnAction;
    id   loadTarget;
    SEL  loadAction;
    id   saveTarget;
    SEL  saveAction;
}

// properties

@property(nonatomic, assign) VGConfigItemType  type;
@property(nonatomic, assign) NSInteger         tag;
@property(nonatomic, retain) UIColor*          color;
@property(nonatomic, retain) NSString*         label;
@property(nonatomic, retain) NSString*         value;
@property(nonatomic, retain) NSString*         placeholder;
@property(nonatomic, retain) UIControl*        control;
@property(nonatomic, retain) UIView*           accessoryView;
@property(nonatomic, assign) CGFloat           editWidth;

@property(nonatomic, retain)  VGConfigGroup*                group;
@property(nonatomic, assign)  UITableViewCellAccessoryType  cellType;
@property(nonatomic, assign)  BOOL                          reload;

@property(nonatomic, assign)  id   bttnTarget;
@property(nonatomic, assign)  SEL  bttnAction;
@property(nonatomic, assign)  id   loadTarget;
@property(nonatomic, assign)  SEL  loadAction;
@property(nonatomic, assign)  id   saveTarget;
@property(nonatomic, assign)  SEL  saveAction;

// class methods

+(CGFloat)cellWidthForMode:(BOOL)portraitMode;
+(VGConfigItem*)itemWithType:(VGConfigItemType)type;

// construction and destruction

-(void)dealloc;
-(id)initWithType:(VGConfigItemType)itemType;

// get and set values

-(BOOL)valueBool;
-(float)valueFloat;
-(NSInteger)valueInteger;
-(NSString*)valueString;

-(void)setValueBool:(BOOL)boolValue;
-(void)setValueFloat:(float)floatValue;
-(void)setValueInteger:(NSInteger)intValue;
-(void)setValueString:(NSString*)stringValue;

// public methods

-(UITableViewCell*)cellForTableView:(UITableView*)tableView;

// @private

-(void)buttonPressAction;

-(UITableViewCell*)createCell:(BOOL)portraitMode;
-(UITableViewCell*)createCellBase;
-(UITableViewCell*)createCellBase:(UITableViewCellStyle)style;
-(UITableViewCell*)createCellBoolean;
-(UITableViewCell*)createCellButton;
-(UITableViewCell*)createCellChooser;
-(UITableViewCell*)createCellDisplay;
-(UITableViewCell*)createCellLabel;
-(UITableViewCell*)createCellNumber:(BOOL)portraitMode;
-(UITableViewCell*)createCellText:(BOOL)portraitMode;
-(UITableViewCell*)createCellURL:(BOOL)portraitMode;

-(BOOL)execCell:(id)sender;
-(void)execCellAction;
-(void)execCellActionAfterDelay:(NSTimeInterval)delay;
-(void)execCellBoolean:(UISwitch*)switchButton;
-(void)execCellButtonChooserItem:(UITableViewCell*)cell;
-(void)execCellButtonPlain:(UITableViewCell*)cell;
-(void)execCellButtonToGroup:(UITableViewCell*)cell;
-(void)execCellChooser:(UITableViewCell*)cell;
-(void)execCellFloat:(UITextField*)textField;
-(void)execCellInteger:(UITextField*)textField;
-(void)execCellText:(UITextField*)textField;

-(NSString*)identifier;
-(void)listSetup:(VGConfigViewController*)parent;

-(BOOL)selectionAllowed;
-(void)selectionExecute:(UITableViewCell*)cell;

-(BOOL)setupValue:(UITableViewCell*)cell;
-(BOOL)setupValueBoolean:(UITableViewCell*)cell;
-(BOOL)setupValueButton:(UITableViewCell*)cell;
-(BOOL)setupValueChooser:(UITableViewCell*)cell;
-(BOOL)setupValueDisplay:(UITableViewCell*)cell;
-(BOOL)setupValueFloat:(UITableViewCell*)cell;
-(BOOL)setupValueInteger:(UITableViewCell*)cell;
-(BOOL)setupValueLabel:(UITableViewCell*)cell;
-(BOOL)setupValueText:(UITableViewCell*)cell;

-(void)targetControl:(UIControl*)cntl selector:(SEL)selector;
-(UITextField*)textField:(BOOL)portraitMode width:(CGFloat)width;

// UITextFieldDelegate methods

-(void)textFieldDidEndEditing:(UITextField*)textField;
-(BOOL)textFieldShouldReturn:(UITextField*)textField;

@end


/******************************************************************************/
/*                                                                            */
/***  ConfigItem class                                                      ***/
/*                                                                            */
/******************************************************************************

overview
--------

an item value that can appear in a configuration group.


multiple edits with one keyboard popup
--------------------------------------

this class used to have a bug, tickled by a particular series of events. the
user touches a text field to start editing. the keyboard pops up, and the user
edits the field contents. then, *without* dismissing the keyboard, the user
touches a *different* text field, and starts editing that one. changes were not
saved for the first text field.

you should not perform end-of-editing chores in the textFieldShouldReturn:
delegate method. that one is called when the user presses the return key on the
keyboard, which doesn't happen in the above scenario. textFieldDidEndEditing: is
the method to use instead.


item.accessoryView
------------------

this is a bad hack that allows code outside this module to specify an accessory
view for VGConfDisplay cells. i should probably come up with a better method.


targeting problem
-----------------

if VGConfigItem is a type that registers itself to get button presses from the
button attached to a UITableViewCell, then is possible that the item will get
destroyed while the button still remembers it as a target. this is because table
view cells get reused. to get around that, VGConfigItem remembers the control it
set itself up for in item.control, and removes itself from that item at
destruction time, and also when it sets itself up to get clicks from a new
button.

this is not a very good fix. a better one would be to not tie VGConfigItem so
closely to its cell. any solution i can think of right now would require major
re-architecting, which i don't have time for.

*/
