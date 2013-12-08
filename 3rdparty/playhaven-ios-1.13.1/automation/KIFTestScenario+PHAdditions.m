/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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

 KIFTestScenario+PHAdditions.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/1/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

#import "KIFTestScenario+PHAdditions.h"
#import "KIFTestStep.h"
#import "KIFTestStep+PHAdditions.h"


@implementation KIFTestScenario (PHAdditions)
+ (id)scenarioToSendOpenRequest
{
    KIFTestScenario *result = [KIFTestScenario scenarioWithDescription:@"Sending an open request..."];

    [result addStepsFromArray:[KIFTestStep stepsToResetApp]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"open"]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"start"]];
    [result addStep:[KIFTestStep stepToWaitForViewWithAccessibilityLabel:@"Request success message"]];

    return result;
}

+ (id)scenarioToSendOpenRequestWithCustomDeviceId
{
    KIFTestScenario *result = [KIFTestScenario scenarioWithDescription:@"Sending an open request with a custom device id..."];

    [result addStepsFromArray:[KIFTestStep stepsToResetApp]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"open"]];
    [result addStep:[KIFTestStep stepToEnterText:@"test_id" intoViewWithAccessibilityLabel:@"custom"]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"start"]];
    [result addStep:[KIFTestStep stepToWaitForViewWithAccessibilityLabel:@"Request success message"]];

    return result;
}

+ (id)scenarioToSendContentRequest
{
    /*
     content type: More Games Widget
     placement: more_games

     testing more games content unit with featured game turn on
    */
    KIFTestScenario *result = [KIFTestScenario scenarioWithDescription:@"Sending a content request..."];

    [result addStepsFromArray:[KIFTestStep stepsToResetApp]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"content"]];
    [result addStep:[KIFTestStep stepToEnterText:@"more_games" intoViewWithAccessibilityLabel:@"placement"]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"start"]];

    // Featured game content unit
    [result addStep:[KIFTestStep stepToWaitForDispatch:@"ph://loadContext" andCallback:YES]];
    [result addStep:[KIFTestStep stepToWaitForTimeInterval:1.0 description:@"HACKY: Waiting for the webview to finish rendering before attempting to tap button."]];
    [result addStep:[KIFTestStep stepToTapElementWithSelector:@"#more_button" inWebViewWithAccessibilityLabel:@"content view"]];
    [result addStep:[KIFTestStep stepToWaitForDispatch:@"ph://subcontent"]];
    [result addStep:[KIFTestStep stepToWaitForDispatch:@"ph://dismiss"]];

    // More games content unit
    [result addStep:[KIFTestStep stepToWaitForDispatch:@"ph://loadContext" andCallback:YES]];
    [result addStep:[KIFTestStep stepToWaitForTimeInterval:1.0 description:@"HACKY: Waiting for the webview to finish rendering before attempting to tap button."]];
    [result addStep:[KIFTestStep stepToTapElementWithSelector:@"#dismiss_button" inWebViewWithAccessibilityLabel:@"content view"]];
    [result addStep:[KIFTestStep stepToWaitForDispatch:@"ph://dismiss"]];
    [result addStep:[KIFTestStep stepToWaitForAbsenceOfViewWithAccessibilityLabel:@"content view"]];

    return  result;
}

+ (id)scenarioToSendContentRequestTestingReward
{
    /*
     content type: reward
     placement: reward

     testing a reward content unit that rewards item 'delicious_cake'
    */
    KIFTestScenario *result = [KIFTestScenario scenarioWithDescription:@"Sending a content request and verifying it returns a reward"];

    [result addStepsFromArray:[KIFTestStep stepsToResetApp]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"content"]];
    [result addStep:[KIFTestStep stepToEnterText:@"reward" intoViewWithAccessibilityLabel:@"placement"]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"start"]];


    // Reward content unit
    [result addStep:[KIFTestStep stepToWaitForDispatch:@"ph://loadContext" andCallback:YES]];
    [result addStep:[KIFTestStep stepToVerifyRewardUnlocked:@"delicious_cake" quantity:1]];
    [result addStep:[KIFTestStep stepToWaitForTimeInterval:1.0 description:@"HACKY: Waiting for the webview to finish rendering before attempting to tap button."]];
    [result addStep:[KIFTestStep stepToTapElementWithSelector:@"#button" inWebViewWithAccessibilityLabel:@"content view"]];
    [result addStep:[KIFTestStep stepToWaitForDispatch:@"ph://dismiss"]];
    [result addStep:[KIFTestStep stepToWaitForAbsenceOfViewWithAccessibilityLabel:@"content view"]];

    return result;
}

+ (id)scenarioToSendContentRequestTestingAnnouncementLaunch
{
    /*
     content type: announcement
     placement:announcement_launch

     testing an announcement with custom URL launch set to 'http://www.playhaven.com'
    */

    KIFTestScenario *result = [KIFTestScenario scenarioWithDescription:@"Sending a content request and testing announcement launch"];
    [result addStepsFromArray:[KIFTestStep stepsToResetApp]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"content"]];
    [result addStep:[KIFTestStep stepToEnterText:@"announcement_launch" intoViewWithAccessibilityLabel:@"placement"]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"start"]];

    // Announcement content unit
    [result addStep:[KIFTestStep stepToWaitForDispatch:@"ph://loadContext" andCallback:YES]];
    [result addStep:[KIFTestStep stepToWaitForTimeInterval:1.0 description:@"HACKY: Waiting for the webview to finish rendering before attempting to tap button."]];
    [result addStep:[KIFTestStep stepToTapElementWithSelector:@"#button" inWebViewWithAccessibilityLabel:@"content view"]];
    [result addStep:[KIFTestStep stepToVerifyLaunchURL:@"http://www.playhaven.com/"]];
    [result addStep:[KIFTestStep stepToWaitForDispatch:@"ph://dismiss"]];
    [result addStep:[KIFTestStep stepToWaitForAbsenceOfViewWithAccessibilityLabel:@"content view"]];

    return result;
}

+ (id)scenarioToLoadiTunesAndVerifyReferral
{
    /*
     just testing the URL loader, no dashboard dependency here
    */

    KIFTestScenario *result = [KIFTestScenario scenarioWithDescription:@"Loading itunes and verifying referral."];
    [result addStepsFromArray:[KIFTestStep stepsToResetApp]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"url loader"]];

    // This is a GeoRiot short link that points to the iTunes version of
    // Justin Bieber's seminal album, My World.
    // We are testing to see if the launched URL contains the partnerId and siteID
    // parameters that are needed for recording affiliate referrals.
    [result addStep:[KIFTestStep stepToEnterText:@"http://glob.ly/2yWE/iso2/EN" intoViewWithAccessibilityLabel:@"URL"]];
    [result addStep:[KIFTestStep stepToTapViewWithAccessibilityLabel:@"start"]];
    [result addStep:[KIFTestStep stepToVerifyLaunchURLContainsHost:@"itunes.apple.com"]];
    [result addStep:[KIFTestStep stepToVerifyLaunchURLContainsParameter:@"partnerId"]];
    [result addStep:[KIFTestStep stepToVerifyLaunchURLContainsParameter:@"siteID"]];

    return  result;
}
@end
