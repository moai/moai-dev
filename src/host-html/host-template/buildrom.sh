python emscripten/tools/file_packager.py moaiapp.rom --preload src/ --pre-run --js-output=moaiapp.rom.js
mv -f moaiapp.rom www/moaiapp.rom
mv -f moaiapp.rom.js www/moaiapp.rom.js
