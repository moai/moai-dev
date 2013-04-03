// ===================================================================
//
// Description		
//		Contains common items shared with all other API files
//
// Revision history
//		Date			Description
//		30-Nov-2012		First version
//
// ===================================================================
#ifndef __Common__
#define __Common__

// -------------------------------------------------------------------
// Define alias to mark exported/imported library items
// -------------------------------------------------------------------
#if defined WIN32
#if defined SMARTFOXCLIENTAPI_EXPORTS
    #define DLLImportExport __declspec(dllexport)
#else
    #define DLLImportExport __declspec(dllimport)
#endif
#else
#define DLLImportExport
#endif

#endif
