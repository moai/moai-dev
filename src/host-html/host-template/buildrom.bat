python file_packager.py moaiapp.rom --preload src@/  --js-output=moaiapp.rom.js --as-json

move /y moaiapp.rom ./www/moaiapp.rom

move /y moaiapp.rom.json ./www/moaiapp.rom.json

erase moaiapp.rom.js