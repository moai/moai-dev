PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:$PATH

if [ ! -f moai-target ]; then
	cp mt.default moai-target
fi

chmod -R 777 ./lua
rm -rf lua
mkdir -p lua

for i in $(cat moai-target); do 

	cp -r ../../$i/* lua
	
	chmod -R 555 ./lua/*
	
	if [ -f ./lua/run.bat ]; then 
		chmod 777 ./lua/run.bat
	fi
	
	if [ -f ./lua/run.sh ]; then 
		chmod 777 ./lua/run.sh
	fi
	
	rm -f lua/run.bat
	rm -f lua/run.sh
done
