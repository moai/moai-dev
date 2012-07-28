/*
 *  This file registers the FreeType modules compiled into the library.
 *
 *  If you use GNU make, this file IS NOT USED!  Instead, it is created in
 *  the objects directory (normally `<topdir>/objs/') based on information
 *  from `<topdir>/modules.cfg'.
 *
 *  Please read `docs/INSTALL.ANY' and `docs/CUSTOMIZE' how to compile
 *  FreeType without GNU make.
 *
 */

FT_USE_MODULE( FT_Module_Class, autofit_module_class ) /* FT2 auto hinter */
FT_USE_MODULE( FT_Driver_ClassRec, tt_driver_class ) /* TrueType */
//FT_USE_MODULE( FT_Driver_ClassRec, t1_driver_class ) /* PS Type1 */
FT_USE_MODULE( FT_Driver_ClassRec, cff_driver_class ) /* CFF font, OpenType */
//FT_USE_MODULE( FT_Driver_ClassRec, t1cid_driver_class ) /* CID-keyed PS font */
//FT_USE_MODULE( FT_Driver_ClassRec, pfr_driver_class ) /* PFR font */
//FT_USE_MODULE( FT_Driver_ClassRec, t42_driver_class ) /* TrueType in PS/PDF, needs tt */
//FT_USE_MODULE( FT_Driver_ClassRec, winfnt_driver_class ) /* Windows bitmap */
//FT_USE_MODULE( FT_Driver_ClassRec, pcf_driver_class ) /* PCF font */

//FT_USE_MODULE( FT_Module_Class, psaux_module_class ) /* used by sfnt, t1, t1cid */
FT_USE_MODULE( FT_Module_Class, psnames_module_class ) /* used by sfnt, t1, t1cid */
FT_USE_MODULE( FT_Module_Class, pshinter_module_class ) /* used by t1, cff, t1cid */ 
FT_USE_MODULE( FT_Renderer_Class, ft_raster1_renderer_class ) /* monochrome scaler */
FT_USE_MODULE( FT_Module_Class, sfnt_module_class ) /* used by tt, cff */
FT_USE_MODULE( FT_Renderer_Class, ft_smooth_renderer_class ) /* grayscale scaler */

//FT_USE_MODULE( FT_Renderer_Class, ft_smooth_lcd_renderer_class ) /* horiz. LCD scaler */
//FT_USE_MODULE( FT_Renderer_Class, ft_smooth_lcdv_renderer_class ) /* vert. LCD scaler */
//FT_USE_MODULE( FT_Driver_ClassRec, bdf_driver_class ) /* BDF font */

/* EOF */
