#!/bin/bash

echo $PWD

target_file=moai-target
target_dir=lua

PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:$PATH

if [ ! -f $target_file ]; then
	echo "No $target_file file, creating one from default file."
	cp mt.default $target_file
fi

if [ -d $target_dir ]; then
	chmod -R 777 $target_dir
	rm -rf $target_dir
 	mkdir -p $target_dir
fi

function processDir {
	
	if [ x"$1" = x -o x"$2" = x ]; then
		return
	fi
	
	#if [ -d $1 ]; then
		
		echo "Copying $1 to $2"
		
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
	#else
	#	echo "Folder not found, skipping $1"
	#fi
}

IFS=$'\n'$'\r'
for line in $(cat $target_file); do
	echo $line
	IFS=$' '
	arguments=($line)
	processDir ${arguments[0]} $target_dir/${arguments[1]}
done

function removeFile {

	if [ -f $1 ]; then 
		rm -f $1
	fi
}

removeFile $target_dir/run.bat
removeFile $target_dir/run.sh
removeFile $target_dir/$target_file-ext

# mark contents of target folder as read-only
chmod -R 555 $target_dir