#ifndef MOAI_VERSION_MINOR_H
#define MOAI_VERSION_MINOR_H

	// DO NOT EVER MODIFY THIS FILE
	// this file should *only* be touched by the build server

	#define MOAI_SDK_VERSION_MINOR 6			// int
	#define MOAI_SDK_VERSION_REVISION 0			// int

	// Build server:

	// Before building a binary release, copy this file
	// to src/config and update the minor and build versions
	// below (as appropriate).

	// When publishing, this should overwrite moai_version.h
	// in config-default and get checked in to both develop
	// and the other branches.

#endif
