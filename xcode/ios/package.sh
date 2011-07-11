PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:$PATH

if [ ! -f moai-target ]; then
	cp mt.default moai-target
fi

read LUA_FOLDER < moai-target

if [ -d lua ]; then
	cd lua
	rsync -ru . ../../../$LUA_FOLDER
	cd ..
fi

rm -rf lua
mkdir -p lua

cp -r ../../$LUA_FOLDER/* lua

rm -f lua/run.bat
rm -f lua/run.sh
