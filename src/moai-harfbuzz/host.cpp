// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-harfbuzz/host.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb.h>
#include <hb-ft.h>

// adapted from https://github.com/lxnt/ex-sdl-freetype-harfbuzz
// just to see if hb is alive

#define NUM_EXAMPLES 3

//================================================================//
// host
//================================================================//

const char *texts [ NUM_EXAMPLES ] = {
    "Ленивый рыжий кот",
    "كسول الزنجبيل القط",
    "懶惰的姜貓",
};

const hb_direction_t text_directions [ NUM_EXAMPLES ] = {
    HB_DIRECTION_LTR,
    HB_DIRECTION_RTL,
    HB_DIRECTION_TTB,
};

/* XXX: These are not correct, though it doesn't seem to break anything
 *      regardless of their value. */
const char *languages [ NUM_EXAMPLES ] = {
    "en",
    "ar",
    "ch",
};

const hb_script_t scripts [ NUM_EXAMPLES ] = {
    HB_SCRIPT_LATIN,
    HB_SCRIPT_ARABIC,
    HB_SCRIPT_HAN,
};

enum {
    ENGLISH,
	ARABIC,
	CHINESE,
};

//----------------------------------------------------------------//
void AKUHarfBuzzContextInitialize () {

//	int ptSize = 50*64;
//    int device_hdpi = 72;
//    int device_vdpi = 72;
//
//    FT_Library ft_library;
//    assert ( !FT_Init_FreeType ( &ft_library ));
//
//    FT_Face ft_face [ NUM_EXAMPLES ];
//	
//	// to run, clone ex-sdl-freetype-harfbuzz and fix these paths to find the included fonts
//	
//    assert(!FT_New_Face ( ft_library, "fonts/DejaVuSerif.ttf", 0, &ft_face [ ENGLISH ]));
//    assert(!FT_Set_Char_Size ( ft_face [ ENGLISH ], 0, ptSize, device_hdpi, device_vdpi ));
//    //force_ucs2_charmap ( ft_face [ ENGLISH ]);
//
//    assert(!FT_New_Face ( ft_library, "fonts/amiri-0.104/amiri-regular.ttf", 0, &ft_face [ ARABIC ]));
//    assert(!FT_Set_Char_Size ( ft_face [ ARABIC ], 0, ptSize, device_hdpi, device_vdpi ));
//    //force_ucs2_charmap ( ft_face [ ARABIC ]);
//
//    assert(!FT_New_Face ( ft_library, "fonts/fireflysung-1.3.0/fireflysung.ttf", 0, &ft_face [ CHINESE ]));
//    assert(!FT_Set_Char_Size ( ft_face [ CHINESE ], 0, ptSize, device_hdpi, device_vdpi ));
//    //force_ucs2_charmap ( ft_face [ CHINESE ]);
//
//    hb_font_t *hb_ft_font [ NUM_EXAMPLES ];
//    hb_ft_font [ ENGLISH ] = hb_ft_font_create ( ft_face [ ENGLISH ], NULL );
//    hb_ft_font [ ARABIC ]  = hb_ft_font_create ( ft_face [ ARABIC ] , NULL );
//    hb_ft_font [ CHINESE ] = hb_ft_font_create ( ft_face [ CHINESE ], NULL );
//
//	for ( int i = 0; i < NUM_EXAMPLES; ++i ) {
//
//		/* Create a buffer for harfbuzz to use */
//		hb_buffer_t* buf = hb_buffer_create ();
//
//		hb_buffer_set_direction ( buf, text_directions [ i ]);
//		hb_buffer_set_script ( buf, scripts [ i ]);
//		hb_buffer_set_language ( buf, hb_language_from_string ( languages [ i ], strlen ( languages [ i ])));
//
//		hb_buffer_add_utf8 ( buf, texts [ i ], strlen ( texts [ i ]), 0, strlen ( texts [ i ]));
//		hb_shape ( hb_ft_font [ i ], buf, NULL, 0 );
//
//		unsigned int glyph_count;
//		hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos ( buf, &glyph_count );
//		hb_glyph_position_t* glyph_pos = hb_buffer_get_glyph_positions ( buf, &glyph_count );
//		
//		printf ( "hb string: %d\n", i );
//		
//		for ( int j = 0; j < glyph_count; ++j ) {
//			
//			hb_glyph_info_t& info = glyph_info [ j ];
//			hb_glyph_position_t& position = glyph_pos [ j ];
//			
//			printf ( "\tglyph: %d xAdv: %d yAdv: %d xOff: %d yOff: %d\n",
//				info.codepoint,
//				position.x_advance,
//				position.y_advance,
//				position.x_offset,
//				position.y_offset
//			);
//		}
//		
//		printf ( "\n" );
//	}
//	
//	FT_Done_FreeType ( ft_library );
}
