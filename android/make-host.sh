#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	# check for command line switches
	usage="usage: $0 -p package [-l appPlatform ] [-thumb] [-q]"
	quiet="false"
	thumb=
	packageName=
	appPlatform=android-8
	
	while [ $# -gt 0 ];	do
	    case "$1" in
	        -thumb)  thumb=-thumb;;
			-p)  packageName="$2"; shift;;
			-l)  appPlatform="$2"; shift;;
			-q)  quiet="true";;
			-*)
		    	echo >&2 \
		    		$usage
		    	exit 1;;
			*)  break;;	# terminate while loop
	    esac
	    shift
	done
		
	if [ "$packageName" = "" ]; then
		echo $usage
		exit 1
	fi

	# remove existing new host folder, if any
	new_host_dir=untitled-host
	
	if [ -d $new_host_dir ]; then
		rm -rf $new_host_dir
	fi

	# if libmoai already exists, find out which package it was build for
	if [ -f libmoai/libs/armeabi/package.txt ]; then
		existing_package=$( sed -n '1p' libmoai/libs/armeabi/package.txt )
		existing_thumb=$( sed -n '2p' libmoai/libs/armeabi/package.txt )
		existing_app_platform=$( sed -n '3p' libmoai/libs/armeabi/package.txt )
	fi
	
	shouldBuild=false

	if [ "$existing_package" != "$packageName" ]; then
		shouldBuild=true
	fi

	if [ "$existing_thumb" == "thumb" ] && [ "$thumb" == "" ]; then
		shouldBuild=true
	fi

	if [ "$existing_thumb" == "arm" ] && [ "$thumb" == "-thumb" ]; then
		shouldBuild=true
	fi

	if [ "$existing_app_platform" != "$appPlatform" ]; then
		shouldBuild=true
	fi

	if [ "$shouldBuild" == "true" ] || [ ! -f libmoai/libs/armeabi/libmoai.so ]; then
		pushd libmoai > /dev/null
			bash build.sh -p $packageName -l $appPlatform $thumb
		popd > /dev/null
	fi

	# copy libmoai into new host template dir
	new_host_lib_dir=$new_host_dir/host-source/project/libs/armeabi
	mkdir -p $new_host_lib_dir
	
	# copy libmoai into new host template dir
	if [ ! -f libmoai/libs/armeabi/libmoai.so ]; then
		echo "*** libmoai.so has not been built for $packageName. Android host will be incomplete!"
		quiet="true"
	else
		cp -f libmoai/libs/armeabi/libmoai.so $new_host_lib_dir/libmoai.so
	fi
	
	# copy default project files into new host dir
	cp -f host-source/d.README.txt $new_host_dir/README.txt
	cp -f host-source/d.run-host.sh $new_host_dir/run-host.sh
	cp -f host-source/d.run-host.bat $new_host_dir/run-host.bat
	cp -f host-source/d.settings-global.sh $new_host_dir/settings-global.sh
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/d.res/. $new_host_dir/res

	# copy source dir into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store --exclude=src/ --exclude=ext/ host-source/source/. $new_host_dir/host-source

	# create package src directories
	OLD_IFS=$IFS
	IFS='.'
	package_path=src
	for word in $packageName; do 
		package_path=$package_path/$word
		mkdir -p $new_host_dir/host-source/project/$package_path
	done
	IFS=$OLD_IFS

	# copy classes into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/source/project/src/. $new_host_dir/host-source/project/$package_path

	# copy external classes into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/source/project/ext/. $new_host_dir/host-source/project/src

	# inject the package path into the run script
	sed -i.backup s%@SETTING_PACKAGE_PATH@%"$package_path"%g $new_host_dir/run-host.sh
	rm -f $new_host_dir/run-host.sh.backup
	
	# inject the package name into the run script
	sed -i.backup s%@SETTING_PACKAGE@%"$packageName"%g $new_host_dir/run-host.sh
	rm -f $new_host_dir/run-host.sh.backup

	# inject the package name into the run batch file
	sed -i.backup s%@SETTING_PACKAGE@%"$packageName"%g $new_host_dir/run-host.bat
	rm -f $new_host_dir/run-host.bat.backup
	
	# echo descriptive messages about this host
	if [ "$quiet" != "true" ]; then
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
	fi
	