// UIControlExtras.h -- extra methods for UIControl
// by allen brunson  february 25 2010


/******************************************************************************/
/*                                                                            */
/***  UIControl category                                                    ***/
/*                                                                            */
/******************************************************************************/

@interface UIControl (UIControlExtras)

-(void)addButtonTarget:(id)target action:(SEL)action;
-(void)removeTarget:(id)target;

@end


/******************************************************************************/
/*                                                                            */
/***  UIControl category                                                    ***/
/*                                                                            */
/******************************************************************************

overview
--------

few extra methods for UIControl

*/
