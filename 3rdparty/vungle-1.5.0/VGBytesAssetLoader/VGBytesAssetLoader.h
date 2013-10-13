//
//  VGBytesAssetLoader.h
//  vungletest
//
//  Created by Rolando Abarca on 7/24/13.
//  (c) 2013, Vungle Inc
//
//

#import <Foundation/Foundation.h>
#import "vunglepub.h"

@interface VGBytesAssetLoader : NSObject <VGAssetLoader>
- (NSData*)vungleLoadAsset:(NSString*)path;
- (UIImage*)vungleLoadImage:(NSString*)path;
@end
