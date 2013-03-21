make doesn't detect abc files, so if changes in Console.as or the VFS are done you need to remove the compiled files:

rm flascc-host/*.a* && rm flascc-host/*.swf && make && open flascc-host/flascc-host.swf 

To debug:

http://livedocs.adobe.com/flex/3/html/help.html?content=logging_04.html

http://www.adobe.com/devnet-docs/flascc/docs/Reference.html#section_gdb