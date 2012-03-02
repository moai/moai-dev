#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e

	# define package
	package=@SETTING_PACKAGE@
	package_path=@SETTING_PACKAGE_PATH@
	out_dir=build
	
	# delete build folder
	rm -rf $out_dir

	# load global settings
	source ./settings-global.sh

	# load local settings
	local_settings_created=false

	if [ ! -f settings-local.sh ]; then
		cp -f host-source/d.settings-local.sh settings-local.sh
		local_settings_created=true
		echo
		echo -e "*** Local file \"settings-local.sh\" created"
	fi

	source ./settings-local.sh

	# check for valid Android SDK root
	if [ "$android_sdk_root" = "" ] || [ ! -d $android_sdk_root ]; then
		
		if [ $local_settings_created = false ]; then
			echo
		fi
		echo -e "*** Please specify a valid path to Android SDK in \"settings-local.sh\""
		echo -e "*** Add this file to the ignore list of your version control system."
		echo
		exit 1
	fi

	# create build directory
	mkdir -p $out_dir/project

	# create assets directory
	mkdir -p $out_dir/project/assets

	# create libs directory and copy the libmoai libraries
	mkdir -p $out_dir/project/libs
	cp -fR	host-source/project/libs/*	$out_dir/project/libs

	# create res directory and copy all resources
	mkdir -p $out_dir/project/res
	cp -fR	host-source/project/res/*	$out_dir/project/res
	
	# create res subdirectories that may be missing
	mkdir -p $out_dir/project/res/drawable-hdpi
	mkdir -p $out_dir/project/res/drawable-ldpi
	mkdir -p $out_dir/project/res/drawable-mdpi
	mkdir -p $out_dir/project/res/raw

	# copy icon files into res
	cp -f $icon_ldpi $out_dir/project/res/drawable-ldpi/icon.png
	cp -f $icon_mdpi $out_dir/project/res/drawable-mdpi/icon.png
	cp -f $icon_hdpi $out_dir/project/res/drawable-hdpi/icon.png
	
	# copy keystore
	if [ "$key_store" != "" ] && [ -f $key_store ]; then
		cp -f $key_store $out_dir/project/`basename $key_store`
	fi
			
	# copy project files that do not need editing
	cp -f host-source/project/.classpath $out_dir/project/.classpath
	cp -f host-source/project/proguard.cfg $out_dir/project/proguard.cfg
	
	# create src directories
	mkdir -p $out_dir/project/$package_path
	
	# create function for easy find and replace
	backup_ext=.backup
	
	function fr () {
		sed -i$backup_ext s%"$2"%"$3"%g $1
		rm -f $1$backup_ext
	}
	
	# replace the app name inside strings.xml
	fr $out_dir/project/res/values/strings.xml @NAME@ "$app_name"
	
	# copy .project file and replace the app name
	cp -f host-source/project/.project $out_dir/project/.project 
	fr $out_dir/project/.project @NAME@ "$project_name"

	# copy build.xml file and replace the app name
	cp -f host-source/project/build.xml $out_dir/project/build.xml
	fr $out_dir/project/build.xml @NAME@ "$project_name"

	# copy AndroidManifest.xml file and replace text
	cp -f host-source/project/AndroidManifest.xml $out_dir/project/AndroidManifest.xml
	fr $out_dir/project/AndroidManifest.xml	@DEBUGGABLE@ "$debug"
	fr $out_dir/project/AndroidManifest.xml	@VERSION_CODE@ "$versionCode"
	fr $out_dir/project/AndroidManifest.xml	@VERSION_NAME@ "$versionName"	
	
	# copy ant.properties file and replace text
	cp -f host-source/project/ant.properties $out_dir/project/ant.properties
	fr $out_dir/project/ant.properties @KEY_STORE@ "$key_store"
	fr $out_dir/project/ant.properties @KEY_ALIAS@ "$key_alias"
	fr $out_dir/project/ant.properties @KEY_STORE_PASSWORD@ "$key_store_password"
	fr $out_dir/project/ant.properties @KEY_ALIAS_PASSWORD@ "$key_alias_password"

	# copy project.properties file and add dependent libraries
	cp -f host-source/project/project.properties $out_dir/project/project.properties
	for (( i=1; i<=${#requires[@]}; i++ )); do
		if [ -f "host-source/external/${requires[$i-1]}/manifest_declarations.xml" ]; then
			awk 'FNR==NR{ _[++d]=$0; next } /EXTERNAL DECLARATIONS:/ { print; print ""; for ( i=1; i<=d; i++ ) { print _[i] } next } 1' "host-source/external/${requires[$i-1]}/manifest_declarations.xml" $out_dir/project/AndroidManifest.xml > /tmp/AndroidManifest.tmp && mv -f /tmp/AndroidManifest.tmp $out_dir/project/AndroidManifest.xml
		fi
		if [ -f "host-source/external/${requires[$i-1]}/manifest_permissions.xml" ]; then
			awk 'FNR==NR{ _[++d]=$0; next } /EXTERNAL PERMISSIONS:/ { print; print ""; for ( i=1; i<=d; i++ ) { print _[i] } next } 1' "host-source/external/${requires[$i-1]}/manifest_permissions.xml" $out_dir/project/AndroidManifest.xml > /tmp/AndroidManifest.tmp && mv -f /tmp/AndroidManifest.tmp $out_dir/project/AndroidManifest.xml
		fi
		if [ -d "host-source/moai/${requires[$i-1]}" ]; then
			rsync -r "host-source/moai/${requires[$i-1]}/." "$out_dir/project/src/com/ziplinegames/moai"
		fi
		if [ -d "host-source/external/${requires[$i-1]}/project" ]; then
			rsync -r "host-source/external/${requires[$i-1]}/project/" "$out_dir/${requires[$i-1]}"
			echo "android.library.reference.${i}=../${requires[$i-1]}/" >> "$out_dir/project/project.properties"
		fi
	done
	
	# replace the package name in AndroidManifest.xml
	fr $out_dir/project/AndroidManifest.xml	@PACKAGE@ "$package"
	
	# copy local.properties file and replace the SDK root
	cp -f host-source/project/local.properties $out_dir/project/local.properties
	for file in `find $out_dir/ -name "local.properties"` ; do fr $file @SDK_ROOT@ "$android_sdk_root" ; done

	# copy all src files
	cp -rf host-source/project/src $out_dir/project/

	# replace text inside required src files
	fr $out_dir/project/$package_path/MoaiActivity.java @WORKING_DIR@ "$working_dir"
	for file in `find $out_dir/project/$package_path/ -name "*.java"` ; do fr $file @PACKAGE@ "$package" ; done
	
	# create run command for the init.lua file
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
	
	# create run commands for the host
	for file in "${run[@]}"; do
		run_command="$run_command, \"$file\""
	done
	
	run_command="runScripts ( new String [] { $run_command } );"
	
	fr $out_dir/project/$package_path/MoaiView.java @RUN_COMMAND@ "$run_command"

	# bundle android-init file
	cp -f host-source/init.lua $out_dir/project/assets/init.lua

	# bundle source folders
	function copyFolder () {
		mkdir -p $2
		rsync -r --exclude=.svn --exclude=.DS_Store --exclude=*.bat --exclude=*.sh $1/. $2
	}
	
	i=0
	for src_dir in "${src_dirs[@]}"; do
		copyFolder $src_dir $out_dir/project/assets/${dest_dirs[i]}
		i=$i+1
	done

	# install release mode of the project
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
	
