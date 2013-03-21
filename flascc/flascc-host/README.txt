make doesn't detect abc files, so if changes in Console.as or the VFS are done you need to remove the compiled files:

rm flascc-host/*.a* && rm flascc-host/*.swf && make && open flascc-host/flascc-host.swf 