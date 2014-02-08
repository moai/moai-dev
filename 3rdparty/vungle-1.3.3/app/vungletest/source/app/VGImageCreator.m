// VGImageCreator.m -- image creation
// by allen brunson  january 29 2012

#import "VGGraphicsMacros.h"
#import "VGImageCreator.h"

static UIImage* defaultImage(const CGSize screenSize, const CGFloat fontSize)
{
    CGRect        bnds = CGRectZero;
    UIFont*       font = [UIFont systemFontOfSize:fontSize];
    UIImage*      imag = nil;
    const CGRect  scrn = VGRectWithSize(screenSize);
    NSString*     text = @"Loading ...";
    
    // set up rect for text drawing
    bnds.size = [text sizeWithFont:font];
    bnds = VGRectCenter(scrn, bnds);
    bnds.origin.y -= floor(scrn.size.height / 10.0);
    
    // set up drawing context
    UIGraphicsBeginImageContext(screenSize);    
    
    // set background color
    [[UIColor grayColor] set];
    UIRectFill(scrn);
    
    // draw text string
    [[UIColor whiteColor] set];
    [text drawInRect:bnds withFont:font];
    
    // finish drawing context
    imag = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return imag;
}

static void imageToFile(UIImage* image, NSString* filename)
{
    NSData* data = UIImagePNGRepresentation(image);
    [data writeToFile:filename atomically:TRUE];
}

static void image(const CGSize size, const CGFloat font, NSString* path)
{
    UIImage* imag = defaultImage(size, font);
    imageToFile(imag, path);
}

// creates a set of bland default images for an iphone/ipad app in the directory
// specified. useful as placeholders until you can get an artist to draw you
// something better.

void VGDefaultImages(NSString* directory)
{
    NSFileManager*  mngr = [NSFileManager defaultManager];
    NSString*       save = [mngr currentDirectoryPath];
    
    // change to directory where the files should go
    [mngr changeCurrentDirectoryPath:directory];
    
    // for older iphones and ipod touch devices
    image(CGSizeMake(320.0, 480.0), 34.0, @"Default.png");
    
    // for iphones and ipod touch devices with retina displays
    image(CGSizeMake(640.0, 960.0), 66.0, @"Default@2x.png");
    
    // ipad landscape
    image(CGSizeMake(1024.0, 768.0), 62.0, @"Default-Landscape.png");
    
    // ipad portrait
    image(CGSizeMake(768.0, 1024.0), 62.0, @"Default-Portrait.png");
    
    // restore original current directory
    [mngr changeCurrentDirectoryPath:save];
}
