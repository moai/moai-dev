#!/bin/bash

target_file=moai-target
target_dir=lua

# extend path to include system folders. on some machines, this is necessary
PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:$PATH

# if we don't already have our own target file, create one based on the default file
if [ ! -f $target_file ]; then
	echo "No $target_file file, creating one from default file."
	cp mt.default $target_file
fi

# remove existing target folder, so we can make a fresh one
if [ -d $target_dir ]; then
	chmod -R 777 $target_dir
	rm -rf $target_dir
fi

# read target file line by line, and copy contents of each folder specified
function processDir {
	
	if [ -d $1 ]; then
		
		mkdir -p $2
		cp -r $1/* $2
		
		# if the source folder contains a target-ext file, add its content as well
		if [ -f $1/$target_file-ext ]; then
			while read line
			do
				arguments=($line)
				processDir $1/${arguments[0]} $2/${arguments[1]}
			done < $1/$target_file-ext
		fi
	else
		echo Could not find directory $1\; skipping this folder.
	fi
}

file=$target_file
while read line
do
	arguments=($line)
	processDir ${arguments[0]} $target_dir/${arguments[1]}
done < $target_file

# mark contents of target folder as read-only
chmod -R 555 $target_dir/*

# remove files which are not needed, if they exist
function removeFile {

	if [ -f $1 ]; then 
		chmod 777 $1
		rm -f $1
	fi
}

removeFile $target_dir/run.bat
removeFile $target_dir/run.sh
removeFile $target_dir/$target_file-ext