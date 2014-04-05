//
//  LoggingTableCell.m
//  SampleAdColonyApp
//
//  Created by Erik on 10/13/10.
//  Copyright 2010 Jirbo. All rights reserved.
//

#import "LoggingTableCell.h"


@implementation LoggingTableCell

@synthesize logLabel;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
        // Initialization code
    }
    return self;
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}


- (void)dealloc {
	[logLabel release];
    [super dealloc];
}


@end
