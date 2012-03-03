#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e

	# check for command line switches
	usage="usage: $0 -p <package> [-s] [-i thumb | arm] [-a all | armeabi | armeabi-v7a] [-l appPlatform] [--disable-tapjoy] [--disable-google-push] [--disable-google-billing] [--disable-crittercism]"
	skip_build="false"
	package_name=
	arm_mode=arm
	arm_arch=armeabi-v7a
	app_platform=android-10
	tapjoy_flags=
	google_push_flags=
	google_billing_flags=
	crittercism_flags=
	
	while [ $# -gt 0 ];	do
	    case "$1" in
			-s)  skip_build="true";;
			-p)  package_name="$2"; shift;;
	        -i)  arm_mode="$2"; shift;;
	        -a)  arm_arch="$2"; shift;;
			-l)  app_platform="$2"; shift;;
			--disable-tapjoy)  tapjoy_flags="--disable-tapjoy";;
			--disable-google-push)  google_push_flags="--disable-google-push";;
			--disable-google-billing)  google_billing_flags="--disable-google-billing";;
			--disable-crittercism)  crittercism_flags="--disable-crittercism";;
			-*)
		    	echo >&2 \
		    		$usage
		    	exit 1;;
			*)  break;;	# terminate while loop
	    esac
	    shift
	done	
		
	if [ x"$package_name" = x ]; then
		echo $usage
		exit 1
	fi

	if [ x"$arm_mode" != xarm ] && [ x"$arm_mode" != xthumb ]; then
		echo $usage
		exit 1		
	fi

	if [ x"$arm_arch" != xarmeabi ] && [ x"$arm_arch" != xarmeabi-v7a ] && [ x"$arm_arch" != xall ]; then
		echo $usage
		exit 1		
	elif [ x"$arm_arch" = xall ]; then
		arm_arch="armeabi armeabi-v7a"
	fi
	
	# remove existing new host folder, if any
	new_host_dir=untitled-host
	
	if [ -d $new_host_dir ]; then
		rm -rf $new_host_dir
	fi
	
	# if the caller has not explicitly told us to skip a libmoai build, build now
	if [ x"$skip_build" != xtrue ]; then
		pushd libmoai > /dev/null
			bash build.sh -p $package_name -i $arm_mode -a $arm_arch -l $app_platform $tapjoy_flags $google_push_flags $google_billing_flags $crittercism_flags
		popd > /dev/null
	fi

	# copy libmoai into new host template dir
	new_host_lib_dir=$new_host_dir/host-source/project/libs
	mkdir -p $new_host_lib_dir
	
	if [ -d libmoai/libs ]; then
		cp -fR libmoai/libs/* $new_host_lib_dir
		rm -f $new_host_lib_dir/package.txt
	else
		echo "*** libmoai has not been built. This host will be incomplete until a version"
		echo "*** of libmoai.so is placed in $new_host_dir/host-source/project/libs/armeabi"
		echo "*** or $new_host_dir/host-source/project/libs/armeabi-v7a."
		echo ""
	fi

	# create function for easy find and replace
	backup_ext=.backup
	
	function fr () { 
		sed -i$backup_ext s%"$2"%"$3"%g $1
		rm -f $1$backup_ext
	}
		
	required_libs="\"facebook\""
	
	if [ x"$tapjoy_flags" == x ]; then
		required_libs="$required_libs \"tapjoy\""
	fi

	if [ x"$google_push_flags" == x ]; then
		required_libs="$required_libs \"google-push\""
	fi

	if [ x"$google_billing_flags" == x ]; then
		required_libs="$required_libs \"google-billing\""
	fi

	if [ x"$crittercism_flags" == x ]; then
		required_libs="$required_libs \"crittercism\""
	fi

	cp -f host-source/d.settings-global.sh $new_host_dir/settings-global.sh
	fr $new_host_dir/settings-global.sh @REQUIRED_LIBS@ "$required_libs"
	
	# copy default project files into new host dir
	cp -f host-source/d.README.txt $new_host_dir/README.txt
	cp -f host-source/d.run-host.sh $new_host_dir/run-host.sh
	cp -f host-source/d.run-host.bat $new_host_dir/run-host.bat
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/d.res/. $new_host_dir/res

	# copy source dir into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store --exclude=src/ --exclude=external/ host-source/source/. $new_host_dir/host-source

	# create package src directories
	mkdir -p $new_host_dir/host-source/project/src/com/ziplinegames/moai
	OLD_IFS=$IFS
	IFS='.'
	package_path=src
	for word in $package_name; do 
		package_path=$package_path/$word
		mkdir -p $new_host_dir/host-source/project/$package_path
	done
	IFS=$OLD_IFS
	
	# copy classes into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/source/project/src/app/. $new_host_dir/host-source/project/$package_path
	rsync -r --exclude=.svn --exclude=.DS_Store --exclude=/*.java host-source/source/project/src/moai/. $new_host_dir/host-source/moai
	rsync --exclude=.svn --exclude=.DS_Store host-source/source/project/src/moai/*.java $new_host_dir/host-source/project/src/com/ziplinegames/moai

	# copy external classes, resources, libs and projects into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/source/project/external/projects/. $new_host_dir/host-source/external
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/source/project/external/src/. $new_host_dir/host-source/project/src
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/source/project/external/libs/. $new_host_lib_dir

	# set the app platform in all project.properties files
	for file in `find $new_host_dir/host-source/ -name "project.properties"` ; do fr $file @APP_PLATFORM@ "$app_platform" ; done

	# inject the package path into the run script
	sed -i.backup s%@SETTING_PACKAGE_PATH@%"$package_path"%g $new_host_dir/run-host.sh
	rm -f $new_host_dir/run-host.sh.backup
	
	# inject the package name into the run script
	sed -i.backup s%@SETTING_PACKAGE@%"$package_name"%g $new_host_dir/run-host.sh
	rm -f $new_host_dir/run-host.sh.backup

	# inject the package name into the run batch file
	sed -i.backup s%@SETTING_PACKAGE@%"$package_name"%g $new_host_dir/run-host.bat
	rm -f $new_host_dir/run-host.bat.backup
	
	# echo descriptive messages about this host
	echo "********************************************************************************"
	echo "* Android host successfully created.                                           *"
	echo "********************************************************************************"
	echo ""
	echo "- The new host is in the \"untitled-host\" directory. Every time this script is"
	echo "  run, it will clobber the contents of the \"untitled-host\" directory so you"
	echo "  will probably want to move this folder elsewhere and rename it."
	echo ""
	echo "- Edit \"settings-global.sh\" to configure your project and point it to your lua"
	echo "  scripts and other resources."
	echo ""
	echo "- The first time you run the host, the file \"settings-local.sh\" is created."
	echo "  You will need to configure the path to your Android SDK therein."
	echo ""
	echo "- Note that host is tied to the package name you specified when it was created."
	echo "  If you wish to change the package name, simply recreate the host with the new"
	echo "  package name and re-apply your settings files (and any resource files) into the"
	echo "  new project."
	echo ""
	echo "********************************************************************************"
	