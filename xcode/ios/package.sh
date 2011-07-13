# extend path to include system folders. on some machines, this is necessary
PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:$PATH

# if we don't already have our own moai-target file, create one based on the default file
if [ ! -f moai-target ]; then
	cp mt.default moai-target
fi

# remove existing lua folder, so we can make a fresh one
chmod -R 777 ./lua
rm -rf lua
mkdir -p lua

# read moai-target file line by line, and copy contents of each folder specified
function processDir {

	if [ -d $1 ]; then
		
		cp -r $1/* lua
		
		# check if destination folder contains a moai-target-ext file
		# if it does, add it's content as well
		if [ -f $1/moai-target-ext ]; then
			for i in $(cat $1/moai-target-ext); do 
				processDir $i
			done
		fi
	fi
}

for i in $(cat moai-target); do 
	processDir $i
done

# mark contents of lua folder as read-only
chmod -R 555 ./lua/*

# remove files which are not needed, if they exist
function removeFile {

	if [ -f $1 ]; then 
		chmod 777 $1
		rm -f $1
	fi
}

removeFile ./lua/run.bat
removeFile ./lua/run.sh
removeFile ./lua/moai-target-ext
