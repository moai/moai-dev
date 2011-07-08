PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:$PATH

if [ ! -f moai-target ]; then
	cp mt.default moai-target
fi

read LUA_FOLDER < moai-target

rm -rf lua
mkdir -p lua

cd ../..
cp -r $LUA_FOLDER/* xcode/ios/lua

cd xcode/ios/lua
rm -f run.bat
rm -f run.sh
