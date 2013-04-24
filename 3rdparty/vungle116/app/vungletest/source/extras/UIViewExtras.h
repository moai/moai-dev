// UIViewExtras.h -- extra UIView methods
// by allen brunson  march 2 2009


/******************************************************************************/
/*                                                                            */
/***  class defines                                                         ***/
/*                                                                            */
/******************************************************************************/

enum
{
    VGAutoresizingWidthHeight =
     UIViewAutoresizingFlexibleWidth +
     UIViewAutoresizingFlexibleHeight,
     
    VGAutoresizingWidthLeft =
     UIViewAutoresizingFlexibleWidth +
     UIViewAutoresizingFlexibleLeftMargin 
};



/******************************************************************************/
/*                                                                            */
/***  UIView category                                                       ***/
/*                                                                            */
/******************************************************************************/

@interface UIView (UIViewExtras)

// properties

@property(nonatomic, assign) CGRect  frameUpdate;  // update only if needed

@end


/******************************************************************************/
/*                                                                            */
/***  UIView category                                                       ***/
/*                                                                            */
/******************************************************************************

overview
--------

extra methods for UIView

*/
