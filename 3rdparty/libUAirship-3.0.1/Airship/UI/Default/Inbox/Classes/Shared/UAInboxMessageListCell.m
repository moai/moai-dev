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


#import "UAInboxMessageListCell.h"
#import "UAGlobal.h"
#import "UADateUtils.h"


@implementation UAInboxMessageListCell


- (void)setData:(UAInboxMessage*)message {

    self.dateView.text = [UADateUtils formattedDateRelativeToNow:message.messageSent];

    [self.title setText: message.title];

    if(!message.unread) {
        self.unreadIndicator.hidden = YES;
    } else {
        self.unreadIndicator.hidden = NO;
    }
}

- (void)layoutSubviews {
    if(self.editing) {
        self.selectionStyle = UITableViewCellSelectionStyleNone;

        if (self.editingStyle == UITableViewCellEditingStyleNone) {
            // batch update
            self.checkmark.hidden = NO;
            if (self.selected) {
                self.checkmark.image = [UIImage imageNamed:@"check.png"];
                self.backgroundView = self.selectedEditingBackgroundView;
            } else {
                self.checkmark.image = [UIImage imageNamed:@"uncheck.png"];
                self.backgroundView = nil;
            }
        } else if (self.editingStyle == UITableViewCellEditingStyleDelete) {
            // swipe deletion
            self.dateView.hidden = YES;
        }
    } else {
        self.backgroundView = nil;
        self.selectionStyle = UITableViewCellSelectionStyleBlue;
        self.checkmark.hidden = YES;
        self.dateView.hidden = NO;
    }

    [super layoutSubviews];
}

@end
