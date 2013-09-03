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

 PHURLLoader.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 2/9/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>

/** @file */

/**
 * The maximum number of redirects to follow.
 **/
#define MAXIMUM_REDIRECTS 10

@protocol PHURLLoaderDelegate;

/**
 * @internal
 *
 * @brief A class that opens a URL by first internally following
 * 302 redirects until they have been exhausted or MAXIMUM_REDIRECTS redirects
 * have been followed. The loader may then open the final URL on the device.
 * Instances of PHURLLoader will retain themselves after open until they
 * launch or stop following redirects.
 * Used to prevent Safari from showing up when loading iTunes store links.
 **/
@interface PHURLLoader : NSObject {
    id<PHURLLoaderDelegate> _delegate;
    NSURLConnection *_connection;
    NSURL           *_targetURL;
    NSInteger        _totalRedirects;
    BOOL             _opensFinalURLOnDevice;
    id               _context;
}

/**
 * Invalidates all loaders that have \c delegate
 * See PHURLLoader#invalidate for details
 *
 * @param delegate
 *   The delegate
 **/
+ (void)invalidateAllLoadersWithDelegate:(id<PHURLLoaderDelegate>)delegate;

/**
 * Opens and returns PHURLLoader instance for the URL string \c url
 *
 * @param url
 *   The string url
 *
 * @return
 *   The returned PHURLLoader opened with the string URL
 **/
+ (PHURLLoader *)openDeviceURL:(NSString*)url;


@property (nonatomic, assign) id<PHURLLoaderDelegate> delegate; /**< The delegate, see PHURLLoaderDelegate for supported methods */
@property (nonatomic, retain) NSURL *targetURL;                 /**< The target URL, will be updated as redirects are followed */
@property (nonatomic, assign) BOOL   opensFinalURLOnDevice;     /**< Controls whether or not targetURL will be opened on the device. Default \c YES */
@property (nonatomic, retain) id     context;                   /**< Context object. Often used to pass along callback information for dispatches. Default nil \c */

/**
 * Follows redirects, starting with PHURLLoader.targetURL
 **/
- (void)open;

/**
 * Stops following redirects and unassigns the delegate
 **/
- (void)invalidate;
@end

/**
 * @internal
 *
 * @brief Delegate protocol for PHURLLoader instances.
 **/
@protocol PHURLLoaderDelegate <NSObject>
@optional

/**
 * The loader has successfully finished following redirects and will proceed to
 * launch the URL on the device if PHURLLoader.opensFinalURLOnDevice is \c YES
 *
 * @param loader
 *   The loader
 **/
- (void)loaderFinished:(PHURLLoader *)loader;

/**
 * The loader has failed to follow redirects (bad network connection, server
 * error) and will not launch a URL
 *
 * @param loader
 *   The loader
 **/
- (void)loaderFailed:(PHURLLoader *)loader;
@end
