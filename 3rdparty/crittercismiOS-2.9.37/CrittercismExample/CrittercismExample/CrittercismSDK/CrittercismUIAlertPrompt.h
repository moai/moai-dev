//
//  CrittercismUIAlertPrompt.h
//  LinkedTest
//
//  Created by Kevin Su on 7/5/11.
//  Copyright 2011 Crittercism. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>

@interface CrittercismUITextField : UITextField
@end

@interface CrittercismUIAlertPrompt : UIAlertView {
    CrittercismUITextField *_textField;
}
@property (nonatomic, retain) CrittercismUITextField *textField;
@property (readonly) NSString *text;
- (id) initWithTitle:(NSString *)title message:(NSString *)message delegate:(id)delegate cancelButtonTitle:(NSString *)cancelButtonTitle okButtonTitle:(NSString *)okButtonTitle;

@end
