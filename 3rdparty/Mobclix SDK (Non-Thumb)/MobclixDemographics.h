//
//  MobclixDemographics.h
//  Mobclix iOS SDK
//
//  Copyright 2011 Mobclix. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum {
	MCDemographicsEducationUnknown = 0,
	MCDemographicsEducationHighSchool,
	MCDemographicsEducationSomeCollege,
	MCDemographicsEducationInCollege,
	MCDemographicsEducationBachelorsDegree,
	MCDemographicsEducationMastersDegree,
	MCDemographicsEducationDoctoralDegree
} MCDemographicsEducation;

typedef enum {
	MCDemographicsEthnicityUnknown = 0,
	MCDemographicsEthnicityMixed,
	MCDemographicsEthnicityAsian,
	MCDemographicsEthnicityBlack,
	MCDemographicsEthnicityHispanic,
	MCDemographicsEthnicityNativeAmerican,
	MCDemographicsEthnicityWhite
} MCDemographicsEthnicity;

typedef enum {
	MCDemographicsReligionUnknown = 0,
	MCDemographicsReligionBuddhism,
	MCDemographicsReligionChristianity,
	MCDemographicsReligionHinduism,
	MCDemographicsReligionIslam,
	MCDemographicsReligionJudaism,
	MCDemographicsReligionUnaffiliated,
	MCDemographicsReligionOther
} MCDemographicsReligion;

typedef enum {
	MCDemographicsGenderUnknown = 0,
	MCDemographicsGenderMale,
	MCDemographicsGenderFemale,
	MCDemographicsGenderBoth,
} MCDemographicsGender;

typedef enum {
	MCDemographicsMaritalUnknown = 0,
	MCDemographicsMaritalSingleAvailable,
	MCDemographicsMaritalSingleUnavailable,
	MCDemographicsMaritalMarried,
} MCDemographicsMaritalStatus;

typedef struct {
	MCDemographicsEducation education;
	MCDemographicsEthnicity ethnicity;
	MCDemographicsReligion religion;
	MCDemographicsGender gender; // MCDemographicsGenderBoth is not valid for this field.
	MCDemographicsGender datingGender; // MCDemographicsGenderBoth is valid for this field.
	MCDemographicsMaritalStatus maritalStatus;
	NSUInteger income;
	NSUInteger areaCode;
	NSInteger dmaCode;
	NSInteger metroCode;
	const char* city;
	const char* country;
	const char* postalCode;
	const char* regionCode;
	double latitude;
	double longitude;
} MCDemographics;

@interface MobclixDemographics : NSObject {

}

+ (void)updateDemographics:(MCDemographics)demographics birthday:(NSDate*)birthday;

+ (MCDemographics)demographics;
+ (NSDate*)birthday;

@end
