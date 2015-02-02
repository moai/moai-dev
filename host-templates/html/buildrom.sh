python file_packager.py moaiapp.rom --preload src@/ --js-output=moaiapp.rom.js --as-json
mv -f moaiapp.rom www/moaiapp.rom
mv -f moaiapp.rom.json www/moaiapp.rom.json
