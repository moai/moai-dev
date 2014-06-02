/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.

 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC ``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 EVENT SHALL URBAN AIRSHIP INC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "UAInboxMessageListController.h"
#import "UAInboxMessageListCell.h"
#import "UABarButtonSegmentedControl.h"
#import "UAInboxUI.h"
#import "UAInboxMessageViewController.h"
#import "UAInbox.h"
#import "UAGlobal.h"
#import "UAInboxMessage.h"
#import "UAInboxMessageList.h"

@interface UAInboxMessageListController()

- (void)updateNavigationTitleText;// update nav controller title with unread count
- (void)refreshBatchUpdateButtons;// indicate edit mode view
- (void)deleteMessageAtIndexPath:(NSIndexPath *)indexPath;
- (void)createToolbarItems;
- (void)editButtonPressed:(id)sender;
- (void)cancelButtonPressed:(id)sender;
- (void)didSelectRowAtIndexPath:(NSIndexPath *)indexPath;
- (void)tableReloadData;
- (void)coverUpEmptyListIfNeeded;
- (void)showLoadingScreen;
- (void)hideLoadingScreen;
- (UAInboxMessage *)messageForIndexPath:(NSIndexPath *)indexPath;

- (void)updateSetOfUnreadMessagesWithMessage:(UAInboxMessage *)message atIndexPath:(NSIndexPath *)indexPath;
- (BOOL)checkSetOfIndexPaths:(NSSet *)setOfPaths forIndexPath:(NSIndexPath *)indexPath;
- (NSUInteger)countOfUnreadMessagesInSetOfIndexPaths:(NSSet *)set;

@property (nonatomic, strong) IBOutlet UITableView *messageTable;
@property (nonatomic, strong) IBOutlet UIView *loadingView;
@property (nonatomic, strong) IBOutlet UABeveledLoadingIndicator *loadingIndicator;
@property (nonatomic, strong) IBOutlet UILabel *loadingLabel;
@property (nonatomic, strong) NSMutableSet *setOfUnreadMessagesInSelection;
@property (nonatomic, strong) NSMutableSet *selectedIndexPathsForEditing;
@property (nonatomic, strong) UABarButtonSegmentedControl *deleteItem;
@property (nonatomic, strong) UIBarButtonItem *markAsReadButtonItem;
@property (nonatomic, strong) UIBarButtonItem *editItem;
@property (nonatomic, strong) UIBarButtonItem *cancelItem;
@property (nonatomic, copy) NSString *cellReusableId;
@property (nonatomic, copy) NSString *cellNibName;
@property (nonatomic, strong) id messageListObserver;

@end

@implementation UAInboxMessageListController


- (void)initNibNames {
    self.cellReusableId = @"UAInboxMessageListCell";
    self.cellNibName = @"UAInboxMessageListCell";
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        [self initNibNames];
        
        self.shouldShowAlerts = YES;

        // make our existing layout work in iOS7
        if ([self respondsToSelector:NSSelectorFromString(@"edgesForExtendedLayout")]) {
            self.edgesForExtendedLayout = UIRectEdgeNone;
        }
    }    
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.editItem = [[UIBarButtonItem alloc]
                initWithBarButtonSystemItem:UIBarButtonSystemItemEdit
                target:self
                action:@selector(editButtonPressed:)];
    self.cancelItem = [[UIBarButtonItem alloc]
                  initWithTitle:UA_INBOX_TR(@"UA_Cancel")
                  style:UIBarButtonItemStyleDone
                  target:self
                  action:@selector(cancelButtonPressed:)];

    self.navigationItem.rightBarButtonItem = self.editItem;

    [self createToolbarItems];

    [self updateNavigationTitleText];

    self.selectedIndexPathsForEditing = [[NSMutableSet alloc] init];
}

- (void)createToolbarItems {
    UIBarButtonItem *fixedSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace
                                                                                target:nil action:nil];
    fixedSpace.width = 0;
    UIBarButtonItem *flexibleSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
                                                                                   target:nil action:nil];
    flexibleSpace.width = 25;

    self.deleteItem = [[UABarButtonSegmentedControl alloc]
                        initWithItems:[NSArray arrayWithObject:UA_INBOX_TR(@"UA_Delete")]];
    self.deleteItem.frame = CGRectMake(0, 0, 130, 30);
    self.deleteItem.segmentedControlStyle = UISegmentedControlStyleBar;
    self.deleteItem.momentary = NO;
    self.deleteItem.selectedSegmentIndex = UISegmentedControlNoSegment;
    [self.deleteItem addTarget:self action:@selector(batchUpdateButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
    [self.deleteItem addTarget:self action:@selector(batchUpdateButtonCanceled:) forControlEvents:UIControlEventTouchUpOutside];
    self.deleteItem.tintColor = [UIColor colorWithRed:0.70 green:0.171 blue:0.1 alpha:1.0];

    UIBarButtonItem *deleteButton = [[UIBarButtonItem alloc] initWithCustomView:self.deleteItem];
    deleteButton.width = 130;
    self.markAsReadButtonItem = [[UIBarButtonItem alloc] initWithTitle:UA_INBOX_TR(@"UA_Mark_as_Read")
                                                style:UIBarButtonItemStyleBordered
                                               target:self action:@selector(batchUpdateButtonPressed:)];
    self.markAsReadButtonItem.width = 130;

    self.toolbarItems = [NSArray arrayWithObjects:fixedSpace, deleteButton, flexibleSpace, self.markAsReadButtonItem, fixedSpace, nil];

}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    if ([UAInbox shared].messageList.isRetrieving) {
        [self showLoadingScreen];
    } else {
        [self hideLoadingScreen];
        [self tableReloadData];
        [self updateNavigationTitleText];
    }
    
    [self.messageTable deselectRowAtIndexPath:[self.messageTable indexPathForSelectedRow] animated:animated];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(messageListWillUpdate)
                                                 name:UAInboxMessageListWillUpdateNotification object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(messageListUpdated)
                                                 name:UAInboxMessageListUpdatedNotification object:nil];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];

    [[NSNotificationCenter defaultCenter] removeObserver:self name:UAInboxMessageListWillUpdateNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UAInboxMessageListUpdatedNotification object:nil];
}

- (void)viewDidUnload {
    // Release any retained subviews of the main view.
    self.loadingIndicator = nil;
    self.loadingLabel = nil;
    self.messageTable = nil;
    [self.selectedIndexPathsForEditing removeAllObjects];
    
    self.selectedIndexPathsForEditing = nil;
    self.setOfUnreadMessagesInSelection = nil;
    self.deleteItem = nil;
    self.markAsReadButtonItem = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

// For batch update/delete
- (void)setEditing:(BOOL)editing animated:(BOOL)animated {
    [super setEditing:editing animated:animated];
    [self.navigationController setToolbarHidden:!editing animated:animated];
    [self.messageTable setEditing:editing animated:animated];
}

- (void)tableReloadData {
    [self.messageTable reloadData];
    [self.messageTable deselectRowAtIndexPath:[self.messageTable indexPathForSelectedRow] animated:NO];
}

- (void)refreshAfterBatchUpdate {
    [self.selectedIndexPathsForEditing removeAllObjects];
    self.cancelItem.enabled = YES;
    [self cancelButtonPressed:nil];
    
    [self.messageTable deselectRowAtIndexPath:[self.messageTable indexPathForSelectedRow] animated:NO];
    
    [self refreshBatchUpdateButtons];
}

- (void)showLoadingScreen {
    self.loadingView.hidden = NO;
    self.loadingLabel.text = UA_INBOX_TR(@"UA_Loading");
    [self.loadingIndicator show];
    self.loadingLabel.hidden = NO;
}

- (void)coverUpEmptyListIfNeeded {
    NSUInteger messageCount = [[UAInbox shared].messageList messageCount];
    
    self.loadingView.hidden = (messageCount != 0);
    
    if (messageCount == 0) {
        self.loadingLabel.text = UA_INBOX_TR(@"UA_No_Messages");
    }
}

- (void)hideLoadingScreen {
    [self.loadingIndicator hide];
    [self coverUpEmptyListIfNeeded];
}

// indexPath.row is for use with grouped table views, see NSIndexPath UIKit Additions
- (UAInboxMessage *)messageForIndexPath:(NSIndexPath *)indexPath {
    NSArray *messages = [[UAInboxMessageList shared] messages];
    return [messages objectAtIndex:indexPath.row];
}

- (void)updateSetOfUnreadMessagesWithMessage:(UAInboxMessage *)message atIndexPath:(NSIndexPath *)indexPath {
    if (nil == self.setOfUnreadMessagesInSelection) {
        self.setOfUnreadMessagesInSelection = [NSMutableSet set];
    }
    BOOL messageIsListedAsUnread = [self checkSetOfIndexPaths:self.setOfUnreadMessagesInSelection forIndexPath:indexPath];
    if(messageIsListedAsUnread && message.unread){
        return;
    }
    if(messageIsListedAsUnread && !message.unread){
        [self.setOfUnreadMessagesInSelection removeObject:indexPath];
    }
    if(!messageIsListedAsUnread && message.unread){
        [self.setOfUnreadMessagesInSelection addObject:indexPath];
    }
    
}

- (NSUInteger)countOfUnreadMessagesInSetOfIndexPaths:(NSSet *)set {
    NSUInteger count = 0;
    BOOL isMarkedUnread = NO;
    for (NSIndexPath *path in set) {
        isMarkedUnread = [self checkSetOfIndexPaths:self.setOfUnreadMessagesInSelection forIndexPath:path];
        if(isMarkedUnread) count++;
    }
    return count;
}

- (BOOL)checkSetOfIndexPaths:(NSSet *)setOfPaths forIndexPath:(NSIndexPath *)indexPath {
    for (NSIndexPath *path in setOfPaths) {
        if ([path compare:indexPath] == NSOrderedSame) {
            return YES;
        };
    }
    return NO;
}

#pragma mark -
#pragma mark Button Action Methods

- (void)editButtonPressed:(id)sender {
    
    self.navigationItem.leftBarButtonItem.enabled = NO;
    
    if ([UAInboxMessageList shared].isBatchUpdating) {
        return;
    }
    
    self.navigationItem.rightBarButtonItem = self.cancelItem;
    [self.messageTable deselectRowAtIndexPath:[self.messageTable indexPathForSelectedRow] animated:YES];
    [self setEditing:YES animated:YES];
    // refresh need to be called after setEdit, because in iPad platform,
    // the trash button is decided by the table list's edit status.
    [self refreshBatchUpdateButtons];
}

- (void)cancelButtonPressed:(id)sender {
    
    self.navigationItem.leftBarButtonItem.enabled = YES;
    
    self.navigationItem.rightBarButtonItem = self.editItem;

    if ([self.selectedIndexPathsForEditing count] > 0) {
        NSSet *visibleCells = [NSSet setWithArray:[self.messageTable indexPathsForVisibleRows]];
        [self.selectedIndexPathsForEditing intersectSet:visibleCells];

        for (NSIndexPath *indexPath in self.selectedIndexPathsForEditing) {
            [self.messageTable cellForRowAtIndexPath:indexPath].selected = NO;
        }

        [self.selectedIndexPathsForEditing removeAllObjects];
    }

    [self setEditing:NO animated:YES];
    [self updateNavigationTitleText];
}

- (void)batchUpdateButtonPressed:(id)sender {
    NSMutableIndexSet *messageIDs = [NSMutableIndexSet indexSet];
    for (NSIndexPath *indexPath in self.selectedIndexPathsForEditing) {
        [messageIDs addIndex:indexPath.row];
    }

    self.cancelItem.enabled = NO;

    if (sender == self.markAsReadButtonItem) {
        [[UAInbox shared].messageList performBatchUpdateCommand:UABatchReadMessages
                                            withMessageIndexSet:messageIDs
                                                    withDelegate:self];
    } else {
        [[UAInbox shared].messageList performBatchUpdateCommand:UABatchDeleteMessages
                                            withMessageIndexSet:messageIDs
                                                   withDelegate:self];
    }

    self.deleteItem.selectedSegmentIndex = UISegmentedControlNoSegment;
    [self refreshBatchUpdateButtons];
}

- (void)batchUpdateButtonCanceled:(id)sender {
    self.deleteItem.selectedSegmentIndex = UISegmentedControlNoSegment;
}

- (void)refreshBatchUpdateButtons {
    NSString* deleteStr = UA_INBOX_TR(@"UA_Delete");
    NSString* markReadStr = UA_INBOX_TR(@"UA_Mark_as_Read");
    
    NSUInteger count = [self.selectedIndexPathsForEditing count];
    if (count == 0) {
        [self.deleteItem setTitle:deleteStr forSegmentAtIndex:0];
        self.markAsReadButtonItem.title = markReadStr;
        self.deleteItem.enabled = NO;
        self.markAsReadButtonItem.enabled = NO;
    } else {
        [self.deleteItem setTitle:[NSString stringWithFormat:@"%@ (%lu)", deleteStr, (unsigned long)count] forSegmentAtIndex:0];
        NSUInteger ureadCountInSelection = [self countOfUnreadMessagesInSetOfIndexPaths:self.selectedIndexPathsForEditing];
        self.markAsReadButtonItem.title = [NSString stringWithFormat:@"%@ (%lu)", markReadStr, (unsigned long)ureadCountInSelection];
        if ([UAInbox shared].messageList.isBatchUpdating) {
            self.deleteItem.enabled = NO;
            self.markAsReadButtonItem.enabled = NO;
        } else {
            self.deleteItem.enabled = YES;
            if (ureadCountInSelection != 0) {
                self.markAsReadButtonItem.enabled = YES;
            } else {
                self.markAsReadButtonItem.enabled = NO;
            }
        }
    }

}

- (void)deleteMessageAtIndexPath:(NSIndexPath *)indexPath {
    NSMutableIndexSet *set = [NSMutableIndexSet indexSet];
    [set addIndex:indexPath.row];
    [self.selectedIndexPathsForEditing removeAllObjects];
    [self.selectedIndexPathsForEditing addObject:indexPath];
    [[UAInbox shared].messageList performBatchUpdateCommand:UABatchDeleteMessages
                                        withMessageIndexSet:set
                                               withDelegate:self];
    [self refreshBatchUpdateButtons];
}

#pragma mark -
#pragma mark UITableViewDataSource

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UAInboxMessageListCell* cell = (UAInboxMessageListCell*)[tableView dequeueReusableCellWithIdentifier:self.cellReusableId];
    if (cell == nil) {
        NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:self.cellNibName owner: nil options: nil];
        cell = [topLevelObjects objectAtIndex:0];
    }

    [cell setData:[[UAInbox shared].messageList messageAtIndex:indexPath.row]];

    cell.editing = tableView.editing;
    if (cell.editing) {
        cell.selected = [self.selectedIndexPathsForEditing containsObject:indexPath];
    } else {
        NSIndexPath *selectedPath = [tableView indexPathForSelectedRow];
        cell.selected = (selectedPath ? selectedPath.row == indexPath.row : NO);
    }

    return cell;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    [self deleteMessageAtIndexPath:indexPath];
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    NSUInteger messageCount = [[UAInbox shared].messageList messageCount];
    self.editItem.enabled = (messageCount == 0) ? NO : YES;
    return messageCount;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    return YES;
}

- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath {
    if (self.editing)
        return UITableViewCellEditingStyleNone;
    else
        return UITableViewCellEditingStyleDelete;
}

- (void)tableView:(UITableView *)tableView willBeginEditingRowAtIndexPath:(NSIndexPath *)indexPath {
    self.navigationItem.rightBarButtonItem.enabled = NO;
}

- (void)tableView:(UITableView *)tableView didEndEditingRowAtIndexPath:(NSIndexPath *)indexPath {
    self.navigationItem.rightBarButtonItem.enabled = YES;
}

#pragma mark -
#pragma mark UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    UAInboxMessage *message = [self messageForIndexPath:indexPath];
    [self updateSetOfUnreadMessagesWithMessage:message atIndexPath:indexPath];
    if (self.editing && ![[UAInbox shared].messageList isBatchUpdating]) {
        if ([self.selectedIndexPathsForEditing containsObject:indexPath]) {
            [tableView deselectRowAtIndexPath:indexPath animated:YES];
            [self.selectedIndexPathsForEditing removeObject:indexPath];
        } else {
            [self.selectedIndexPathsForEditing addObject:indexPath];
        }
        [self refreshBatchUpdateButtons];
        [[tableView cellForRowAtIndexPath:indexPath] setNeedsLayout];
    } else if (!self.editing) {
        [self didSelectRowAtIndexPath:indexPath];
    }
   
}


- (void)didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    UAInboxMessage *message = [[UAInboxMessageList shared] messageAtIndex:indexPath.row];
    [UAInbox displayMessageWithID:message.messageID inViewController:self.navigationController];
}

#pragma mark -
#pragma mark NSNotificationCenter callbacks

- (void)messageListWillUpdate {
    [self showLoadingScreen];
}

- (void)messageListUpdated {
    UA_LDEBUG(@"UAInboxMessageListController messageListUpdated");
	
    [self hideLoadingScreen];
    
    [self tableReloadData];
    [self updateNavigationTitleText];
}

#pragma mark -
#pragma mark UAInboxMessageListDelegate

- (void)batchMarkAsReadFinished {
    [self.messageTable reloadRowsAtIndexPaths:[self.selectedIndexPathsForEditing allObjects]
                        withRowAnimation:UITableViewRowAnimationNone];
    [self refreshAfterBatchUpdate];
}


- (void)batchMarkAsReadFailed {
    if (self.shouldShowAlerts) {
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:UA_INBOX_TR(@"UA_Mailbox_Error_Title")
                                                        message:UA_INBOX_TR(@"UA_Error_Mark_Read_Message")
                                                       delegate:nil
                                              cancelButtonTitle:UA_INBOX_TR(@"UA_OK")
                                              otherButtonTitles:nil];
        [alert show];
        
    }
    [self refreshAfterBatchUpdate];
}


- (void)batchDeleteFinished {
    [self.messageTable beginUpdates];
    [self.messageTable deleteRowsAtIndexPaths:[self.selectedIndexPathsForEditing allObjects]
                        withRowAnimation:UITableViewRowAnimationLeft];
    [self.messageTable endUpdates];
    
    [self refreshAfterBatchUpdate];
}


- (void)batchDeleteFailed {
    if (self.shouldShowAlerts) {
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:UA_INBOX_TR(@"UA_Mailbox_Error_Title")
                                                        message:UA_INBOX_TR(@"UA_Error_Delete_Message")
                                                       delegate:nil
                                              cancelButtonTitle:UA_INBOX_TR(@"UA_OK")
                                              otherButtonTitles:nil];
        [alert show];
        
    }
    [self refreshAfterBatchUpdate];
}


- (void)singleMessageMarkAsReadFinished:(UAInboxMessage *)m {
    NSUInteger row = [[UAInbox shared].messageList indexOfMessage:m];
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:0];
    UAInboxMessageListCell *cell = (UAInboxMessageListCell *)[self.messageTable cellForRowAtIndexPath:indexPath];
    cell.unreadIndicator.hidden = YES;
    [self updateNavigationTitleText];
}

#pragma mark -
#pragma mark Navigation Title Unread Count Updates


- (void)updateNavigationTitleText {
    NSInteger count = [UAInbox shared].messageList.unreadCount;

    if (count < 0) {
        count = 0;
    }

    self.title = [NSString stringWithFormat:UA_INBOX_TR(@"UA_Inbox_List_Title"), count];
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
                                         duration:(NSTimeInterval)duration{
    [self updateNavigationTitleText];
}


@end
