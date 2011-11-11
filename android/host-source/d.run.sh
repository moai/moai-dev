#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e

	# define package
	package=@SETTING_PACKAGE@
	package_path=@SETTING_PACKAGE_PATH@
	
	# delete build folder
	rm -rf build

	# load global settings
	source settings-global.sh

	# load local settings
	local_settings_created=false

	if [ ! -f settings-local.sh ]; then
		cp -f host-source/d.settings-local.sh settings-local.sh
		local_settings_created=true
		echo
		echo -e "*** Local file \"settings-local.sh\" created"
	fi

	source settings-local.sh

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
	if [ ! -d build ]; then
		mkdir -p build
	fi

	# create assets directory
	mkdir -p build/assets

	# create libs directory (and subdirectories)
	mkdir -p build/libs/armeabi
	
	# create res directory (and subdirectories)
	mkdir -p build/res
	mkdir -p build/res/drawable-hdpi
	mkdir -p build/res/drawable-ldpi
	mkdir -p build/res/drawable-mdpi
	mkdir -p build/res/raw
	mkdir -p build/res/values

	# copy keystore
	if [ "$key_store" != "" ] && [ -f $key_store ]; then
		cp -f $key_store build/`basename $key_store`
	fi
		
	# copy libmoai
	cp -f	host-source/project/libs/armeabi/libmoai.so	build/libs/armeabi/libmoai.so

	# copy icon files
	cp -f	$icon_ldpi		build/res/drawable-ldpi/icon.png
	cp -f	$icon_mdpi		build/res/drawable-mdpi/icon.png
	cp -f	$icon_hdpi		build/res/drawable-hdpi/icon.png
	
	# copy project files that do not need editing
	cp -f 	host-source/project/.classpath 				build/.classpath
	cp -f 	host-source/project/proguard.cfg			build/proguard.cfg
	cp -f 	host-source/project/project.properties		build/project.properties
	
	# create src directories
	mkdir -p build/$package_path
	
	# create function for easy find and replace
	backup_ext=.backup
	
	function fr () { 
		sed -i$backup_ext s%$2%"$3"%g $1
		rm -f $1$backup_ext
	}
	
	# copy .project file and replace text inside
	cp -f	host-source/project/.project	build/.project 
	fr build/.project 	@NAME@	$project_name

	# copy AndroidManifest.xml file and replace text inside
	cp -f	host-source/project/AndroidManifest.xml		build/AndroidManifest.xml
	fr build/AndroidManifest.xml	@NAME@			$project_name
	fr build/AndroidManifest.xml	@PACKAGE@		$package
	fr build/AndroidManifest.xml	@DEBUGGABLE@	$debug

	# copy ant.properties file and replace text inside
	cp -f	host-source/project/ant.properties	build/ant.properties
	fr build/ant.properties		@KEY_STORE@		$key_store
	fr build/ant.properties		@KEY_ALIAS@		$key_alias

	# copy build.xml file and replace text inside
	cp -f	host-source/project/build.xml	build/build.xml
	fr build/build.xml		@NAME@		$project_name

	# copy local.properties file and replace text inside
	cp -f	host-source/project/local.properties	build/local.properties
	fr build/local.properties		@SDK_ROOT@		$android_sdk_root

	# copy local.properties file and replace text inside
	cp -f 	host-source/project/res/values/strings.xml	build/res/values/strings.xml
	fr build/res/values/strings.xml	@NAME@		$project_name

	# copy MoaiActivity.java file and replace text inside
	cp -f 	host-source/project/$package_path/MoaiActivity.java	build/$package_path/MoaiActivity.java
	fr build/$package_path/MoaiActivity.java	@PACKAGE@	$package

	# copy MoaiView.java file and replace text inside
	cp -f 	host-source/project/$package_path/MoaiView.java	build/$package_path/MoaiView.java
	fr build/$package_path/MoaiView.java	@PACKAGE@		$package
	fr build/$package_path/MoaiView.java	@WORKING_DIR@	$working_dir

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

	fr 	build/$package_path/MoaiView.java	@RUN_INIT_DIR@ 	$init_dir
	
	# create run commands for the host
	for file in "${run[@]}"; do
		run_command=`echo -e $run_command"AKURunScript\(\""$file"\"\)\;\n"`
	done

	fr 	build/$package_path/MoaiView.java	@RUN@ 	$run_command

	# create bundle directory
	bundle_file_name=bundle
	bundle_dir=build/$bundle_file_name

	if [ -d $bundle_dir ]; then
		rm -rf $bundle_dir
	fi
	
	mkdir -p $bundle_dir

	# bundle android-init file
	cp -f host-source/init.lua $bundle_dir/init.lua

	# bundle source folders
	function copyFolderIntoBundle () {
		mkdir -p $2
		rsync -r --exclude=.svn --exclude=.DS_Store --exclude=*.bat --exclude=*.sh $1/. $2
	}
	
	i=0
	for src_dir in "${src_dirs[@]}"; do
		copyFolderIntoBundle $src_dir $bundle_dir/${dest_dirs[i]}
		i=$i+1
	done

	# create final bundle file
	rm -f build/res/raw/$bundle_file_name
	
	pushd $bundle_dir > /dev/null
		zip -9 -r -q ../res/raw/$bundle_file_name . *.*
	popd > /dev/null
	
	pushd build/res/raw > /dev/null
		mv $bundle_file_name.zip $bundle_file_name
	popd > /dev/null
	
	# install release mode of the project
	if [ "$debug" == "true" ]; then
		install_cmd="ant debug install"
	else
		install_cmd="ant release install"		
	fi
	
	if [ $OSTYPE != cygwin ]; then
		pushd build > /dev/null
			ant uninstall
			ant clean
			$install_cmd
			adb shell am start -a android.intent.action.MAIN -n $package/$package.MoaiActivity
			# adb logcat MoaiJNI:V MoaiLog:V *:S
			adb logcat MoaiLog:V *:S
		popd > /dev/null
	fi
	
