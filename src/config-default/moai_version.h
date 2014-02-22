#ifndef MOAI_VERSION_H
#define MOAI_VERSION_H

	#define MOAI_SDK_VERSION_MAJOR_MINOR 1.5		// decimal
	#define MOAI_SDK_VERSION_REVISION 1			// signed int
	#define MOAI_SDK_VERSION_AUTHOR "MoaiEdition"		// string
	
	// SDK developers:
	// Copy this file to src/config and change MOAI_SDK_VERSION_AUTHOR to
	// your name. When Moai SDK builds its version string, the revision
	// number will be omitted if it is < 0. The 'ad hoc' author attribution
	// will be omitted if MOAI_SDK_VERSION_AUTHOR is the empty string.
	
	// If MOAI_SDK_VERSION_REVISION is greated than one then the 'ad hoc'
	// author attribution will be instead used as a note.
	
	// Examples:
	// "Moai SDK 1.7 (ad hoc build by Patrick Meehan)" - REVISION < 0 and AUTHOR is "Patrick Meehan"
	// "Moai SDK 1.7 revision 12 (staging)" - REVISION >= 0 and AUTHOR is "staging"
	// "Moai SDK 1.7 revision 2" - REVISION >= 0 and AUTHOR is ""
	
#endif
