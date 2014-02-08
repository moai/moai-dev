// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


typedef enum TJCEventRequestTypeEnum {
	TJEventRequestInAppPurchase = 1,
	TJEventRequestVirtualGood,
	TJEventRequestCurrency,
	TJEventRequestNavigation
} TJCEventRequestType;


/**
 A request for an app to take an action, triggered by TJEvent content.
 
 Your app should perform an action based on the value of the request type property:
 
 - `TJEventRequestInAppPurchase`: an in-app purchase request.  Your app should initiate an in-app
 purchase of the product identified by the identifier property.  If the purchase is completed
 successfully, the app should invoke the completed method; otherwise, it must invoke the
 cancelled method.
 - `TJEventRequestVirtualGood`: a virtual good award request.  Your app should award the user the
 item specified by the identifier with the amount specified by quantity.  If the virtual good
 was successfully rewarded, the app should invoke the completed method; otherwise, it must invoke the
 cancelled method
 - `TJEventRequestCurrency`: a currency change notification.  The user has been  awarded the currency
 specified with identifier, with the amount specified by quantity.  The app should invoke the completed
 method once the request is handled.
 - `TJEventRequestNavigation`: an in-app navigation request.  Your app should attempt to navigate
 to the location specified by the identifier.  If the identifier is recognized and the app navigates
 successfully, the app should invoke the completed method; otherwise, it must invoke the
 cancelled method.
 
 The "identifiers" provided by the request here are the identifiers that you configure in the Tapjoy
 dashboard when creating content.  So, for example, you will only receive TJEventRequestVirtualGood
 requests with identifiers that you have configured yourself.  As such, you should carefully consider
 in advance what kinds of virtual good and navigation locations your app should support.  By implementing
 as many possible options upfront, your app will be more flexible and be able to make the best use
 of Tapjoy's publisher messaging.
 
 Your app *must* call either completed or cancelled to complete the lifecycle of the request, otherwise
 the content may fail to dismiss and yield control back to your app.
 */
@interface TJEventRequest : NSObject

/**
 * The type of the request
 *
 * This request type indicates the action that your app should take.  The value is one of
 * `TJEventRequestInAppPurchase`, `TJEventRequestVirtualGood`, `TJEventRequestCurrency`  or  `TJEventRequestNavigation`.
 * Your app should take action according to the request type as described in the class documentation above.
 */
@property (nonatomic,assign) TJCEventRequestType type;

/**
 * The identifier associated with the request.
 *
 * The use of the identifier varies depending on the type of the request.  See the class documentation
 * for more details on how to handle the various request types.
 */
@property (nonatomic,copy) NSString *identifier;

/**
 * The quantity associated with the request.
 *
 * This quantity is meaningful only for
 * `TJEventRequestVirtualGood` and `TJEventRequestCurrency` events.
 */
@property (nonatomic,assign) int quantity;

/**
 * Called by your app to indicate that the request was processed successfully.
 *
 * Should be called when the event request is completed successfully.  In the case of
 * `TJEventRequestInAppPurchase`, this indicates specifically that the user successfully
 * completed the purchase.
 */
- (void)completed;

/**
 * Called by your app to indicate that the request was not processed successfully.
 *
 * Should be called when the event request is cancelled or otherwise not successfully completed.
 */
- (void)cancelled;

@property (nonatomic, copy) id callback;

@end

@class TJEvent;

/**
  The Tapjoy Event Delegate protocol. Adopt this protocol in order to handle responses that are received upon sending a TJEvent.
 
  The methods to prepare are:
 
  - sendEventComplete:withContent: Called when an event is sent successfully
  - sendEventFail:error: Called when an error occurs while sending the event
  - contentWillAppear: Called when event content will appear
  - contentDidAppear: Called when event content did appear
  - contentWillDisappear: Called when event content will disappear
  - contentDidDisappear: Called when event content did disappear
  - event:didRequestAction: Called when an action occurs, such as in-app purchases, and currency or virtual goods rewards
 
  As a result of executing the send method, one of these delegate callbacks will be invoked, and the application should act accordingly, presenting any content that is returned. For example:
 
    -(void) sendEventComplete: (TJEvent*)event withContent:(BOOL)contentIsAvailable
    {
        if(contentIsAvailable){
            [event presentContentWithViewController:self];
        }
        else{
            NSLog(@"No content is available");
        }
    }
 */
@protocol TJEventDelegate <NSObject>

@optional

/**
 * Called when an event is sent successfully
 * @param event The TJEvent that was sent
 * @param contentIsAvailable YES if content is available, otherwise NO
 * @return n/a
 */
- (void)sendEventComplete:(TJEvent*)event withContent:(BOOL)contentIsAvailable;

/**
 * Called when an error occurs while sending the event
 * @param event The TJEvent that was sent
 * @param error The NSError that occured 
 * @return n/a
 */
- (void)sendEventFail:(TJEvent*)event error:(NSError*)error;

/**
 * Called when event content will appear
 * @param event The TJEvent that was sent
 * @return n/a
 */
- (void)contentWillAppear:(TJEvent*)event;

/**
 * Called when event content did appear
 * @param event The TJEvent that was sent
 * @return n/a
 */
- (void)contentDidAppear:(TJEvent*)event;

/**
 * Called when event content will disappear
 * @param event The TJEvent that was sent
 * @return n/a
 */
- (void)contentWillDisappear:(TJEvent*)event;

/**
 * Called when event content did disappear
 * @param event The TJEvent that was sent
 * @return n/a
 */
- (void)contentDidDisappear:(TJEvent*)event;

/**
 * Called when an action is requested of your app.
 * @param event The TJEvent that was triggered the action request
 * @param description The TJEventRequest object describing the request.  Your app should process the request
 *                    according to the documentation laid out in the TJEventRequest class.
 * @return n/a
 */
- (void)event:(TJEvent*)event didRequestAction:(TJEventRequest*)request;

@end

/**
  The Tapjoy Event-Based Framework allows one to identify key events within their application during development,
  and then reconfigure them on the dashboard as desired to help maximize monetization and engagement, without the need to update or resubmit the application.
  
  Use the TJEvent class to define event points in your application where ads and other content can be served.
  Events could include launching the application, completing an achievement, finishing a level, or any other moment conducive to communicating with your users.
 
  During your application development process, the key steps are to:
 
  1. Create and configure each event as a TJEvent
 
        TJEvent *event = [TJEvent eventWithNAme: @"level_complete" delegate: self];
 
  2. Send the event
 
        [event send];
 
  3. Present any content that is returned by the event callbacks defined in TJEventDelegate
 */
@interface TJEvent : NSObject

@property (nonatomic, unsafe_unretained) id<TJEventDelegate> delegate;
@property (nonatomic, copy) NSString *eventName;
@property (nonatomic, strong) NSString *eventValue;
@property (nonatomic, assign) BOOL presentAutomatically;
@property (nonatomic, retain) UIViewController* presentationViewController;

/**
 * Creates a new instance of TJEvent
 * @param eventName The name of the event
 * @param delegate The class that implements the TJEventDelegate protocol
 */
+ (id)eventWithName:(NSString*)eventName delegate:(id<TJEventDelegate>)delegate;

/**
 * Creates a new instance of TJEvent with additional parameters
 * @param eventName The name of the event
 * @param delegate The class that implements the TJEventDelegate protocol
 * @param value NSString that specifies the additional event value
 */
+ (id)eventWithName:(NSString*)eventName delegate:(id<TJEventDelegate>)delegate value:(NSString*)value;

/**
 * Sends the event to the server
 *
 * @return n/a
 */
- (void)send;

/**
 * Shows the content that was received from the server after sending this event
 * @param viewController The ViewController to show the content in
 * @return n/a
 */
- (void)presentContentWithViewController:(UIViewController*)viewController;

@end
