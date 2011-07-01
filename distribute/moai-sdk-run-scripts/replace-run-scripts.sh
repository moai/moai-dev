# remove existing run.bat files, and replace with sdk run scripts
for f in $(find ../moai-sdk -type f -name *.bat); do 
	
	rm -f $f; 
	cp -f run.sh ${f%/*}/run.sh;
	cp -f run.bat ${f%/*}/run.bat;
done