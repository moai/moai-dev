// VGGraphicsMacros.h -- inline graphics functions
// by allen brunson  february 17 2010


/******************************************************************************/
/*                                                                            */
/***  inline graphics functions                                             ***/
/*                                                                            */
/******************************************************************************/

// true if running on an ipad

CG_INLINE BOOL VGDeviceIsPad(void)
{
    return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
}

// center 'target' over 'rect'

CG_INLINE CGRect VGRectCenter(CGRect rect, CGRect target)
{
    const CGFloat  xoff = floor((rect.size.width - target.size.width) / 2.0);
    const CGFloat  yoff = floor((rect.size.height - target.size.height) / 2.0);
    
    target.origin.x = rect.origin.x + xoff;
    target.origin.y = rect.origin.y + yoff;
    
    return target;
}

// move 'target' to the bottom of 'rect'

CG_INLINE CGRect VGRectBottom(CGRect rect, CGRect target)
{
    target.origin.y = rect.origin.y + rect.size.height - target.size.height;
    
    return target;
}

// move 'target' to the left of 'rect'

CG_INLINE CGRect VGRectLeft(CGRect rect, CGRect target)
{
    target.origin.x = rect.origin.x;
    
    return target;
}

// move 'target' to the right of 'rect'

CG_INLINE CGRect VGRectRight(CGRect rect, CGRect target)
{
    target.origin.x = rect.origin.x + rect.size.width - target.size.width;
    
    return target;
}

// move 'target' to the top of 'rect'

CG_INLINE CGRect VGRectTop(CGRect rect, CGRect target)
{
    target.origin.y = rect.origin.y;
    
    return target;
}

// make a CGRect from a CGSize

CG_INLINE CGRect VGRectWithSize(CGSize size)
{
    CGRect rect = CGRectZero;
    rect.size = size;
    return rect;
}

// "zooms" a rect by adding zoomSize pixels to each side. the returned rect
// will be centered directly over the old one.

CG_INLINE CGRect VGRectZoom(CGRect rect, CGFloat zoomSize)
{
    rect.origin.x    -= zoomSize;
    rect.origin.y    -= zoomSize;

    rect.size.width  += (zoomSize * 2.0);
    rect.size.height += (zoomSize * 2.0);

    return rect;
}
