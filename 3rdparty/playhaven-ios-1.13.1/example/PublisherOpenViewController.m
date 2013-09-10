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

 PublisherOpenViewController.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 4/25/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PublisherOpenViewController.h"

@interface PHAPIRequest (Private)
+ (void)setSession:(NSString *)session;
@end

@implementation PublisherOpenViewController
@synthesize customUDIDField;
@synthesize pluginIdentifier;

- (void)startRequest
{
    [super startRequest];

    /*
     * This is an alternate implementation which allows you you get response
     * data from API requests. This isn't necessary for most developers.
     */

    if (self.pluginIdentifier.text && ![self.pluginIdentifier.text isEqualToString:@""])
        [PHAPIRequest setPluginIdentifier:self.pluginIdentifier.text];
    else
        [PHAPIRequest setPluginIdentifier:nil];

    PHPublisherOpenRequest *request = [PHPublisherOpenRequest requestForApp:self.token secret:self.secret];

    if (self.customUDIDField.text && ![self.customUDIDField.text isEqualToString:@""])
        request.customUDID = self.customUDIDField.text;
    else
        request.customUDID = nil;

    request.delegate = self;

    [request send];

    [self.customUDIDField resignFirstResponder];
}

- (void)dealloc
{
    [PHAPIRequest cancelAllRequestsWithDelegate:self];
    [customUDIDField release];
    [pluginIdentifier release];
    [super dealloc];
}

#pragma mark - PHAPIRequestDelegate
- (void)request:(PHAPIRequest *)request didSucceedWithResponse:(NSDictionary *)responseData
{
    NSString *message = [NSString stringWithFormat:@"[OK] Success with response: %@", responseData];
    [self addMessage:message];

    [self finishRequest];
}

- (void)request:(PHAPIRequest *)request didFailWithError:(NSError *)error
{
    NSString *message = [NSString stringWithFormat:@"[ERROR] Failed with error: %@", error];
    [self addMessage:message];

    [self finishRequest];
}

- (void)requestFinishedPrefetching:(PHAPIRequest *)request
{
    [self addMessage:@"Finished prefetching!"];
    [self addElapsedTime];
}

- (void)viewDidLoad
{
    if ([PHAPIRequest customUDID])
        customUDIDField.text  = [PHAPIRequest customUDID];

    if (![[PHAPIRequest pluginIdentifier] isEqualToString:@"ios"])
        pluginIdentifier.text = [PHAPIRequest pluginIdentifier];

    [super viewDidLoad];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
}

- (IBAction)touchedClearGID:(id)sender
{
    PHClearGID();
    [self addMessage:@"GID cleared!"];
}

- (IBAction)touchedClearSession:(id)sender
{
    [PHAPIRequest setSession:nil];
    [self addMessage:@"Session cleared!"];
}

- (IBAction)touchedClearCustomUDID:(id)sender
{
    [PHAPIRequest setCustomUDID:nil];
    customUDIDField.text = nil;

    [self addMessage:@"Custom UDID cleared!"];
}

- (IBAction)touchedClearPluginIdentifier:(id)sender
{
    [PHAPIRequest setPluginIdentifier:nil];
    pluginIdentifier.text = nil;

    [self addMessage:@"Plugin Identifier cleared!"];
}
@end
