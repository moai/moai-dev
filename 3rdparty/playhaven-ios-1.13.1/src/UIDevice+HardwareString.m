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

 UIDevice+PlatformString.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 4/20/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "UIDevice+HardwareString.h"
#include <sys/sysctl.h>

@implementation UIDevice (HardwareString)
- (NSString *)hardware
{
#if TARGET_IPHONE_SIMULATOR
    // Use idiom to send appropriate string
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        return @"iPad Simulator";
    } else {
        return @"iPhone Simulator";
    }
#else
    // Use actual hw machine name
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = malloc(size);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *hardware = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
    free(machine);
    return hardware;
#endif
}
@end
