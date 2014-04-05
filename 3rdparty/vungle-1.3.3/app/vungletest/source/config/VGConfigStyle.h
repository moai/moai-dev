// VGConfigStyle.h -- config view style options
// by allen brunson  january 19 2009


/******************************************************************************/
/*                                                                            */
/***  config style functions                                                ***/
/*                                                                            */
/******************************************************************************/

// use black color scheme or not

BOOL VGColorStyleBlack(void);

// for all color schemes

UIColor* VGConfigAccessoryColor(void);
UIColor* VGConfigBackgroundColor(void);
UIColor* VGConfigCellBackgroundColor(void);
UIFont*  VGConfigCellFont(void);
UIColor* VGConfigCellTextColor(void);
UIColor* VGConfigDisabledColor(void);

// for the black color scheme

UIColor* VGConfigHeaderTextColor(void);
UIColor* VGConfigHeaderTextColorShadow(void);
UIFont*  VGConfigHeaderTextFont(void);


/******************************************************************************/
/*                                                                            */
/***  config style functions                                                ***/
/*                                                                            */
/******************************************************************************

overview
--------

colors and fonts for the config window

*/
