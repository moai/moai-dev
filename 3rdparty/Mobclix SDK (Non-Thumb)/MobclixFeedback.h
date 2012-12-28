//
//  MobclixAds.h
//  Mobclix iOS SDK
//
//  Copyright 2011 Mobclix. All rights reserved.
//

typedef enum {
	MCFeedbackRatingUnknown = 0,
	MCFeedbackRatingPoor,
	MCFeedbackRatingFair,
	MCFeedbackRatingGood,
	MCFeedbackRatingVeryGood,
	MCFeedbackRatingExcellent
} MCFeedbackRating;

typedef struct {
	MCFeedbackRating categoryA;
	MCFeedbackRating categoryB;
	MCFeedbackRating categoryC;
	MCFeedbackRating categoryD;
	MCFeedbackRating categoryE;
} MCFeedbackRatings;

@protocol MobclixFeedbackDelegate;

@interface MobclixFeedback : NSObject {

}

- (void)sendComment:(NSString*)comment;
- (void)sendRatings:(MCFeedbackRatings)ratings;

@property(nonatomic,assign) id<MobclixFeedbackDelegate> delegate;
@property(nonatomic,assign,readonly,getter=isSendingComment) BOOL sendingComment;
@property(nonatomic,assign,readonly,getter=isSendingRatings) BOOL sendingRatings;

#pragma mark -
#pragma mark Deprecated methods

// These methods have been deprecated as of Mobclix SDK 4.2 and will be removed in a future version.
+ (void)sendComment:(NSString*)comment;
+ (void)sendRatings:(MCFeedbackRatings)ratings;

@end

#pragma mark -

@protocol MobclixFeedbackDelegate<NSObject>
@optional

// Comment Delgates
- (void)mobclixFeedbackSentComment:(MobclixFeedback*)feedback;
- (void)mobclixFeedbackFailedToSendComment:(MobclixFeedback*)feedback withError:(NSError*)error;

// Feedback Delegates
- (void)mobclixFeedbackSentRatings:(MobclixFeedback*)feedback;
- (void)mobclixFeedbackFailedToSendRatings:(MobclixFeedback*)feedback withError:(NSError*)error;

@end