#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

# TODO: replace schemes with with targets (if possible)
# TODO: use libmoai-all scheme or target for ios and osx

# TODO: need to build both 32 and 64 bit flavors (for iOS, too?)

# osx stuff
osx_schemes="libmoai-osx libmoai-osx-3rdparty libmoai-osx-luaext libmoai-osx-untz libmoai-osx-zlcore"
osx_sdks=( "macosx" )
osx_architectures_macosx=( "i386" ) # is this right?

# ios stuff
ios_schemes="libmoai-ios libmoai-ios-3rdparty libmoai-ios-facebook libmoai-ios-luaext libmoai-ios-tapjoy libmoai-ios-untz libmoai-ios-zlcore"
ios_sdks="iphoneos iphonesimulator"
ios_architectures_iphonesimulator="i386" # $(ARCHS_STANDARD) can?
ios_architectures_iphoneos="armv7 armv7s" # are these correct? can we use the settings from xcode instead?

# all this stuff would be more maintainable using the moaitools/lua paradigm
# the only thing we need command line for is xcodebuild itself and lipo

usage() {
	echo >&2 "usage: $0 [-v] [-j <jobName>] [-c Debug|Release|all] [-p osx|ios|all]"
	exit 1
}
job="moai"
configurations="all"
platforms="all"
verbose=false

while getopts c:j:p: o; do
	case $o in
	c)	configurations=$OPTARG;;
	j)	job=$OPTARG;;
	p)	platforms=$OPTARG;;
	v)	verbose=true;;
	\?)	usage;;
	esac
done
shift `expr $OPTIND - 1`

if [ $# -gt 0 ]; then
	usage
fi

if ! [[ $job =~ ^[a-zA-Z0-9_\-]+$ ]]; then
	echo -e "*** Illegal job name specified: $job..."
	echo -e "    > Job names may only contain letters, numbers, dashes and underscores"
	echo
	exit 1
fi

if [ x"$configurations" != xDebug ] && [ x"$configurations" != xRelease ] && [ x"$configurations" != xall ]; then
	echo $usage
	exit 1
elif [ x"$configurations" = xall ]; then
	configurations="Debug Release"
fi

if [ x"$platforms" != xosx ] && [ x"$platforms" != xios ] && [ x"$platforms" != xall ]; then
	echo $usage
	exit 1
elif [ x"$platforms" = xall ]; then
	platforms="osx ios"
fi

basedir="/tmp/$job"

# some kind of crappy shell script build function
build() {
	dir=${basedir}/${platform}/${scheme}/${sdk}/${config}
	mkdir -p $dir
	cmd="xcodebuild -configuration $config -workspace libmoai.xcodeproj/project.xcworkspace -scheme $scheme -sdk $sdk build CONFIGURATION_BUILD_DIR=$dir"
	msg="Building libmoai/$scheme/$sdk for $platform $config..."

	if $verbose; then
		printf "%s\n" "$msg"
		if $cmd; then
			echo "Build OK."
		else
			echo "Build failed, aborting."
			exit 1
		fi
	else
		printf "%s" "$msg"
		log=$dir/xcodebuild.log
		if $cmd > $log; then
			echo "Done."
		else
			echo "Failed."
			echo >&2 "Logs in $dir/xcodebuild.log. Last few lines of failure:"
			tail >&2 $dir/xcodebuild.log
			exit 1
		fi
	fi
}

# iterate through all the platforms, configs and sdks we care about and call the build function
# configs are 'Debug', 'Release'
for platform in $platforms; do

	schemes=
	sdks=
	architectures=

	eval schemes=\$${platform}_schemes # this is from the top of the script; shouldn't need to do if we use the *_all scheme/target
	eval sdks=\$${platform}_sdks # also defined at top of script

	for config in $configurations; do
		for sdk in $sdks; do		
			eval architectures=\$${platform}_architectures_$sdk
			for scheme in $schemes; do
				build
			done
		done
	done


	# make a directory for universal binaries and use lipo to join together each architecture
	for config in $configurations; do
		rm -rf "$basedir/$platform/$config/universal" # clean out the old dir (if any)
		mkdir -p "$basedir/$platform/$config/universal" # make the new dir

		# TODO: since we don't want to list each target/scheme, we should replace this with a directory iterator
		for scheme in $schemes; do

			libs=
			# iterate through each sdk (for example: iphonesimulator, iphoneos) and make a list of the source library filenames
			# OR: change the build directories to be per target and output each platform's *.a into them; then we can use * notation below
			for sdk in $sdks; do
				libs="$libs $basedir/$platform/$scheme/$sdk/$config/$scheme.a" # '$scheme' is from the list above; if we use an iterator, we can just use the filename
			done

			# pass the lib names we just generated to lipo to produce a universal binary
			if ! xcrun -sdk $sdk lipo -create -output "$basedir/$platform/$config/universal/$scheme.a" $libs; then
				echo >&2 "lipo failed, giving up."
				exit 1
			fi
		done
	done

	# what does this do? why are we doing this? are we just taking them back out again? WTF, Becky
	for config in $configurations; do
		for sdk in $sdks; do
			eval architectures=\$${platform}_architectures_$sdk
			for arch in $architectures; do
				rm -rf "$basedir/$platform/$config/$arch"
				mkdir -p "$basedir/$platform/$config/$arch"
				for scheme in $schemes; do
					if !  xcrun -sdk $sdk lipo -thin $arch -output "$basedir/$platform/$config/$arch/$scheme.a" "$basedir/$platform/$config/universal/$scheme.a"; then
						echo >&2 "lipo failed, giving up."
						exit 1
					fi
				done
			done
		done
	done
done
