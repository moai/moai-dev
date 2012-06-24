//
//  BurstlyUserInfo.h
//  Burstly
//
//  Created by Evgeny Dedovec on 12/5/11.
//  Copyright 2011 Burstly. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BurstlyUserInfo : NSObject {
    NSString *dateOfBitrh_;
    NSString *postalCode_;
    NSString *zipCode_;
    NSString *areaCode_;
    NSString *regionCode_;
    NSString *city_;
    NSString *latitude_;
    NSString *longitude_;
    NSString *gender_;
    NSString *keyWords_;
    NSString *blockKeywords_;
    NSString *income_;
    NSString *educationType_;
    NSString *ethnicityType_;
    NSString *email_;
    NSString *language_;
    NSString *country_;
    NSString *interests_;
    NSString *searchString_;
} 

- (NSDictionary*)userParams;
- (void)setDefaults;

@property (nonatomic, retain) NSString  *dateOfBitrh;
@property (nonatomic, retain) NSString  *postalCode;
@property (nonatomic, retain) NSString  *areaCode;
@property (nonatomic, retain) NSString  *zipCode;
@property (nonatomic, retain) NSString  *regionCode;
@property (nonatomic, retain) NSString  *city;
@property (nonatomic, retain) NSString  *latitude;
@property (nonatomic, retain) NSString  *longitude;
@property (nonatomic, retain) NSString  *gender;
@property (nonatomic, retain) NSString  *keyWords;
@property (nonatomic, retain) NSString  *blockKeywords;
@property (nonatomic, retain) NSString  *income;
@property (nonatomic, retain) NSString  *educationType;
@property (nonatomic, retain) NSString  *ethnicityType;
@property (nonatomic, retain) NSString  *email;
@property (nonatomic, retain) NSString  *language;
@property (nonatomic, retain) NSString  *country;
@property (nonatomic, retain) NSString  *interests;
@property (nonatomic, retain) NSString  *searchString;

@end
