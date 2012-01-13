#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	# check for command line switches
	usage="usage: $0 -p <package> [-i thumb | arm] [-a all | armeabi | armeabi-v7a] [-l appPlatform ] [-s]"
	package_name=
	arm_mode=arm
	arm_arch=armeabi-v7a
	app_platform=android-10
	skip_build="false"
	
	while [ $# -gt 0 ];	do
	    case "$1" in
			-p)  package_name="$2"; shift;;
	        -i)  arm_mode="$2"; shift;;
	        -a)  arm_arch="$2"; shift;;
			-l)  app_platform="$2"; shift;;
			-s)  skip_build="true";;
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

	# if libmoai already exists, find out which package it was build for
	if [ -f libmoai/libs/package.txt ]; then
		existing_package_name=$( sed -n '1p' libmoai/libs/package.txt )
		existing_arm_mode=$( sed -n '2p' libmoai/libs/package.txt )
		existing_arm_arch=$( sed -n '3p' libmoai/libs/package.txt )
		existing_app_platform=$( sed -n '4p' libmoai/libs/package.txt )
	fi
	
	shouldBuild=false

	if [ x"$existing_package_name" != x"$package_name" ]; then
		shouldBuild=true
	fi

	if [ x"$existing_arm_mode" != x"$arm_mode" ]; then
		shouldBuild=true
	fi

	if [ x"$existing_arm_arch" != x"$arm_arch" ]; then
		shouldBuild=true
	fi

	if [ x"$existing_app_platform" != x"$app_platform" ]; then
		shouldBuild=true
	fi
	
	# if the caller has not explicitly told us to skip a libmoai build, check
	# to see if we should build libmoai first.
	if [ x"$skip_build" != xtrue ]; then
		if [ x"$shouldBuild" == xtrue ]; then
			pushd libmoai > /dev/null
				bash build.sh -p $package_name -i $arm_mode -a $arm_arch -l $app_platform
			popd > /dev/null
		elif [ ! -f libmoai/libs/armeabi/libmoai.so ] && [ x"$arm_arch" = xarmeabi ] || [ x"$arm_arch" = xall ]; then
			pushd libmoai > /dev/null
				bash build.sh -p $package_name -i $arm_mode -a $arm_arch -l $app_platform
			popd > /dev/null
		elif [ ! -f libmoai/libs/armeabi-v7a/libmoai.so ] && [ x"$arm_arch" = xarmeabi-v7a ] || [ x"$arm_arch" = xall ]; then
			pushd libmoai > /dev/null
				bash build.sh -p $package_name -i $arm_mode -a $arm_arch -l $app_platform
			popd > /dev/null
		fi
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
	
	# copy default project files into new host dir
	cp -f host-source/d.README.txt $new_host_dir/README.txt
	cp -f host-source/d.run-host.sh $new_host_dir/run-host.sh
	cp -f host-source/d.run-host.bat $new_host_dir/run-host.bat
	cp -f host-source/d.settings-global.sh $new_host_dir/settings-global.sh
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/d.res/. $new_host_dir/res

	# copy source dir into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store --exclude=src/ --exclude=external/ host-source/source/. $new_host_dir/host-source

	# create function for easy find and replace
	backup_ext=.backup
	
	function fr () { 
		sed -i$backup_ext s%$2%"$3"%g $1
		rm -f $1$backup_ext
	}

	# set the app platform in the project properties
	fr $new_host_dir/host-source/project/project.properties	@APP_PLATFORM@ "$app_platform"
	
	# create package src directories
	OLD_IFS=$IFS
	IFS='.'
	package_path=src
	for word in $package_name; do 
		package_path=$package_path/$word
		mkdir -p $new_host_dir/host-source/project/$package_path
	done
	IFS=$OLD_IFS

	# copy classes into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/source/project/src/. $new_host_dir/host-source/project/$package_path

	# copy external classes and resources into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/source/project/external/src/. $new_host_dir/host-source/project/src
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/source/project/external/res/. $new_host_dir/host-source/project/res

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
	