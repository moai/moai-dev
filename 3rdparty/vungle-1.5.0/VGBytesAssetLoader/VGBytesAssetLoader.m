//
//  VGBytesAssetLoader.m
//  vungletest
//
//  Created by Rolando Abarca on 7/24/13.
//  (c) 2013, Vungle Inc
//
//

#import "VGBytesAssetLoader.h"
#include "vg_button_close.png.h"
#include "vg_button_mute_plain.png.h"
#include "vg_button_mute_selected.png.h"

@implementation VGBytesAssetLoader
- (NSData*)vungleLoadAsset:(NSString*)path {
	if ([path isEqualToString:@"vg_button_close.png"]) {
		return [NSData dataWithBytes:vg_button_close_png length:sizeof(vg_button_close_png)];
	} else if ([path isEqualToString:@"vg_button_mute_plain.png"]) {
		return [NSData dataWithBytes:vg_button_mute_plain_png length:sizeof(vg_button_mute_plain_png)];
	} else if ([path isEqualToString:@"vg_button_mute_selected.png"]) {
		return [NSData dataWithBytes:vg_button_mute_selected_png length:sizeof(vg_button_mute_selected_png)];
	}
	return nil;
}

- (UIImage*)vungleLoadImage:(NSString*)path {
	NSData* data = [self vungleLoadAsset:path];
	if (data) {
		return [UIImage imageWithData:data];
	}
	return nil;
}

@end
