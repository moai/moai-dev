#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e
	
	# check for command line switches
	usage="usage: $0 [-r localRootFolder]"
	local_root=
	
	while [ $# -gt 0 ];	do
	    case "$1" in
	        -r)  local_root="$2"; shift;;
			-*)
		    	echo >&2 \
		    		$usage
		    	exit 1;;
			*)  break;;	# terminate while loop
	    esac
	    shift
	done

	package=@SETTING_PACKAGE@
	package_path=@SETTING_PACKAGE_PATH@
	out_dir="`pwd`/build"
	
	rm -rf $out_dir

	source ./settings-global.sh
	source ./settings-local.sh

	if [ x"$android_sdk_root" != x ] && [ x"$local_root" != x ] && [[ ! $android_sdk_root == /* ]]; then
		android_sdk_root=$local_root/$android_sdk_root
	fi
	
	if [ x"$android_sdk_root" = x ] || [ ! -d $android_sdk_root ]; then		
		echo -e "*** Please specify a valid path to the Android SDK in \"settings-local.sh\""
		echo
		exit 1
	fi

	mkdir -p $out_dir/project

	mkdir -p $out_dir/project/assets

	mkdir -p $out_dir/project/libs
	cp -fR	host-source/project/libs/*	$out_dir/project/libs

	mkdir -p $out_dir/project/res
	cp -fR	host-source/project/res/*	$out_dir/project/res
	
	mkdir -p $out_dir/project/res/drawable-ldpi
	mkdir -p $out_dir/project/res/drawable-mdpi
	mkdir -p $out_dir/project/res/drawable-hdpi
	mkdir -p $out_dir/project/res/drawable-xhdpi
	mkdir -p $out_dir/project/res/raw

	if [ x"$icon_ldpi" != x ] && [ -f $icon_ldpi ]; then
		cp -f $icon_ldpi $out_dir/project/res/drawable-ldpi/icon.png
	fi

	if [ x"$icon_mdpi" != x ] && [ -f $icon_mdpi ]; then
		cp -f $icon_mdpi $out_dir/project/res/drawable-mdpi/icon.png
	fi

	if [ x"$icon_hdpi" != x ] && [ -f $icon_hdpi ]; then
		cp -f $icon_hdpi $out_dir/project/res/drawable-hdpi/icon.png
	fi

	if [ x"$icon_xhdpi" != x ] && [ -f $icon_xhdpi ]; then
		cp -f $icon_xhdpi $out_dir/project/res/drawable-xhdpi/icon.png
	fi
	
	if [ x"$key_store" != x ] && [ x"$local_root" != x ] && [[ ! $key_store == /* ]]; then
		key_store=$local_root/$key_store
	fi
	
	if [ x"$key_store" != x ] && [ -f $key_store ]; then
		cp -f $key_store $out_dir/project/`basename $key_store`
	fi
			
	cp -f host-source/project/.classpath $out_dir/project/.classpath
	cp -f host-source/project/proguard.cfg $out_dir/project/proguard.cfg
	
	mkdir -p $out_dir/project/$package_path
	
	backup_ext=.backup
	
	function fr () {
		sed -i$backup_ext s%"$2"%"$3"%g $1
		rm -f $1$backup_ext
	}
	
	fr $out_dir/project/res/values/strings.xml @NAME@ "$app_name"
	
	cp -f host-source/project/.project $out_dir/project/.project 
	fr $out_dir/project/.project @NAME@ "$project_name"

	cp -f host-source/project/build.xml $out_dir/project/build.xml
	fr $out_dir/project/build.xml @NAME@ "$project_name"

	cp -f host-source/project/AndroidManifest.xml $out_dir/project/AndroidManifest.xml
	fr $out_dir/project/AndroidManifest.xml	@DEBUGGABLE@ "$debug"
	fr $out_dir/project/AndroidManifest.xml	@VERSION_CODE@ "$version_code"
	fr $out_dir/project/AndroidManifest.xml	@VERSION_NAME@ "$version_name"	
	
	cp -f host-source/project/ant.properties $out_dir/project/ant.properties
	if [ x"$key_store" != x ]; then
		key_store=`basename $key_store`
	fi
	fr $out_dir/project/ant.properties @KEY_STORE@ "$key_store"
	fr $out_dir/project/ant.properties @KEY_ALIAS@ "$key_alias"
	fr $out_dir/project/ant.properties @KEY_STORE_PASSWORD@ "$key_store_password"
	fr $out_dir/project/ant.properties @KEY_ALIAS_PASSWORD@ "$key_alias_password"

	cp -f host-source/project/project.properties $out_dir/project/project.properties

	dependency_index=1
	for (( i=0; i<${#requires[@]}; i++ )); do
		library=${requires[$i]}
		if ! [[ $library =~ ^[a-zA-Z0-9_\-]+$ ]]; then
			echo -e "*** Illegal optional component specified: $library, skipping..."
			echo -e "    > Optional component references may only contain letters, numbers, dashes and underscores"
			echo
			continue
		fi
		if [ -f host-source/external/$library/manifest_declarations.xml ]; then
			awk 'FNR==NR{ _[++d]=$0; next } /EXTERNAL DECLARATIONS:/ { print; print ""; for ( i=1; i<=d; i++ ) { print _[i] } next } 1' host-source/external/$library/manifest_declarations.xml $out_dir/project/AndroidManifest.xml > /tmp/AndroidManifest.tmp && mv -f /tmp/AndroidManifest.tmp $out_dir/project/AndroidManifest.xml
		fi
		if [ -f host-source/external/$library/manifest_permissions.xml ]; then
			awk 'FNR==NR{ _[++d]=$0; next } /EXTERNAL PERMISSIONS:/ { print; print ""; for ( i=1; i<=d; i++ ) { print _[i] } next } 1' host-source/external/$library/manifest_permissions.xml $out_dir/project/AndroidManifest.xml > /tmp/AndroidManifest.tmp && mv -f /tmp/AndroidManifest.tmp $out_dir/project/AndroidManifest.xml
		fi
		if [ -f host-source/external/$library/classpath.xml ]; then
			awk 'FNR==NR{ _[++d]=$0; next } /EXTERNAL ENTRIES:/ { print; print ""; for ( i=1; i<=d; i++ ) { print _[i] } next } 1' host-source/external/$library/classpath.xml $out_dir/project/.classpath > /tmp/.classpath.tmp && mv -f /tmp/.classpath.tmp $out_dir/project/.classpath
		fi
		if [ -d host-source/moai/$library ]; then
#			rsync -r --exclude=.svn --exclude=.DS_Store "host-source/moai/$library/." "$out_dir/project/src/com/ziplinegames/moai"
			pushd host-source/moai/$library > /dev/null
				find . -name ".?*" -type d -prune -o -type f -print0 | cpio -pmd0 --quiet $out_dir/project/src/com/ziplinegames/moai
			popd > /dev/null
		fi
		if [ -d host-source/external/$library/project ]; then
#			rsync -r --exclude=.svn --exclude=.DS_Store "host-source/external/$library/project/" "$out_dir/$library"
			pushd host-source/external/$library/project > /dev/null
				find . -name ".?*" -type d -prune -o -type f -print0 | cpio -pmd0 --quiet $out_dir/$library
			popd > /dev/null
			echo "android.library.reference.${dependency_index}=../$library/" >> $out_dir/project/project.properties
			dependency_index=$(($dependency_index+1))
		fi
		if [ -d host-source/external/$library/lib ]; then
#			rsync -r --exclude=.svn --exclude=.DS_Store "host-source/external/$library/lib/" "$out_dir/project/libs"
			pushd host-source/external/$library/lib > /dev/null
				find . -name ".?*" -type d -prune -o -type f -print0 | cpio -pmd0 --quiet $out_dir/project/libs
			popd > /dev/null
		fi
		if [ -d host-source/external/$library/src ]; then
#			rsync -r --exclude=.svn --exclude=.DS_Store "host-source/external/$library/src/" "$out_dir/project/src"
			pushd host-source/external/$library/src > /dev/null
				find . -name ".?*" -type d -prune -o -type f -print0 | cpio -pmd0 --quiet $out_dir/project/src
			popd > /dev/null
		fi
	done
	
	fr $out_dir/project/AndroidManifest.xml	@PACKAGE@ "$package"
	
	cp -f host-source/project/local.properties $out_dir/project/local.properties
	for file in `find $out_dir/ -name "local.properties"` ; do fr $file @SDK_ROOT@ "$android_sdk_root" ; done

	cp -rf host-source/project/src $out_dir/project/

	fr $out_dir/project/$package_path/MoaiActivity.java @WORKING_DIR@ "$working_dir"
	for file in `find $out_dir/project/$package_path/ -name "*.java"` ; do fr $file @PACKAGE@ "$package" ; done
	
	working_dir_depth=`grep -o "\/" <<<"$working_dir" | wc -l`
	(( working_dir_depth += 1 ))
	
	for (( i=1; i<=$working_dir_depth; i++ )); do
		if [ $i == 1 ]; then
			init_dir=\.\.
		else
			init_dir=$init_dir\/\.\.
		fi
	done
	
	run_command="\"$init_dir/init.lua\""
	
	for file in "${run[@]}"; do
		run_command="$run_command, \"$file\""
	done
	
	run_command="runScripts ( new String [] { $run_command } );"
	
	fr $out_dir/project/$package_path/MoaiView.java @RUN_COMMAND@ "$run_command"

	cp -f host-source/init.lua $out_dir/project/assets/init.lua
	
	for (( i=0; i<${#src_dirs[@]}; i++ )); do
#		rsync -r --exclude=.svn --exclude=.DS_Store --exclude=*.bat --exclude=*.sh ${src_dirs[$i]}/. $out_dir/project/assets/${dest_dirs[$i]}
		source_dir=${src_dirs[$i]}
		if [ x"$source_dir" != x ] && [ x"$local_root" != x ] && [[ ! $source_dir == /* ]]; then
			source_dir=$local_root/$source_dir
		fi
		pushd $source_dir > /dev/null
			find . -name ".?*" -type d -prune -o -name "*.sh" -type f -prune -o -name "*.bat" -type f -prune -o -type f -print0 | cpio -pmd0 --quiet $out_dir/project/assets/${dest_dirs[$i]}
		popd > /dev/null
	done

	if [ "$debug" == "true" ]; then
		install_cmd="ant debug install"
	else
		install_cmd="ant release install"		
	fi
	
	if [ $OSTYPE != cygwin ]; then
		pushd $out_dir/project > /dev/null
			ant uninstall
			ant clean
			$install_cmd
			adb shell am start -a android.intent.action.MAIN -n $package/$package.MoaiActivity
			adb logcat -c
			adb logcat MoaiLog:V AndroidRuntime:E *:S
		popd > /dev/null
	fi