//
//  LoggingTableCell.h
//  SampleAdColonyApp
//
//  Created by Erik on 10/13/10.
//  Copyright 2010 Jirbo. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface LoggingTableCell : UITableViewCell {
	IBOutlet UILabel *logLabel;
}

@property (nonatomic, retain) UILabel *logLabel;

@end
