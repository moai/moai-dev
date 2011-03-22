rm -rf iPhoneSnapSample*

svn export . iPhoneSnapSample
pushd iPhoneSnapSample
rm *.psd
rm buildsample.sh

cd Objective-Chipmunk-iPhone
lipo libObjectiveChipmunk-iPhone.a -thin i386 -output libObjectiveChipmunk-iPhone.a

popd
tar -czf iPhoneSnapSample.tgz iPhoneSnapSample
rm -rf iPhoneSnapSample
