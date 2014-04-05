make doesn't detect abc files, so if changes in Console.as or the VFS are done you need to remove the compiled files:

rm flascc-host/*.a* && rm flascc-host/*.swf && make && open flascc-host/flascc-host.swf 

To debug:

http://livedocs.adobe.com/flex/3/html/help.html?content=logging_04.html

http://www.adobe.com/devnet-docs/flascc/docs/Reference.html#section_gdb


You'll also need to update some variables in flascc/CMakeLists.txt

#
# FlasCC-Specific configurations
#
if ( FLASCC_BUILD )
  set ( FLASCC "/Users/nictuku/flascc/sdk")
  set ( JAVA /usr/bin/java )
  set ( FLEX "/Users/nictuku/flex_sdk")
endif ( FLASCC_BUILD )

Use your own paths for it.