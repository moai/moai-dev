// VGConfigStyle.m -- config view style options
// by allen brunson  january 19 2009

#import "VGConfigStyle.h"

BOOL VGColorStyleBlack(void)
{
    if (VGDeviceIsPad()) return FALSE;
    // return TLAppController.appDelegate.colorStyleBlack;
    return TRUE;
}

UIColor* VGConfigAccessoryColor(void)
{
    return [UIColor grayColor];
}

UIColor* VGConfigBackgroundColor(void)
{
    // ipad color
    if (VGDeviceIsPad())
    {
        return [UIColor integerRed:0xDE green:0xE0 blue:0xE6];
    }
    
    // iphone/ipod colors
    if (VGColorStyleBlack())
    {
        return [UIColor integerRed:0x99 green:0x99 blue:0x99];
    }
    else
    {
        return [UIColor groupTableViewBackgroundColor];
    }
}

UIColor* VGConfigCellBackgroundColor(void)
{
    return [UIColor whiteColor];
}

UIFont* VGConfigCellFont(void)
{
    return [UIFont boldSystemFontOfSize:17.0];
}

UIColor* VGConfigCellTextColor(void)
{
    return [UIColor blackColor];
}

UIColor* VGConfigDisabledColor(void)
{
    return [UIColor integerRed:168 green:171 blue:173];
}

UIColor* VGConfigHeaderTextColor(void)
{
    return [UIColor integerRed:0x00 green:0x33 blue:0x66];
}

UIColor* VGConfigHeaderTextColorShadow(void)
{
    return [UIColor integerRed:0xFF green:0xCC blue:0xCC];
}

UIFont* VGConfigHeaderTextFont(void)
{
    return VGConfigCellFont();
}
