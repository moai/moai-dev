set minilua=%1
set outdir=%2
set luajit=..\..\3rdparty\luajit-2.0.1

if not exist %outdir% mkdir -p %outdir%
%minilua% %luajit%\dynasm\dynasm.lua -LN -D WIN -D JIT -D FFI -o %outdir%\buildvm_arch.h %luajit%\src\vm_x86.dasc
