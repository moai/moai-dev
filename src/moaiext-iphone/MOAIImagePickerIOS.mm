//
//  ImagePicker.m
//  SocialArcade
//
//  Created by Megan Peterson on 6/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <moaiext-iphone/MOAIImagePickerIOS.h>

//
// BhImagePicker.mm
// Image Picker plugin for Gideros Studio (IOS Only)
//
// MIT License
// Copyright (C) 2012. Andy Bower, Bowerhaus LLP
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//#include "gideros.h"
#import "mach/mach.h"
#import <iostream>
#import <UIKit/UIKit.h>

//Utilities
/*
static vm_size_t usedMemory(void) {
    // http://stackoverflow.com/questions/2921560/memory-troubles-with-uiimagepicker
    
    struct task_basic_info info;
    mach_msg_type_number_t size = sizeof(info);
    kern_return_t kerr = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size);
    return (kerr == KERN_SUCCESS) ? info.resident_size : 0;   // size in bytes
}

static natural_t freeMemory(void) {
    // http://stackoverflow.com/questions/2921560/memory-troubles-with-uiimagepicker
    
    mach_port_t           host_port = mach_host_self();
    mach_msg_type_number_t host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    vm_size_t              pagesize;
    vm_statistics_data_t   vm_stat;
    
    host_page_size(host_port, &pagesize);
    (void) host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size);
    return vm_stat.free_count * pagesize;
}
*/
static int stackdump(lua_State* l)
{
    // Thanks, Caroline Begbie
    
    NSLog(@"stackdump");
    
    int i;
    int top = lua_gettop(l);
    
    printf("total in stack: %d\n",top);
    
    for (i = 1; i <= top; i++)
    {  //repeat for each item in the stack
        printf("  ");
        int t = lua_type(l, i);
        switch (t) {
            case LUA_TSTRING:  //strings
                printf("string: '%s'\n", lua_tostring(l, i));
                break;
            case LUA_TBOOLEAN:  //booleans
                printf("boolean %s\n",lua_toboolean(l, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:  //numbers
                printf("number: %g\n", lua_tonumber(l, i));
                break;
            default:  //other values
                printf("%s\n", lua_typename(l, t));
                break;
        }
    }
    printf("\n");
    return 0;
}

@interface ImagePickerHelper : NSObject <UIImagePickerControllerDelegate, UIPopoverControllerDelegate, UINavigationControllerDelegate>
- (bool)pickImageIpad:(UIImagePickerControllerSourceType)sourceType pickerWidth:(int)width pickerHeight:(int)height pickerXOrigin:(int)x pickerYOrigin:(int)y;
- (bool)pickImage:(UIImagePickerControllerSourceType)sourceType;
- (bool)resizePickedImage:(CGSize)size;
@end

@interface ImagePickerHelper ()
- (ImagePickerHelper *)initWithL:(lua_State *)state targetRef:(int)targetRef_ onImageReadyRef:(int)onImageReadyRef_ onPickerDismissedRef:(int)onPickerDismissedRef_;
- (void)dealloc;
- (void)dismissPicker;

- (NSString *)pathForTemporaryFileWithFormat:(NSString *)format;

- (void)saveImageAsync;
- (void)onImagePicked;
- (void)onPickerDismissed:(bool)withImage;

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info;
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController;
@end

@implementation ImagePickerHelper {
    lua_State *L;
    int targetRef;
    int onImagePickedRef;
    int onPickerDismissedRef;
    UIImage *image;
    NSString *imageFile;
    UIViewController *container;
    UIPopoverController *popover;
    UIImagePickerController *picker;
}

- (ImagePickerHelper *)initWithL:(lua_State *)state targetRef: (int)targetRef_ onImageReadyRef:(int)onImageReadyRef_ onPickerDismissedRef:(int)onPickerDismissedRef_ {
    self = [super init];
    if (self) {
        L=state;
        onImagePickedRef =onImageReadyRef_;
        onPickerDismissedRef=onPickerDismissedRef_;
        targetRef=targetRef_;
    }
    return self;
}

-(void) dealloc {
    luaL_unref(L, LUA_REGISTRYINDEX, onImagePickedRef);
    luaL_unref(L, LUA_REGISTRYINDEX, onPickerDismissedRef);
    
    [self dismissPicker];
    [image release];
    [imageFile release];
    
    [super dealloc] ;
}

-(void) dismissPicker {
    if (picker) {
        [picker dismissModalViewControllerAnimated:YES];
        [picker.view removeFromSuperview];
        [picker release];
        picker=nil;
    }
    if (popover) {
        [popover dismissPopoverAnimated:YES];
        [popover release];
        popover=nil;
    }
}

-(bool)pickPhotoIphone:(UIImagePickerControllerSourceType)sourceType{
	picker	= [[UIImagePickerController alloc]init];
	picker.delegate = self;
	picker.sourceType = sourceType;
	picker.wantsFullScreenLayout = YES;
    
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	
	UIViewController* controller = [ window rootViewController ];
	
    [controller presentModalViewController:picker animated:YES];
    
    int old_fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
    return true;
}

-(bool)pickImageIpad: (UIImagePickerControllerSourceType)sourceType pickerWidth: (int)width pickerHeight: (int)height pickerXOrigin: (int) x pickerYOrigin: (int) y {
    picker = [[UIImagePickerController alloc] init];
    picker.delegate = self;
    picker.sourceType = sourceType;
    picker.wantsFullScreenLayout = YES;
    
    popover = [[UIPopoverController alloc] initWithContentViewController:picker];
    popover.delegate = self;
    
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	
	UIViewController* controller = [ window rootViewController ];
    UIView* rootView = controller.view;
    CGRect bounds=[rootView bounds];
    if (width!=-1 && height!=-1)
        [popover setPopoverContentSize:CGSizeMake(width, height) animated:NO];
    else
        // 600 is max width allowed
        [popover setPopoverContentSize:CGSizeMake(600, bounds.size.height) animated:NO];
    
    CGRect origin;
    if (x!=-1 && y!=-1)
        origin = CGRectMake(x, y ,1, 1);
    else
        origin = CGRectMake(0, bounds.size.height/2, 1, 1);
    [popover presentPopoverFromRect: origin inView:rootView permittedArrowDirections:UIPopoverArrowDirectionAny animated:NO];
    return true;
}

-(bool)pickImage:(UIImagePickerControllerSourceType)sourceType{
    if ([[[UIDevice currentDevice] model] isEqualToString: @"iPad"])
        return [self pickImageIpad:sourceType pickerWidth:-1 pickerHeight:-1 pickerXOrigin:-1 pickerYOrigin:-1];
    else
        return [self pickPhotoIphone:sourceType];
    return false;
}

- (NSString *)pathForTemporaryFileWithFormat:(NSString *)format
{
    NSString *  result;
    CFUUIDRef   uuid;
    CFStringRef uuidStr;
    
    uuid = CFUUIDCreate(NULL);
    uuidStr = CFUUIDCreateString(NULL, uuid);
    result = [NSTemporaryDirectory() stringByAppendingPathComponent:[NSString stringWithFormat: format, uuidStr]];
    assert(result != nil);
    
    CFRelease(uuidStr);
    CFRelease(uuid);
    
    return result;
}

-(void)saveImageAsync {
    // Create path for output images
    imageFile = [[self pathForTemporaryFileWithFormat:@"img%@.png"] retain];
    

	
    // Write image to PNG
    [UIImagePNGRepresentation(image) writeToFile:imageFile atomically:YES];
    
    // We don't want to keep an expensive image kicking around any longer than necessary
    [image release];
    image=nil;
    
    // Now inform the main thread that the image is ready
    [self performSelectorOnMainThread:@selector(onImagePicked) withObject:nil waitUntilDone:YES];
}

-(void)onImagePicked {
    // Come here when we have successfully saved a picked image to our temporary file (imageFile)
    // Callback into Lua to do something with the file - it will be deleted when the Lua function returns.
    // Callback as:
    //      function onImagePicked(target, sImageFilename) or
    //      function TargetClass:onImagePicked(sImageFilename)
    
	printf("imageFile: '%s'\n", [imageFile UTF8String]);
	
    lua_rawgeti(L, LUA_REGISTRYINDEX, onImagePickedRef);
    lua_rawgeti(L, LUA_REGISTRYINDEX, targetRef);
    lua_pushstring(L, [imageFile UTF8String]);
    lua_call(L, 2, 0);
    
    // Tidy up by removing the temporary file
    [[NSFileManager defaultManager] removeItemAtPath:imageFile error: nil];
    
    // We're done, free up the static Picker instance
    [picker release];
    picker=nil;
    
}

-(void)onPickerDismissed: (bool)withImage {
    // Come here when the image picker has been dismissed. The boolean (withImage) indicates whether
    // an image was picked (=true) or whether the picker was cancelled (=false).
    // Callback into Lua to inform the application. This callback is particularly useful if you
    // wish to display an activity indicator during the (likely lengthy) process of saving the picked
    // image to disk.
    // Callback as:
    //      function onPickerDismissed(target, bWithImage) or
    //      function TargetClass:onPickerDismissed(bWithImage)
 
	
	//printf("on picker dismissed", withImage);
    
	printf("on picker dismissed: %s",(withImage)?"true":"false");	
	
	lua_rawgeti(L, LUA_REGISTRYINDEX, onPickerDismissedRef);
    lua_rawgeti(L, LUA_REGISTRYINDEX, targetRef);
    lua_pushboolean(L, withImage);
    lua_call(L, 2, 0);
    
    if (!withImage) {
        // We're done here free up our static Picker instance
        [picker release];
        picker=nil;
    }
}

-(void)imagePickerController:(UIImagePickerController *)picker
didFinishPickingMediaWithInfo:(NSDictionary *)info{
    // Called as delegate when an image has been picked (or the camera has created a new image)
    #pragma unused(picker)
    // Dismiss and release (if necessary) BEFORE accessing the info dictionary
    // See: http://stackoverflow.com/questions/3088874/didfinishpickingmediawithinfo-return-nil-photo/4192109
    [self dismissPicker];
    
    image = [[info objectForKey:@"UIImagePickerControllerOriginalImage"] retain];
    
    // Callback here to allow application chance to resize image if required
    [self onPickerDismissed: true];
    
    // On background thread we save image.
    [NSThread detachNewThreadSelector:@selector(saveImageAsync) toTarget:self withObject:nil];
}

-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    #pragma unused(picker)
    // Called as delegate if the pick is cancelled
    [self dismissPicker];

    [self onPickerDismissed: false];
    

}

-(void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController {
    // Called as delegate by the iPad picker if the user touches outside of the popup controller.
    // Effectively a cancel of the pick.
    #pragma unused(popoverController)
    [self dismissPicker];


    [self onPickerDismissed: false];
}

-(bool)resizePickedImage: (CGSize) size {
    if (image==nil)
        return false;
    
    CGSize currentSize=[image size];
    float scaleX=size.width/currentSize.width;
    float scaleY=size.height/currentSize.height;
    float scale=MIN(scaleX, scaleY);
    CGSize newSize=CGSizeMake(currentSize.width*scale, currentSize.height*scale);
    
    UIGraphicsBeginImageContext(newSize);
    [image drawInRect:CGRectMake(0, 0, newSize.width, newSize.height)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();    
    UIGraphicsEndImageContext();
    
    [image release];
    image= [newImage retain];
    return true;
}
@end

//***************************

static ImagePickerHelper *pickerHelper;

static int resizePickedImage(lua_State *L) {
    assert(pickerHelper !=nil && @"No picker available");
    int width = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);
    bool result=[pickerHelper resizePickedImage:CGSizeMake(width, height)];
    lua_pushboolean(L, result);
    return 1;
}

static int pickImageIpad(lua_State *L) {
    bool result=false;
    
    //@try {
        NSUInteger sourceType = (NSUInteger) luaL_checkinteger(L, 1);
        int pickerWidth = luaL_checkinteger(L, 4);
        int pickerHeight = luaL_checkinteger(L, 5);
        int pickerXOrigin = luaL_checkinteger(L, 6);
        int pickerYOrigin = luaL_checkinteger(L, 7);
        
        lua_pop(L,4);
        stackdump(L);
        int onPickerDismissedRef=luaL_ref(L, LUA_REGISTRYINDEX);
        int onImageReadyRef=luaL_ref(L, LUA_REGISTRYINDEX);
        int targetRef=luaL_ref(L, LUA_REGISTRYINDEX);
        
        pickerHelper = [[ImagePickerHelper alloc] initWithL: L targetRef: targetRef onImageReadyRef: onImageReadyRef onPickerDismissedRef: onPickerDismissedRef];
        result=[pickerHelper pickImageIpad:sourceType pickerWidth:pickerWidth pickerHeight:pickerHeight pickerXOrigin:pickerXOrigin pickerYOrigin:pickerYOrigin];
    //}
    //@catch (id e) {
    //    NSLog(@"pickImageIpad: Caught %@", e);
   // }
    lua_pushboolean(L, result);
    return 1;
}

static int pickImage(lua_State *L) {
    bool result=false;
    
    //@try {
        NSUInteger sourceType = (NSUInteger) luaL_checkinteger(L, 1);
        int onPickerDismissedRef=luaL_ref(L, LUA_REGISTRYINDEX);
        int onImageReadyRef=luaL_ref(L, LUA_REGISTRYINDEX);
        int targetRef=luaL_ref(L, LUA_REGISTRYINDEX);
        
        pickerHelper = [[ImagePickerHelper alloc] initWithL: L targetRef: targetRef onImageReadyRef: onImageReadyRef onPickerDismissedRef: onPickerDismissedRef];
        result=[pickerHelper pickImage: sourceType];
    //}
   // @catch (id e) {
   //     NSLog(@"pickImage: Caught %@", e);
   // }
    lua_pushboolean(L, result);
    return 1;
}

static int isSourceTypeAvailable(lua_State *L) {
    NSUInteger sourceType = (NSUInteger) luaL_checkinteger(L, 1);
    bool result=[UIImagePickerController isSourceTypeAvailable:sourceType];
    lua_pushboolean(L, result);
    return 1;
}

void MOAIImagePickerIOS::RegisterLuaClass ( MOAILuaState& state ) {

  	state.SetField ( -1, "SOURCE_TYPE_CAMERA", 	( u32 )UIImagePickerControllerSourceTypeCamera );
	state.SetField ( -1, "SOURCE_TYPE_PHOTO_LIBRARY", 	( u32 )UIImagePickerControllerSourceTypePhotoLibrary );
	state.SetField ( -1, "SOURCE_TYPE_SAVED_PHOTOS_ALBUM", 	( u32 )UIImagePickerControllerSourceTypeSavedPhotosAlbum );
    
	luaL_Reg regTable [] = {
		{ "resizePickedImage",		resizePickedImage },
		{ "pickImageIpad",			pickImageIpad },
		{ "pickImage",              pickImage },
		{ "isSourceTypeAvailable",	isSourceTypeAvailable},
		{ NULL, NULL }	
	};
    
	luaL_register( state, 0, regTable );  
    
    
}

MOAIImagePickerIOS::MOAIImagePickerIOS () {
    
    
    
}


//----------------------------------------------------------------//
MOAIImagePickerIOS::~MOAIImagePickerIOS () {
    
	//[ mAppId release ];
	//[ mZones release ];
	//[ mAdColonyDelegate release ];
	//[ mTakeoverDelegate release ];
}


//REGISTER_PLUGIN("bhimagepicker", "1.0")