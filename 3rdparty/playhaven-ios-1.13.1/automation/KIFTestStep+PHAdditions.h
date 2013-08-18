/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 KIFTestStep+PHAdditions.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/4/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "KIFTestStep.h"

@interface KIFTestStep (PHAdditions)
+ (NSArray *)stepsToResetApp;

// Runs arbitrary javascript in webviews. This means we can
+ (id)stepToWaitForWebViewWithAccessibilityLabelToFinishLoading:(NSString *)label;
+ (id)stepToRunJavascript:(NSString *)javascript inWebViewWithAccessibilityLabel:(NSString *)label;
+ (id)stepToRunJavascript:(NSString *)javascript inWebViewWithAccessibilityLabel:(NSString *)label expectedResult:(NSString *)expectedResult;

+ (id)stepToTapElementWithSelector:(NSString *)selector inWebViewWithAccessibilityLabel:(NSString *)label;

+ (id)stepToClearTextFromViewWithAccessibilityLabel:(NSString *)label;

// SDK specific steps
+ (id)stepToVerifyRewardUnlocked:(NSString *)reward quantity:(NSInteger)quantity;
+ (id)stepToVerifyLaunchURL:(NSString *)urlPath;
+ (id)stepToVerifyLaunchURLContainsHost:(NSString *)host;
+ (id)stepToVerifyLaunchURLContainsParameter:(NSString *)parameter;
+ (id)stepToWaitForDispatch:(NSString *)dispatch;
+ (id)stepToWaitForDispatch:(NSString *)dispatch andCallback:(BOOL)waitForCallback;
@end
