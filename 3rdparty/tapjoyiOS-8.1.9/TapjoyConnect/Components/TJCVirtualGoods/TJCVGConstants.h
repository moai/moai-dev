// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#ifndef TJC_VG_GLOBALCONSTANTS
#define TJC_VG_GLOBALCONSTANTS


#define TJC_BOX_BCK_280_300_IMG_NAME	@"UIPanel-ShowSubmitHighScore.png"

#define TJC_REFRESH_ICON_NAME	@"refresh.png"
#define TJC_INFO_ICON_NAME		@"info-icon.png"

// These are used to help construct the background view of the table view cells.
#define TJC_VG_CELL_COLOR_TOP		TJC_HEXCOLOR(0xE6E6E6FF)
#define TJC_VG_CELL_COLOR_BOTTOM	TJC_HEXCOLOR(0xC8C8C8FF)
#define TJCVG_WHITE_ROW_GRAY_SEP	TJC_HEXCOLOR(0xadadadff)
#define TJCVG_WHITE_ROW_WHITE_SEP	TJC_HEXCOLOR(0xfafafaff)

// Used in the TJCVGView class.
// The alpha value of the loading screen with the activity idicator. It should be semi-transparent for the best look.
static const float TJCVG_LOADING_VIEW_ALPHA = 0.5f;
// The duration that the loading view takes to animate fully into view.
static const float TJCVG_LOADING_VIEW_ANIM_DURATION = 0.25f;
// The general duration of view animations.
static const float TJCVG_VIEW_ANIM_DURATION = 0.25f;


// Used in the TJCVGPageView class.
// The percentage of the width of the screen that the button width will be.
static const float TJCVG_BUTTON_W_SIZE_PERCENTAGE = .45f;
static const float TJCVG_BUTTON_H_SIZE_PERCENTAGE = .45f;
// Total number of buttons per page.
static const int TJCVG_NUM_BUTTONS = 4;
// The number of buttons on one row.
static const int TJCVG_NUM_BUTTONS_ROW = 2;
// The number of buttons on one column.
static const int TJCVG_NUM_BUTTONS_COL = 2;


// Used in the TJCVGItemButtonView class.
#define TJCVG_BUTTON_CORNER_RADIUS	7
#define TJCVG_BUTTON_BORDER_WIDTH	2
#define TJCVG_BUTTON_SHADOW_OPACITY	(0.3f)


// Used in the TJCVGItemDetail class.
static const int TJCVG_ITEMDETAIL_CORNER_RADIUS = 7;

// This is the amount that the user defined color will be lightened for certain views.
static const int TJCVG_COLOR_LIGHTEN_VALUE = 100;
static const int TJCVG_COLOR_LIGHTEN_VALUE1 = 150;


// Reuse identifiers for the table view cells.
#define TJC_VG_TABLEVIEWCELL_ID_AVAILABLE	@"vgViewCellIdentifierAvailable"
#define TJC_VG_TABLEVIEWCELL_ID_PURCAHSED	@"vgViewCellIdentifierPurchased"


#endif