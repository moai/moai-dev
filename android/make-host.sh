#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	# check for command line switches
	usage="usage: $0 -p package [-thumb]"
	thumb=
	packageName=
	
	while [ $# -gt 0 ];	do
	    case "$1" in
	        -thumb)  thumb=-thumb;;
			-p)  packageName="$2"; shift;;
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

	# build libmoai for the specified package
	if [ -f libmoai/libs/armeabi/package.txt ]; then
		read existing_package < libmoai/libs/armeabi/package.txt
	fi
	
	if [ "$existing_package" != "$packageName" ] || [ ! -f libmoai/libs/armeabi/libmoai.so ]; then
		pushd libmoai > /dev/null
			bash build.sh -p $packageName $thumb
		popd > /dev/null
	fi

	# copy libmoai into new host template dir
	new_host_lib_dir=$new_host_dir/host-source/project/libs/armeabi
	mkdir -p $new_host_lib_dir
	cp -f libmoai/libs/armeabi/libmoai.so $new_host_lib_dir/libmoai.so
	
	# copy default project files into new host dir
	cp -f host-source/d.run.sh $new_host_dir/run.sh
	cp -f host-source/d.run.bat $new_host_dir/run.bat
	cp -f host-source/d.settings-global.sh $new_host_dir/settings-global.sh
	rsync -r --exclude=.svn --exclude=.DS_Store host-source/d.res/. $new_host_dir/res

	# copy source dir into new host dir
	rsync -r --exclude=.svn --exclude=.DS_Store --exclude=src/ host-source/source/. $new_host_dir/host-source

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

	# inject the package path into the run script
	sed -i.backup s%@SETTING_PACKAGE_PATH@%"$package_path"%g $new_host_dir/run.sh
	rm -f $new_host_dir/run.sh.backup
	
	# inject the package name into the run script
	sed -i.backup s%@SETTING_PACKAGE@%"$packageName"%g $new_host_dir/run.sh
	rm -f $new_host_dir/run.sh.backup

	# inject the package name into the run batch file
	sed -i.backup s%@SETTING_PACKAGE@%"$packageName"%g $new_host_dir/run.bat
	rm -f $new_host_dir/run.bat.backup
	
	# echo descriptive messages about this host
	echo ""
	echo "****************************************************************"
	echo "* Android host successfully created."
	echo "****************************************************************"
	echo ""
	echo "-The host is in the \"untitled-host\" directory. Rename it to anything you'd like."
	echo ""
	echo "-Edit \"settings-global.sh\" to configure your project."
	echo ""
	echo "-This is a standalone Android host. You can copy it anywhere on your system. However, you'll need to edit \"settings-global.sh\" to specify the new paths to your Lua resource files."
	echo ""
	echo "-The first time you run the host, the file \"settings-local.sh\" is created. This file should not be checked into your version control system."
	echo ""
	echo "-This host is tied to the package name you specified when it was created. If you wish to change the package name, simply recreate the project with the new package name and copy over your settings files (and any resource files) into the new project."
	echo ""
	echo "****************************************************************"
	echo ""
	