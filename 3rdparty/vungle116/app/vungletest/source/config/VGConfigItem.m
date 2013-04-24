// VGConfigItem.m -- program settings item
// by allen brunson  april 14 2009

#import "VGConfigDataSource.h"
#import "VGConfigGroup.h"
#import "VGConfigItem.h"
#import "VGConfigStyle.h"
#import "VGConfigViewController.h"

@implementation VGConfigItem

@synthesize type, tag, color;
@synthesize label, value, placeholder;
@synthesize control, accessoryView;
@synthesize editWidth;
@synthesize group, cellType, reload;
@synthesize bttnTarget, bttnAction;
@synthesize loadTarget, loadAction;
@synthesize saveTarget, saveAction;

// for regular buttons, the saveTarget/saveAction method is called when the
// button is pressed. but some buttons don't directly cause a method to be
// called, like the ones that display a new group. so you can also set the
// bttnTarget/bttnAction for a button. it will always be called when the
// button is pressed, before the usual action is taken.

-(void)buttonPressAction
{
    VGPerformIfAbleWithObject(self.bttnTarget, self.bttnAction, self);
}

// get a reusable cell from the given table view. if an existing cell can't
// be found, a new one is created

-(UITableViewCell*)cellForTableView:(UITableView*)tableView
{
    UITableViewCell*   cell = nil;
    UIViewController*  cntl = (UIViewController*) tableView.delegate;
    NSString*          name = self.identifier;

    VG_ASSERT(VGIsKindOf(cntl, [UIViewController class]));

    cell = [tableView dequeueReusableCellWithIdentifier:name];
    if (!cell) cell = [self createCell:cntl.isPortraitMode];

    [self setupValue:cell];
    [cell setSelected:FALSE];
    
    return cell;
}

// hard-coded pixel width for table cells. iphone and ipad both have a few
// unusable border pixels on either side of the cell, which is also hard-coded
// into this method. assumes the table view takes up the entire width of the
// screen.

+(CGFloat)cellWidthForMode:(BOOL)portraitMode
{
    CGFloat       bord = 0.0;
    const CGRect  scrn = [[UIScreen mainScreen] bounds];
    CGFloat       wdth = 0.0;
    
    if (VGDeviceIsPad())
    {
        bord = 44.0;
    }
    else
    {
        bord = 10.0;
    }
    
    if (portraitMode)
    {
        wdth = scrn.size.width;
    }
    else
    {
        wdth = scrn.size.height;
    }
    
    return wdth - (bord * 2.0);
}

// create a brand-new cell, appropriate for this config item's type

-(UITableViewCell*)createCell:(BOOL)portraitMode
{
    switch (self.type)
    {
        case VGConfBoolean:
        return [self createCellBoolean];

        case VGConfButtonChooserItem:
        case VGConfButtonPlain:
        case VGConfButtonToGroup:
        return [self createCellButton];

        case VGConfChooser:
        return [self createCellChooser];

        case VGConfDisplay:
        return [self createCellDisplay];

        case VGConfFloat:
        case VGConfInteger:
        return [self createCellNumber:portraitMode];

        case VGConfLabel:
        return [self createCellLabel];
        
        case VGConfText:
        return [self createCellText:portraitMode];

        case VGConfURL:
        return [self createCellURL:portraitMode];
        
        case VGConfUnknown:
        break;
    }

    VG_ASSERT(FALSE);
    return nil;
}

-(UITableViewCell*)createCellBase
{
    return [self createCellBase:UITableViewCellStyleDefault];
}

-(UITableViewCell*)createCellBase:(UITableViewCellStyle)style
{
    UITableViewCell*  cell = nil;
    NSString*         name = self.identifier;

    cell = [UITableViewCell alloc];
    cell = [[cell initWithStyle:style reuseIdentifier:name] autorelease];
    
    return cell;
}

-(UITableViewCell*)createCellBoolean
{
    UISwitch*         bttn = nil;
    UITableViewCell*  cell = [self createCellBase];

    bttn = [[[UISwitch alloc] initWithFrame:CGRectZero] autorelease];
    cell.accessoryView = bttn;

    return cell;
}

-(UITableViewCell*)createCellButton
{
    return [self createCellBase];
}

-(UITableViewCell*)createCellChooser
{
    UITableViewCell*  cell = nil;

    cell = [self createCellBase:UITableViewCellStyleValue1];
    cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;

    return cell;
}

-(UITableViewCell*)createCellDisplay
{
    return [self createCellBase];
}

-(UITableViewCell*)createCellLabel
{
    return [self createCellBase:UITableViewCellStyleValue1];
}

// note: we always force portrait mode here. that prevents the text field
// from getting too wide in landscape mode

-(UITableViewCell*)createCellNumber:(BOOL)portraitMode
{
    UITableViewCell*  cell = [self createCellBase];
    UITextField*      text = [self textField:TRUE width:0.25];
    
    text.autoresizingMask = UIViewAutoresizingNone;
    text.keyboardType     = UIKeyboardTypeNumbersAndPunctuation;
    text.textAlignment    = UITextAlignmentRight;

    cell.accessoryView = text;

    return cell;
}

-(UITableViewCell*)createCellText:(BOOL)portraitMode
{
    UITableViewCell*  cell = [self createCellBase];
    UITextField*      text = [self textField:portraitMode width:0.50];

    cell.accessoryView = text;

    return cell;
}

-(UITableViewCell*)createCellURL:(BOOL)portraitMode
{
    UITableViewCell*  cell = [self createCellText:portraitMode];
    UITextField*      text = (UITextField*) cell.accessoryView;
    
    VG_ASSERT(VGIsKindOf(cell, [UITableViewCell class]));
    VG_ASSERT(VGIsKindOf(text, [UITextField class]));

    text.autocapitalizationType = UITextAutocapitalizationTypeNone;
    text.autocorrectionType     = UITextAutocorrectionTypeNo;
    text.keyboardType           = UIKeyboardTypeURL;

    return cell;
}

-(void)dealloc
{
    [self targetControl:nil selector:nil];
    
    self.color         = nil;
    self.label         = nil;
    self.value         = nil;
    self.placeholder   = nil;
    self.control       = nil;
    self.accessoryView = nil;
    self.group         = nil;
    
    [super dealloc];
}

-(BOOL)execCell:(id)sender
{
    BOOL  rval = TRUE;

    switch (self.type)
    {
        case VGConfBoolean:
        [self execCellBoolean:sender];
        break;

        case VGConfButtonChooserItem:
        [self execCellButtonChooserItem:sender];
        break;

        case VGConfButtonPlain:
        [self execCellButtonPlain:sender];
        break;

        case VGConfButtonToGroup:
        [self execCellButtonToGroup:sender];
        break;

        case VGConfChooser:
        [self execCellChooser:sender];
        break;

        case VGConfFloat:
        [self execCellFloat:sender];
        break;

        case VGConfInteger:
        [self execCellInteger:sender];
        break;

        case VGConfText:
        case VGConfURL:
        [self execCellText:sender];
        break;
        
        case VGConfDisplay:
        case VGConfLabel:
        case VGConfUnknown:
        rval = FALSE;
        break;
    }

    VG_ASSERT(rval);
    return rval;
}

-(void)execCellAction
{
    [self execCellActionAfterDelay:0.0];
}

-(void)execCellActionAfterDelay:(NSTimeInterval)delay
{
    if (!VGRespondsTo(self.saveTarget, self.saveAction)) return;

    if (delay <= 0.0)
    {
        [self.saveTarget performSelector:self.saveAction withObject:self];
        return;
    }

    [saveTarget performSelector:saveAction withObject:self afterDelay:delay];
}

-(void)execCellBoolean:(UISwitch*)switchButton
{
    VG_ASSERT(VGIsKindOf(switchButton, [UISwitch class]));
    
    [self setValueBool:switchButton.on];
    [self buttonPressAction];
    [self execCellAction];
}

-(void)execCellButtonChooserItem:(UITableViewCell*)cell
{
    VG_ASSERT(VGIsKindOf(cell, [UITableViewCell class]));
    VG_ASSERT(VGIsKindOf(self.group, [VGConfigGroup class]));

    cell.selected = TRUE;
    [self.group checkItemWithValue:self.valueInteger];
    
    [self buttonPressAction];
    [self execCellActionAfterDelay:VGConfigButtonAnimateDelay];
}

-(void)execCellButtonPlain:(UITableViewCell*)cell
{
    VG_ASSERT(VGIsKindOf(cell, [UITableViewCell class]));
    VG_ASSERT(self.group == nil);

    cell.selected = TRUE;
    
    [self buttonPressAction];
    [self execCellActionAfterDelay:VGConfigButtonAnimateDelay];
}

-(void)execCellButtonToGroup:(UITableViewCell*)cell
{
    UINavigationController*  cnav = self.group.parent.navigationController;
    VGConfigViewController*  cntl = [VGConfigViewController controller];

    VG_ASSERT(VGIsKindOf(cell, [UITableViewCell class]));
    VG_ASSERT(VGIsKindOf(self.group, [VGConfigGroup class]));
    VG_ASSERT(VGIsKindOf(cnav, [UINavigationController class]));

    cell.selected = TRUE;
    self.group.label = nil;
    cntl.title = self.label;

    cntl.dataSource.groupList = [NSArray arrayWithObject:self.group];
    
    [self buttonPressAction];
    [cnav pushViewController:cntl animated:YES];
}

-(void)execCellChooser:(UITableViewCell*)cell
{
    UINavigationController*  cnav = self.group.parent.navigationController;
    VGConfigViewController*  cntl = [VGConfigViewController controller];

    VG_ASSERT(VGIsKindOf(cell, [UITableViewCell class]));
    VG_ASSERT(VGIsKindOf(self.group, [VGConfigGroup class]));
    VG_ASSERT(VGIsKindOf(self.group.parent, [VGConfigViewController class]));
    VG_ASSERT(VGIsKindOf(cnav, [UINavigationController class]));

    cell.selected = TRUE;
    [self.group checkItemWithValue:self.valueInteger];

    cntl.dataSource.groupList = [NSArray arrayWithObject:self.group];
    cntl.title = self.label;
    
    [self buttonPressAction];
    [cnav pushViewController:cntl animated:TRUE];
}

-(void)execCellFloat:(UITextField*)textField
{
    VG_ASSERT(VGIsKindOf(textField, [UITextField class]));
    
    [self setValueFloat:textField.text.floatValue];
    textField.text = [NSString stringWithFloat:self.valueFloat];
    [self execCellAction];
}

-(void)execCellInteger:(UITextField*)textField
{
    VG_ASSERT(VGIsKindOf(textField, [UITextField class]));
    
    [self setValueInteger:textField.text.integerValue];
    textField.text = [NSString stringWithInteger:self.valueInteger];
    [self execCellAction];
}

-(void)execCellText:(UITextField*)textField
{
    VG_ASSERT(VGIsKindOf(textField, [UITextField class]));
    
    [self setValueString:VGTrim(textField.text)];
    [self execCellAction];
}

-(NSString*)identifier
{
    switch (self.type)
    {
        case VGConfBoolean:
        return @"VGConfigItemBoolean";

        case VGConfButtonChooserItem:
        case VGConfButtonPlain:
        case VGConfButtonToGroup:
        return @"VGConfigItemButton";

        case VGConfChooser:
        return @"VGConfigItemChooser";

        case VGConfDisplay:
        return @"VGConfigItemDisplay";

        case VGConfFloat:
        return @"VGConfigItemFloat";

        case VGConfInteger:
        return @"VGConfigItemInteger";

        case VGConfLabel:
        return @"VGConfigItemLabel";
        
        case VGConfText:
        return @"VGConfigItemText";

        case VGConfURL:
        return @"VGConfigItemURL";
        
        case VGConfUnknown:
        break;
    }

    VG_ASSERT(FALSE);
    return nil;
}

-(id)initWithType:(VGConfigItemType)itemType
{
    self = [super init];
    if (!self) return nil;

    self.type     = itemType;
    self.cellType = UITableViewCellAccessoryNone;

    return self;
}

+(VGConfigItem*)itemWithType:(VGConfigItemType)type
{
    return [[[self alloc] initWithType:type] autorelease];
}

// setup that happens at list load time

-(void)listSetup:(VGConfigViewController*)parent
{
    VGConfigItem*  item = nil;
    NSInteger      size = 0;

    VG_ASSERT(VGIsKindOf(parent, [VGConfigViewController class]));

    if (self.group)
    {
        [self.group listSetup:parent];
    }

    if (!(self.type == VGConfChooser)) return;
    VG_ASSERT(VGIsKindOf(self.group, [VGConfigGroup class]));

    size = self.group.itemCount;
    VG_ASSERT(size >= 1);

    for (NSInteger inum = 0; inum < size; inum++)
    {
        item = [self.group itemAt:inum];
        VG_ASSERT(VGIsKindOf(item, self.class));
        VG_ASSERT(item.type == VGConfButtonChooserItem);

        item.tag        = self.tag;
        item.reload     = TRUE;
        item.loadTarget = nil;
        item.loadAction = nil;
        item.saveTarget = self.saveTarget;
        item.saveAction = self.saveAction;
    }

    self.saveTarget = nil;
    self.saveAction = nil;
}

// true if this item can be selected by the user

-(BOOL)selectionAllowed
{
    switch (self.type)
    {
        case VGConfButtonChooserItem:
        case VGConfButtonPlain:
        case VGConfButtonToGroup:
        case VGConfChooser:
        return TRUE;

        case VGConfBoolean:
        case VGConfDisplay:
        case VGConfFloat:
        case VGConfInteger:
        case VGConfLabel:
        case VGConfText:
        case VGConfURL:
        return FALSE;

        case VGConfUnknown:
        break;
    }

    VG_ASSERT(FALSE);
    return FALSE;
}

// called when this item has been selected

-(void)selectionExecute:(UITableViewCell*)cell
{
    VG_ASSERT(self.selectionAllowed);
    [self execCell:cell];
}

// load this object's value into a cell and wire up the cell's control to
// update us when it's activated

-(BOOL)setupValue:(UITableViewCell*)cell
{
    VGPerformIfAbleWithObject(self.loadTarget, self.loadAction, self);
    
    switch (self.type)
    {
        case VGConfBoolean:
        return [self setupValueBoolean:cell];

        case VGConfButtonChooserItem:
        case VGConfButtonPlain:
        case VGConfButtonToGroup:
        return [self setupValueButton:cell];

        case VGConfChooser:
        return [self setupValueChooser:cell];

        case VGConfDisplay:
        return [self setupValueDisplay:cell];

        case VGConfFloat:
        return [self setupValueFloat:cell];

        case VGConfInteger:
        return [self setupValueInteger:cell];

        case VGConfLabel:
        return [self setupValueLabel:cell];
        
        case VGConfText:
        case VGConfURL:
        return [self setupValueText:cell];
        
        case VGConfUnknown:
        break;
    }

    VG_ASSERT(FALSE);
    return FALSE;
}

-(BOOL)setupValueBoolean:(UITableViewCell*)cell
{
    UISwitch*  bttn = (UISwitch*) cell.accessoryView;
    
    VG_ASSERT(VGIsKindOf(bttn, [UISwitch class]));

    cell.textLabel.text = self.label;
    bttn.on = self.valueBool;

    [self targetControl:bttn selector:@selector(execCell:)];
    
    return TRUE;
}

-(BOOL)setupValueButton:(UITableViewCell*)cell
{
    if (self.type == VGConfButtonToGroup)
    {
        self.cellType = UITableViewCellAccessoryDisclosureIndicator;
    }

    cell.textLabel.text = self.label;
    cell.accessoryType  = self.cellType;

    return TRUE;
}

-(BOOL)setupValueChooser:(UITableViewCell*)cell
{
    const NSInteger  ival = self.valueInteger;
    NSString*        text = [self.group labelForValue:ival];

    cell.textLabel.text       = self.label;
    cell.detailTextLabel.text = text;

    return TRUE;
}

// code outside this class can set self.accessoryView to be used here

-(BOOL)setupValueDisplay:(UITableViewCell*)cell
{
    const CGFloat  size = [UIFont labelFontSize];
    UIFont*        font = [UIFont systemFontOfSize:size];

    cell.textLabel.font = font;
    cell.textLabel.text = self.label;
    cell.accessoryView  = self.accessoryView;

    if (VGIsKindOf(self.color, [UIColor class]))
    {
        cell.textLabel.textColor = self.color;
    }
    else
    {
        cell.textLabel.textColor = VGConfigCellTextColor();
    }

    return TRUE;
}

-(BOOL)setupValueFloat:(UITableViewCell*)cell
{
    const float   fval = self.valueFloat;
    UITextField*  text = (UITextField*) cell.accessoryView;

    cell.textLabel.text = self.label;
    text.text = [NSString stringWithFloat:fval];
    text.delegate = self;

    return TRUE;
}

-(BOOL)setupValueInteger:(UITableViewCell*)cell
{
    UITextField*  text = (UITextField*) cell.accessoryView;

    cell.textLabel.text = self.label;
    text.text = [NSString stringWithInteger:self.valueInteger];
    text.delegate = self;

    return TRUE;
}

-(BOOL)setupValueLabel:(UITableViewCell*)cell
{
    cell.textLabel.text       = self.label;
    cell.detailTextLabel.text = self.valueString;

    return TRUE;
}

-(BOOL)setupValueText:(UITableViewCell*)cell
{
    UITextField*  text = (UITextField*) cell.accessoryView;
    
    VG_ASSERT(VGIsKindOf(text, [UITextField class]));

    cell.textLabel.text = self.label;
    text.text = self.valueString;
    text.delegate = self;
    text.placeholder = self.placeholder;

    return TRUE;
}

-(void)setValueBool:(BOOL)boolValue
{
    self.value = [NSString stringWithBool:boolValue];
}

-(void)setValueFloat:(float)floatValue
{
    self.value = [NSString stringWithFloat:floatValue];
}

-(void)setValueInteger:(NSInteger)intValue
{
    self.value = [NSString stringWithInteger:intValue];
}

-(void)setValueString:(NSString*)stringValue
{
    self.value = stringValue;
}

-(void)targetControl:(UIControl*)cntl selector:(SEL)selector
{
    // get rid of old target
    [self.control removeTarget:self];
    [self setControl:nil];
    
    // set new target
    if (VGIsKindOf(cntl, [UIControl class]))
    {
        [cntl addButtonTarget:self action:selector];
        [self setControl:cntl];
    }
}

-(void)textFieldDidEndEditing:(UITextField*)textFieldLocal
{
    [self execCell:textFieldLocal];
}

-(BOOL)textFieldShouldReturn:(UITextField*)textFieldLocal
{
    [textFieldLocal resignFirstResponder];
    return TRUE;
}

// width supplied is a percentage of the cell width, from 0.0 to 1.0

-(UITextField*)textField:(BOOL)portraitMode width:(CGFloat)width
{
    CGRect         rect = CGRectZero;
    const CGFloat  size = [self.class cellWidthForMode:portraitMode];
    UITextField*   text = nil;
    
    VG_ASSERT((width > 0.0) && (width < 1.0));
    
    if (self.editWidth > 0.0)
    {
        width = self.editWidth;
    }

    rect.size.width  = floor(size * width);
    rect.size.height = 30.0;

    text = [[[UITextField alloc] initWithFrame:rect] autorelease];

    text.autoresizingMask = VGAutoresizingWidthLeft;
    text.borderStyle      = UITextBorderStyleRoundedRect;
    text.clearButtonMode  = UITextFieldViewModeWhileEditing;
    text.textColor        = VGConfigCellTextColor();
    text.backgroundColor  = VGConfigCellBackgroundColor();
    text.font             = [UIFont systemFontOfSize:17.0];
    text.keyboardType     = UIKeyboardTypeDefault;
    text.returnKeyType    = UIReturnKeyDone;

    return text;
}

-(BOOL)valueBool
{
    return [self.value boolValue];
}

-(float)valueFloat
{
    return [self.value floatValue];
}

-(NSInteger)valueInteger
{
    return [self.value intValue];
}

-(NSString*)valueString
{
    return self.value;
}

@end
