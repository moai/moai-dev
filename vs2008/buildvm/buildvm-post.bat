set buildvm=%1
set outdir=%2
set luajit=..\..\3rdparty\luajit-2.0.1

if not exist %outdir% mkdir -p %outdir%

pushd %luajit%\src

%buildvm% -m peobj -o %outdir%\lj_vm.obj
%buildvm% -m bcdef -o %outdir%\lj_bcdef.h lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c 
%buildvm% -m ffdef -o %outdir%\lj_ffdef.h lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c 
%buildvm% -m libdef -o %outdir%\lj_libdef.h lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c 
%buildvm% -m recdef -o %outdir%\lj_recdef.h lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c 
%buildvm% -m vmdef -o %outdir%\vmdef.lua lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c 
%buildvm% -m folddef -o %outdir%\lj_folddef.h lj_opt_fold.c

popd

