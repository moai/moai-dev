set buildvm=%1
set outdir=%2

if not exist %outdir% mkdir -p %outdir%

set src=..\..\3rdparty\luajit-2.0.1\src
set sources=%src%\lib_base.c %src%\lib_math.c %src%\lib_bit.c %src%\lib_string.c %src%\lib_table.c %src%\lib_io.c %src%\lib_os.c %src%\lib_package.c %src%\lib_debug.c %src%\lib_jit.c %src%\lib_ffi.c

%buildvm% -m peobj -o %outdir%\lj_vm.obj %sources%
%buildvm% -m bcdef -o %outdir%\lj_bcdef.h %sources%
%buildvm% -m ffdef -o %outdir%\lj_ffdef.h %sources%
%buildvm% -m libdef -o %outdir%\lj_libdef.h %sources%
%buildvm% -m recdef -o %outdir%\lj_recdef.h %sources%
%buildvm% -m vmdef -o %outdir%\vmdef.lua %sources%
%buildvm% -m folddef -o %outdir%\lj_folddef.h %src%\lj_opt_fold.c
